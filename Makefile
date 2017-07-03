LIBS=-lncurses
FLAGS=
PROG=EHLO

ehlo:
	gcc ${PROG}.c -o ${PROG} ${LIBS} ${FLAGS}
