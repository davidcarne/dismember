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

#include "dsmem_trace.h"
#include "callback.h"

// TODO: why is this in this file? Move after big changes committed so git can track move
void MemoryBackedProjectModel::registerMemlocHook(CallbackBase<MemlocData *> *cb)
{ memloc_hooks.push_back(cb); }
void MemoryBackedProjectModel::registerXrefHook(CallbackBase<Xref *> *cb)
{ xref_hooks.push_back(cb); }
void MemoryBackedProjectModel::registerSymbolHook(CallbackBase<Symbol *> *cb)
{ symbol_hooks.push_back(cb); }

void MemoryBackedProjectModel::unregisterMemlocHook(CallbackBase<MemlocData *> *cb)
{ memloc_hooks.remove(cb); }
void MemoryBackedProjectModel::unregisterXrefHook(CallbackBase<Xref *> *cb)
{ xref_hooks.remove(cb); }
void MemoryBackedProjectModel::unregisterSymbolHook(CallbackBase<Symbol *> *cb)
{ symbol_hooks.remove(cb); }


#define NOTIFY_MACRO(a,b,c) \
  a##_hook_list::iterator i = a##_hooks.begin(); \
  for (; i != a##_hooks.end(); ++i) \
    (*(i))->callback(b, c);

void MemoryBackedProjectModel::notifyMemlocChange(MemlocData *data, HookChange ch)
{ NOTIFY_MACRO(memloc, data, ch) }
void MemoryBackedProjectModel::notifyXrefChange(Xref *data, HookChange ch)
{ NOTIFY_MACRO(xref, data, ch) }
void MemoryBackedProjectModel::notifySymbolChange(Symbol *data, HookChange ch)
{ NOTIFY_MACRO(symbol, data, ch) }
