#ifndef _ARM_H_
#define _ARM_H_

#include "../../trace.h"

class ARMArchitecture : public Architecture {
public:
	virtual std::string get_short_name() const; 
	virtual Instruction * create_instruction(Trace * ctx, address_t addr);
	virtual endian_e getArchEndian(void);
	// also here we have the create from serializer factory method
};

#endif

