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
static bool ik_inplace;
static daos_handle_t ik_toh = DAOS_HDL_INVAL;
static umem_off_t ik_root_off = UMOFF_NULL;

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

void
tb_create_cmd(bool feat_uint_key, bool feat_embed_first, bool inplace, uint32_t order)
{
	uint64_t feats = 0;
	int rc;
	int      rc_exp = 0;

	if (feat_uint_key) {
		feats += BTR_FEAT_UINT_KEY;
	}
	if (feat_embed_first) {
		feats += BTR_FEAT_EMBED_FIRST;
	}

	if (order == 0){
		order = IK_ORDER_DEF;
	}

	ik_inplace = inplace;

	/* If the tree is already there */
	if (daos_handle_is_valid(ik_toh)) {
		rc_exp = -DER_NO_PERM;
	}

	if (ik_inplace) {
		rc = dbtree_create_inplace(IK_TREE_CLASS, feats, order, ik_uma, ik_root, &ik_toh);
	} else {
		rc = dbtree_create(IK_TREE_CLASS, feats, order, ik_uma, &ik_root_off, &ik_toh);
	}

	assert(rc == rc_exp);
}

void
tb_close_cmd()
{
	int rc_exp = daos_handle_is_valid(ik_toh) ? 0 : -DER_NO_HDL;
	int rc = dbtree_close(ik_toh);
	assert(rc == rc_exp);
	if (rc_exp == 0) {
		ik_toh = DAOS_HDL_INVAL;
	}
}

void
tb_destroy_cmd()
{
	int rc_exp = daos_handle_is_valid(ik_toh) ? 0 : -DER_NO_HDL;
	int rc = dbtree_destroy(ik_toh, NULL);
	assert(rc == rc_exp);
	if (rc_exp == 0) {
		ik_toh = DAOS_HDL_INVAL;
	}
}

void
tb_open_cmd()
{
	int rc;

	/* dbtree_open*() has no safeguards. */
	if (daos_handle_is_valid(ik_toh)) {
		return;
	}

	if (ik_inplace) {
		rc = dbtree_open_inplace(ik_root, ik_uma, &ik_toh);
	} else {
		rc = dbtree_open(ik_root_off, ik_uma, &ik_toh);
	}

	assert(rc == 0);
}

void
tb_update_cmd()
{

}
