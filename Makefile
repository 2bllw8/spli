# Copyright (c) 2025 2bllw8
# SPDX-License-Identifier: GPL-3.0-only

CC     ?= gcc
CFLAGS = -Wall -Werror

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: main.o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o main

.DEFAULT_GOAL := main
