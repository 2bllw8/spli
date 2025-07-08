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
	if (f.type == atom_t_builtin) {
		return (*f.value.builtin)(args, result);
	}
	if (f.type != atom_t_closure) {
		return err_type("cannot apply something that is not a closure");
	}

	// (env (params...) expr...)
	struct atom env = env_create(car(f));
	struct atom params = car(cdr(f));
	struct atom body = cdr(cdr(f));

	// Bind the arguments
	while (!is_nil(params)) {
		if (params.type == atom_t_symbol) {
			env_set(env, params, args);
			args = nil;
			break;
		}
		if (is_nil(args)) {
			// Mismatched count
			return err_args("missing arguments");
		}
		env_set(env, car(params), car(args));
		params = cdr(params);
		args = cdr(args);
	}
	if (!is_nil(args)) {
		// More args than parameters
		return err_args("too many arguments");
	}
	// Evaluate the body
	while (!is_nil(body)) {
		error err = eval_expr(env, car(body), result);
		if (err.type) {
			return err;
		}
		body = cdr(body);
	}
	return err_ok;
}

error eval_define(struct atom env, struct atom args, struct atom *result)
{
	if (is_nil(args) || is_nil(cdr(args))) {
		return err_args("define expects at least 2 arguments");
	}
	struct atom sym = car(args);
	struct atom value;
	if (sym.type == atom_t_list) {
		error err = make_closure(env, cdr(sym), cdr(args), &value);
		if (err.type) {
			return err;
		}
		sym = car(sym);
		if (sym.type != atom_t_symbol) {
			return err_type("missing body of function definition");
		}
	} else if (sym.type == atom_t_symbol) {
		if (!is_nil(cdr(cdr(args)))) {
			return err_args("symbol define expects 2 arguments");
		}
		error err = eval_expr(env, car(cdr(args)), &value);
		if (err.type) {
			return err;
		}
	}
	*result = sym;
	return env_set(env, sym, value);
}

error eval_quote(struct atom args, struct atom *result)
{
	if (is_nil(args) || !is_nil(cdr(args))) {
		return err_args("quote expects 1 argument");
	}
	*result = car(args);
	return err_ok;
}

error eval_lambda(struct atom env, struct atom args, struct atom *result)
{
	if (is_nil(args) || is_nil(cdr(args))) {
		return err_args("lambda expects 2 arguments");
	}
	return make_closure(env, car(args), cdr(args), result);
}

error eval_if(struct atom env, struct atom args, struct atom *result)
{
	if (is_nil(args) || is_nil(cdr(args)) || is_nil(cdr(cdr(args))) ||
	    !is_nil(cdr(cdr(cdr(args))))) {
		return err_args("if expects 3 arguments");
	}
	struct atom cond;
	error err = eval_expr(env, car(args), &cond);
	if (err.type) {
		return err;
	}
	struct atom branch = is_nil(cond) ? car(cdr(cdr(args))) :
					    car(cdr(args));
	return eval_expr(env, branch, result);
}

error eval_defmacro(struct atom env, struct atom args, struct atom *result)
{
	if (is_nil(args) || is_nil(cdr(args))) {
		return err_args("defmacro expects at least 2 arguments");
	}
	if (car(args).type != atom_t_list) {
		return err_type("first argument of defmacro must be a list");
	}

	struct atom name = car(car(args));
	if (name.type != atom_t_symbol) {
		return err_args("name of defmacro is not a symbol");
	}
	struct atom macro;
	error err = make_closure(env, cdr(car(args)), cdr(args), &macro);
	if (err.type) {
		return err;
	}
	macro.type = atom_t_macro;
	*result = name;
	return env_set(env, name, macro);
}

error eval_macro(struct atom env, struct atom op, struct atom args,
		 struct atom *result)
{
	struct atom expansion;
	op.type = atom_t_closure;
	error err = apply(op, args, &expansion);
	if (err.type) {
		return err;
	}
	return eval_expr(env, expansion, result);
}

error eval_list(struct atom env, struct atom expr, struct atom *result)
{
	struct atom op = car(expr);
	struct atom args = cdr(expr);

	if (op.type == atom_t_symbol) {
		if (strcmp(op.value.symbol, "define") == 0) {
			return eval_define(env, args, result);
		} else if (strcmp(op.value.symbol, "quote") == 0) {
			return eval_quote(args, result);
		} else if (strcmp(op.value.symbol, "λ") == 0 ||
			   strcmp(op.value.symbol, "\\") == 0 ||
			   strcmp(op.value.symbol, "lambda") == 0) {
			return eval_lambda(env, args, result);
		} else if (strcmp(op.value.symbol, "if") == 0) {
			return eval_if(env, args, result);
		} else if (strcmp(op.value.symbol, "defmacro") == 0) {
			return eval_defmacro(env, args, result);
		}
	}

	// Eval operator
	error err = eval_expr(env, op, &op);
	if (err.type) {
		return err;
	}

	// Eval macro
	if (op.type == atom_t_macro) {
		return eval_macro(env, op, args, result);
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
	case atom_t_macro:
		return eval_list(env, expr, result);
	case atom_t_builtin:
	case atom_t_nil:
	case atom_t_integer:
		return eval_itself(expr, result);
	default:
		return err_syntax("???");
	}
}
