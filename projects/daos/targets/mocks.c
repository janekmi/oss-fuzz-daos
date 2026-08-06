//
// Copyright 2026 Hewlett Packard Enterprise Development LP
//
// SPDX-License-Identifier: BSD-2-Clause-Patent
//

#include <daos/btree.h>

int daos_tree_logfac_cache[24];
int daos_tests_logfac_cache[24];
int daos_common_logfac_cache[24];
int daos_daos_logfac_cache[24];
int daos_tree_logfac;
int daos_tests_logfac;
int daos_common_logfac;
int daos_daos_logfac;

/* An alternative assert function. Set with d_register_alt_assert() */
void (*d_alt_assert)(const int, const char*, const char*, const int);

void
d_free(void *ptr)
{
	free(ptr);
}

void *
d_calloc(size_t count, size_t eltsize)
{
	return calloc(count, eltsize);
}

struct d_log_xstate d_log_xst;

void d_vlog(int flags, const char *fmt, va_list ap)
{
        abort();
}

void d_log_sync(void)
{
        abort();
}

const char *
d_errdesc(int errnum){
	abort();
}

const char *
d_errstr(int errnum)
{
	abort();
}

struct d_fault_attr_t *d_fault_attr_mem;
unsigned int           d_fault_inject;

uint64_t
d_hash_murmur64(const unsigned char *key, unsigned int key_len, unsigned int seed)
{
	abort();
}

uint32_t
d_hash_string_u32(const char *string, unsigned int len)
{
	abort();
}

bool
d_should_fail(struct d_fault_attr_t *fault_attr)
{
	abort();
}

int
daos_fail_check(uint64_t fail_loc)
{
	abort();
}

d_dbug_t DB_MEM;
d_dbug_t DB_TRACE;
