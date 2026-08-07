#ifndef TARGET_BTREE_OPS_H
#define TARGET_BTREE_OPS_H

void
tb_init(void);

void
tb_create_cmd(bool feat_uint_key, bool feat_embed_first, bool inplace, uint32_t order);

void
tb_close_cmd(void);

void
tb_destroy_cmd(void);

void
tb_open_cmd(void);

#endif /** TARGET_BTREE_OPS_H */
