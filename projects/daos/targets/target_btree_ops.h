#ifndef TARGET_BTREE_OPS_H
#define TARGET_BTREE_OPS_H

void
tb_init(void);

void
tb_create_cmd(bool feat_uint_key, bool feat_embed_first, bool inplace, uint32_t order);

#endif /** TARGET_BTREE_OPS_H */
