/*
 * Copyright (c) 2025 2bllw8
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "atom.h"

struct atom cons(struct atom car_val, struct atom cdr_val)
{
	struct atom p = {
		.type = atom_t_list,
		.value.list = malloc(sizeof(struct list)),
	};
	car(p) = car_val;
	cdr(p) = cdr_val;
	return p;
}

static struct atom sym_table = { atom_t_nil };

struct atom make_sym(const char *s)
{
	// TODO: replace with hash table
	struct atom p = sym_table;
	struct atom a;
	// Look up symbol
	while (!is_nil(p)) {
		a = car(p);
		if (strcmp(a.value.symbol, s) == 0) {
			return a;
		} else {
			p = cdr(p);
		}
	}

	// Create new symbol
	a.type = atom_t_symbol;
	a.value.symbol = strdup(s);
	sym_table = cons(a, sym_table);
	return a;
}

struct atom make_builtin(builtin_function f)
{
	struct atom a = {
		.type = atom_t_builtin,
		.value.builtin = f,
	};
	return a;
}

bool is_list(struct atom expr)
{
	while (!is_nil(expr)) {
		if (expr.type != atom_t_list) {
			return false;
		}
		expr = cdr(expr);
	}
	return true;
}
