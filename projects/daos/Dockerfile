#
# Copyright 2026 Hewlett Packard Enterprise Development LP
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#

FROM gcr.io/oss-fuzz-base/base-builder
RUN apt-get update && apt-get install -y libprotobuf-dev protobuf-compiler
RUN git clone --depth 1 https://github.com/daos-stack/daos.git
RUN git clone --depth 1 --branch v1.5 https://github.com/google/libprotobuf-mutator.git
WORKDIR $SRC/
COPY build.sh .
COPY targets targets
