//
// Copyright 2026 Hewlett Packard Enterprise Development LP
//
// SPDX-License-Identifier: BSD-2-Clause-Patent
//

#include <syslog.h>
#include <src/libfuzzer/libfuzzer_macro.h>

#include "gen/example.pb.h"

extern "C" {
#include <stdint.h>
void daos_fuzz_btree(double a, uint64_t b);
}

DEFINE_PROTO_FUZZER(const example::Msg& input) {
	syslog(LOG_INFO, "a: %f, b: %lu", input.a(), input.b());
	daos_fuzz_btree(input.a(), input.b());
	if (input.a() > 1) {
		abort();
	}
}
