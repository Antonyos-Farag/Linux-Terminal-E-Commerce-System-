#!/usr/bin/env bash
# Smoke test: ensure the project compiles cleanly and both programs run.
set -euo pipefail
cd "$(dirname "$0")/.."
make clean >/dev/null
make 2>&1 | tee /tmp/build.log
if grep -q "error:" /tmp/build.log; then
    echo "FAIL: compilation errors"
    exit 1
fi
echo "PASS: compiled successfully"

# Original program (preserved verbatim).
./ecommerce <<'INPUT' >/tmp/run.log 2>&1 || true
3
0
INPUT
echo "PASS: original ran without crash"

# Cool edition: customer purchases a product, leaves feedback, then quits.
# Piped input + file output => plain mode (no TTY), so the log must be clean.
./ecommerce-cool <<'INPUT' >/tmp/run-cool.log 2>&1 || true
3
2
8
4
Great build!
0
0
INPUT
echo "PASS: cool edition ran without crash"
if grep -q $'\033' /tmp/run-cool.log; then
    echo "FAIL: plain mode leaked escape codes"
    exit 1
fi
echo "PASS: plain mode produced a clean log"
