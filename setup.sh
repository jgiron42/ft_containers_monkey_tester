#!/bin/sh

prompt_yn()
{
  while true
  do
    read answer
    [ "$answer" = "y" ] || [ "$answer" = "Y" ] && return 0
    [ "$answer" = "n" ] || [ "$answer" = "N" ] || [ -z "$answer" ] && return 1
  done
}

rm bin/* 2>/dev/null
mkdir -p bin
mkdir -p log
echo "Please enter the list of your sources directories separated by spaces:"
read dirs
echo -n "Would you like to use the awesome cpp logging feature (y/N)? "
EXTRA_DEFINE=""
if prompt_yn
then
 EXTRA_DEFINE=" -D CPP_LOGGING"
 echo "When using monkey.sh the tester will generate a file containing"
 echo "cpp instructions to reproduce the same run without randomness"
 echo "which allow you to easily use gdb or any other compiler\n"
 echo "Use compile_log.sh to compile the generated file\n"
 [ ! -f compile_log.sh ] && cp .compile_log.sh compile_log.sh
else
 [ -f compile_log.sh ] && rm compile_log.sh
fi
cat >.setup <<EOF
INCLUDE_DIRECTORIES=( $dirs )
CFLAGS="-Wall -Werror -Wextra -std=c++98 $EXTRA_DEFINE"
EOF


echo "To run this configuration again please use ./setup.sh"
echo "(press enter...)"
read VOID

