CC = gcc
stego: main.c $(wildcard steganography/*.c) image-rw/imagerw.c $(wildcard fsm/*.c)
	$(CC) -o stego main.c $(wildcard steganography/*.c) image-rw/imagerw.c $(wildcard fsm/*.c)