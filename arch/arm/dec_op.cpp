/*
 * This file handles all opcode recognition.
 * Most of this code, including opcode parsing is taking from armemu
 * Credit goes to Joshua Oreman, David Carne, and Courtney Cavin
 *
 * The main function in this file, get_op, takes a 32 bit
 * long word and a pointer to a structure, and parses the ocpode information.
 *
 */

#include "dsmem_trace.h"
#include "memlocdata.h"

#include "arm_instruction.h"

#include <string.h>


void ARMInstruction::decode_data_refs()
{
	if ((m_opcode & 0x0c000000) == 0x04000000) {
		int regN = (m_opcode >> 16) & 0xf;
		//int regD = (m_opcode >> 12) & 0xf;
		u32 offset = 0;
		int IPUBWL = (m_opcode >> 20) & 0x3f;
		
		if (regN != 15)
			return;

		if (IPUBWL & (1 << 5)) { // I -> not an immediate
			return;
		} else {
			offset = m_opcode & 0xfff;
		}

		
		address_t base = get_addr() + 8;
		
		address_t taddr;
		
		if (IPUBWL & (1 << 4)) { // P
			if (IPUBWL & (1 << 3)) // U
				taddr = base + offset;
			else
				taddr = base - offset;
		} else {
			taddr = base;
		}
		
		if (IPUBWL & (1 << 0)) { // ldr
			m_pcflags |= PCFLAG_DREF;

			if (IPUBWL & (1 << 2)) // ldrb
				m_pcflags |= PCFLAG_DSBYTE;
			else
				m_pcflags |= PCFLAG_DSWORD;
			
			m_data_ref_addr = taddr;
		} 
	}
}

void ARMInstruction::decode_pcflow()
{
	const char *instrname = 0;

	u32 pcflags = 0;
	address_t ddest;
	
	u32 instr = m_opcode;
	address_t addr = get_addr();
	
	/* By default, each instruction carries onto the next. Only certain don't */
	pcflags |= PCFLAG_CONTINUE;
	
	// BX
	if ((instr & 0x0ffffff0) == 0x012fff10) {
		
		// We don't continue
		pcflags &= ~PCFLAG_CONTINUE;
		
		// Jump location is indirectly determined
		pcflags |= PCFLAG_INDLOC;
		
		instrname = "bx";
	}

	// SWI
	else if ((instr & 0x0f000000) == 0x0f000000) {
		// Jump location is indirectly determined
		pcflags |= PCFLAG_INDLOC;
		
		instrname = "swi";
	}

	// B, BL
	else if ((instr & 0x0e000000) == 0x0a000000) {
		// We don't necessarily continue
		pcflags &= ~PCFLAG_CONTINUE;
		pcflags |= PCFLAG_DIRLOC;
		
		if (instr & (1 << 24)) // L
		{
			// since its a link, we do continue + mark it as a link
			pcflags |= PCFLAG_CONTINUE | PCFLAG_DIRLK;
		}
		
		
		s32 off = (instr & 0x00ffffff) << 2;
		if (off & (1 << 25)) off |= 0xfc000000; // sign-extend
		
		ddest = addr + off + 8;
		
		instrname = "b";
	}

	// MSR, MRS
	else if (((instr & 0x0fbf0fff) == 0x010f0000) || // MRS
		 ((instr & 0x0fb0fff0) == 0x0120f000) || // MSR_reg
		 ((instr & 0x0fb0f000) == 0x0320f000)) { // MSR_immed
		if ((instr & 0x0fbf0fff) == 0x010f0000) { // MRS
			
		} 
		else {

		}
		instrname = "mrs";
	}

	// MUL, MLA
	else if ((instr & 0x0fc000f0) == 0x00000090) {
		int regD   = (instr >> 16) & 0xf;
		//int regN   = (instr >> 12) & 0xf;
		//int regS   = (instr >>  8) & 0xf;
		//int regM   = (instr >>  0) & 0xf;

		if (regD == 15)
		{
			// We don't continue
			pcflags &= ~PCFLAG_CONTINUE;
			
			// Jump location is indirectly determined
			pcflags |= PCFLAG_INDLOC;
		}
		
		instrname = "mul";
	}

	// UMULL, UMLAL, SMULL, SMLAL
	else if ((instr & 0x0f8000f0) == 0x00800090) {
		int regDhi = (instr >> 16) & 0xf;
		int regDlo = (instr >> 12) & 0xf;
		//int regS   = (instr >>  8) & 0xf;
		//int regM   = (instr >>  0) & 0xf;

		if (regDhi == 15 || regDlo == 15)
		{
			// We don't continue
			pcflags &= ~PCFLAG_CONTINUE;
			
			// Jump location is indirectly determined
			pcflags |= PCFLAG_INDLOC;
		}
		
		instrname = "umull";
	}

	// LDR, STR, LDRB, STRB
	else if ((instr & 0x0c000000) == 0x04000000) {
		//int regN = (instr >> 16) & 0xf;
		int regD = (instr >> 12) & 0xf;
		//u32 offset = 0;
		int IPUBWL = (instr >> 20) & 0x3f;


		if (IPUBWL & (1 << 0)) { // ldr
			if (regD == 15)
			{
				// We don't continue
				pcflags &= ~PCFLAG_CONTINUE;
				
				// Jump location is indirectly determined
				pcflags |= PCFLAG_INDLOC;
			}
			
			instrname = "ldr";
		} 
		else { // str
			instrname = "str";
		}
	}

	// MCR, MRC
	else if (((instr & 0x0f000010) == 0x0e000010)) {
		int regD = (instr >> 12) & 0xf;

		if ((instr >> 20) & 1) { // MRC
			if (regD == 15) {
				// We don't continue
				pcflags &= ~PCFLAG_CONTINUE;
				
				// Jump location is indirectly determined
				pcflags |= PCFLAG_INDLOC;
			}
			instrname = "mrc";
		} else { // MCR
			instrname = "mcr";
		}
	}

	// SWP
	else if ((instr & 0x0fb00ff0) == 0x01000090) {
		//int regN   = (instr >> 16) & 0xf;
		int regD   = (instr >> 12) & 0xf;
		//int regM   =  instr	& 0xf;
		//int byte   =  instr	& (1 << 22);
		
		if (regD == 15)
		{
			// We don't continue
			pcflags &= ~PCFLAG_CONTINUE;
			
			// Jump location is indirectly determined
			pcflags |= PCFLAG_INDLOC;
		}

		instrname = "swp";
	}

	// LDRH, STRH, LDRSB, LDRSH
	else if ((instr & 0x0e000090) == 0x00000090) {
		//int regN = (instr >> 16) & 0xf;
		int regD = (instr >> 12) & 0xf;
		//u32 offset = 0;
		int PUIWL = (instr >> 20) & 0x1f;
		int SH = (instr >> 5) & 3;

		switch (SH) {
		case 3: // LDRSH
		case 1: // <LD|ST>RH
			if (PUIWL & (1 << 0)) { // ldrh
					
				if (regD == 15)
				{
					// We don't continue
					pcflags &= ~PCFLAG_CONTINUE;
						
					// Jump location is indirectly determined
					pcflags |= PCFLAG_INDLOC;
				}
			} 
			break;

		case 2: // LDRSB
			if (regD == 15)
			{
				// We don't continue
				pcflags &= ~PCFLAG_CONTINUE;
				
				// Jump location is indirectly determined
				pcflags |= PCFLAG_INDLOC;
			}
			break;
		}


		instrname = "ldrh";
	}

	// LDM, STM
	else if ((instr & 0x0e000000) == 0x08000000) {
		//int regN   = (instr >> 16) & 0xf;
		int PUSWL  = (instr >> 20) & 0x1f;
		int reglist = instr & 0xffff;
		int r_inc;

		for (r_inc = 0; r_inc < 16; r_inc++) {
			if (reglist & (1 << r_inc)) {
				if (PUSWL & 1) { // load
					if (r_inc == 15)
					{
						// We don't continue
						pcflags &= ~PCFLAG_CONTINUE;
						
						// Jump location is indirectly determined
						pcflags |= PCFLAG_INDLOC;
					}
				}
			}
		}

		instrname = (PUSWL & 1) ? "ldm" : "stm";
	}

	// ALU instrs
	else if ((instr & 0x0c000000) == 0) {
		int opcode = (instr >> 21) & 0xf;
		//int regA   = (instr >> 16) & 0xf;
		int regD   = (instr >> 12) & 0xf;

		int writeres = 1;
		int req_regN = 1;
		u32 result;
		u32 opB;

		if (instr & (1 << 25)) { // immediate
			opB = instr & 0xff;
			int ror = (instr & 0xf00) >> 7;
			opB = (opB >> ror) | (opB << (32 - ror));
		}

		// do the op
		switch (opcode) {
		case 8: /* TST */
			writeres = 0;
			instrname = "tst";
			break;
		case 0: /* AND */
			instrname = "and";
			break;
		case 9: /* TEQ */
			writeres = 0;
			instrname = "teq";
			break;
		case 1: /* EOR */
			instrname = "eor";
			break;
		case 0xC: /* ORR */
			instrname = "orr";
			break;
		case 0xA: /* CMP */
			writeres = 0;
			instrname = "cmp";
			break;
		case 6: /* SBC */
			instrname = "sbc";
		case 2: /* SUB */
			instrname = "sub";
			break;
		case 7: /* RSC */
			instrname = "rsc";
			break;
		case 3: /* RSB */
			instrname = "rsb";
			break;
		case 0xB: /* CMN */
			writeres = 0;
			instrname = "cmn";
			break;
		case 5: /* ADC */
			instrname = "adc";
			break;
		case 4: /* ADD */
			instrname = "add";
			break;
		case 0xD: /* MOV */
			req_regN = 0;
			result = opB;
			instrname = "mov";
			break;
		case 0xE: /* BIC */
			instrname = "bic";
			break;
		case 0xF: /* MVN */
			req_regN = 0;
			result = ~opB;
			instrname = "mvn";
			break;
		}

		// write result
		if (writeres && regD == 15) {
			// We don't continue
			pcflags &= ~PCFLAG_CONTINUE;
			if (!req_regN && (instr & (1 << 25))) { // immediate
				// Jump location is directly determined
				pcflags |= PCFLAG_DIRLOC;
				ddest = get_ctx()->locateAddress(result);
			} else {
				// Jump location is indirectly determined
				pcflags |= PCFLAG_INDLOC;
			}
		}
	}
	else {
		instrname = "undf";
		pcflags &= ~PCFLAG_CONTINUE;
		m_pcflags = pcflags;
		return;
	}

	
	if ((instr >> 28) != 0xE)
	{
		// If the instruction could be skipped, we could continue
		pcflags |= PCFLAG_CONTINUE;
	}
	
	m_pcflags = pcflags;
	m_ddest = ddest;
}
