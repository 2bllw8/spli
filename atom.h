/*
 * Copyright (c) 2025 2bllw8
 * SPDX-License-Identifier: GPL-3.0-only
 */

#ifndef SPLI_ATOM_H
#define SPLI_ATOM_H

#include <stdbool.h>

struct atom {
	enum { atom_t_nil, atom_t_list, atom_t_symbol, atom_t_integer } type;
	union {
		struct list *list;
		const char *symbol;
		long integer;
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

bool is_list(struct atom expr);

#endif
