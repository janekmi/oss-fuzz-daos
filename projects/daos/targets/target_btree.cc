//
// Copyright 2026 Hewlett Packard Enterprise Development LP
//
// SPDX-License-Identifier: BSD-2-Clause-Patent
//

#include <syslog.h>
#include <src/libfuzzer/libfuzzer_macro.h>

#include "gen/target_btree.pb.h"

extern "C" {
#include "target_btree_helpers.h"
}

DEFINE_PROTO_FUZZER(const target_btree::Msg& input) {
	static int a = 0;
	if (a == 0) {
		tb_init();
		a++;
	}

	syslog(LOG_INFO, "a: %f, b: %lu", input.a(), input.b());
	daos_fuzz_btree(input.a(), input.b());
	if (input.a() > 1) {
		abort();
	}
}
