# Copyright (c) 2025 2bllw8
# SPDX-License-Identifier: GPL-3.0-only

CC     ?= gcc
CFLAGS = -Wall -Werror
LDLIBS = -lreadline

DEPS = atom.o \
       env.o \
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
