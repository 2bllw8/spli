/*
 * Copyright (c) 2025 2bllw8
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "atom.h"
#include <stdio.h>

#include "io.h"

void print_expr(struct atom atom)
{
	switch (atom.type) {
	case atom_t_nil:
		printf("nil");
		break;
	case atom_t_list:
		putchar('(');
		print_expr(car(atom));
		atom = cdr(atom);
		while (!is_nil(atom)) {
			if (atom.type == atom_t_list) {
				putchar(' ');
				print_expr(car(atom));
				atom = cdr(atom);
			} else {
				printf(" . ");
				print_expr(atom);
				break;
			}
		}
		putchar(')');
		break;
	case atom_t_symbol:
		printf("%s", atom.value.symbol);
		break;
	case atom_t_integer:
		printf("%ld", atom.value.integer);
		break;
	case atom_t_builtin:
		printf("#builtin@%p", atom.value.builtin);
		break;
	}
}
