#include "des.h"
#include "memory.h"
#include "system.h"

int main(void)
{
	unsigned char src[_64b];
	unsigned char dst[_64b];
	unsigned char key[_64b];

	/** Example 1: simple DES encryption, 8 bytes of input **/
	mem_clr(src, _64b);					// Clean buffers
	mem_clr(dst, _64b);
	mem_clr(key, _64b);

	mem_frd(src, "usr/plaintext1.txt", _64b);		// Initialize buffers
	mem_frd(key, "usr/key.txt", _64b);

	des(dst, src, key, true);				// Run DES encryption
	mem_fwt(dst, "usr/ciphertext1.txt", _64b);		// Write output buffer to text file

	mem_rnd(src, _64b);					// Clear buffers
	mem_rnd(dst, _64b);
	mem_rnd(key, _64b);

	/** Example 2: simple DES decryption, 8 bytes of input **/
	mem_clr(src, _64b);					// Clean buffers
	mem_clr(dst, _64b);
	mem_clr(key, _64b);

	mem_frd(src, "usr/ciphertext1.txt", _64b);		// Initialize buffers
	mem_frd(key, "usr/key.txt", _64b);

	des(dst, src, key, false);				// Run DES decryption
	mem_fwt(dst, "usr/plaintext2.txt", _64b);		// Write output buffer to text file

	mem_rnd(src, _64b);					// Clear buffers
	mem_rnd(dst, _64b);
	mem_rnd(key, _64b);


	unsigned char src_big[_4096b];				// Set maximum input size to 512 bytes
	unsigned char dst_big[_4096b+_64b];			// Account for potential padding byte
	size_t src_big_len = 0;

	/** Example 3: simple DES encryption, unknown input length **/
	/** Note: comment '#undef PKCS' in 'des.c' to allow PKCS#5 padding scheme **/
	mem_clr(src_big, _4096b);				// Clean buffers
	mem_clr(dst_big, _4096b+_64b);
	mem_clr(key, _64b);

	mem_frd(key, "usr/key.txt", _64b);			// Initialize buffers and get input size
	src_big_len = mem_frd(src_big, "usr/plaintext3.txt", _4096b);

	des_pad(src_big, src_big_len);				// Pad input buffer to multiple of 8

	for (size_t i=0; i<src_big_len/8+1; ++i)		// Run DES encryption for each block of 8 bytes
	       	des(dst_big+i*8, src_big+i*8, key, true);
	mem_fwt(dst_big, "usr/ciphertext3.txt", src_big_len+(8-src_big_len%8));

	mem_rnd(src_big, _4096b);				// Clear buffers
	mem_rnd(dst_big, _4096b+_64b);
	mem_rnd(key, _64b);

	/** Example 4: simple DES decryption, unknown input length **/
	mem_clr(src_big, _4096b);				// Clean buffers
	mem_clr(dst_big, _4096b+_64b);
	mem_clr(key, _64b);

	mem_frd(key, "usr/key.txt", _64b);			// Initialize buffers and get input size
	src_big_len = mem_frd(src_big, "usr/ciphertext3.txt", _4096b);

	for (size_t i=0; i<src_big_len/8; ++i)			// Run DES decryption for each block of 8 bytes
       		des(dst_big+i*8, src_big+i*8, key, false);
	mem_fwt(dst_big, "usr/plaintext4.txt", src_big_len);

	mem_rnd(src_big, _4096b);				// Clear buffers
	mem_rnd(dst_big, _4096b+_64b);
	mem_rnd(key, _64b);


	exit(EXIT_SUCCESS);
}

