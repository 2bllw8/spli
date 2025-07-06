/*
 * Copyright (c) 2025 2bllw8
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <string.h>

#include "atom.h"
#include "env.h"
#include "err.h"
#include "eval.h"

error eval_symbol(struct atom env, struct atom expr, struct atom *result)
{
	return env_get(env, expr, result);
}

error eval_itself(struct atom expr, struct atom *result)
{
	*result = expr;
	return err_ok;
}

struct atom copy_list(struct atom list)
{
	if (is_nil(list)) {
		return nil;
	}

	struct atom a = cons(car(list), nil);
	struct atom p = a;
	list = cdr(list);
	while (!is_nil(list)) {
		cdr(p) = cons(car(list), nil);
		p = cdr(p);
		list = cdr(list);
	}
	return a;
}

error apply(struct atom f, struct atom args, struct atom *result)
{
	return f.type == atom_t_builtin ? (*f.value.builtin)(args, result) :
					  err_type;
}

error eval_list(struct atom env, struct atom expr, struct atom *result)
{
	struct atom op = car(expr);
	struct atom args = cdr(expr);

	if (op.type == atom_t_symbol) {
		if (strcmp(op.value.symbol, "define") == 0) {
			// define
			if (is_nil(args) || is_nil(cdr(args)) ||
			    !is_nil(cdr(cdr(args)))) {
				return err_args(
					"define expects 1 symbol and 1 expression");
			}
			struct atom sym = car(args);
			if (sym.type != atom_t_symbol) {
				return err_type(
					"first argument of define must be a symbol");
			}
			struct atom value;
			error err = eval_expr(env, car(cdr(args)), &value);
			if (err.type) {
				return err;
			}
			*result = sym;
			return env_set(env, sym, value);
		} else if (strcmp(op.value.symbol, "quote") == 0) {
			// quote
			if (is_nil(args) || !is_nil(cdr(args))) {
				return err_args("quote expects 1 argument");
			}
			*result = car(args);
			return err_ok;
		} else if (strcmp(op.value.symbol, "λ") == 0 ||
			   strcmp(op.value.symbol, "\\") == 0 ||
			   strcmp(op.value.symbol, "lambda") == 0) {
			if (is_nil(args) || is_nil(cdr(args))) {
				return err_args("lambda expects 2 arguments");
			}
			return make_closure(env, car(args), cdr(args), result);
		}
	}

	// Eval operator
	error err = eval_expr(env, op, &op);
	if (err.type) {
		return err;
	}

	// Eval args
	args = copy_list(args);
	struct atom p = args;
	while (!is_nil(p)) {
		err = eval_expr(env, car(p), &car(p));
		if (err.type) {
			return err;
		}
		p = cdr(p);
	}

	return apply(op, args, result);
}

error eval_expr(struct atom env, struct atom expr, struct atom *result)
{
	switch (expr.type) {
	case atom_t_symbol:
		return eval_symbol(env, expr, result);
	case atom_t_list:
		return eval_list(env, expr, result);
	case atom_t_builtin:
	case atom_t_nil:
	case atom_t_integer:
		return eval_itself(expr, result);
	default:
		return err_syntax("???");
	}
}
