# des
A simple implementation of the Data Encryption Standard (DES)


# What

This project provides an implementation of the DES algorithm, intended to be easy to read and re-use. The src/main.c contains four examples:
- Encryption with 8 bytes of input
- Decryption with 8 bytes of input
- Encryption with an unknown input length, with a choice of padding between zeroes and PKCS#5
- Decryption with an unkown input length


Key and plaintexts can be modified directly from their respective text files (see the usr/ folder).

# How

Compile and run the code
- make clean
- make
- ./bin/des


Test the correct execution for Exercices 1 and 2
- hexdump -C usr/plaintext1.txt
- hexdump -C usr/ciphertext1.txt
- hexdump -C usr/plaintext2.txt

The files usr/plaintext1.txt and usr/plaintext2.txt should give the same output.

Test the correct execution for Exercices 3 and 4
- hexdump -C usr/plaintext3.txt
- hexdump -C usr/ciphertext3.txt
- hexdump -C usr/plaintext4.txt

The files usr/plaintext3.txt and usr/plaintext4.txt should give the same output, and you should observe the additionnal padding in usr/plaintext4.txt.

# Additional notes

**Warning**: This code is provided as-is. It is intended for research and educational purpose only. You are responsible for protecting yourself, your property and data, and others from any risks caused by this project.

The current version of the project has been validated against memory leaks as follows
- valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./bin/des

If you decide to use the 'memory.c: mem_san()' funcion, forbidden characters will be replaced, causing usr/plaintext1.txt and usr/plaintext2.txt (or usr/plaintext3.txt and usr/plaintext4.txt) to be different. The character whitelist is specififed in 'memory.c:authorized[]'. In order to sanitize input files, uncomment the appropiate line in 'memory.c: mem_frd()'.

