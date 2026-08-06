//
// Copyright 2026 Hewlett Packard Enterprise Development LP
//
// SPDX-License-Identifier: BSD-2-Clause-Patent
//

#define D_LOGFAC DD_FAC(tests)

#include <daos/btree.h>

#include "target_btree_ops.h"
#include "target_btree_ik_tree.h"
#include "utest_common.h"

extern btr_ops_t ik_ops;

struct utest_context *ik_utx;

static struct btr_root *ik_root;
static struct umem_attr *ik_uma;

void
tb_init(void)
{
	int rc;

	/* dynamic_flag */
	rc = dbtree_class_register(IK_TREE_CLASS, BTR_FEAT_EMBED_FIRST | BTR_FEAT_UINT_KEY, &ik_ops);
	D_ASSERT(rc == 0);
	
	rc = utest_vmem_create(sizeof(*ik_root), &ik_utx);
	D_ASSERT(rc == 0);
	ik_root = utest_utx2root(ik_utx);
	ik_uma = utest_utx2uma(ik_utx);
}
