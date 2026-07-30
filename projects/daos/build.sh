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
LPM_SRC_DIR=../libprotobuf-mutator
make EXTRA_INCLUDES="-I$LPM_SRC_DIR" LPM_SRC_DIR=$LPM_SRC_DIR ${1:-}
