/*
 * Copyright (c) 2025 2bllw8
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "atom.h"
#include "builtins.h"
#include "env.h"
#include "err.h"
#include "eval.h"
#include "io.h"
#include "parser.h"
#include <stdlib.h>

void load_file(struct atom env, const char *path)
{
	char *content = read_file(path);
	if (!content) {
		return;
	}

	const char *p = content;
	struct atom expr;
	while (read_expr(p, &p, &expr).type == 0) {
		struct atom result;
		error err = eval_expr(env, expr, &result);
		if (err.type) {
			fprintf(stderr, "Error in expression:\n\t");
			print_expr(stderr, result);
			fprintf(stderr, "\n");
		} else {
			print_expr(stdout, result);
			fprintf(stdout, "\n");
		}
	}

	free(content);
}

void init_env(struct atom env)
{
	env_set(env, make_sym("car"), make_builtin(builtin_car));
	env_set(env, make_sym("cdr"), make_builtin(builtin_cdr));
	env_set(env, make_sym("cons"), make_builtin(builtin_cons));

	env_set(env, make_sym("+"), make_builtin(builtin_add));
	env_set(env, make_sym("-"), make_builtin(builtin_sub));
	env_set(env, make_sym("*"), make_builtin(builtin_mul));
	env_set(env, make_sym("/"), make_builtin(builtin_div));

	env_set(env, make_sym("#t"), make_sym("#t"));
	env_set(env, make_sym("#f"), nil);
	env_set(env, make_sym("not"), make_builtin(builtin_not));
	env_set(env, make_sym("="), make_builtin(builtin_integer_eq));
	env_set(env, make_sym(">"), make_builtin(builtin_integer_lt));
	env_set(env, make_sym("<"), make_builtin(builtin_integer_gt));

	env_set(env, make_sym("apply"), make_builtin(builtin_apply));

	env_set(env, make_sym("are-same"), make_builtin(builtin_are_same));
	env_set(env, make_sym("is-list"), make_builtin(builtin_is_list));

	load_file(env, "init.lisp");
}

int main(int argc, char **argv)
{
	char *input;
	struct atom env = env_create(nil);
	init_env(env);

	while ((input = readline("> ")) != NULL) {
		add_history(input);
		const char *p = input;
		struct atom expr;
		struct atom result;
		error err = read_expr(p, &p, &expr);
		if (!err.type) {
			err = eval_expr(env, expr, &result);
		}
		switch (err.type) {
		case err_t_ok:
			print_expr(stdout, result);
			fprintf(stdout, "\n");
			break;
		default:
			fprintf(stderr, "%s\n", err.message);
			break;
		}
	}

	return 0;
}
