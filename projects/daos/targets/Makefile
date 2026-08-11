#
# Copyright 2026 Hewlett Packard Enterprise Development LP
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#

INCLUDES := -I$(LPM_SRC_DIR) -I$(DAOS_SRC_DIR)/include -I$(DAOS_SRC_DIR)/common/tests
LDFLAGS := -v -lprotobuf -luuid -lstdc++

SRCS_COMMON := \
        gen/target_btree.pb.cc \
        target_btree_ops.c \
        target_btree_ik_tree.c \
        mocks.c \
        records.c \
        $(DAOS_SRC_DIR)/common/btree.c \
        $(DAOS_SRC_DIR)/common/mem.c \
        $(DAOS_SRC_DIR)/common/tests/utest_common.c

SRCS_TARGET := \
        $(LPM_SRC_DIR)/src/libfuzzer/libfuzzer_macro.cc \
        $(LPM_SRC_DIR)/src/libfuzzer/libfuzzer_mutator.cc \
        $(LPM_SRC_DIR)/src/binary_format.cc \
        $(LPM_SRC_DIR)/src/text_format.cc \
        $(LPM_SRC_DIR)/src/mutator.cc \
        $(LPM_SRC_DIR)/src/utf8_fix.cc

OBJS_COMMON := $(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(SRCS_COMMON)))
OBJS_TARGET := $(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(SRCS_TARGET)))
TARGET := target_btree
TARGET_REPRO := target_btree_repro

.PHONY: all clean

all: $(TARGET) $(TARGET_REPRO)

$(TARGET): $(OBJS_COMMON) $(OBJS_TARGET) target_btree.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) -fsanitize=fuzzer

$(TARGET_REPRO): $(OBJS_COMMON) target_btree.o.repro
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cc
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<
        
%.o.repro: %.cc
	$(CC) $(CFLAGS) -DREPRO_BINARY $(INCLUDES) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -f $(OBJS_COMMON) $(OBJS_TARGET) $(TARGET) $(TARGET_REPRO)
