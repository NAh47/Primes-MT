CC = gcc
CFLAGS = -Wall -Wshadow -Werror -Wunreachable-code -Wredundant-decls -Wextra
CFLAGS += -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes
CFLAGS += -Wdeclaration-after-statement -Wno-return-local-addr -Wuninitialized -Wunsafe-loop-optimizations

all:
	$(CC) $(CFLAGS) -o primesMT primesMT.c
primesMT:
	$(CC) $(CFLAGS) -o primesMT primesMT.c
primesMT.o:
	$(CC) $(CFLAGS) -o primesMT primesMT.c

clean:
	rm -f primesMT *.o *~ \#*
