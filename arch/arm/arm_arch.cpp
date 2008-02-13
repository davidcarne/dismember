#include "arm.h"
#include "../../memlocdata.h"
#include "arm_instruction.h"


std::string ARMArchitecture::get_short_name() const
{
	return "ARM";
}

Instruction * ARMArchitecture::create_instruction(Trace * ctx, address_t addr)
{
	return new ARMInstruction(ctx, addr, ctx->ldw(addr));
}

endian_e ARMArchitecture::getArchEndian(void)
{
	// HACK HACK HACK - this should be queried on creation
	return ENDIAN_LITTLE;
}