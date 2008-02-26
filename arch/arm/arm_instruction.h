#ifndef _ARM_INSTRUCTION_H_
#define _ARM_INSTRUCTION_H_
#include "../../trace.h"

#include "../../instruction.h"

/**
 * \brief Arm specific subclass of Instruction memory type
 */
class ARMInstruction : public Instruction {
public:
	virtual ~ARMInstruction() {};
	/**
	 * Create a new arm instruction.
	 * \deprecated - should only be created through the appropriate datatype that doesn't exist yet
	 * \todo protect me
	 */
	ARMInstruction(const Trace * ctx, address_t address, u32 opcode);
	
	virtual u32			get_length() const;
	
	virtual bool is_executable() const;
	
	/* The slist requirement will go away later when this returns tokens */
	virtual const std::string get_textual();
	
	virtual void mark_fn_ent(bool fnent);
	
	// Uncommon / deprecated interfaces
	virtual address_t	get_direct_jump_addr() const;
	virtual u32	get_pcflags() const;
	virtual address_t	get_data_ref_addr() const;
protected:
		
private:
		u32 m_opcode;
	
	u32 m_pcflags;
	
	// For direct locations - addr of instruction
	// already PC-8 compensated
	address_t m_ddest;
	
	address_t m_data_ref_addr;
	
	void decode_pcflow();
	void decode_data_refs();
	
	u32	get_opcode() const;
};

#endif
