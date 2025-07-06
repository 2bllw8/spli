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
