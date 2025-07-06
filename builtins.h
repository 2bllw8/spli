/*
 * Copyright (c) 2025 2bllw8
 * SPDX-License-Identifier: GPL-3.0-only
 */

#ifndef SPLI_BUILTINS_H
#define SPLI_BUILTINS_H

#include "atom.h"
#include "err.h"

int builtin_car(struct atom args, struct atom *result);
int builtin_cdr(struct atom args, struct atom *result);
int builtin_cons(struct atom args, struct atom *result);

#endif
