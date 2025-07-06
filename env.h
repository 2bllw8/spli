/*
 * Copyright (c) 2025 2bllw8
 * SPDX-License-Identifier: GPL-3.0-only
 */

#ifndef SPLI_ENV_H
#define SPLI_ENV_H

#include "atom.h"
#include "err.h"

struct atom env_create(struct atom parent);
err_t env_get(struct atom env, struct atom symbol, struct atom *result);
err_t env_set(struct atom env, struct atom symbol, struct atom value);

#endif
