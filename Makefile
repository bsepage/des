CC		= clang
SRC		= src/*.c
CFLAGS	= -Wall -std=c11 -fdeclspec -pedantic -g
BIN		= bin/des

target:
	clear
	$(CC) $(SRC) $(CFLAGS) -o $(BIN)

clean:
	rm -f $(BIN)
	rm usr/ciphertext1.txt
	rm usr/plaintext2.txt
	rm usr/ciphertext3.txt
	rm usr/plaintext4.txt
