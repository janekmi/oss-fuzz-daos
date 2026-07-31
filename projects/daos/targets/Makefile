#
# Copyright 2026 Hewlett Packard Enterprise Development LP
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#

INCLUDES := -I$(LPM_SRC_DIR) -I$(DAOS_SRC_DIR)/include
LDFLAGS := -v -fsanitize=fuzzer -lprotobuf -luuid

SRCS := target_btree.cc \
        gen/target_btree.pb.cc \
        $(LPM_SRC_DIR)/src/libfuzzer/libfuzzer_macro.cc \
        $(LPM_SRC_DIR)/src/libfuzzer/libfuzzer_mutator.cc \
        $(LPM_SRC_DIR)/src/binary_format.cc \
        $(LPM_SRC_DIR)/src/text_format.cc \
        $(LPM_SRC_DIR)/src/mutator.cc \
        $(LPM_SRC_DIR)/src/utf8_fix.cc \
        target_btree_helpers.c \
        $(DAOS_SRC_DIR)/common/btree.c \
        $(DAOS_SRC_DIR)/common/fail_loc.c \
        $(DAOS_SRC_DIR)/common/mem.c \
        $(DAOS_SRC_DIR)/gurt/dlog.c \
        $(DAOS_SRC_DIR)/gurt/debug.c \
        $(DAOS_SRC_DIR)/gurt/fault_inject.c \
        $(DAOS_SRC_DIR)/gurt/errno.c \
        $(DAOS_SRC_DIR)/gurt/hash.c \
        $(DAOS_SRC_DIR)/gurt/misc.c
OBJS := $(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(SRCS)))
TARGET := target_btree

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cc
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
