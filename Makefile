#
# Makefile for Randomizer
#
CC=g++
CFLAGS+=
CPPFLAGS+=
LDLIBS=
SRC_FILES=$(wildcard *.cpp)
BIN_FILE=randomizer

.PHONY: all
all: $(BIN_FILE)

$(BIN_FILE): $(SRC_FILES)

.PHONY: debug
debug: CPPFLAGS+=-D_DEBUG_
debug: $(BIN_FILE)

.PHONY: clean
clean:
	$(RM) $(BIN_FILE)
