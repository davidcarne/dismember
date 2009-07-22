/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "run_queue.h"
#include "exception.h"
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include <cstdio> // work around bug in boost < 1.38
#include <queue>


IRunQueueJob::~IRunQueueJob()
{}

bool operator<(const sp_RunQueueJob &j1, const sp_RunQueueJob &j2)
{
	return j1->getPriority() < j2->getPriority();
}

sp_RunQueueJob createFunctorRunQueueJob(std::string jobname,
		int priority, FunctorRunQueueJob::jobfun_t job)
{
	return sp_RunQueueJob(new FunctorRunQueueJob(jobname, priority, job));
}

bool FunctorRunQueueJob::exec()
{
	return m_jobfunctor();
}

const std::string & FunctorRunQueueJob::getName() const
{
	return m_jobname;
}

int FunctorRunQueueJob::getPriority() const
{
	return m_priority;
}


FunctorRunQueueJob::~FunctorRunQueueJob()
{}

FunctorRunQueueJob::FunctorRunQueueJob(std::string jobname,
		int priority, jobfun_t functor)
 : m_jobname(jobname), m_priority(priority), m_jobfunctor(functor)
{}

class RunQueueControl : public IRunQueueControl {
	
private:
	struct threadObj {
		RunQueueControl * me;
		void operator()()
		{
			(*me)();
		}
	} m_mythreadfunctor;
	
	std::priority_queue <sp_RunQueueJob> m_jobs;
	sp_RunQueueJob m_currentTask;
	bool m_done;
	bool m_taskRunning;
public:
	
	boost::condition m_runQueueWaitCondition;
	boost::mutex m_jobFifoMutex;
	boost::thread * m_controlthread;
	
	RunQueueControl();
	~RunQueueControl();
	void startThread();
	void operator()();
	virtual void run();
	virtual void submit(sp_RunQueueJob job);
	virtual void stop();
	
	virtual const std::string &currentTaskName();

	virtual bool taskInProgress();
	
	virtual bool isRunning();
	
	virtual bool isEmpty();
};




RunQueueControl::RunQueueControl() {
	m_done = false;
	m_taskRunning = false;
}

RunQueueControl::~RunQueueControl() {
	{
		boost::mutex::scoped_lock lock(m_jobFifoMutex);
		m_done = true;
	}
	m_runQueueWaitCondition.notify_all();
}

void RunQueueControl::startThread()
{

	m_mythreadfunctor.me = this;
	m_controlthread = new boost::thread(m_mythreadfunctor);
}
void RunQueueControl::operator()()
{
	while (!m_done)
	{
		
		sp_RunQueueJob nextjob;
		{
			// Acquire a lock on the job fifo until the end of scope
			boost::mutex::scoped_lock lock(m_jobFifoMutex);
			
			// Fetch the next runqueue object
			if (!m_jobs.empty())
			{
				nextjob = m_jobs.top();
				m_jobs.pop();
			}
			
			// If we couldn't fetch a job and the queue should keep running....
			if (!nextjob && !m_done)
			{
				// Then we block on a semaphor so we wait until a new job is added
				//    rather than drop the thread or busy wait
				// Wait releases the lock, and reacquires after wait
				m_runQueueWaitCondition.wait(lock);	
			}
		}		
		
		// Try running the job
		if (nextjob)
		{
			m_currentTask = nextjob;
			m_taskRunning = true;
			try {
				nextjob->exec();
			} catch (Exception &e) {
				fprintf(stderr, "Thread error: %s\n",
						e.getMessage());
				e.printStackTrace();
			} catch (std::exception &e) {
				fprintf(stderr, "Thread error: %s\n", e.what());
			}
			
			m_taskRunning = false;
		}
	}
}

void RunQueueControl::run()
{
	
}

void RunQueueControl::submit(sp_RunQueueJob job)
{
	boost::mutex::scoped_lock lock(m_jobFifoMutex);
	
	m_jobs.push(job);
	m_runQueueWaitCondition.notify_all();
}

void RunQueueControl::stop()
{
	
}

const std::string &RunQueueControl::currentTaskName()
{
	static std::string empty = "";
	if (!m_taskRunning)
		return empty;
	return m_currentTask->getName();
}

bool RunQueueControl::taskInProgress()
{
	return m_taskRunning;
}

bool RunQueueControl::isRunning()
{
	return m_taskRunning;
}

bool RunQueueControl::isEmpty()
{
	return m_jobs.empty();
}


IRunQueueControl * createRunQueueControl()
{
	RunQueueControl * rq = new RunQueueControl();
	rq->startThread();
	
	return rq;
}
