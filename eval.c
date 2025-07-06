/*
 * Copyright (c) 2025 2bllw8
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <string.h>

#include "atom.h"
#include "env.h"
#include "err.h"
#include "eval.h"

err_t eval_symbol(struct atom env, struct atom expr, struct atom *result)
{
	return env_get(env, expr, result);
}

err_t eval_itself(struct atom expr, struct atom *result)
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

err_t apply(struct atom f, struct atom args, struct atom *result)
{
	return f.type == atom_t_builtin ? (*f.value.builtin)(args, result) :
					  err_type;
}

err_t eval_list(struct atom env, struct atom expr, struct atom *result)
{
	struct atom op = car(expr);
	struct atom args = cdr(expr);

	if (op.type == atom_t_symbol) {
		if (strcmp(op.value.symbol, "define") == 0) {
			// define
			if (is_nil(args) || is_nil(cdr(args)) ||
			    !is_nil(cdr(cdr(args)))) {
				return err_args;
			}
			struct atom sym = car(args);
			if (sym.type != atom_t_symbol) {
				return err_type;
			}
			struct atom value;
			err_t err = eval_expr(env, car(cdr(args)), &value);
			if (err) {
				return err;
			}
			*result = sym;
			return env_set(env, sym, value);
		} else if (strcmp(op.value.symbol, "quote") == 0) {
			// quote
			if (is_nil(args) || !is_nil(cdr(args))) {
				return err_args;
			}
			*result = car(args);
			return err_ok;
		}
	}

	// Eval operator
	err_t err = eval_expr(env, op, &op);
	if (err) {
		return err;
	}

	// Eval args
	args = copy_list(cdr(expr));
	struct atom p = args;
	while (!is_nil(p)) {
		err = eval_expr(env, car(p), &car(p));
		if (err) {
			return err;
		}
		p = cdr(p);
	}

	return apply(op, args, result);
}

err_t eval_expr(struct atom env, struct atom expr, struct atom *result)
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
		return err_syntax;
	}
}
