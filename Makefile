LIBS=-lncurses
FLAGS=
PROG=win

win:
	gcc ${PROG}.c -o ${PROG} ${LIBS} ${FLAGS}

clean:
	rm ${PROG}

run:
	./${PROG}
