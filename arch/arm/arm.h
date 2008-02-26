#ifndef _ARM_H_
#define _ARM_H_

#include "../../trace.h"
#include "../../architecture.h"

/**
 * Arm architecture, implements arm specific architecture.
 */
class ARMArchitecture : public Architecture {
public:
	virtual std::string get_short_name() const; 
	
	static void bind_type();
	virtual DataType * createDataType(Trace *) const;
	
	//virtual Instruction * create_instruction(Trace * ctx, address_t addr);
	//virtual endian_e getArchEndian(void);
	// also here we have the create from serializer factory method
};

#define  CPSR_T	0x20
#define  CPSR_F	0x40
#define  CPSR_I	0x80
#define  CPSR_flags	0xf0000000
#define  CPSR_V	0x10000000
#define  CPSR_C	0x20000000
#define  CPSR_Z 0x40000000
#define  CPSR_N	0x80000000
#define  CPSR_Vbits	28
#define  CPSR_Cbits	29
#define  CPSR_Zbits	30
#define  CPSR_Nbits	31
#define SPSR -1

#endif

