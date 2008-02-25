/*
 *  wxguiutils.h
 *  dismember
 *
 *  Created by David Carne on 14/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _wxguiutils_H_
#define _wxguiutils_H_

#if defined(wxUSE_UNICODE) && !defined(_U)
#define _U(x) wxString((x),wxConvUTF8)
#elif !defined(_U)
#define _U(x) (x)
#endif


#endif


