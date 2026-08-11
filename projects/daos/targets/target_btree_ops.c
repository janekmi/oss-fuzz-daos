//
// Copyright 2026 Hewlett Packard Enterprise Development LP
//
// SPDX-License-Identifier: BSD-2-Clause-Patent
//

#define D_LOGFAC DD_FAC(tests)

#include <daos/btree.h>

#include "target_btree_ops.h"
#include "target_btree_ik_tree.h"
#include "records.h"
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

	srand(42);

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

#define TB_UPDATE_ENTRIES_MAX 30

void
tb_update_cmd(uint32_t entries_num)
{
	const int use_existing_chance = 30;
	int idx;
	d_iov_t	key_iov;
	d_iov_t	val_iov;
	int rc;

	if (!daos_handle_is_valid(ik_toh)) {
		uint64_t blob = rand();
		d_iov_set(&key_iov, &blob, sizeof(blob));
		d_iov_set(&val_iov, &blob, sizeof(blob));
		rc = dbtree_update(ik_toh, &key_iov, &val_iov);
		assert(rc == -DER_NO_HDL);
		return;
	}

	entries_num = entries_num % TB_UPDATE_ENTRIES_MAX + 1;

	for (uint32_t i = 0; i < entries_num; ++i) {
		// if possible give a chance to use an existing key
		bool use_existing = ((rand() % 100) < use_existing_chance);
		if (records_used == 0) {
			use_existing = false;
		}
		if (use_existing) {
			idx = rand() % records_used;
		} else {
			idx = record_get_empty();
			records[idx].key = rand();
		}
		// rand a new value
		value_rand(&records[idx]);
		// update the tree
		d_iov_set(&key_iov, &records[idx].key, sizeof(records[idx].key));
		d_iov_set(&val_iov, records[idx].value,
			sizeof(char) * records[idx].value_size);
		rc = dbtree_update(ik_toh, &key_iov, &val_iov);
		assert(rc == 0);
	}
}

static int
op_iter_probe_rand(daos_handle_t ih) {
	// if (records_used == 0) {
	// 	return -DER_NONEXIST;
	// }
	dbtree_probe_opc_t opc = BTR_PROBE_FIRST;
	int idx;
	d_iov_t	key_iov;
	d_iov_t	*key_iovp = NULL;
	int rc;
	int                rc_exp = 0;
	switch (rand() % 3) {
	case 0:
		opc = BTR_PROBE_FIRST;
		break;
	case 1:
		opc = BTR_PROBE_LAST;
		break;
	case 2:
		opc = BTR_PROBE_EQ;
		break;
		/* XXX: Probes? */
	}
	if (opc == BTR_PROBE_EQ) {
		if (records_used > 0) {
			idx = rand() % records_used;
			d_iov_set(&key_iov, &records[idx].key, sizeof(records[idx].key));
			key_iovp = &key_iov;
		} else {
			opc = BTR_PROBE_FIRST; /* fall back */
		}
	}
	if (records_used == 0) {
		rc_exp = -DER_NONEXIST;
	}
	/* XXX: Other intents? */
	/* XXX: Anchors? */
	rc = dbtree_iter_probe(ih, opc, DAOS_INTENT_DEFAULT, key_iovp, NULL);
	assert(rc == rc_exp);
	return rc;
}

void
tb_iter_cmd(uint32_t entries_num) {
	daos_handle_t ih;
	d_iov_t	key_iov;
	d_iov_t	val_iov;
	int rc;

	if (!daos_handle_is_valid(ik_toh)) {
		uint64_t blob = rand();
		d_iov_set(&key_iov, &blob, sizeof(blob));
		d_iov_set(&val_iov, &blob, sizeof(blob));
		rc = dbtree_update(ik_toh, &key_iov, &val_iov);
		assert(rc == -DER_NO_HDL);
		return;
	}

	rc = dbtree_iter_prepare(ik_toh, BTR_ITER_EMBEDDED, &ih);
	assert(rc == 0);
	if (op_iter_probe_rand(ih) != 0) {
		return;
	}
	int  steps_num = entries_num;
	bool prev;
	for (int i = 0; i < steps_num; ++i) {
		int steps_remaining = steps_num - i;
		if (rand() % 2 == 0) {
			/* fetch and check the value is as expected */
			d_iov_set(&key_iov, NULL, 0);
			d_iov_set(&val_iov, NULL, 0);
			rc = dbtree_iter_fetch(ih, &key_iov, &val_iov, NULL);
			assert(rc == 0);
			record_check(*((uint64_t *)key_iov.iov_buf), (char *)val_iov.iov_buf,
				     val_iov.iov_len);
		}
		if (rand() % steps_num > steps_remaining) {
			/* fetch the key to remove it from the records */
			d_iov_set(&key_iov, NULL, 0);
			d_iov_set(&val_iov, NULL, 0);
			rc = dbtree_iter_fetch(ih, &key_iov, &val_iov, NULL);
			assert(rc == 0);
			record_delete(*(uint64_t *)key_iov.iov_buf, RECORD_IDX_UNKNOWN);
			/* delete the entry */
			assert(dbtree_iter_delete(ih, NULL) == 0);
			/* re-probe since after the delete the iterator is not ready */
			if (op_iter_probe_rand(ih) != 0) {
				return;
			}
		}
		prev = rand() % 2;
		if (prev) {
			rc = dbtree_iter_prev(ih);
		} else {
			rc = dbtree_iter_next(ih);
		}
		assert(rc == 0 || rc == -DER_NONEXIST);
		if (rc == -DER_NONEXIST) {
			/* re-probe since after hitting a non-existing entry the iterator is not ready */
			if (op_iter_probe_rand(ih) != 0) {
				return;
			}
		}
	}
	dbtree_iter_finish(ih);
}

void
tb_query_cmd() {
	struct btr_attr attr;
	struct btr_stat stat;
	int rc;
	int             rc_exp = 0;

	if (!daos_handle_is_valid(ik_toh)) {
		rc_exp = -DER_NO_HDL;
	}

	rc = dbtree_query(ik_toh, &attr, &stat);
	assert(rc == rc_exp);
}
