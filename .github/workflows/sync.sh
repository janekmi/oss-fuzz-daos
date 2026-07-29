#!/bin/bash
#
# Copyright 2026 Hewlett Packard Enterprise Development LP
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
# Syncs DAOS fuzz projects into an OSS-Fuzz checkout.
#

set -euo pipefail

ROOT_DIR=$(realpath "$(dirname "$0")/../..")

usage() {
    echo "Usage: $0 <oss-fuzz-dir>"
    exit 1
}

if [ $# -lt 1 ]; then
    usage
fi

if [ ! -d $ROOT_DIR/projects ]; then
    echo "Error: '$ROOT_DIR/projects' is not a directory"
    usage
fi

if [ ! -d "$1" ]; then
    echo "Error: '$1' is not a directory"
    usage
fi

cp -rd $ROOT_DIR/projects $1/
