CC=gcc
LIBS=-lncursesw -ljson-c
CFLAGS=
OBJ=win.o
PROG=win
CHECKINS_OBJ=checkins.o
CHECKINS_PROG=checkins
BEERS_OBJ=beers.o
BEERS_PROG=beers

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

win: ${OBJ}
	gcc ${PROG}.c -o ${PROG} ${LIBS} ${FLAGS}

checkins: ${CHECKINS_OBJ}
	gcc ${CHECKINS_PROG}.c -o ${CHECKINS_PROG} ${LIBS} ${FLAGS}

beers: ${BEERS_OBJ}
	gcc ${BEERS_PROG}.c -o ${BEERS_PROG} ${LIBS} ${FLAGS}

clean:
	rm ${PROG}

run:
	./${PROG}

update:
	git pull
