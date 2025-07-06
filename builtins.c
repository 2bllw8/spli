/*
 * Copyright (c) 2025 2bllw8
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "atom.h"
#include "err.h"

#include "builtins.h"

int builtin_car(struct atom args, struct atom *result)
{
	if (is_nil(args) || !is_nil(cdr(args))) {
		return err_args;
	}
	if (is_nil(car(args))) {
		*result = nil;
		return err_ok;
	}
	if (car(args).type == atom_t_list) {
		*result = car(car(args));
		return err_ok;
	}
	return err_type;
}

int builtin_cdr(struct atom args, struct atom *result)
{
	if (is_nil(args) || !is_nil(cdr(args))) {
		return err_args;
	}
	if (is_nil(car(args))) {
		*result = nil;
		return err_ok;
	}
	if (car(args).type == atom_t_list) {
		*result = cdr(car(args));
		return err_ok;
	}
	return err_type;
}

int builtin_cons(struct atom args, struct atom *result)
{
	if (is_nil(args) || is_nil(cdr(args)) || !is_nil(cdr(cdr(args)))) {
		return err_args;
	}

	*result = cons(car(args), car(cdr(args)));
	return err_ok;
}

#define bin_op(op, args, result)                                            \
	do {                                                                \
		if (is_nil((args)) || is_nil(cdr((args))) ||                \
		    !is_nil(cdr(cdr((args))))) {                            \
			return err_args;                                    \
		}                                                           \
		struct atom a = car((args));                                \
		struct atom b = car(cdr((args)));                           \
		if (a.type == atom_t_integer && b.type == atom_t_integer) { \
			*(result) = make_integer(                           \
				a.value.integer op b.value.integer);        \
			return err_ok;                                      \
		} else {                                                    \
			return err_type;                                    \
		}                                                           \
	} while (0)

int builtin_add(struct atom args, struct atom *result)
{
	bin_op(+, args, result);
}

int builtin_sub(struct atom args, struct atom *result)
{
	bin_op(-, args, result);
}

int builtin_mul(struct atom args, struct atom *result)
{
	bin_op(*, args, result);
}

int builtin_div(struct atom args, struct atom *result)
{
	bin_op(/, args, result);
}
