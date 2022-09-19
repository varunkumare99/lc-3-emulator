#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "util.h"
#include "lc3_arch.h"
#include "OP_TRAP_code.h"

uint16_t memory[MEMORY_MAX]; /* memory map */
uint16_t registers[R_COUNT]; /* register map */

int main(int argc, const char* argv[]) {

	if (argc < 2) {
		printf("lc3 [image-file1] ...\n");
		exit(2);
	}


	for (unsigned int i = 1; i < argc; ++i) {
		if (!read_image(argv[i])) {
			printf("failed to load image: %s\n", argv[i]);
			exit(1);
		}
	}

	signal(SIGINT, handle_interrupt);
	disable_input_buffering();

	/* since exactly one condition flag shold be set at any given time,  set the Z flag */
	registers[R_COND] = FL_ZER;

	// set PC to the start position
	// 0x3000 is the default
	registers[R_PC] = PC_START;

	unsigned int lc3_running = 1;

	while (lc3_running) {
		uint16_t curr_instr = memory_read(registers[R_PC]++);
		uint16_t opCode = curr_instr >> 12; //get first 4 bits

		switch (opCode) {
			case OP_ADD:
				add_instr(curr_instr);
				break;
			case OP_LD:
				ld_instr(curr_instr);
				break;
			case OP_ST:
				st_instr(curr_instr);
				break;
			case OP_BR:
				br_instr(curr_instr);
				break;
			case OP_JSR:
				jsr_instr(curr_instr);
				break;
			case OP_AND:
				and_instr(curr_instr);
				break;
			case OP_LDR:
				ldr_instr(curr_instr);
				break;
			case OP_STR:
				str_instr(curr_instr);
				break;
			case OP_LEA:
				lea_instr(curr_instr);
				break;
			case OP_LDI:
				ldi_instr(curr_instr);
				break;
			case OP_STI:
				sti_instr(curr_instr);
				break;
			case OP_JMP:
				jmp_instr(curr_instr);
				break;
			case OP_TRAP:
				{
					registers[R_R7] = registers[R_PC];
					switch (curr_instr & 0xFF) {
						case TRAP_GETC:
							getc_trap();
							break;
						case TRAP_OUT:
							out_trap();
							break;
						case TRAP_PUTS:
							puts_trap();
							break;
						case TRAP_IN:
							in_trap();
							break;
						case TRAP_PUTSP:
							putsp_trap();
							break;
						case TRAP_HALT:
							halt_trap();
							lc3_running = 0;
							break;
					}
					break;
				}
				break;
			case OP_NOT:
				not_instr(curr_instr);
				break;
			case OP_RES:
				abort();
				/* ret_instr(); */
				break;
			case OP_RTI:
			default:
				abort();
				break;
		}
	}
	restore_input_buffering();
}
