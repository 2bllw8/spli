# Copyright (c) 2025 2bllw8
# SPDX-License-Identifier: GPL-3.0-only

CC     ?= gcc
CFLAGS = -Wall -Werror
LDLIBS = -lreadline

ifeq ($(DEBUG),true)
CFLAGS += --enable-checking -g -O0 -fsanitize=address
endif

DEPS = atom.o \
       builtins.o \
       env.o \
       err.o \
       eval.o \
       io.o \
       parser.o

%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

main: main.o $(DEPS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDLIBS)

clean:
	rm -f *.o main

.DEFAULT_GOAL := main
