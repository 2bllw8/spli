/*
 * Copyright (c) 2025 2bllw8
 * SPDX-License-Identifier: GPL-3.0-only
 */

#ifndef SPLI_DEBUG_H
#define SPLI_DEBUG_H

#include "stdio.h"

#define DEBUG(s) printf("%s:%d: %s\n", __FILE__, __LINE__, (s));

#endif
