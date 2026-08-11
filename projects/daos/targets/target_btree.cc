//
// Copyright 2026 Hewlett Packard Enterprise Development LP
//
// SPDX-License-Identifier: BSD-2-Clause-Patent
//

#include <syslog.h>
#include <src/libfuzzer/libfuzzer_macro.h>

#include "gen/target_btree.pb.h"

extern "C" {
#include "target_btree_ops.h"
}

DEFINE_PROTO_FUZZER(const target_btree::Msg& input) {
	static bool init_done = 0;
	if (!init_done) {
		tb_init();
		init_done = true;
	}

	// Print the number of commands in the input message
	syslog(LOG_INFO, "Number of commands: %d\n", input.commands_size());

	// Loop over messages and print arguments of each of the commands
	for (int i = 0; i < input.commands_size(); i++) {
		const target_btree::Msg_Command& cmd = input.commands(i);

		switch (cmd.cmd_case()) {
		case target_btree::Msg_Command::kCreate: {
			const auto& c = cmd.create();
			syslog(LOG_INFO, "Create: feat_uint_key=%c feat_embed_first=%c inplace=%c order=%u",
			       (char)c.feat_uint_key(), (char)c.feat_embed_first(), (char)c.inplace(), c.order());
			tb_create_cmd(c.feat_uint_key(), c.feat_embed_first(), c.inplace(), c.order());
			break;
		}
		case target_btree::Msg_Command::kClose: {
			syslog(LOG_INFO, "Close");
			tb_close_cmd();
			break;
		}
		case target_btree::Msg_Command::kDestroy: {
			syslog(LOG_INFO, "Destroy");
			tb_destroy_cmd();
			break;
		}
		case target_btree::Msg_Command::kOpen: {
			syslog(LOG_INFO, "Open");
			tb_open_cmd();
			break;
		}
		case target_btree::Msg_Command::kUpdate: {
			const auto& c = cmd.update();
			syslog(LOG_INFO, "Update: entries_num=%u", c.entries_num());
			tb_update_cmd(c.entries_num());
			break;
		}
		case target_btree::Msg_Command::kIter: {
			const auto& c = cmd.iter();
			syslog(LOG_INFO, "Iter: entries_num=%lu", c.entries_num());
			tb_iter_cmd(c.entries_num());
			break;
		}
		case target_btree::Msg_Command::kQuery: {
			syslog(LOG_INFO, "Query");
			tb_query_cmd();
			break;
		}
		case target_btree::Msg_Command::kLookup: {
			const auto& c = cmd.lookup();
			syslog(LOG_INFO, "Lookup: entries_num=%lu", c.entries_num());
			break;
		}
		case target_btree::Msg_Command::kDelete: {
			const auto& c = cmd.delete_();
			syslog(LOG_INFO, "Delete: entries_num=%lu", c.entries_num());
			break;
		}
		case target_btree::Msg_Command::kDrain: {
			const auto& c = cmd.drain();
			syslog(LOG_INFO, "Drain: credits_num=%lu", c.credits_num());
			break;
		}
		default:
			syslog(LOG_INFO, "Unknown command: cmd_case=%d", cmd.cmd_case());
			abort();
			break;
		}
	}

	if (input.commands_size() > 0) {
		abort();
	}
}
