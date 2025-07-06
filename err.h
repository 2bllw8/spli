/*
 * Copyright (c) 2025 2bllw8
 * SPDX-License-Identifier: GPL-3.0-only
 */

#ifndef SPLI_ERR_H
#define SPLI_ERR_H

typedef enum {
	err_ok = 0,
	/**
	 * Syntax error.
	 */
	err_syntax,
	/**
	 * Name not bound.
	 */
	err_not_bound,
	/**
	 * Mismatched number of arguments.
	 */
	err_args,
	/**
	 * Type error.
	 */
	err_type,
} err_t;

#endif
