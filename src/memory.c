#include "memory.h"

/**< White list of accepted characters */
static char authorized[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-+.";

/** \brief Copy src buffer into dst buffer.
 *
 * \param dst unsigned char* Ouput buffer.
 * \param src const unsigned char* Input buffer.
 * \param len const size_t Size (bytes) of dst buffer.
 *
 */
void mem_cpy(unsigned char *dst, const unsigned char *src, const size_t len) {
	for (size_t i=0; i<len; ++i)
   		dst[i] = src[i];
}

/** \brief Print buf buffer.
 *
 * \param buf const unsigned char* Buffer.
 * \param len const size_t Size (bytes) of buf buffer.
 *
 */
void mem_prt(const unsigned char *buf, const size_t len) {
   	printf("0x");
   	for (size_t i=0; i<len; ++i)
   		printf("%02X", buf[i]);
   	putchar('\n');
}

/** \brief Clear buf buffer.
 *
 * \param buf volatile unsigned char* Buffer.
 * \param len const size_t Size (bytes) of buf buffer.
 *
 */
void mem_clr(volatile unsigned char *buf, const size_t len) {
   	for (size_t i=0; i<len; ++i)
   		buf[i] = 0x00;
}

/** \brief Perform XOR operation between src buffer and dst buffer, and store result into dst buffer.
 *
 * \param dst unsigned char* Output buffer.
 * \param src const unsigned char* Input buffer.
 * \param len const size_t Size (bytes) of dst buffer.
 *
 */
void mem_xor(unsigned char *dst, const unsigned char *src, const size_t len) {
   	for (size_t i=0; i<len; ++i)
   		dst[i] ^= src[i];
}

/** \brief Fill buf buffer with random data.
 *
 * \param buf volatile unsigned char* Buffer.
 * \param len const size_t Size (bytes) of buf buffer.
 *
 */
void mem_rnd(volatile unsigned char *buf, const size_t len) {
   	for (size_t i=0; i<len; ++i)
   		buf[i] = (unsigned char) rand();
}

/** \brief Sanitise buf buffer. Replace forbidden characters with underscore.
 *
 * \param buf unsigned char* Buffer.
 * \param len const size_t Size (bytes) of buf buffer.
 *
 */
void mem_san(unsigned char *buf, const size_t len) {
	const unsigned char *end = (unsigned char*) (buf + len);
	for (buf+=strspn((char*) buf, authorized); *buf!=*end; buf+=strspn((char*) buf, authorized))
		*buf = 0x5F;
}

/** \brief Read file.
 *
 * \param buf unsigned char* Output buffer.
 * \param path const char* Path to file.
 * \param len const size_t Maximum size (bytes) to read from file.
 * \return size_t Size (bytes) of buf buffer.
 *
 */
size_t mem_frd(unsigned char *buf, const char *path, const size_t len) {
   	size_t i = 0, n = 0;
   	FILE *f = fopen(path, "r");
   	if (!f)
	       	HANDLE_ERROR("fopen");
   	for (buf[i]=(unsigned char)fgetc(f); !feof(f) && i<len; i++, buf[i]=(unsigned char)fgetc(f))
	       	n++;
   	if (fclose(f))
	       	HANDLE_ERROR("fclose");
/*	mem_san(buf, n);*/
   	return n;
}

/** \brief Write file.
 *
 * \param buf unsigned char* Input buffer.
 * \param path const char* Path to file.
 * \param len const size_t Size (bytes) of buf buffer.
 *
 */
void mem_fwt(unsigned char *buf, const char *path, const size_t len) {
   	size_t i = 0;
   	FILE *f = fopen(path, "w");
   	if (!f)
	       	HANDLE_ERROR("fopen");
   	for (fputc((char)buf[i], f); i<len-1; i++, fputc((char)buf[i], f))
	       	;
   	if (fclose(f))
	       	HANDLE_ERROR("fclose");
}
