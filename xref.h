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

#ifndef _xref_H_
#define _xref_H_

#include "types.h"

class MemlocData;
class I_ProjectModel;

/**
 * \brief represents a reference, aka, a link between two objects.
 */
class Xref {
private:
	typedef MemlocData xr_mtype;
	
public:
	/**
	 * Xref type enum
	 */
	typedef enum {
		/**
		 * \brief Xref of type jump
		 */
		XR_TYPE_JMP,
		
		/**
		 * \brief Xref of type function call
		 */
		XR_TYPE_FNCALL,
		
		/**
		 * \brief Xref of type data reference. See source pcflags for more info
		 */
		XR_TYPE_DATA
	} xref_type_e;

	
	
	/**
	 * \brief get the source address of the xref
	 * @return source address of the xref
	 */
	address_t get_src_addr() const;
	
	/**
	 * \brief get the dest address of the xref
	 * @return dest address of the xref
	 */
	address_t get_dst_addr() const;
	
	/**
	 * \brief get the source memory location of the xref
	 * @return a pointer to the source memory location, or NULL if it has not been analyzed / is not present in memory
	 */
	const xr_mtype * get_src_inst() const;
	
	/**
	 * \brief get the dest memory location of the xref
	 * @return a pointer to the dest memory location, or NULL if it has not been analyzed / is not present in memory
	 */
	const xr_mtype * get_dst_inst() const;
	
	/**
	 * \brief get the dest memory location of the xref
	 * @return a pointer to the dest memory location, or NULL if it has not been analyzed / is not present in memory
	 */
	u32 get_type() const;
	
	/**
	 * \brief comparison operator for two xrefs
	 * @return true if both Xrefs are identical
	 */
	bool operator==(const Xref & x);
	
protected:
	/**
	 * \brief create a new xref - protected as only trace should call
	 * @param lookup trace context
	 * @param srcaddr source address of the xref
	 * @param dstaddr dest address of the xref
	 * @param type type of the xref
	 */
	Xref(I_ProjectModel * lookup, address_t srcaddr, address_t dstaddr, xref_type_e type);

	
private:
	friend class XrefManager;
	
	address_t m_srcaddr;
	address_t m_dstaddr;
	
	xref_type_e m_type;
	
	I_ProjectModel * m_lookuptrace;
};

#endif


