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
protoc --cpp_out=gen target_btree.proto

# build targets
CC=${CC:-clang}
CFLAGS=${CFLAGS:-"-O0 -ggdb -Wall -Wextra -fsanitize=fuzzer-no-link -Wno-pointer-arith -Wint-conversion -D_GNU_SOURCE"}
LPM_SRC_DIR=${LPM_SRC_DIR:-../libprotobuf-mutator}
DAOS_SRC_DIR=${DAOS_SRC_DIR:-../daos/src}

make \
    CC="$CC" \
    CFLAGS="$CFLAGS" \
    LPM_SRC_DIR="$LPM_SRC_DIR" \
    DAOS_SRC_DIR="$DAOS_SRC_DIR" \
    ${1:-}
