#!/usr/bin/env bash
# Regenerate Golden Master approved files from current SHealth behavior.
# Usage (from repo root):
#   ./scripts/update_golden.sh
#   ./scripts/update_golden.sh 'SHealthGoldenFixture.GivenSixAgeBands*'

set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BUILD="${ROOT}/build"
FILTER="${1:-SHealthGoldenFixture.*}"

mkdir -p "${BUILD}"
(cd "${BUILD}" && cmake .. && cmake --build . --target SHealthBMITest)

export SHEALTH_UPDATE_GOLDEN=1
(cd "${BUILD}" && ./SHealthBMITest --gtest_filter="${FILTER}")
unset SHEALTH_UPDATE_GOLDEN

echo "Approved files updated under src/test/golden/"
echo "Review diff, then run: cd build && ctest --output-on-failure"
