//
// Copyright 2026 Hewlett Packard Enterprise Development LP
//
// SPDX-License-Identifier: BSD-2-Clause-Patent
//

#define D_LOGFAC DD_FAC(tests)

#include <daos/btree.h>

int daos_tree_logfac_cache[24];
int daos_common_logfac_cache[24];
int daos_tree_logfac;
int daos_common_logfac;

void daos_fuzz_btree(double a, uint64_t b)
{
	struct btr_root ik_root = {};

	(void)a;
	(void)b;
	(void)ik_root;

        abort();
}
