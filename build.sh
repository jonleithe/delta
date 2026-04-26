#!/usr/bin/env bash

set -euo pipefail

usage() {
    echo "Usage: $0 {debug|release|docs}"
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
    docs)
        cmake --preset release-gcc
        if cmake --build --preset build-release --target docs; then
            echo "Built docs target."
        else
            echo "Docs target is not configured for this project."
            exit 1
        fi
        ;;
    *)
        usage
        exit 2
        ;;
esac
