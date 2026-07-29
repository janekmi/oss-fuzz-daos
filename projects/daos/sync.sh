#!/bin/bash

ROOT_DIR=$(realpath "$(dirname "$0")/../..")

echo $ROOT_DIR

if [ ! -d "$1" ]; then
    echo "Error: $1 is not a directory"
    exit 1
fi

cp -rd $ROOT_DIR/projects $1/
