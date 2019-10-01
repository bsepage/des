#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#include "system.h"

void mem_cpy(unsigned char *dst, const unsigned char *src, const size_t len);
void mem_prt(const unsigned char *buf, const size_t len);
void mem_clr(volatile unsigned char *buf, const size_t len);
void mem_xor(unsigned char *dst, const unsigned char *src, const size_t len);
void mem_rnd(volatile unsigned char *buf, const size_t len);
void mem_san(unsigned char *buf, const size_t len);
size_t mem_frd(unsigned char *buf, const char *path, const size_t len);
void mem_fwt(unsigned char *buf, const char *path, const size_t len);

#endif
