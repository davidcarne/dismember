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

#include "i_projectmodel.h"

#include "i_memlocdata.h"

#include "symlist.h"

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
  /* The % escapes are as such:
     * %a - LDR addressing mode
     * %A - LDRH addressing mode (doesn't have a barrel shifter)
     * %b - "b" if bit 22 is set, "" if not
     * %B - "h" if bits 6:5 are 01, "sb" if they're 10, "sh" if
     * 	    they're 11, "?" if they're 00
     * %c - the condition code (eq, cs, etc.)
     * %<nn>C - coprocessor register <nn> to <nn>+3
     * %d - "d" if  bit 22 is set, "" if not
     * %i - a 24-bit hex value in the bottom 24 bits of the instruction(for swi)
     * %j - a 24-bit signed instruction count for branches
     * %k - a 16-bit immediate value, the top 12 bits in 19:8 and the rest 3:0
     * %l - "l" if bit 22 is set, "" if not
     * %m - a multiple-reg argument in bits 15:0
     * %M - a multiple-reg indexing mode
     * %o - the shift specification from bits 0 to 11, with the I bit at bit 25
     * %p - coprocessor # from bits 8:12
     * %<nn>r - the register from bits <nn> to <nn>+3. <nn> is 0 if !specified.
     * %s - "s" if bit 20 is set, "" if not
     * %t - "2" if condition code is 0b1111 and ARM_v5, else same as %c
     * %u - "s" if bit 22 is set, "u" if not
     * %<nn>W - if bit <nn> set "w" and if bit 6 set "t" else "b"
     *     or if bit <nn> is 0 and if bits 6:5 00 "bb", if 01 "bt",
     *     if 10 "tb", if 11 "tt"
     * %<nn>x - opcode from <nn> to <nn>+4 & ~3. 0 if <nn> !specified
     * %<nn>X - same as %x if result is > 0, else ""
     * %! - "!" if bit 21 is set, "" if not
     * %^ - "^" if bit 22 is set, "" if not
     */

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

   /* LDR, STR, LDRB, STRB, LDRT, STRT, LDRBT, STRBT */
    { 0x0d700000, 0x04300000, "ldr%c%bt\t%12r, %a" },
    { 0x0d700000, 0x04200000, "str%c%bt\t%12r, %a" },
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
    { 0x0df00000, 0x01500000, "cmp%c\t%16r, %o" },
    { 0x0de00000, 0x01600000, "cmn%c\t%16r, %o" },
    { 0x0de00000, 0x01800000, "orr%c%s\t%12r, %16r, %o" },
    { 0x0fffffff, 0x01a00000, "nop%c" },
    { 0x0de00000, 0x01a00000, "mov%c%s\t%12r, %o" },
    { 0x0de00000, 0x01c00000, "bic%c%s\t%12r, %16r, %o" },
    { 0x0de00000, 0x01e00000, "mvn%c%s\t%12r, %o" },

    { 0xfff000f0, 0xe1200070, "bkpt\t%k" },
    { 0xfe000000, 0xfc000000, "blx\t%j" },
    { 0x0ff000f0, 0x01200030, "blx%c\t%r" },
    { 0x0ff000f0, 0x01600010, "clz%c\t%12r, %r" },

    /* LDRD, STRD */
    { 0x0e100ff0, 0x001000f0, "ldr%cd\t%12r, %A" },
    { 0x0e100ff0, 0x000000f0, "str%cd\t%12r, %A" },

    /* Co-processor functions */
    { 0x0f100010, 0x0e000010, "mcr%t\t%p, %21x, %12r, %16C, %C, %5x" },
    { 0x0f100010, 0x0e100010, "mrc%t\t%p, %21x, %12r, %16C, %C, %5x" },
    { 0x0e100000, 0x0c000000, "stc%t%l\t%p, %12C, %a" },
    { 0x0e100000, 0x0c100000, "ldc%t%l\t%p, %12C, %a" },
    { 0x0f000010, 0x0e000000, "cdp%t\t%p, %20x, %12C, %16C, %C, %5x" },
    { 0x0ff00000, 0x0c400000, "mcrr%c\t%p, %x, %12r, %16r, %C" },
    { 0x0ff00000, 0x0c500000, "mrrc%c\t%p, %x, %12r, %16r, %C" },

    /* PLD */
    { 0xfd70f000, 0xf550f000, "pld\t%a" },

    /* QDADD, QDSUB, QADD, QSUB */
    { 0x0fb000f0, 0x01000050, "q%dadd%c\t%12r, %r, %16r" },
    { 0x0fb000f0, 0x01200050, "q%dsub%c\t%12r, %r, %16r" },

    { 0x0fb00090, 0x01200080, "smul%22W%c\t%16r, %r, %8r" },
    /* SMLABB, SMLABT, SMLATB, SMLATT, + SMLAL and SMLAW varieties */
    { 0x0f900090, 0x01000080, "smla%l%21W%c\t%16r, %r, %8r, %12r" },

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
                           "r8", "r9", "r10", "fp",
                           "ip", "sp", "lr", "pc" };

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

static bool ldw(const I_ProjectModel * t, address_t addr, u32 * data)
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


static void write_addr (const I_ProjectModel *t, char **retp, int Pre, int Up, int Writeback, int Immed, int offset, int base, address_t pc)
{
	char *p = *retp;
	if (base == 15 && !Writeback && Immed) {
		u32 destword;
		if (ldw(t, pc + offset, &destword)) {
			*retp += sprintf (*retp, "=0x%08x", destword);
			return;
		}
	}
	*p++ = '[';
	write_reg (&p, base);
	if (Immed && offset) {
		address_t mj = pc + offset;
		p += sprintf (p, "%s, #%s0x%x%s%s",
				Pre? "" : "]",
				Up? "" : "-",
            			offset,
				Pre? "]" : "",
				(Writeback && Pre)? "!" : "");
		if (base == 15 && mj.isValid())
			p += sprintf (p, "\t; %s", mj.toString().c_str());
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

static char * comment_resolve(address_t a, u32 b)
{
	return NULL;
}

static void write_jump (const I_ProjectModel * m_ctx, char **retp, u32 jumpfield, address_t pc) 
{
    u32 jumpbytes = jumpfield << 2;
    if (jumpbytes & (1 << 25))
        jumpbytes |= 0xfc000000;
    s32 jump = (s32)jumpbytes;

    address_t mj = pc + jump;
	
	const Symbol * s = m_ctx->lookup_symbol(mj);
	
    if (s)
        *retp += sprintf(*retp, "%s", s->get_name().c_str());
    else if (mj.isValid())
        *retp += sprintf (*retp, "%s", mj.toString().c_str());
    else if (pc.isValid() && (jump < 0 && (u32)-jump > pc.getValue()))
        *retp += sprintf (*retp, "<stub_symbol>");
}

static int bitcount(unsigned int n)
{
#ifdef __GNUC__
	return __builtin_popcount(n);
#else
	// MIT HAKMEM 169
	register unsigned int t;
	t = n - ((n >> 1) & 033333333333) - ((n >> 2) & 011111111111);
	return ((t + (t >> 3)) & 030707070707) % 63;
#endif
}

static void writeImmed(char **retp, u32 imm)
{
	int ubc = bitcount(imm & 0xffff);
	int lbc = bitcount(imm >> 16);
	int simm = (s32)imm;
	if (simm < 256 && simm > -256)
		*retp += sprintf(*retp, "#%d", simm);
	else if (ubc <= 4 || lbc <= 4 || ubc == 16 || lbc == 16)
		*retp += sprintf(*retp, "#0x%x", imm);
	else if (simm % 1000 == 0)
		*retp += sprintf(*retp, "#%d", simm);
	else
		*retp += sprintf(*retp, "#%d\t; 0x%x", simm, imm);
}

const std::string decodeText(const I_ProjectModel * ctx, u32 instr, const address_t & addr)
{
	
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
                    case 'b':
                    case 'd':
                    case 'l':
                    case '^':
                        if (instr & (1 << 22))
                            *retp++ = *p;
                        break;
                    case 'a':
                        // write_addr (slixt, pptr, Pre, Up, Writeback, Immbit, Barrelshift);
                        write_addr (ctx, &retp, !!(instr & (1 << 24)), !!(instr & (1 << 23)),
                                    !!(instr & (1 << 21)), !(instr & (1 << 25)),
                                    instr & 0xfff, (instr >> 16) & 0xf, addr + 8);
                        break;
                    case 'A':
                        if (instr & (1 << 22))
                            write_addr (ctx, &retp, !!(instr & (1 << 24)), !!(instr & (1 << 23)),
                                        !!(instr & (1 << 21)), 1,
                                        ((instr >> 4) & 0xf0) | (instr & 0xf), (instr >> 16) & 0xf,
                                        addr + 8);
                        else
                            write_addr (ctx, &retp, !!(instr & (1 << 24)), !!(instr & (1 << 23)),
                                        !!(instr & (1 << 21)), 0, instr & 0xf, (instr >> 16) & 0xf,
                                        addr + 8);
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
                    case 'C':
                        *retp++ = 'c';
                        write_reg (&retp, (instr >> arg) & 0xf);
                        break;
                    case 'i':
                        retp += sprintf (retp, "0x%06x", instr & 0xffffff);
                        break;
                    case 'j':
                        write_jump (ctx, &retp, instr & 0xffffff, addr + 8);
                        break;
                    case 'k':
                        retp += sprintf (retp, "%04x", ((instr >> 4) & 0xfff0) | (instr & 0xf));
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
			    writeImmed(&retp, imm);
                        } else {
                            write_barrelshift (&retp, instr & 0xfff);
                        }
                        break;
                    case 'p':
                        retp += sprintf (retp, "p%d", (instr >> 8) & 0xf);
                        break;
                    case 'r':
                        write_reg (&retp, (instr >> arg) & 0xf);
                        break;
                    case 's':
                        if (instr & (1 << 20))
                            *retp++ = 's';
                        break;
                    case 't':
                        if (((instr >> 28) & 0xf) == 0xf) {
                            *retp++ = '2'; /* guess that this proc is armv5+ */
                            break;
                        }
                        write_cond (&retp, (instr >> 28)  & 0xf);
                        break;
                    case 'u':
                        if (instr & (1 << 22))
                            *retp++ = 's';
                        else
                            *retp++ = 'u';
                        break;
                    case 'W':
                        if (instr & (1 << arg)) {
                            *retp++ = 'w';
                            *retp++ = (instr & (1 << 6)) ? 't' : 'b';
                        }
                        else {
                            *retp++ = (instr & (1 << 5)) ? 't' : 'b';
                            *retp++ = (instr & (1 << 6)) ? 't' : 'b';
                        }
                        break;
                    case 'x':
                        retp += sprintf (retp, "%d", (instr >> arg) & ((arg % 4) ? 0x7 : 0xf));
                        break;
                    case 'X':
                        if ((arg = (instr >> arg) & ((arg % 4) ? 0x7 : 0xf)))
                            retp += sprintf (retp, "%d", arg);
                        break;
                    case '!':
                        if (instr & (1 << 21))
                            *retp++ = '!';
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

