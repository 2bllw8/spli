/*
 * Copyright (c) 2025 2bllw8
 * SPDX-License-Identifier: GPL-3.0-only
 */

#ifndef SPLI_EVAL_H
#define SPLI_EVAL_H

#include "atom.h"
#include "err.h"

error eval_expr(struct atom env, struct atom atom, struct atom *result);
error apply(struct atom f, struct atom args, struct atom *result);

#endif
