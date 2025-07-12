/*
 * Copyright (c) 2025 2bllw8
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "builtins.h"
#include "atom.h"
#include "err.h"
#include "eval.h"
#include <stdbool.h>

error builtin_car(struct atom args, struct atom *result)
{
	if (is_nil(args) || !is_nil(cdr(args))) {
		return err_args("car takes 1 argument");
	}
	if (is_nil(car(args))) {
		*result = nil;
		return err_ok;
	}
	if (car(args).type == atom_t_list) {
		*result = car(car(args));
		return err_ok;
	}
	return err_type("car takes 1 list");
}

error builtin_cdr(struct atom args, struct atom *result)
{
	if (is_nil(args) || !is_nil(cdr(args))) {
		return err_args("cdr takes 1 argument");
	}
	if (is_nil(car(args))) {
		*result = nil;
		return err_ok;
	}
	if (car(args).type == atom_t_list) {
		*result = cdr(car(args));
		return err_ok;
	}
	return err_type("cdr takes 1 list");
}

error builtin_cons(struct atom args, struct atom *result)
{
	if (is_nil(args) || is_nil(cdr(args)) || !is_nil(cdr(cdr(args)))) {
		return err_args("cons takes 2 arguments");
	}

	*result = cons(car(args), car(cdr(args)));
	return err_ok;
}

#define bin_op(op, args, result)                                            \
	do {                                                                \
		if (is_nil((args)) || is_nil(cdr((args))) ||                \
		    !is_nil(cdr(cdr((args))))) {                            \
			return err_args("binary op takes 2 arguments");     \
		}                                                           \
		struct atom a = car((args));                                \
		struct atom b = car(cdr((args)));                           \
		if (a.type == atom_t_integer && b.type == atom_t_integer) { \
			*(result) = make_integer(                           \
				a.value.integer op b.value.integer);        \
			return err_ok;                                      \
		} else {                                                    \
			return err_type("binary op takes 2 integers");      \
		}                                                           \
	} while (0)

error builtin_add(struct atom args, struct atom *result)
{
	bin_op(+, args, result);
}

error builtin_sub(struct atom args, struct atom *result)
{
	bin_op(-, args, result);
}

error builtin_mul(struct atom args, struct atom *result)
{
	bin_op(*, args, result);
}

error builtin_div(struct atom args, struct atom *result)
{
	bin_op(/, args, result);
}

#undef bin_op

error builtin_not(struct atom args, struct atom *result)
{
	if (is_nil(args)) {
		return err_args("unary not expects 1 argument");
	}
	*result = is_nil(car(args)) ? make_sym("#t") : nil;
	return err_ok;
}

error builtin_same(struct atom args, struct atom *result)
{
	if (is_nil(args) || is_nil(cdr(args)) || !is_nil(cdr(cdr(args)))) {
		return err_args("same expects 2 arguments");
	}

	struct atom a = car(args);
	struct atom b = car(cdr(args));

	bool are_eq;

	if (a.type == b.type) {
		switch (a.type) {
		case atom_t_nil:
			are_eq = true;
			break;
		case atom_t_list:
		case atom_t_closure:
		case atom_t_macro:
			are_eq = a.value.list == b.value.list;
			break;
		case atom_t_symbol:
			are_eq = a.value.symbol == b.value.symbol;
			break;
		case atom_t_integer:
			are_eq = a.value.integer == b.value.integer;
			break;
		case atom_t_builtin:
			are_eq = a.value.builtin == b.value.builtin;
			break;
		default:
			return err_args("unknown atom type");
		}
	} else {
		are_eq = false;
	}

	*result = are_eq ? make_sym("#t") : nil;
	return err_ok;
}

#define integer_cmp(op, args, result)                                       \
	do {                                                                \
		if (is_nil((args)) || is_nil(cdr((args))) ||                \
		    !is_nil(cdr(cdr((args))))) {                            \
			return err_args(                                    \
				"integer comparison expects 2 arguments");  \
		}                                                           \
		struct atom a = car((args));                                \
		struct atom b = car(cdr((args)));                           \
		if (a.type == atom_t_integer && b.type == atom_t_integer) { \
			*(result) = (a.value.integer op b.value.integer) ?  \
					    make_sym("#t") :                \
					    nil;                            \
			return err_ok;                                      \
		}                                                           \
		return err_type("integer comparison takes 2 integers");     \
	} while (0)

error builtin_integer_eq(struct atom args, struct atom *result)
{
	integer_cmp(==, args, result);
}

error builtin_integer_gt(struct atom args, struct atom *result)
{
	integer_cmp(>, args, result);
}
error builtin_integer_lt(struct atom args, struct atom *result)
{
	integer_cmp(<, args, result);
}

#undef integer_cmp

error builtin_apply(struct atom args, struct atom *result)
{
	if (is_nil(args) || is_nil(cdr(args)) || !is_nil(cdr(cdr(args)))) {
		return err_args("apply expects 2 arguments");
	}

	struct atom fun = car(args);
	struct atom params = car(cdr(args));

	if (!is_list(params)) {
		return err_syntax(
			"apply expects the 2nd argument to be a list");
	}

	return apply(fun, params, result);
}
