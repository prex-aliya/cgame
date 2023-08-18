NAME 	= cGAME
VER		= 0.0.1
# 		  ^ ^ ^- PATCH, spelling, small update.
#		  | +--- MINOR, fatel bugs, etc.
#		  +----- MAJOR, updates, Minor features, etc.
# Semantic Versioning: https://semver.org/

# Compiler and Linker
CC		= gcc

OPP		= -Ofast
SOUND	= -lrt -lm -pthread
CFLAGS	= $(OPP) $(SOUND)

LIBD	= ./lib
PTAUDIO	= $(LIBD)/libportaudio.a
LIB		= $(PTAUDIO)

BINNAME	= a.out
BIND	= ./bin/$(BINNAME)

SRCD	= ./src
SRC		= $(SRCD)/main.c

all: compile options
	@echo Finished Building $(NAME) v$(VER)

options:
	@echo Build Options:

test: compile
	$(BIND)

compile: $(SRCD)
	$(CC) $(CFLAGS) $(SRC) $(LIB) -o $(BIND)

install: all compile
	@echo "No install option at this moment"


.PHONY: all options clean install uninstall
