#!/bin/bash
#
# Copyright 2026 Hewlett Packard Enterprise Development LP
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
# Build DAOS fuzz targets.
#

set -euo pipefail

cd targets

# generate C++ code from protobuf definitions
mkdir -p gen
protoc --cpp_out=gen example.proto

# build targets
CC=${CC:-clang}
CFLAGS=${CFLAGS:-"-std=c++17 -Wall -Wextra -fsanitize=fuzzer-no-link"}
LPM_SRC_DIR=${LPM_SRC_DIR:-../libprotobuf-mutator}

make \
    CC="$CC" \
    CFLAGS="$CFLAGS" \
    INCLUDES="-I$LPM_SRC_DIR" \
    LPM_SRC_DIR=$LPM_SRC_DIR ${1:-}
