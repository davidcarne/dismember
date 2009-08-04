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


