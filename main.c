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

	while ((input = readline("> ")) != NULL) {
		const char *p = input;
		struct atom expr;
		struct atom result;
		err_t err = read_expr(p, &p, &expr);
		if (!err) {
			err = eval_expr(env, expr, &result);
		}
		switch (err) {
		case err_ok:
			print_expr(result);
			putchar('\n');
			break;
		case err_syntax:
			puts("Syntax error!");
			break;
		case err_not_bound:
			puts("Symbol not bound");
			break;
		case err_args:
			puts("Wrong number of arguments");
			break;
		case err_type:
			puts("Wrong type");
			break;
		}
	}

	return 0;
}
