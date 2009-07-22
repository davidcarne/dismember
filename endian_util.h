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


