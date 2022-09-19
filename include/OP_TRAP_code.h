#ifndef OP_TRAP_CODE_H
#define OP_TRAP_CODE_H

#include <stdint.h>

//op codes
void add_instr(uint16_t instr);  /* OP_ADD */
void ldi_instr(uint16_t instr);  /* OP_LDI */
void sti_instr(uint16_t instr);  /* OP_STI */
void ld_instr(uint16_t instr);   /* OP_LD */
void st_instr(uint16_t instr);   /* OP_ST */
void and_instr(uint16_t instr);  /* OP_AND */
void not_instr(uint16_t instr);  /* OP_NOT */
void lea_instr(uint16_t instr);  /* OP_LEA */
void br_instr(uint16_t instr);   /* OP_BR */
void jmp_instr(uint16_t instr);  /* OP_JMP */
void jsr_instr(uint16_t instr);  /* OP_JSR */
void ldr_instr(uint16_t instr);  /* OP_LDR */
void str_instr(uint16_t instr);	 /* OP_STR */
void ret_instr();				 /* OP_RET */

//trap codes
void getc_trap();				/* TRAP_GETC */
void halt_trap();				/* TRAP_HALT */
void putsp_trap();				/* TRAP_PUTSP */
void in_trap(); 				/* TRAP_IN */
void puts_trap(); 				/* TRAP_PUTS */
void out_trap(); 				/* TRAP_OUT */

#endif
