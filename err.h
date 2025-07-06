/*
 * Copyright (c) 2025 2bllw8
 * SPDX-License-Identifier: GPL-3.0-only
 */

#ifndef SPLI_ERR_H
#define SPLI_ERR_H

typedef enum {
	err_t_ok = 0,
	/**
	 * Syntax error.
	 */
	err_t_syntax,
	/**
	 * Name not bound.
	 */
	err_t_not_bound,
	/**
	 * Mismatched number of arguments.
	 */
	err_t_args,
	/**
	 * Type error.
	 */
	err_t_type,
} err_t;

typedef struct {
	err_t type;
	const char *message;
} error;

static const error err_ok = { .type = err_t_ok };

#define err_syntax(s) (make_err(err_t_syntax, __FILE__, __LINE__, (s)))
#define err_not_bound(s) (make_err(err_t_not_bound, __FILE__, __LINE__, (s)))
#define err_args(s) (make_err(err_t_args, __FILE__, __LINE__, (s)))
#define err_type(s) (make_err(err_t_type, __FILE__, __LINE__, (s)))

error make_err(err_t type, const char *file, int line, const char *message);

#endif
