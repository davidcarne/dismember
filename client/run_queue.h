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

#ifndef _run_queue_H_
#define _run_queue_H_

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include <string>

class IRunQueueJob;
typedef boost::shared_ptr<IRunQueueJob> sp_RunQueueJob;

/**
 * Abstract interface to a run queue job
 */
class IRunQueueJob {
public:
	/**
	 * Execute the job
	 */
	virtual bool exec() = 0;

	/**
	 * Get the priority of the queue job
	 */
	virtual int getPriority() const = 0;
	
	/**
	 * Get the name of the queue job
	 */
	virtual const std::string & getName() const = 0;
	
	virtual ~IRunQueueJob() = 0;
};

/**
 * Run queue job specialization for executing a functor
 */
class FunctorRunQueueJob : public IRunQueueJob {
public:
	/**
	 * Job functor type
	 */
	typedef boost::function<bool()> jobfun_t;

	virtual bool exec();
	virtual  int getPriority() const;
	virtual const std::string & getName() const;
	virtual ~FunctorRunQueueJob();
	
private:
	std::string m_jobname;
	int m_priority;
	jobfun_t m_jobfunctor;
	
	FunctorRunQueueJob(std::string jobname, int priority, jobfun_t functor);
	friend sp_RunQueueJob createFunctorRunQueueJob(std::string jobname,
			int priority, FunctorRunQueueJob::jobfun_t job);
};


sp_RunQueueJob createFunctorRunQueueJob(std::string jobname,
		int priority, FunctorRunQueueJob::jobfun_t job);

/**
 * Interface to the run queue controller
 */
class IRunQueueControl {
public:
	/**
	 * Destroy the run queue controller
	 */
	virtual ~IRunQueueControl(){};
	
	/**
	 * Cause the run queue to begin running jobs in its run thread
	 */
	virtual void run() = 0;
	
	/**
	 * Add a new job to the bottom of the queue
	 */
	virtual void submit(sp_RunQueueJob job) = 0;
	
	/**
	 * Cause the queue to stop processing jobs as soon as it finishes the current job
	 */
	virtual void stop() = 0;
	
	/**
	 * boolean indicating if the queue is currently processing a job
	 */
	virtual bool taskInProgress() = 0;

	/**
	 * name of current task, or "" if none
	 */
	virtual const std::string &currentTaskName() = 0;
	
	/**
	 * boolean indicating if the queue is currently executing jobs, or if it will execute a job immediately on submission
	 */
	virtual bool isRunning() = 0;
	
	/**
	 * boolean indicating if the queue is empty
	 */
	virtual bool isEmpty() = 0;
};

IRunQueueControl * createRunQueueControl();

#endif


