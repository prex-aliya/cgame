VERSION = 0.0.0
# 		  ^ ^ ^- Patches
#		  | +--- Minor bugs, spelling mistakes, etc.
#		  +----- Major updates, Minor features, etc.
# Semantic Versioning: https://semver.org/

# Compiler and Linker
CC		= gcc

OPP		= -Ofast
SOUND	= -lrt -lm -pthread
CFLAGS	= $(OPP) $(SOUND)

LIBD	= ./lib
LIB		= $(LIBD)/libportaudio.a

BINNAME	= a.out
BIND	= ./bin/$(BINNAME)

SRCD	= ./src
SRC		= $(SRCD)/main.c

all: compile options
	@echo Finished

options:
	@echo Build Options: audio$()

test: compile
	$(BIND)

compile: $(SRCD)
	$(CC) $(CFLAGS) $(SRC) $(LIB) -o $(BIND)

install: all compile
	@echo "No install option at this moment"


.PHONY: all options clean install uninstall
