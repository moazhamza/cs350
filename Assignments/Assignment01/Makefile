CC=gcc
CFLAG= -g -Wextra -Wall -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -pedantic

all: minish

minish: minish.c
	$(CC) $(CFLAG) minish.c -o minish

run: minish
	./minish

clean: 
	rm -rf *.o minish
	rm -rf *.dSYM
