/*
 * Copyright (c) 2025 2bllw8
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <stdio.h>

#include "err.h"

error make_err(err_t type, const char *file, int line, const char *message)
{
	const char *type_str;
	switch (type) {
	case err_t_ok:
		type_str = "ok";
		break;
	case err_t_syntax:
		type_str = "syntax error";
		break;
	case err_t_not_bound:
		type_str = "name error";
		break;
	case err_t_args:
		type_str = "args mismatch error";
		break;
	case err_t_type:
		type_str = "type error";
		break;
	}
	char *msg;
	asprintf(&msg, "[%s:%03d] %s: %s", file, line, type_str, message);
	error err = {
		.type = type,
		.message = msg,
	};
	return err;
}
