#include <stdlib.h>
#include <stdbool.h>

#include "des.h"
#include "memory.h"
#include "tables.h"

#define	PKCS
/*#undef	PKCS*/

#define SHF(i)		( 7 - (i)%8 )
#define BIT(pB,i)	( (*((pB) + (i)/8) >> (7 - (i)%8)) & 0x01 )
#define SET(pB,i)	( *((pB) + (i)/8) |= (0x01 << (7 - ((i)%8))) )
#define CLR(pB,i)	( *((pB) + (i)/8) &= ~(0x01 << (7 - ((i)%8))) )
#define LKP(B)		( (((B) & 0x01) | (((B) & 0x20) >> 4))*16 + (((B) & 0x1E) >> 1) )

void des_pmt(unsigned char *dst, const unsigned char *src, const unsigned char *tbl, const size_t len);
void des_rot_lft(unsigned char *key, size_t len);
unsigned char *des_idx_sbt(const size_t index);
void des_sbt(unsigned char *dst, const unsigned char *src, const size_t len);
void des_cph(unsigned char *dst, const unsigned char *src, const unsigned char *key);

/** \brief Perform bit-by-bit permutation operations.
 *
 * \param dst unsigned char* Output buffer.
 * \param src const unsigned char* Input buffer.
 * \param tbl const unsigned char* Permutation table.
 * \param len const size_t Size (bytes) of dst buffer.
 *
 */
void des_pmt(unsigned char *dst, const unsigned char *src, const unsigned char *tbl, const size_t len) {
	mem_clr(dst, len);
	for (size_t i=0; i<len*8; ++i)
		dst[i/8] |= BIT(src, tbl[i]-1) << SHF(i);
}

/** \brief Rotate to the left the two 28-bit key halves.
 *
 * \param key unsigned char* Key buffer.
 * \param len size_t Number of rotations to be performed.
 *
 */
void des_rot_lft(unsigned char *key, size_t len) {
	for (size_t j=0; j<len; ++j) {
		unsigned char b0 = BIT(key, 0), b28 = BIT(key, 28);
		for (size_t i=0; i<7; ++i) {
			if ((key[i] & 0x80) && i)
        	        key[i-1] |= 0x01;
			key[i] <<= 1;
		}
		(b0) ? SET(key, 27) : CLR(key, 27);
		(b28) ? SET(key, 55) : CLR(key, 55);
	}
}

/** \brief Return pointer to substitution table
 *
 * \param index const size_t Index of substitution table.
 * \return unsigned char* Pointer to substitution table.
 *
 */
unsigned char *des_idx_sbt(const size_t index) {
	if 	(index==0)  return tbl_txt_sb1;
	else if (index==1)  return tbl_txt_sb2;
	else if (index==2)  return tbl_txt_sb3;
	else if (index==3)  return tbl_txt_sb4;
	else if (index==4)  return tbl_txt_sb5;
	else if (index==5)  return tbl_txt_sb6;
	else if (index==6)  return tbl_txt_sb7;
	else                return tbl_txt_sb8;
}

/** \brief Perform eight substitution operations.
 *
 * \param dst unsigned char* Output buffer.
 * \param src const unsigned char* Input buffer.
 * \param len const size_t Size (bytes) of dst buffer.
 *
 */
void des_sbt(unsigned char *dst, const unsigned char *src, const size_t len) {
	mem_clr(dst, len);
	for (size_t j=0; j<8; ++j) {
		unsigned char tmp = 0;
		for (size_t i=0; i<6; ++i)
			tmp |= BIT(src, (j*6+i)) << (5 - i%6);
		dst[j/2] |= (j%2) ? *(des_idx_sbt(j)+LKP(tmp)) : *(des_idx_sbt(j)+LKP(tmp))<<4;
	}
}

/** \brief Feistel cipher.
 *
 * \param dst unsigned char* Output buffer.
 * \param src const unsigned char* Input buffer.
 * \param key const unsigned char* Size (bytes) of dst buffer.
 *
 */
void des_cph(unsigned char *dst, const unsigned char *src, const unsigned char *key) {
	unsigned char buf_32b[_32b];
	unsigned char buf_48b[_48b];

	des_pmt(buf_48b, src, tbl_txt_exp_pmt, _48b);			// Text expansion permutation (32b --> 48b)
	mem_xor(buf_48b, key, _48b);
	des_sbt(buf_32b, buf_48b, _32b);				// Text substitution (48b --> 32b)
	des_pmt(dst, buf_32b, tbl_txt_pmt, _32b);			// Text permutation (32b --> 32b)

	mem_rnd(buf_32b, _32b);
	mem_rnd(buf_48b, _48b);
}

/** \brief Perform simple DES encryption/decryption.
 *
 * \param dst unsigned char* Output buffer.
 * \param src unsigned char* Input buffer
 * \param key const unsigned char* Key buffer.
 * \param enc const bool Flag: true=encryption, false=decryption.
 *
 */
void des(unsigned char *dst, const unsigned char *src, const unsigned char *key, const bool enc) {
	unsigned char buf_64b[_64b];
	unsigned char buf_32b[_32b];
	unsigned char key_56b[_56b];
	unsigned char pTo_key[16][_48b];
	unsigned char *pTo_tbl_key_shf = tbl_key_shf;

	des_pmt(key_56b, key, tbl_key_pmt, _56b);			// Key permutation (64b --> 56b)
	for (size_t i=0; i<16; ++i, ++pTo_tbl_key_shf) {
		des_rot_lft(key_56b, *pTo_tbl_key_shf);
		des_pmt(pTo_key[i], key_56b, tbl_key_cmp, _48b);	// Key compression (56b --> 48b)
	}

	des_pmt(buf_64b, src, tbl_txt_ini_pmt, _64b);			// Text initial permutation (64b --> 64b)
	for (size_t i=0; i<16; ++i, ++pTo_tbl_key_shf){
		(enc) ? des_cph(buf_32b, buf_64b+_32b, pTo_key[i]) :
			des_cph(buf_32b, buf_64b+_32b, pTo_key[15-i]);
		mem_xor(buf_32b, buf_64b, _32b);
		mem_cpy(buf_64b, buf_64b+_32b, _32b);
		mem_cpy(buf_64b+_32b, buf_32b, _32b);
	}

	mem_cpy(buf_64b+_32b, buf_64b, _32b);
	mem_cpy(buf_64b, buf_32b, _32b);
	des_pmt(dst, buf_64b, tbl_txt_fin_pmt, _64b);			// Text final permutation (64b --> 64b)

	mem_rnd(buf_64b, _64b);
	mem_rnd(buf_32b, _32b);
	mem_rnd(key_56b, _56b);
	for (size_t i=0; i<16; ++i)
		mem_rnd(pTo_key[i], _56b);
}

/** \brief Pad a buffer with zeros or according to PKCS#5 scheme.
 *
 * \param buf unsigned char* Buffer.
 * \param len const size_t Size (bytes) of buf buffer.
 *
 */
void des_pad(unsigned char *buf, const size_t len){
   	size_t n = 8 - len%8;
   	if (!n)
       	n = 8;
#ifdef	PKCS
	for (size_t i=0; i<n; ++i)
	       	buf[len+i] = (unsigned char) n;
#else
	for (size_t i=0; i<n; ++i)
   		buf[len+i] = 0x00;
#endif
}

