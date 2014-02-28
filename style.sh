#!/usr/bin/env bash

if ! type -path clang-format &> /dev/null; then
  echo "clang-format is not installed" >&2
  exit 1;
fi

wdir=$(dirname "$(readlink -f "$0")")
srcdir="$wdir"
remove_eol_spaces=yes

function format_source {
  if ! [ -d "$srcdir" ]; then
    echo "source code directory not found at [$srcdir]" >&2
    exit 1;
  fi
  find "$srcdir" -iname \*.cc \
              -o -iname \*.h |
    xargs -r -n 1 -I@ clang-format -style=LLVM -i @

  if [ "$remove_eol_spaces" = yes ]; then
    find "$srcdir" -iname \*.cc \
                -o -iname \*.h |
      xargs -r -n 1 -I@ sed -i 's!\s*$!!' @
  fi
}

function modernize {
  local build_path="$1"; shift
  if ! [ -d "$srcdir" ]; then
    echo "source code directory not found at [$srcdir]" >&2
    exit 1;
  fi
  if ! [ -d "$build_path" ]; then
    echo "build path not found at [$build_path]" >&2
    exit 1;
  fi

  find "$srcdir" -name \*.cc |
    xargs clang-modernize -p "$build_path" -format -style=LLVM
}

format_source "$@"
#modernize "$@"
