/*
 * Copyright (c) 2025 2bllw8
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "atom.h"
#include "env.h"
#include "err.h"

/*
 * Environment:
 *
 * (NIL
 *  (a . 2)
 *  (b . nil)
 *  (c . (x y)))
 * +---+-------+
 * | a | 2     |
 * | b | nil   |
 * | c | (x y) |
 * +---+-------+
 */

struct atom env_create(struct atom parent)
{
	return cons(parent, nil);
}

error env_get(struct atom env, struct atom symbol, struct atom *result)
{
	struct atom parent = car(env);
	struct atom bs = cdr(env);

	// Lookup binding in the current env
	while (!is_nil(bs)) {
		struct atom b = car(bs);
		if (car(b).value.symbol == symbol.value.symbol) {
			*result = cdr(b);
			return err_ok;
		}
		bs = cdr(bs);
	}

	// Look up the symbol in the parent env
	return is_nil(parent)
		? err_not_bound(symbol.value.symbol)
		: env_get(parent, symbol, result);
}

error env_set(struct atom env, struct atom symbol, struct atom value)
{
	struct atom bs = cdr(env);
	struct atom b = nil;

	// Try to update an existing binding
	while (!is_nil(bs)) {
		b = car(bs);
		if (car(b).value.symbol == symbol.value.symbol) {
			cdr(b) = value;
			return err_ok;
		}
		bs = cdr(bs);
	}

	// Create new binding
	b = cons(symbol, value);
	cdr(env) = cons(b, cdr(env));

	return err_ok;
}
