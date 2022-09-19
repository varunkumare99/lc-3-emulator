#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/termios.h>
#include <sys/time.h>
#include "util.h"
#include "lc3_arch.h"

struct termios orig_term;

uint16_t sign_extend(uint16_t x, int bit_count) {
	if ((x >> (bit_count - 1)) & 1) { //check if x is negative
		x |= (0xFFFF << (bit_count)); //sign extend with 1's
	}
	return x;
}

void update_flags(uint16_t reg_index) {
	if (registers[reg_index] == 0) {
		registers[R_COND] = FL_ZER;
	}

	else if (registers[reg_index] >> 15) { /* negative value sign bit */
		registers[R_COND] = FL_NEG;
	}

	else {
		registers[R_COND] = FL_POS;
	}
}

void memory_write(uint16_t address, uint16_t value) {
	memory[address] = value;
}

uint16_t memory_read(uint16_t address) {
	if (address == MR_KBSR) {
		if(check_key()) {
			memory[MR_KBSR] = (1<<15); 		/* turn on keyboard status register */
			memory[MR_KBDR] = getchar();	/* read data into keyboard data register */
		}
		else {
			memory[MR_KBSR] = 0;
		}
	}
	return memory[address];
}

void disable_input_buffering() {
	tcgetattr(STDIN_FILENO, &orig_term);
	struct termios new_term = orig_term;
	new_term.c_lflag &= ~ICANON & ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
}

void restore_input_buffering() {
	/* TCASNOW make the change immediately */
	tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);
}

uint16_t check_key() {
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(STDIN_FILENO, &readfds);

	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	/* polling until the readfds are ready for input */
	return select(1, &readfds, NULL, NULL, &timeout) != 0;
}

void handle_interrupt(int signal) {
	restore_input_buffering();
	printf("\n");
	exit(-2);
}


void read_image_file(FILE *file) {
	uint16_t origin;
	//read binary data of size 1block fo size uint16_t from file and store at origin, at the beginning of memory
	//the first 16bits represnts the size of the binary object file
	fread(&origin, sizeof(origin), 1, file);
	origin = swap16(origin);

	/* we know the maximum file size so we only need one fread */
	uint16_t max_read = MEMORY_MAX - origin;
	uint16_t *p = memory + origin;
	size_t read = fread(p, sizeof(uint16_t), max_read, file);

	/*swap to little endian */
	while (read-- > 0) {
		*p = swap16(*p);
		++p;
	}
}

/*convert to little endian (first byte is least significant */
uint16_t swap16(uint16_t val) {
	/* swap the 2 bytes in uint16_t*/
	return (val << 8 | val >> 8);
}

int read_image(const char* image_path) {
	/* open a binary file for reading */
	FILE* file = fopen(image_path, "rb");
	if (!file)
		return 0;
	read_image_file(file);
	fclose(file);
	return 1;
}
