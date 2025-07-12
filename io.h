/*
 * Copyright (c) 2025 2bllw8
 * SPDX-License-Identifier: GPL-3.0-only
 */

#ifndef SPLI_IO_H
#define SPLI_IO_H

#include <stdio.h>

#include "atom.h"

void print_expr(FILE *file, struct atom atom);

char *read_file(const char *path);

#endif
