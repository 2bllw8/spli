/*
 * Copyright (c) 2025 2bllw8
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <stdio.h>
#include <stdlib.h>

#include "io.h"

void print_expr(FILE *file, struct atom atom)
{
	switch (atom.type) {
	case atom_t_nil:
		fprintf(file, "nil");
		break;
	case atom_t_list:
		fprintf(file, "(");
		print_expr(file, car(atom));
		atom = cdr(atom);
		while (!is_nil(atom)) {
			if (atom.type == atom_t_list) {
				fprintf(file, " ");
				print_expr(file, car(atom));
				atom = cdr(atom);
			} else {
				fprintf(file, " . ");
				print_expr(file, atom);
				break;
			}
		}
		fprintf(file, ")");
		break;
	case atom_t_symbol:
	case atom_t_macro:
		fprintf(file, "%s", atom.value.symbol);
		break;
	case atom_t_integer:
		fprintf(file, "%ld", atom.value.integer);
		break;
	case atom_t_builtin:
		fprintf(file, "#builtin@%p", atom.value.builtin);
		break;
	case atom_t_closure:
		fprintf(file, "#closure@%p", atom.value.list);
		break;
	default:
		fprintf(stderr, "? (%d)", atom.type);
		break;
	}
}

char *read_file(const char *path)
{
	FILE *file = fopen(path, "r");
	if (!file) {
		return NULL;
	}
	
	fseek(file, 0, SEEK_END);
	long len = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* buf = (char *) malloc(len + 1);
	if (!buf) {
		fclose(file);
		return NULL;
	}

	fread(buf, 1, len, file);
	buf[len] = '\0';
	fclose(file);
	return buf;
}

