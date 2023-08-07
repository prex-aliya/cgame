VERSION = 0.0.0
# 		  ^ ^ ^- Patches
#		  | +--- Minor bugs, spelling mistakes, etc.
#		  +----- Major updates, Minor features, etc.
# Semantic Versioning: https://semver.org/

# Compiler and Linker
CC = cc
BINNAME = a.out
BIND = ./bin/$(BINNAME)

SRCD = ./src
SRC = $(SRCD)/main.c

all: compile options

options:
	@echo build options

test: compile
	$(BIND)

compile:
	$(CC) -o $(BIND) $(SRC)

install: all compile
	@echo "No install option at this moment"


.PHONY: all options clean install uninstall
