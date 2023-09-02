CC			= gcc
FILENAME 	= main.c

SDL_FLAGS	= -lSDL2
FLAGS		= -Wall -Werror -Wpedantic $(SDL_FLAGS)

all: $(FILENAME)
	$(CC) -o a.out $(FILENAME) $(FLAGS)
test: all
	./a.out
