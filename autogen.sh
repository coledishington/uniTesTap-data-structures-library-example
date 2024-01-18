#!/bin/sh

# Avoid errors by disallowing expansions of unset variables
set -u

# Constants
YES='yes'

# Autoconf aux and m4 will always go in <project-root>/build
AUX_DIR="build/autotools/aux"
M4_DIR="build/autotools/m4"
PROJECT_ROOT=$(cd "${0%autogen.sh}" && pwd)

# autogen arguments
BUILD_DIR=build
VERBOSE=
CHECK=
CLEAN=
QUIET=
HELP=

help() {
    cat <<eof
usage: autogen.sh [-h|OPTION]...
Build data-structures

positional arguments:
  builddir                   The root of the build tree. (default: <project-root>/build)

options:
  -h, --help                 show this help message and exit
  -q, --quiet                suppress all normal output
  -v, --verbose              print steps of execution
  --check                    run unit tests
  --clean                    re-build everything
eof
}

argparse() {
    while [ $# -gt 0 ]; do
        case "$1" in
            -h | --help)
                HELP=$YES
                ;;
            -v | --verbose)
                VERBOSE=$YES
                ;;
            -q | --quiet)
                QUIET=$YES
                ;;
            --check)
                CHECK=$YES
                ;;
            --clean)
                CLEAN=$YES
                ;;
            -*)
                printf 'Unknown option %s\n' "$1" >&2
                return 1
                ;;
            *)
                printf 'Unknown positional argument: %s' "$1" >&2
                return 1
                ;;
        esac
        shift
    done
    return 0
}

get_autotools_var() {
    printf '@%s@' "$1" | (cd "$BUILD_DIR" && ./config.status --file=-)
}

make_() {
    CC=$(get_autotools_var "CC")
    # Try to generate compiler_commands.json for editors
    intercept-build \
        --cdb "$BUILD_DIR/compile_commands.json" \
        --use-cc "$CC" \
        --append \
        make ${VERBOSE+-d}
}

build() (
    # All build steps must be successful
    set -e

    mkdir -p "$BUILD_DIR"

    mkdir -p "$AUX_DIR" "$M4_DIR"

    # Go looking for the tap-driver if it is missing
    if ! [ -e "$AUX_DIR/tap-driver.sh" ]; then
        if ! AUTOMAKE_LIBDIR=$(automake --print-libdir); then
            printf 'automake error: cannot find tap-driver.sh' >&2
        fi
        cp "$AUTOMAKE_LIBDIR/tap-driver.sh" "$AUX_DIR"
    fi

    autoreconf -i ${CLEAN+-f} ${VERBOSE+-v} 2>&1
    cd "$BUILD_DIR"

    # Tidy up files created by make and configure
    if [ "$CLEAN" = "$YES" ]; then
        make distclean || true # Makefile may not exist
    fi

    "$PROJECT_ROOT"/configure
    make_
)

check() (
    cd "$BUILD_DIR"
    make check
)

if ! argparse "$@"; then
    exit 1
fi

if [ "$QUIET" = "$YES" ]; then
    exec 1>/dev/null
fi

cd "$PROJECT_ROOT"
mkdir "$BUILD_DIR"
BUILD_DIR=$(cd $BUILD_DIR && pwd)

if [ "$HELP" = "$YES" ]; then
    help
    exit 1
fi

if [ "$VERBOSE" = "$YES" ]; then
    set -x
fi

if ! build; then
    exit 1
fi

if [ "$CHECK" = "$YES" ]; then
    check
fi
