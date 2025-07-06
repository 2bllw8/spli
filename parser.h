/*
 * Copyright (c) 2025 2bllw8
 * SPDX-License-Identifier: GPL-3.0-only
 */

#ifndef SPLI_PARSER_H
#define SPLI_PARSER_H

#include "atom.h"
#include "err.h"

/**
 * Parse the input to read an expression.
 */
error read_expr(const char *input, const char **end, struct atom *result);

#endif
