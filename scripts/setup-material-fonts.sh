#!/usr/bin/env bash
set -euo pipefail
ROOT="${1:-}"
if [[ -z "${ROOT}" ]]; then
  echo "usage: setup-material-fonts.sh <QmlMaterial_SOURCE_DIR>"
  exit 1
fi
FONT="${ROOT}/assets/MaterialSymbolsRounded.wght_400.opsz_24.fill_0.woff2"
if [[ -f "${FONT}" ]] && head -c 40 "${FONT}" | grep -q 'git-lfs'; then
  git lfs pull --include="assets/*.woff2" 2>/dev/null || git lfs pull
fi
