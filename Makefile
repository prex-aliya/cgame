NAME 	= cGAME
VER		= 0.0.7
# 		  ^ ^ ^- PATCH, spelling, small update.
#		  | +--- MINOR, fatel bugs, etc.
#		  +----- MAJOR, updates, Minor features, etc.
# Semantic Versioning: https://semver.org/
# v1.0.0 will be offical release.

# Compiler and Linker
CC	= gcc

OPP	= -Ofast
SOUND	= -lrt -lm
THREAD	= -lpthread
WARN	= -Wall -Werror -Wpedantic
CFLAGS	= $(WARN) $(OPP) $(THREAD) $(SOUND)

LIBD	= ./lib
PTAUDIO	= $(LIBD)/libportaudio.a
LIB	= $(PTAUDIO)

BINNAME	= a.out
BIND	= ./bin/$(BINNAME)

SRCD	= ./src
SRCM	= $(SRCD)/main.c
SRC	= $(SRCM)


debug:
	CFLAGS += -D DEBUG_ENABLE=true


all: compile options
	@echo Finished Building $(NAME) v$(VER)

options:
	@echo Build Options:

test: compile
	$(BIND)

compile: $(SRCD)
	$(CC) $(CFLAGS) $(SRC) $(LIB) -o $(BIND)

install: all compile
	@echo "No install option at the moment"


.PHONY: all options clean install uninstall
