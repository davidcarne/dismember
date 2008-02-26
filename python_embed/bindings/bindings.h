/*
 *  bindings.h
 *  dismember
 *
 *  Created by David Carne on 14/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _bindings_H_
#define _bindings_H_

void bind_document();
void bind_trace();
void bind_codeblocks();
void bind_datatype();
void bind_types();
void bind_arch();
void bind_analysis();
void bind_runqueue();

extern "C" void init_dismember_core();
extern "C" void init_dismember_datatype_builtins();
extern "C" void init_dismember_analysis_builtins();
#endif


