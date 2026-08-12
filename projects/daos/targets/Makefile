#
# Copyright 2026 Hewlett Packard Enterprise Development LP
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#

INCLUDES := -I$(LPM_SRC_DIR) -I$(DAOS_SRC_DIR)/include -I$(DAOS_SRC_DIR)/common/tests
LDFLAGS := -v -fsanitize=fuzzer -luuid

# Some EL9 systems do not provide a static libprotobuf, but Ubuntu used by OSS-Fuzz struggles to find the dynamic one at runtime.
# So, static version is the default one to make sure the build system works.
# EL9 (and possibly other systems) are treated as an exception which prefers the dynamic version.
OS_ID_LIKE := $(shell . /etc/os-release && echo "$${ID_LIKE:-}")
OS_VER := $(shell . /etc/os-release && echo "$${VERSION_ID:-}")
RHEL9_5 := $(shell echo "$(OS_ID_LIKE)" | grep -qw rhel && [ "$(OS_VER)" = "9.5" ] && echo 1 || echo 0)

ifeq ($(RHEL9_5),1)
LDFLAGS += -lprotobuf
else
LDFLAGS += -Wl,-Bstatic -lprotobuf -Wl,-Bdynamic
endif

SRCS := target_btree.cc \
        gen/target_btree.pb.cc \
        $(LPM_SRC_DIR)/src/libfuzzer/libfuzzer_macro.cc \
        $(LPM_SRC_DIR)/src/libfuzzer/libfuzzer_mutator.cc \
        $(LPM_SRC_DIR)/src/binary_format.cc \
        $(LPM_SRC_DIR)/src/text_format.cc \
        $(LPM_SRC_DIR)/src/mutator.cc \
        $(LPM_SRC_DIR)/src/utf8_fix.cc \
        target_btree_ops.c \
        target_btree_ik_tree.c \
        mocks.c \
        records.c \
        $(DAOS_SRC_DIR)/common/btree.c \
        $(DAOS_SRC_DIR)/common/mem.c \
        $(DAOS_SRC_DIR)/common/tests/utest_common.c

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
