/*
 * Copyright (c) 2025 2bllw8
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <stdlib.h>
#include <string.h>

#include "atom.h"
#include "err.h"
#include "parser.h"

/**
 * Find the begin and end of the next token.
 */
error lex_next_token(const char *s, const char **begin, const char **end)
{
	// Skip whitespace at the beginning
	s += strspn(s, " \t\n");
	if (s[0] == '\0') {
		// Empty!
		*begin = *end = NULL;
		return err_syntax("EOL");
	}

	// prefix: "()'"
	// delim:  "() \t\n"
	*begin = s;
	*end = s + (strchr("()'", s[0]) == NULL ? strcspn(s, "() \t\n") : 1);
	return err_ok;
}

error parse_simple(const char *begin, const char *end, struct atom *result)
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

error parse_list(const char *begin, const char **end, struct atom *result)
{
	struct atom p = nil;
	*result = nil;
	*end = begin;

	while (1) {
		const char *token;
		struct atom item;
		error err = lex_next_token(*end, &token, end);
		if (err.type) {
			return err;
		}
		if (token[0] == ')') {
			return err_ok;
		}
		if (token[0] == '.' && *end - token == 1) {
			if (is_nil(p)) {
				return err_syntax("expected expression");
			}
			err = read_expr(*end, end, &item);
			if (err.type) {
				return err;
			}
			cdr(p) = item;

			// Expected end of list
			err = lex_next_token(*end, &token, end);
			return err.type || token[0] == ')' ?
				       err :
				       err_syntax("missing expected )");
		}

		err = read_expr(token, end, &item);
		if (err.type) {
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

error parse_quote(const char **end, struct atom *result)
{
	*result = cons(make_sym("quote"), cons(nil, nil));
	return read_expr(*end, end, &car(cdr(*result)));
}

error read_expr(const char *input, const char **end, struct atom *result)
{
	const char *token;
	error err = lex_next_token(input, &token, end);
	if (err.type) {
		return err;
	}
	switch (token[0]) {
	case '(':
		return parse_list(*end, end, result);
	case ')':
		return err_syntax("Unexpected )");
	case '\'':
		return parse_quote(end, result);
	default:
		return parse_simple(token, *end, result);
	}
}
