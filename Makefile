CC=gcc
LIBS=-lncursesw -ljson-c
CFLAGS=
OBJ=win.o
CHECKINS_OBJ=checkins.o
PROG=win
CHECKINS_PROG=checkins

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

win: ${OBJ}
	gcc ${PROG}.c -o ${PROG} ${LIBS} ${FLAGS}

checkins: ${CHECKINS_OBJ}
	gcc ${CHECKINS_PROG}.c -o ${CHECKINS_PROG} ${LIBS} ${FLAGS}

clean:
	rm ${PROG}

run:
	./${PROG}

update:
	git pull
