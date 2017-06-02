all: const filter window spawn streamproc

const: const.c auxs.c
	gcc -o const const.c auxs.c

filter: filter.c auxs.c
	gcc -o filter filter.c auxs.c

window: window.c auxs.c
	gcc -o window window.c auxs.c

spawn: spawn.c auxs.c
	gcc -o spawn spawn.c auxs.c

streamproc: streamproc.c auxs.c
	gcc -o streamproc streamproc.c auxs.c
