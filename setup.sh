#!/bin/sh

echo "please enter the list of your sources directories (map, vector, stack, set, pair):"
read dirs
cat >.setup <<EOF
INCLUDE_DIRECTORIES=( $dirs )
CFLAGS="-Wall -Werror -Wextra -std=c++98"
EOF