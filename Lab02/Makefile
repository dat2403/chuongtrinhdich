CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: scanner

scanner: scanner.o reader.o charcode.o token.o error.o
	${CC} scanner.o reader.o charcode.o token.o error.o -o scanner

reader.o: reader.c reader.h
	${CC} ${CFLAGS} reader.c

scanner.o: scanner.c
	${CC} ${CFLAGS} scanner.c

charcode.o: charcode.c charcode.h
	${CC} ${CFLAGS} charcode.c

token.o: token.c token.h
	${CC} ${CFLAGS} token.c

error.o: error.c error.h
	${CC} ${CFLAGS} error.c

clean:
	rm -f *.o *~

