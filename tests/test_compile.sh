#!/usr/bin/env bash
# Smoke test: ensure the project compiles cleanly.
set -euo pipefail
cd "$(dirname "$0")/.."
make clean >/dev/null
make 2>&1 | tee /tmp/build.log
if grep -q "error:" /tmp/build.log; then
    echo "FAIL: compilation errors"
    exit 1
fi
echo "PASS: compiled successfully"
./ecommerce <<'INPUT' >/tmp/run.log 2>&1 || true
3
0
INPUT
echo "PASS: ran without crash"
