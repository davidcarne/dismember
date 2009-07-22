#include "dsmem_trace.h"
#include "callback.h"

void ProjectModel::registerMemlocHook(CallbackBase<MemlocData *> *cb)
{ memloc_hooks.push_back(cb); }
void ProjectModel::registerXrefHook(CallbackBase<Xref *> *cb)
{ xref_hooks.push_back(cb); }
void ProjectModel::registerSymbolHook(CallbackBase<Symbol *> *cb)
{ symbol_hooks.push_back(cb); }

void ProjectModel::unregisterMemlocHook(CallbackBase<MemlocData *> *cb)
{ memloc_hooks.remove(cb); }
void ProjectModel::unregisterXrefHook(CallbackBase<Xref *> *cb)
{ xref_hooks.remove(cb); }
void ProjectModel::unregisterSymbolHook(CallbackBase<Symbol *> *cb)
{ symbol_hooks.remove(cb); }


#define NOTIFY_MACRO(a,b,c) \
  a##_hook_list::iterator i = a##_hooks.begin(); \
  for (; i != a##_hooks.end(); ++i) \
    (*(i))->callback(b, c);

void ProjectModel::notifyMemlocChange(MemlocData *data, HookChange ch)
{ NOTIFY_MACRO(memloc, data, ch) }
void ProjectModel::notifyXrefChange(Xref *data, HookChange ch)
{ NOTIFY_MACRO(xref, data, ch) }
void ProjectModel::notifySymbolChange(Symbol *data, HookChange ch)
{ NOTIFY_MACRO(symbol, data, ch) }
