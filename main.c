/*
 * Copyright (c) 2025 2bllw8
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "eval.h"
#include <stdio.h>
#include <readline/readline.h>

#include "err.h"
#include "env.h"
#include "io.h"
#include "parser.h"

int main(int argc, char **argv)
{
	char *input;
	struct atom env = env_create(nil);

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
