#include "trace.h"
#include "callback.h"

void Trace::registerMemlocHook(CallbackBase<MemlocData *> *cb)
{ memloc_hooks.push_back(cb); }
void Trace::registerXrefHook(CallbackBase<Xref *> *cb)
{ xref_hooks.push_back(cb); }
void Trace::registerSymbolHook(CallbackBase<Symbol *> *cb)
{ symbol_hooks.push_back(cb); }

void Trace::unregisterMemlocHook(CallbackBase<MemlocData *> *cb)
{ memloc_hooks.remove(cb); }
void Trace::unregisterXrefHook(CallbackBase<Xref *> *cb)
{ xref_hooks.remove(cb); }
void Trace::unregisterSymbolHook(CallbackBase<Symbol *> *cb)
{ symbol_hooks.remove(cb); }


#define NOTIFY_MACRO(a,b,c) \
  a##_hook_list::iterator i = a##_hooks.begin(); \
  for (; i != a##_hooks.end(); ++i) \
    (*(i))->callback(b, c);

void Trace::notifyMemlocChange(MemlocData *data, HookChange ch)
{ NOTIFY_MACRO(memloc, data, ch) }
void Trace::notifyXrefChange(Xref *data, HookChange ch)
{ NOTIFY_MACRO(xref, data, ch) }
void Trace::notifySymbolChange(Symbol *data, HookChange ch)
{ NOTIFY_MACRO(symbol, data, ch) }
