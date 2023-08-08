all:
	cc -o diffkeygrab.out diffkeygrab.c
	cc -o diffrender.out diffrender.c

keygrab:
	cc -o a.out diffkeygrab.c
render:
	cc -o a.out diffrender.c

test: all
	./a.out
