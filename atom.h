/*
 * Copyright (c) 2025 2bllw8
 * SPDX-License-Identifier: GPL-3.0-only
 */

#ifndef SPLI_ATOM_H
#define SPLI_ATOM_H

#include <stdbool.h>

#include "err.h"

struct atom;

typedef error (*builtin_function)(struct atom args, struct atom *result);

struct atom {
	enum {
		atom_t_nil,
		atom_t_builtin,
		atom_t_closure,
		atom_t_integer,
		atom_t_list,
		atom_t_symbol,
	} type;
	union {
		builtin_function builtin;
		long integer;
		struct list *list;
		const char *symbol;
	} value;
};

struct list {
	struct atom atom[2];
};

#define car(p) ((p).value.list->atom[0])
#define cdr(p) ((p).value.list->atom[1])
#define is_nil(atom) ((atom).type == atom_t_nil)

static const struct atom nil = {
	.type = atom_t_nil,
};

struct atom cons(struct atom car_val, struct atom cdr_val);
struct atom make_sym(const char *s);
struct atom make_builtin(builtin_function f);
struct atom make_integer(long integer);
error make_closure(struct atom env, struct atom args, struct atom body,
		   struct atom *result);

bool is_list(struct atom expr);

#endif
