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

#ifndef _CALLBACK_H_
#define _CALLBACK_H_

typedef enum {
	HOOK_MODIFY,
	HOOK_DELETE,
	HOOK_CREATE
} HookChange;

template <class C>
class CallbackBase
{
 public:
 	CallbackBase() {}
 	virtual ~CallbackBase() {}
	virtual void callback(C, HookChange) = 0;
};


template <class B, class C>
class Callback : public CallbackBase<B>
{
 public:
	typedef void (C::*method_t)(B, HookChange);

	Callback(C *clas = 0, method_t met = 0)
	 : m_class(clas), m_callback(met) {}

	void setClass(C *clas) { m_class = clas; }
	void setCallback(method_t method) { m_callback = method; }

	void callback(B d, HookChange c)
	{
		if (m_class && m_callback)
			(m_class->*m_callback)(d, c);
	}

 private:
 	C* m_class;
	method_t m_callback;
};
#endif
