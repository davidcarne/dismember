/*
 *  endian_util.h
 *  dismember
 *
 *  Created by David Carne on 05/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _endian_util_H_
#define _endian_util_H_

#ifdef _BIG_ENDIAN
#define endian_swap32( V )           \
( (((V) & 0x000000ff )<<24) | \
(((V) & 0x0000ff00 )<<8) |  \
(((V) & 0x00ff0000 )>>8) |  \
(((V) & 0xff000000 )>>24) )
#define endian_swap16( V )   \
( (((V) & 0xff00) >> 8) | (((V) & 0x00ff) << 8)  )
#else
#define endian_swap32( V ) (V)
#define endian_swap16( V ) (V)
#endif


#endif


