/*
 * Copyright (c) 2025 2bllw8
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <stdio.h>
#include <readline/readline.h>

#include "atom.h"
#include "builtins.h"
#include "env.h"
#include "err.h"
#include "eval.h"
#include "io.h"
#include "parser.h"

int main(int argc, char **argv)
{
	char *input;
	struct atom env = env_create(nil);

	env_set(env, make_sym("car"), make_builtin(builtin_car));
	env_set(env, make_sym("cdr"), make_builtin(builtin_cdr));
	env_set(env, make_sym("cons"), make_builtin(builtin_cons));

	env_set(env, make_sym("+"), make_builtin(builtin_add));
	env_set(env, make_sym("-"), make_builtin(builtin_sub));
	env_set(env, make_sym("*"), make_builtin(builtin_mul));
	env_set(env, make_sym("/"), make_builtin(builtin_div));

	while ((input = readline("> ")) != NULL) {
		const char *p = input;
		struct atom expr;
		struct atom result;
		error err = read_expr(p, &p, &expr);
		if (!err.type) {
			err = eval_expr(env, expr, &result);
		}
		switch (err.type) {
		case err_t_ok:
			print_expr(result);
			putchar('\n');
			break;
		default:
			puts(err.message);
			break;
		}
	}

	return 0;
}
