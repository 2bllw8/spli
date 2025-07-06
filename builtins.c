/*
 * Copyright (c) 2025 2bllw8
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "atom.h"
#include "err.h"

#include "builtins.h"

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
			return err_type("binary op takes two integers");    \
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
