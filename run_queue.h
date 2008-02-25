/*
 *  run_queue.h
 *  dismember
 *
 *  Created by David Carne on 24/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _run_queue_H_
#define _run_queue_H_

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include <string>

class RunQueueJob;
typedef boost::shared_ptr<RunQueueJob> sp_RunQueueJob;

class RunQueueJob{
public:
	typedef boost::function<bool()> jobfun_t;

	
	bool exec();
	const std::string & getName();
	
private:
	std::string m_jobname;
	jobfun_t m_jobfunctor;
	
	RunQueueJob(std::string jobname, jobfun_t functor);
	friend sp_RunQueueJob createRunQueueJob(std::string jobname, RunQueueJob::jobfun_t job);
};


sp_RunQueueJob createRunQueueJob(std::string jobname, RunQueueJob::jobfun_t job);

class IRunQueueControl {
public:
	virtual ~IRunQueueControl(){};
	virtual void run() = 0;
	virtual void submit(sp_RunQueueJob job) = 0;
	virtual void stop() = 0;
	virtual bool taskInProgress() = 0;
	virtual bool isRunning() = 0;
	virtual bool isEmpty() = 0;
};

IRunQueueControl * createRunQueueControl();

#endif


