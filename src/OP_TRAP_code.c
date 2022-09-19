#include <stdio.h>
#include <stdlib.h>
#include <sys/termios.h>
#include "util.h"
#include "lc3_arch.h"
#include "OP_TRAP_code.h"


/* OP_ADD */
void add_instr(uint16_t instr) {
	//dest register
	uint16_t dest_reg = (instr >> 9) & 0x7;

	//src1 register
	uint16_t src1_reg = (instr >> 6) & 0x7;

	//get mode bit 
	uint16_t mode_bit = (instr >> 5) & 0x1;

	if (mode_bit) {
		uint16_t imm5 = instr & 0x1F;
		imm5 = sign_extend(imm5, 5);
		uint16_t operand1 = registers[src1_reg];
		registers[dest_reg] = operand1 + imm5;
	}
	else {
		uint16_t src2_reg = instr & 0x7;
		uint16_t operand1 = registers[src1_reg];	
		uint16_t operand2 = registers[src2_reg];	

		registers[dest_reg] = operand1 + operand2;	
	}
	update_flags(dest_reg);
}

/* OP_LDI */
void ldi_instr(uint16_t instr) {
	//destination register
	uint16_t dest_reg = (instr >> 9) & 0x7;

	//PC offset
	uint16_t pc_offset = instr & 0x1FF;
	pc_offset = sign_extend(pc_offset, 9);

	uint16_t eff_address = registers[R_PC] + pc_offset;
	registers[dest_reg] = memory_read(memory_read(eff_address));

	update_flags(dest_reg);
}

/* OP_LDR */
void ldr_instr(uint16_t instr) {
	//destination register
	uint16_t dest_reg = (instr >> 9) & 0x7;

	//base register
	uint16_t base_reg = (instr >> 6) & 0x7;

	//base reg offset
	uint16_t base_reg_offset = instr & 0x3F;
	base_reg_offset = sign_extend(base_reg_offset, 6);

	uint16_t eff_address = registers[base_reg]  + base_reg_offset;
	registers[dest_reg] = memory_read(eff_address);

	update_flags(dest_reg);
}

/* OP_STR */
void str_instr(uint16_t instr) {
	//src register
	uint16_t src_reg = (instr >> 9) & 0x7;

	//base register
	uint16_t base_reg = (instr >> 6) & 0x7;

	//base reg offset
	uint16_t base_reg_offset = instr & 0x3F;
	base_reg_offset = sign_extend(base_reg_offset, 6);

	uint16_t eff_address = registers[base_reg] +  base_reg_offset;
	memory_write(eff_address, registers[src_reg]);
}

/* OP_STI */
void sti_instr(uint16_t instr) {
	//src register
	uint16_t src_reg = (instr >> 9) & 0x7;

	//PC offset
	uint16_t pc_offset = instr & 0x1FF;
	pc_offset = sign_extend(pc_offset, 9);

	uint16_t eff_address = registers[R_PC] + pc_offset;
	memory_write(memory_read(eff_address), registers[src_reg]);
}

/* OP_LD */
void ld_instr(uint16_t instr) {
	//destination register
	uint16_t dest_reg = (instr >> 9) & 0x7;

	//PC offset
	uint16_t pc_offset = instr & 0x1FF;
	pc_offset = sign_extend(pc_offset, 9);

	uint16_t eff_address = registers[R_PC] + pc_offset;
	registers[dest_reg] = memory_read(eff_address);

	update_flags(dest_reg);
}

/* OP_ST */
void st_instr(uint16_t instr) {
	//src register
	uint16_t src_reg = (instr >> 9) & 0x7;

	//PC offset
	uint16_t pc_offset = instr & 0x1FF;
	pc_offset = sign_extend(pc_offset, 9);

	uint16_t eff_address = registers[R_PC] + pc_offset;
	memory_write(eff_address, registers[src_reg]);
}

/* OP_AND */
void and_instr(uint16_t instr) {
	//dest register
	uint16_t dest_reg = (instr >> 9) & 0x7;

	//src1 register
	uint16_t src1_reg = (instr >> 6) & 0x7;

	//get mode bit 
	uint16_t mode_bit = (instr >> 5) & 0x1;

	if (mode_bit) {
		uint16_t imm5 = instr & 0x1F;
		imm5 = sign_extend(imm5, 5);
		uint16_t operand1 = registers[src1_reg];
		registers[dest_reg] = operand1 & imm5;
	}
	else {
		uint16_t src2_reg = instr & 0x7;
		uint16_t operand1 = registers[src1_reg];	
		uint16_t operand2 = registers[src2_reg];	

		registers[dest_reg] = operand1 & operand2;	
	}
	update_flags(dest_reg);
}

/* OP_NOT */
void not_instr(uint16_t instr) {
	//dest register
	uint16_t dest_reg = (instr >> 9) & 0x7;

	//src register
	uint16_t src_reg = (instr >> 6) & 0x7;

	registers[dest_reg] = ~registers[src_reg]; //taking complement
	update_flags(dest_reg);
}

/* OP_LEA */
void lea_instr(uint16_t instr) {
	//destination register
	uint16_t dest_reg = (instr >> 9) & 0x7;

	//PC offset
	uint16_t pc_offset = instr & 0x1FF;
	pc_offset = sign_extend(pc_offset, 9);

	registers[dest_reg] = registers[R_PC] + pc_offset;

	update_flags(dest_reg);
}

/* OP_BR */
void br_instr(uint16_t instr) {
	
	//PC offset
	uint16_t pc_offset = instr & 0x1FF;
	pc_offset = sign_extend(pc_offset, 9);

	uint16_t cond_flag = (instr >> 9) & 0x7;
	if (cond_flag & registers[R_COND]) {
		//branch unconditionally
		registers[R_PC] = registers[R_PC] + pc_offset;
	}
}

/* OP_JMP */
void jmp_instr(uint16_t instr) {

	//baseR
	uint16_t base_reg = (instr >> 6) & 0x7;

	//brach to value in register
	registers[R_PC] = registers[base_reg];
}

/* OP_JSR */
void jsr_instr(uint16_t instr) {

	//save next pc in r7
	registers[R_R7] = registers[R_PC];
	uint16_t mode_bit = (instr >> 11) & 1;

	if (mode_bit) {
		//JSR
		//PC offset
		uint16_t pc_offset = instr & 0x07FF;
		pc_offset = sign_extend(pc_offset, 11);

		registers[R_PC] = registers[R_PC] + pc_offset;

	}
	else {
		//JSRR
		//base register
		uint16_t base_reg = (instr >> 6) & 0x7;
		//value in base_reg in to PC
		registers[R_PC] = registers[base_reg];
	}
}

/* OP_RET */
void ret_instr() {
	//save value in R7 in pc
	registers[R_PC] = registers[R_R7];
}

/* TRAP_GETC */
void getc_trap() {
	/* read a single ASCII char */
	char inputChar;
	scanf("%c", &inputChar);

	//store ASCII code
	registers[R_R0] = (uint16_t)inputChar;
	update_flags(R_R0);
}

/* TRAP_OUT */
void out_trap() {
	char outputChar;
	outputChar = registers[R_R0] & 0xFF;
	printf("%c", outputChar);
	fflush(stdout);
}

/* TRAP_PUTS */
void puts_trap() {
	/* one char per word */
	uint16_t address = registers[R_R0];
	char outputChar;
	/* output string until null character is found */
	while (memory_read(address) != 0x0000) {
		outputChar = memory_read(address);
		printf("%c", outputChar);
		address++;
	}
	fflush(stdout);
}

/* TRAP_IN */
void in_trap() {
	char inputChar;
	printf("enter a character:");
	scanf("%c", &inputChar);
	printf("%c", inputChar);
	fflush(stdout);

	//store ASCII code
	registers[R_R0] = (uint16_t)inputChar;
	registers[R_R0] &= 0xF;
	update_flags(R_R0);
}

/* TRAP_PUTSP */
void putsp_trap() {
	/* one char per byte (two bytes per word)
	   here we need to swap back to
	   big endian format */
	uint16_t address = memory_read(registers[R_R0]);
	char firstChar = address & 0xF;
	char secondChar = (address >> 8) & 0xF;

	while (firstChar != '\0') {
		printf("%c", firstChar);
		if (secondChar == '\0')
			break;
		printf("%c", secondChar);

		address++;
		firstChar = address & 0xF;
		secondChar = (address >> 8) & 0xF;
	}
}

/* TRAP_HALT */
void halt_trap() {
	printf("\nprogram halt\n");
	fflush(stdout);
}
