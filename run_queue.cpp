/*
 *  run_queue.cpp
 *  dismember
 *
 *  Created by David Carne on 24/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "run_queue.h"
#include <boost/thread.hpp>
#include <queue>

sp_RunQueueJob createFunctorRunQueueJob(std::string jobname, FunctorRunQueueJob::jobfun_t job)
{
	return sp_RunQueueJob(new FunctorRunQueueJob(jobname, job));
}

bool FunctorRunQueueJob::exec()
{
	return m_jobfunctor();
}

const std::string & FunctorRunQueueJob::getName()
{
	return m_jobname;
}


FunctorRunQueueJob::FunctorRunQueueJob(std::string jobname, jobfun_t functor) : m_jobname(jobname), m_jobfunctor(functor)
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
	
	std::queue <sp_RunQueueJob> m_jobs;
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
			boost::mutex::scoped_lock lock(m_jobFifoMutex);
			
			// Fetch the next runqueue object
			if (!m_jobs.empty())
			{
				nextjob = m_jobs.front();
				m_jobs.pop();
			}
			if (!nextjob && !m_done)
			{
				m_runQueueWaitCondition.wait(lock);	
			}
		}		
		
		// Try running the job
		if (nextjob)
		{
			m_taskRunning = true;
			nextjob->exec();	
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

bool RunQueueControl::taskInProgress()
{
	return m_taskRunning;
}

bool RunQueueControl::isRunning()
{
	
}

bool RunQueueControl::isEmpty()
{
	
}


IRunQueueControl * createRunQueueControl()
{
	RunQueueControl * rq = new RunQueueControl();
	rq->startThread();
	
	return rq;
}
