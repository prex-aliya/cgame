NAME 		= cGAME
VER			= 0.0.8
# 				^ ^ ^- PATCH, spelling, small update.
#					| +--- MINOR, fatel bugs, etc.
#					+----- MAJOR, updates, Minor features, etc.
# Semantic Versioning: https://semver.org/
# v1.0.0 will be offical release.


# Compiler and Linker
CC			= gcc

# Compiling Options
OPP			= -Ofast
SOUND		= -lrt -lm
THREAD	= -lpthread
WARN		= -Wall -Werror -Wpedantic
CFLAGS	= $(WARN) $(OPP) $(THREAD) $(SOUND)

# Libraries
LIBD		= ./lib
PTAUDIO	= $(LIBD)/libportaudio.a
LIB			= $(PTAUDIO)

# Binarys
BINNAME	= a.out
BINDIR	= ./bin/
BIN			= $(BINDIR)$(BINNAME)

SRCNAME	= main.c
SRCDIR	= ./src/
SRC			= $(SRCDIR)$(SRCNAME)



debug: compile
# DEBUG = -D DEBUG_ENABLE=true


all: compile options
	@echo Finished Building $(NAME) v$(VER)

options:
	@echo "Build Options:"

test: compile
	$(BIN) # Run The Binary
compile: $(SRCD)
	$(CC) $(CFLAGS) $(SRC) $(LIB) -o $(BIN) $(DEBUG)
install: all compile
	@echo "No install option at the moment"
uninstall: all compile
	@echo "No uninstall option at the moment"


.PHONY: all options clean install uninstall
