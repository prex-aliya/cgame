all:
	cc -o diffkeygrab.out diffkeygrab.c
	gcc -lm -o diffrender.out diffrender.c

keygrab:
	cc -o a.out diffkeygrab.c
render:
	gcc -o a.out diffrender.c -lm

test: all
	./a.out
