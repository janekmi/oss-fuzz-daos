#!/bin/bash
#
# Copyright 2026 Hewlett Packard Enterprise Development LP
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
# Build DAOS fuzz targets.
#

set -euo pipefail

ldconfig -p | grep libprotobuf # XXX debug

cd targets

# generate C++ code from protobuf definitions
if [ "x${1:-}" == "xclean" ]; then
    rm -rf gen
else
    mkdir -p gen
    protoc --cpp_out=gen target_btree.proto
fi

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

if [ "x${OUT:-}" != "x" ]; then
    cp -v target_btree "$OUT"
fi
