#ifndef DES_H_INCLUDED
#define DES_H_INCLUDED

#include "system.h"

void des(unsigned char *dst, const unsigned char *src, const unsigned char *key, const bool enc);
void des_pad(unsigned char *buf, const size_t len);

#endif
