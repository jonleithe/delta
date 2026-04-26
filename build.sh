#!/usr/bin/env bash

set -euo pipefail

usage() {
    echo "Usage: $0 {debug|release}"
}

mode="${1:-debug}"

case "$mode" in
    debug)
        cmake --preset debug-gcc
        cmake --build --preset build-debug
        ;;
    release)
        cmake --preset release-gcc
        cmake --build --preset build-release
        ;;
    *)
        usage
        exit 2
        ;;
esac
