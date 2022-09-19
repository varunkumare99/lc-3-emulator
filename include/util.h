#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stdio.h>

uint16_t sign_extend(uint16_t x, int bit_count);
void update_flags(uint16_t reg_index); /* registers[R_COND] */

uint16_t memory_read(uint16_t address);
void memory_write(uint16_t address, uint16_t val);

void disable_input_buffering();
void restore_input_buffering();

uint16_t check_key();

void handle_interrupt(int signal);
uint16_t swap16(uint16_t val);
int read_image(const char* image_path);
void read_image_file(FILE *file);

extern struct termios orig_term;

#endif
