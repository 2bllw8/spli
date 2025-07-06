/*
 * Copyright (c) 2025 2bllw8
 * SPDX-License-Identifier: GPL-3.0-only
 */

#ifndef SPLI_BUILTINS_H
#define SPLI_BUILTINS_H

#include "atom.h"
#include "err.h"

error builtin_car(struct atom args, struct atom *result);
error builtin_cdr(struct atom args, struct atom *result);
error builtin_cons(struct atom args, struct atom *result);

error builtin_add(struct atom args, struct atom *result);
error builtin_sub(struct atom args, struct atom *result);
error builtin_mul(struct atom args, struct atom *result);
error builtin_div(struct atom args, struct atom *result);

#endif
