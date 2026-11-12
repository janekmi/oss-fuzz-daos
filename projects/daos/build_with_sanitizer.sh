#!/bin/bash
#
# Copyright 2026 Hewlett Packard Enterprise Development LP
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#

set -euo pipefail

# Usage: build_with_sanitizer.sh [address|undefined|memory|clean]
SANITIZER=${1:-address}
if [ $# -gt 0 ]; then
    shift
fi

case "$SANITIZER" in
address)
    SANITIZER_FLAGS="-fsanitize=address -fsanitize-address-use-after-scope"
    ;;
undefined)
    # Makefile links with CC; C++ objects need the C++ UBSan runtime.
    SANITIZER_FLAGS="-fsanitize=array-bounds,bool,builtin,enum,function,integer-divide-by-zero,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,unsigned-integer-overflow,unreachable,vla-bound,vptr -fno-sanitize-recover=array-bounds,bool,builtin,enum,function,integer-divide-by-zero,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,unreachable,vla-bound,vptr"
    ;;
memory)
    SANITIZER_FLAGS="-fsanitize=memory -fsanitize-memory-track-origins"
    ;;
clean)
    ./build.sh clean
    exit 0
    ;;
*)
    echo "Unsupported sanitizer: $SANITIZER (expected: address, undefined, memory)" >&2
    exit 1
    ;;
esac

export CFLAGS="-O1 -fno-omit-frame-pointer -gline-tables-only -DFUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION $SANITIZER_FLAGS -fsanitize=fuzzer-no-link"
export LDFLAGS="$SANITIZER_FLAGS -fsanitize=fuzzer-no-link"

./build.sh
