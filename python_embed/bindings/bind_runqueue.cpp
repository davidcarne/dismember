/*
 *  bind_runqueue.cpp
 *  dismember
 *
 *  Created by David Carne on 25/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#include "run_queue.h"

#include <boost/python.hpp>
using namespace boost::python;

void bind_runqueue()
{
	class_<IRunQueueJob, sp_RunQueueJob, boost::noncopyable>("IRunQueueJob", no_init);
	class_<IRunQueueControl, boost::noncopyable>("IRunQueueControl", no_init)
	.def("submit", &IRunQueueControl::submit);
	//.add_property("name", &IRunQueueJob.getName);
}
