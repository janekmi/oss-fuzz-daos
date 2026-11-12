#!/bin/bash
#
# Copyright 2026 Hewlett Packard Enterprise Development LP
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
# Replays crash-* artifacts produced by ClusterFuzzLite against the built targets.
# Expects SANITIZER and IMAGE in the environment and runs from the workspace root.
#

set -uo pipefail

logdir="$RUNNER_TEMP/reproduce"
mkdir -p "$logdir"
total=0
reproduced=0

{
    echo "### Crash reproduction ($SANITIZER)"
    echo
    echo "| Target | Artifact | Result | Exit code |"
    echo "|---|---|---|---|"
} >> "$GITHUB_STEP_SUMMARY"

while IFS= read -r crash; do
    total=$((total + 1))
    name=$(basename "$crash")
    target=$(basename "$(dirname "$(dirname "$crash")")")
    log="$logdir/$name.log"
    rc=0
    if [ -x "build-out/$target" ]; then
        timeout 300 docker run --rm \
            -v "$GITHUB_WORKSPACE:/workspace" \
            --entrypoint "/workspace/build-out/$target" \
            "$IMAGE" "/workspace/$crash" > "$log" 2>&1 || rc=$?
    else
        echo "binary build-out/$target not found" > "$log"
        rc=127
    fi
    if [ "$rc" -eq 124 ]; then
        result="TIMEOUT"
    elif [ "$rc" -ne 0 ] || grep -q 'runtime error:' "$log"; then
        result="CRASHED"
        reproduced=$((reproduced + 1))
    else
        result="NOT REPRODUCED"
    fi
    echo "| $target | $name | $result | $rc |" >> "$GITHUB_STEP_SUMMARY"
    echo "::group::$result: $name"
    cat "$log"
    echo "::endgroup::"
done < <(find out/artifacts -type f -path "*/$SANITIZER/crash-*" ! -name '*.summary' 2>/dev/null | sort)

if [ "$total" -eq 0 ]; then
    echo "No crash-* artifacts found for $SANITIZER." >> "$GITHUB_STEP_SUMMARY"
else
    echo "" >> "$GITHUB_STEP_SUMMARY"
    echo "Reproduced $reproduced of $total crash artifact(s)." >> "$GITHUB_STEP_SUMMARY"
fi
echo "reproduced=$([ "$reproduced" -gt 0 ] && echo true || echo false)" >> "$GITHUB_OUTPUT"
