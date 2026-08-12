#ifndef TARGET_BTREE_OPS_H
#define TARGET_BTREE_OPS_H

void
tb_init(void);

void
tb_clean_check(void);

/**
 * Cleanup the test environment.
 *
 * Note: It does not cleanup after the tb_init() function, but it resets the state after a sequence of commands.
 * So, the next round can start clean.
 */
void
tb_cleanup(void);

void
tb_create_cmd(bool feat_uint_key, bool feat_embed_first, bool inplace, uint32_t order);

void
tb_close_cmd(void);

void
tb_destroy_cmd(void);

void
tb_open_cmd(void);

void
tb_update_cmd(uint32_t entries_num);

void
tb_iter_cmd(uint32_t entries_num);

void
tb_query_cmd(void);

void
tb_lookup_cmd(uint32_t entries_num);

void
tb_delete_cmd(uint32_t entries_num);

void
tb_drain_cmd(uint32_t credits);

#endif /** TARGET_BTREE_OPS_H */
