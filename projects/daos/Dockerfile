#
# Copyright 2026 Hewlett Packard Enterprise Development LP
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#

FROM gcr.io/oss-fuzz-base/base-builder
RUN apt-get update
RUN git clone --depth 1 https://github.com/daos-stack/daos.git daos
WORKDIR $SRC/
COPY build.sh $SRC/
