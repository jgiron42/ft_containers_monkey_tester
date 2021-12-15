#!/bin/sh

mkdir -p bin
echo "please enter the list of your sources directories separated by spaces:"
read dirs
cat >.setup <<EOF
INCLUDE_DIRECTORIES=( $dirs )
CFLAGS="-Wall -Werror -Wextra -std=c++98"
EOF
echo "to run this configuration again please use ./setup.sh"
