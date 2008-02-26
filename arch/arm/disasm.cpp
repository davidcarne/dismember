#include "../../trace.h"
#include "../../memlocdata.h"

#include "arm_instruction.h"

#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct arm_insn 
{
    u32 mask;
    u32 value;
    const char *fmt;
} arm_insns[] = {
    // The % escapes are as such:
    // %a - LDR addressing mode
    // %A - LDRH addressing mode (doesn't have a barrel shifter)
    // %b - "b" if bit 22 is set, "" if not
    // %B - "h" if bits 6:5 are 01, "sb" if they're 10, "sh" if they're 11, "?" if they're 00
    // %c - the condition code (eq, cs, etc.)
    // %i - a 24-bit hex value in the bottom 24 bits of the instruction (for swi)
    // %j - a 24-bit signed instruction count for branches
    // %m - a multiple-reg argument in bits 15:0
    // %M - a multiple-reg indexing mode
    // %o - the shift specification from bits 0 to 11, with the I bit at bit 25
    // %<nn>r - the register from bits <nn> to <nn>+3. <nn> is 0 if not specified.
    // %s - "s" if bit 20 is set, "" if not
    // %u - "s" if bit 22 is set, "u" if not
    // %! - "!" if bit 21 is set, "" if not
    // %^ - "^" if bit 22 is set, "" if not

    /* BX */
    { 0x0ffffff0, 0x012fff10, "bx%c\t%r" },

    /* SWI */
    { 0x0f000000, 0x0f000000, "swi%c\t%i" },

    /* B, BL */
    { 0x0f000000, 0x0a000000, "b%c\t%j" },
    { 0x0f000000, 0x0b000000, "bl%c\t%j" },

    /* MUL, MLA */
    { 0x0fe000f0, 0x00000090, "mul%c%s\t%16r, %r, %8r" },
    { 0x0fe000f0, 0x00200090, "mla%c%s\t%16r, %r, %8r, %12r" },

    /* UMULL, UMLAL, SMULL, SMLAL */
    { 0x0fa000f0, 0x00800090, "%umull%c%s\t%12r, %16r, %r, %8r" },
    { 0x0fa000f0, 0x00a00090, "%umlal%c%s\t%12r, %16r, %r, %8r" },

    /* MRS, MSR */
    { 0x0fff0fff, 0x010f0000, "mrs%c\t%12r, cpsr" },
    { 0x0fff0fff, 0x014f0000, "mrs%c\t%12r, spsr" },
    { 0x0db1f000, 0x0121f000, "msr%c\tcpsr_all, %r" },
    { 0x0db1f000, 0x0161f000, "msr%c\tspsr_all, %r" },
    { 0x0db1f000, 0x0120f000, "msr%c\tcpsr_flg, %o" },
    { 0x0db1f000, 0x0160f000, "msr%c\tspsr_flg, %o" },

    /* LDR, STR, LDRB, STRB */
    { 0x0c100000, 0x04100000, "ldr%c%b\t%12r, %a" },
    { 0x0c100000, 0x04000000, "str%c%b\t%12r, %a" },

    /* SWP */
    { 0x0fb00ff0, 0x01000090, "swp%c%b\t%12r, %r, [%16r]" },

    /* Halfword and SXB loads/stores */
    { 0x0e100090, 0x00100090, "ldr%c%B\t%12r, %A" },
    { 0x0e100090, 0x00000090, "str%c%B\t%12r, %A" },

    /* Multiple loads/stores */
    { 0x0e100000, 0x08100000, "ldm%c%M\t%16r%!, %m%^" },
    { 0x0e100000, 0x08000000, "stm%c%M\t%16r%!, %m%^" },

    /* ALU ops */
    { 0x0de00000, 0x00000000, "and%c%s\t%12r, %16r, %o" },
    { 0x0de00000, 0x00200000, "eor%c%s\t%12r, %16r, %o" },
    { 0x0de00000, 0x00400000, "sub%c%s\t%12r, %16r, %o" },
    { 0x0de00000, 0x00600000, "rsb%c%s\t%12r, %16r, %o" },
    { 0x0de00000, 0x00800000, "add%c%s\t%12r, %16r, %o" },
    { 0x0de00000, 0x00a00000, "adc%c%s\t%12r, %16r, %o" },
    { 0x0de00000, 0x00c00000, "sbc%c%s\t%12r, %16r, %o" },
    { 0x0de00000, 0x00e00000, "rsc%c%s\t%12r, %16r, %o" },
    { 0x0de00000, 0x01000000, "tst%c\t%16r, %o" },
    { 0x0de00000, 0x01200000, "teq%c\t%16r, %o" },
    { 0x0de00000, 0x01400000, "cmp%c\t%16r, %o" },
    { 0x0de00000, 0x01600000, "cmn%c\t%16r, %o" },
    { 0x0de00000, 0x01800000, "orr%c%s\t%12r, %16r, %o" },
    { 0x0de00000, 0x01a00000, "mov%c%s\t%12r, %o" },
    { 0x0de00000, 0x01c00000, "bic%c%s\t%12r, %16r, %o" },
    { 0x0de00000, 0x01e00000, "mvn%c%s\t%12r, %o" },

    { 0, 0, "invalid" }
};


#define T_DPI_F1(op_1, name) { 0xFE00, 0x1800 | (op_1 << 9), name "\t%0r, %3r, %6r" } 
#define T_DPI_F2(op_2, name) { 0xFE00, 0x1C00 | (op_2 << 9), name "\t%0r, %3r, %6i" } 
#define T_DPI_F3(op_3, name) { 0xF800, 0x2000 | (op_3 << 11), name "\t%8r, %I" } 
#define T_DPI_F4(op_4, name) { 0xF800, 0x0000 | (op_5 << 11), name "\t%0r, %3r, %o" } 
#define T_DPI_F5(op_5, name) { 0xFFC0, 0x4000 | (op_5 << 6), name "\t%0r, %3r" }
#define T_DPI_F6(reg, name, dest)  { 0xF800, 0xA000 | (reg << 11), name "\t%8r, " dest ", %I" }
#define T_DPI_F7(op_6, name) { 0xFF80, 0xB000 | (op_6 << 7), name "\tSP, SP, %j" } 
#define T_DPI_F8(op_7, name) { 0xFF00, 0x4400 | (op_7 << 8), name "\t%q" } 
struct thumb_insn 
{
    uint16_t mask;
    uint16_t value;
    const char *fmt;
} thumb_insns[] = {
    // The % escapes are as such:
    // %<nn>r - the register from bits <nn> to <nn>+2. <nn> is 0 if not specified.
    // %<nn>r - the register from bits <nn> to <nn>+2. <nn> is 0 if not specified.
	// %o immediate shift specification @ bit 6-10 
	// %<nn>i immediate_3 @ bit <nn>
	// %<nn>I immediate_8 @ bit <nn>
	// %<nn>j immediate_7 @ bit <nn>
	// %q large op funny decoding mode
	
		// %a - LDR addressing mode
		// %A - LDRH addressing mode (doesn't have a barrel shifter)
		// %b - "b" if bit 22 is set, "" if not
		// %B - "h" if bits 6:5 are 01, "sb" if they're 10, "sh" if they're 11, "?" if they're 00
		// %c - the condition code (eq, cs, etc.)
		// %i - a 24-bit hex value in the bottom 24 bits of the instruction (for swi)
		// %j - a 24-bit signed instruction count for branches
		// %m - a multiple-reg argument in bits 15:0
		// %M - a multiple-reg indexing mode
		// %o - the shift specification from bits 0 to 11, with the I bit at bit 25
	
    // %s - "s" if bit 20 is set, "" if not
    // %u - "s" if bit 22 is set, "u" if not
    // %! - "!" if bit 21 is set, "" if not
    // %^ - "^" if bit 22 is set, "" if not
	{ 0xFFC0, 0x4140, "adc\t%r, %3r"},
	
	{ 0xFE00, 0x1C00, "add\t%r, %3r, %6i"},

    { 0, 0, "invalid" }
};

static void write_reg (char **retp, int reg) 
{
    const char *regs[] = { "r0", "r1", "r2", "r3",
                           "r4", "r5", "r6", "r7",
                           "r8", "r9", "r10", "r11",
                           "r12", "sp", "lr", "pc" };

    *retp += sprintf (*retp, regs[reg]);
}

static void write_multindex (char **retp, int Pre, int Up) 
{
    const char *indexmodes[] = { "da", "ia", "db", "ib" };
    *retp += sprintf (*retp, indexmodes[Pre * 2 + Up]);
}

static void write_barrelshift (char **retp, int bsh) 
{
    char *p = *retp;

    write_reg (&p, bsh & 0xf);

    if (bsh >> 4) {
        const char *shifts[] = { "lsl", "lsr", "asr", "ror" };

        *p++ = ',';
        *p++ = ' ';
        
        if (bsh & (1 << 4)) { // Reg shift
            p += sprintf (p, shifts[(bsh >> 5) & 3]);
            *p++ = ' ';
            write_reg (&p, (bsh >> 8) & 0xf);
        } else {
            int shift = (bsh >> 7) & 0x1f;
            int shtype = (bsh >> 5) & 3;
            if (!shift && (shtype == 3)) {
                p += sprintf (p, "rrx");
            } else {
                p += sprintf (p, "%s #%d", shifts[shtype], shift);
            }
        }
    }

    *retp = p;
}

static bool ldw(const Trace * t, address_t addr, u32 * data)
{
	u8 dataar[4];
	if (!t->readBytes(addr, 4, dataar))
		return false;
	*data = dataar[3];
	*data <<= 8;
	*data |= dataar[2];
	*data <<= 8;
	*data |= dataar[1];
	*data <<= 8;
	*data |= dataar[0];
	return true;
}


static void write_addr (const Trace *t, char **retp, int Pre, int Up, int Writeback, int Immed, int offset, int base, u32 pc)
{
    char *p = *retp;
    if (base == 15 && !Writeback && Immed) {
		u32 destword;
		if (ldw(t, pc + offset, &destword))
			*retp += sprintf (*retp, "=0x%08x", destword);
        return;
    }
    *p++ = '[';
    write_reg (&p, base);
    if (Immed && offset) {
        p += sprintf (p, "%s, #%s%d%s%s\t; 0x%x", Pre? "" : "]", Up? "" : "-",
                      offset, Pre? "]" : "", (Writeback && Pre)? "!" : "",
                      (base == 15)? pc + offset : offset);
    } else if (Immed) {
        p += sprintf (p, "]%s", Writeback? "!" : "");
    } else {
        p += sprintf (p, "%s, %s", Pre? "" : "]", Up? "" : "-");
        write_barrelshift (&p, offset);
        p += sprintf (p, "%s", Pre? Writeback? "]!" : "]" : "");
    }

    *retp = p;
}

static void write_cond (char **retp, int cond) 
{
    const char *conds[] = { "eq", "ne", "cs", "cc", "mi", "pl", "vs", "vc",
                            "hi", "ls", "ge", "lt", "gt", "le", "", "nv?" };
    *retp += sprintf (*retp, cond >= 16? "!" : conds[cond]);
}

static void write_multireg (char **retp, int reglist)
{
    char *p = *retp;
    int reg;
    int needscomma = 0;

    *p++ = '{';
    for (reg = 0; reg <= 15; reg++) {
        if (reglist & (1 << reg)) {
            int ereg = reg;
            for (; (ereg <= 15) && (reglist & (1 << ereg)); ereg++)
                ;
            ereg--;
            
            if (needscomma) {
                *p++ = ',';
                *p++ = ' ';
            }

            write_reg (&p, reg);
            if (ereg != reg) {
                *p++ = '-';
                write_reg (&p, ereg);
            }

            reg = ereg;
            needscomma = 1;
        }
    }
    *p++ = '}';
    
    *retp = p;
}

static char * comment_resolve(u32 a, u32 b)
{
	return NULL;
}
static void write_jump (const Trace * m_ctx, char **retp, u32 jumpfield, u32 pc) 
{
    u32 jumpbytes = jumpfield << 2;
    if (jumpbytes & (1 << 25))
        jumpbytes |= 0xfc000000;
    s32 jump = (s32)jumpbytes;

    address_t mj = pc + jump;
	
	const Symbol * s = m_ctx->lookup_symbol(mj);
	
    if (s)
        *retp += sprintf(*retp, "%s", s->get_name().c_str());
    else
        *retp += sprintf (*retp, "%08x", pc + jump);
}

const std::string ARMInstruction::get_textual()
{
	u32 instr = m_opcode;
	u32 addr = get_addr();
	
    struct arm_insn *insn;
    
	char ret[128];
	
    char *retp = ret;
    // The last insn matches all, which provides an exit from the loop.
    for (insn = arm_insns ;; insn++) {
        if ((instr & insn->mask) == (insn->value & insn->mask)) {
            char *p = (char *)insn->fmt;
            while (*p) {
                if (*p != '%') {
                    *retp++ = *p;
                } else {
                    int arg = 0;
                cont:
                    p++;
                    switch (*p) {
                    case '0' ... '9':
                        arg *= 10;
                        arg += *p - '0';
                        goto cont;
                    case 'a':
                        // write_addr (slixt, pptr, Pre, Up, Writeback, Immbit, Barrelshift);
                        write_addr (get_ctx(), &retp, !!(instr & (1 << 24)), !!(instr & (1 << 23)),
                                    !!(instr & (1 << 21)), !(instr & (1 << 25)),
                                    instr & 0xfff, (instr >> 16) & 0xf, addr + 8);
                        break;
                    case 'A':
                        if (instr & (1 << 22))
                            write_addr (get_ctx(), &retp, !!(instr & (1 << 24)), !!(instr & (1 << 23)),
                                        !!(instr & (1 << 21)), 1,
                                        ((instr >> 4) & 0xf0) | (instr & 0xf), (instr >> 16) & 0xf,
                                        addr + 8);
                        else
                            write_addr (get_ctx(), &retp, !!(instr & (1 << 24)), !!(instr & (1 << 23)),
                                        !!(instr & (1 << 21)), 0, instr & 0xf, (instr >> 16) & 0xf,
                                        addr + 8);
                        break;
                    case 'b':
                        if (instr & (1 << 22))
                            *retp++ = 'b';
                        break;
                    case 'B':
                        switch ((instr >> 5) & 3) {
                        case 0:
                            *retp++ = '?';
                            break;
                        case 1:
                            *retp++ = 'h';
                            break;
                        case 2:
                            *retp++ = 's';
                            *retp++ = 'b';
                            break;
                        case 3:
                            *retp++ = 's';
                            *retp++ = 'b';
                            break;
                        }
                        break;
                    case 'c':
                        write_cond (&retp, (instr >> 28) & 0xf);
                        break;
                    case 'i':
                        retp += sprintf (retp, "0x%06x", instr & 0xffffff);
                        break;
                    case 'j':
                        write_jump (get_ctx(), &retp, instr & 0xffffff, addr + 8);
                        break;
                    case 'm':
                        write_multireg (&retp, instr & 0xffff);
                        break;
                    case 'M':
                        write_multindex (&retp, !!(instr & (1 << 24)), !!(instr & (1 << 23)));
                        break;
                    case 'o':
                        if (instr & (1 << 25)) {
                            u32 nsimm = instr & 0xff;
                            int rot = (instr >> 7) & 0x1e;
                            u32 imm = (nsimm >> rot) | (nsimm << (32 - rot));
                            int simm = (s32)imm;
                            retp += sprintf (retp, "#%d\t; 0x%x", simm, imm);
                        } else {
                            write_barrelshift (&retp, instr & 0xfff);
                        }
                        break;
                    case 'r':
                        write_reg (&retp, (instr >> arg) & 0xf);
                        break;
                    case 's':
                        if (instr & (1 << 20))
                            *retp++ = 's';
                        break;
                    case 'u':
                        if (instr & (1 << 22))
                            *retp++ = 's';
                        else
                            *retp++ = 'u';
                        break;
                    case '!':
                        if (instr & (1 << 21))
                            *retp++ = '!';
                        break;
                    case '^':
                        if (instr & (1 << 22))
                            *retp++ = '^';
                        break;
                    case '%':
                        *retp++ = '%';
                        break;
                    default:
                        break;
                    }
                }
                p++;
            }
            break;
        }
    }

    {
        const char *comment;
        if ((comment = comment_resolve(addr, 0))) {
            if (strchr(ret, ';'))
                retp = strchr(ret, ';');
            else *retp++ = ' ';
            retp += sprintf(retp, "; %s", comment);
        }
    }
    
    *retp = 0;
	std::string ret_s(ret);
    return ret_s;
}

