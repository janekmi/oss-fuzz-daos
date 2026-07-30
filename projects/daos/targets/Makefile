#
# Copyright 2026 Hewlett Packard Enterprise Development LP
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#

LDFLAGS := -v -fsanitize=fuzzer -lprotobuf

SRCS := target.cc \
        gen/example.pb.cc \
        $(LPM_SRC_DIR)/src/libfuzzer/libfuzzer_macro.cc \
        $(LPM_SRC_DIR)/src/libfuzzer/libfuzzer_mutator.cc \
        $(LPM_SRC_DIR)/src/binary_format.cc \
        $(LPM_SRC_DIR)/src/text_format.cc \
        $(LPM_SRC_DIR)/src/mutator.cc \
        $(LPM_SRC_DIR)/src/utf8_fix.cc
OBJS := $(SRCS:.cc=.o)
TARGET := target

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cc
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
