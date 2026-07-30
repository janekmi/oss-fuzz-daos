#!/bin/bash
#
# Copyright 2026 Hewlett Packard Enterprise Development LP
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
# Build DAOS fuzz targets.
#

set -euo pipefail

LPM_SRC_DIR=/home/michalsk/work/google/libprotobuf-mutator

make EXTRA_INCLUDES="-I$LPM_SRC_DIR" LPM_SRC_DIR=$LPM_SRC_DIR ${1:-}
