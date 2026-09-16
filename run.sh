#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")" && pwd)"
QT_DIR="${QT_INSTALL_DIR:-$HOME/Qt/6.11.2/gcc_64}"
export PATH="${QT_DIR}/bin:${PATH}"
export CMAKE_PREFIX_PATH="${QT_DIR}"
BUILD="${ROOT}/build-dev"
cmake -S "${ROOT}" -B "${BUILD}" -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build "${BUILD}" -j"$(nproc)"
exec "${BUILD}/gemsight_app"
