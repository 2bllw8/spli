/*
 * Copyright (c) 2025 2bllw8
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "atom.h"
#include "err.h"
#include <stdlib.h>
#include <string.h>

#include "parser.h"

/**
 * Find the begin and end of the next token.
 */
err_t lex_next_token(const char *s, const char **begin, const char **end)
{
	// Skip whitespace at the beginning
	s += strspn(s, " \t\n");
	if (s[0] == '\0') {
		// Empty!
		*begin = *end = NULL;
		return err_syntax;
	}

	// prefix: "()"
	// delim:  "() \t\n"
	*begin = s;
	*end = s + (strchr("()", s[0]) == NULL ? strcspn(s, "() \t\n") : 1);
	return err_ok;
}

int parse_simple(const char *begin, const char *end, struct atom *result)
{
	char *p;
	// Integer
	long val = strtol(begin, &p, 10);
	if (p == end) {
		result->type = atom_t_integer;
		result->value.integer = val;
		return err_ok;
	}

	// Symbols
	char *buf = malloc(end - begin + 1);
	p = buf;
	while (begin != end) {
		*p = *begin;
		p++;
		begin++;
	}
	*p = '\0';

	*result = strcmp(buf, "nil") == 0 ? nil : make_sym(buf);
	free(buf);
	return err_ok;
}

int parse_list(const char *begin, const char **end, struct atom *result)
{
	struct atom p = nil;
	*result = nil;
	*end = begin;

	while (1) {
		const char *token;
		struct atom item;
		err_t err = lex_next_token(*end, &token, end);
		if (err) {
			return err;
		}
		if (token[0] == ')') {
			return err_ok;
		}
		if (token[0] == '.' && *end - token == 1) {
			if (is_nil(p)) {
				return err_syntax;
			}
			err = read_expr(*end, end, &item);
			if (err) {
				return err;
			}
			cdr(p) = item;

			// Expected end of list
			err = lex_next_token(*end, &token, end);
			return err || token[0] == ')' ? err : err_syntax;
		}

		err = read_expr(token, end, &item);
		if (err) {
			return err;
		}
		if (is_nil(p)) {
			// First item
			*result = cons(item, nil);
			p = *result;
		} else {
			// Other item
			cdr(p) = cons(item, nil);
			p = cdr(p);
		}
	}
}

err_t read_expr(const char *input, const char **end, struct atom *result)
{
	const char *token;
	err_t err = lex_next_token(input, &token, end);
	if (err) {
		return err;
	}
	switch (token[0]) {
	case '(':
		return parse_list(*end, end, result);
	case ')':
		return err_syntax;
	default:
		return parse_simple(token, *end, result);
	}
}
