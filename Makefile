CC=gcc
LIBS=-lncurses -ljson-c
CFLAGS=
OBJ=win.o
PROG=win

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

win: ${OBJ}
	gcc ${PROG}.c -o ${PROG} ${LIBS} ${FLAGS}

clean:
	rm ${PROG}

run:
	./${PROG}
