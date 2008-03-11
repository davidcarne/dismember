/*
 *  xref.h
 *  dismember
 *
 *  Created by David Carne on 13/02/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _xref_H_
#define _xref_H_

#include "types.h"

class MemlocData;
class Trace;

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
	Xref(Trace * lookup, address_t srcaddr, address_t dstaddr, xref_type_e type);

	
private:
	friend class XrefManager;
	
	address_t m_srcaddr;
	address_t m_dstaddr;
	
	xref_type_e m_type;
	
	Trace * m_lookuptrace;
};

#endif


