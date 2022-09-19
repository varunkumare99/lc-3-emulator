#ifndef LC3_ARCH_H
#define LC3_ARCH_H

#include <stdint.h>

#define PC_START 0x3000
//memory storage
#define MEMORY_MAX (1 << 16)
extern uint16_t memory[MEMORY_MAX]; /* 65536 locations */

/*Registers
 * total 10 register each of 16-bit each
 * R0-R7 are general purpose registers
 * PC - program counter
 * COND - condition flags
 */

enum {
	R_R0 = 0,
	R_R1,
	R_R2,
	R_R3,
	R_R4,
	R_R5,
	R_R6,
	R_R7,
	R_PC,
	R_COND,
	R_COUNT
};
// Register storage
extern uint16_t registers[R_COUNT];

/* instruction opcodes
 * each instruction is 16bits long.
 * 4 bits are for code
 * and remaining 10 for the paramters
 */

enum {
	OP_BR = 0,  /* branch */
	OP_ADD,     /* Add */
	OP_LD,      /* load */
	OP_ST,      /* store */
	OP_JSR,     /* jump register */
	OP_AND,     /* bitwise and */
	OP_LDR,     /* load register */
	OP_STR,     /* store register */
	OP_RTI,     /* unused */
	OP_NOT,     /* bitwise not */
	OP_LDI,     /* load indirect */
	OP_STI,     /* store indirect */
	OP_JMP,     /* jump */
	OP_RES,     /* return from subroutine */
	OP_LEA,     /* load effective address */
	OP_TRAP,    /* execute TRAP */
};

//condition flags
enum {
	FL_POS = 1 << 0, /* P */
	FL_ZER = 1 << 1, /* Z */
	FL_NEG = 1 << 2, /* N */
};

//Trap codes
enum {
	TRAP_GETC = 0x20,  /* get character from keyboard, not echoed onto the terminal */
	TRAP_OUT = 0x21,   /* Ouput a character */
	TRAP_PUTS = 0x22,  /* Output a word string */
	TRAP_IN = 0x23,    /* get character from keyboard, echoed onto the terminal */
	TRAP_PUTSP = 0x24, /* execute a byte string */
	TRAP_HALT = 0x25   /* halt the program */
};

//Memory mapped registers
enum {
	MR_KBSR = 0xFE00, /* keyboard status */
	MR_KBDR = 0xFE02, /* keyboard data */
};

#endif
