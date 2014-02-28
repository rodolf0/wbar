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

format_source "$@"
