#
# Copyright 2026 Hewlett Packard Enterprise Development LP
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#

CXX      := clang
CXXFLAGS := -std=c++17 -Wall -Wextra -fsanitize=fuzzer-no-link
INCLUDES := -I./include

# Add extra include dirs: make EXTRA_INCLUDES="-I/some/path -I/another"
INCLUDES += $(EXTRA_INCLUDES)

LDFLAGS := -v -fsanitize=fuzzer -lprotobuf

# Add extra linker flags: make EXTRA_LDFLAGS="-L/some/path -lfoo"
LDFLAGS += $(EXTRA_LDFLAGS)

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
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
