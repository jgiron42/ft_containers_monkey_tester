#!/bin/bash 

set -- $(getopt "ifsauvh" $@)
MODE=""
FILTER="cat"
CFLAGS=""
RECOMPILE="no"
for arg in $@
do
  shift
  [ $arg == "-i" ] && MODE=infinite_ RECOMPILE=yes
  [ $arg == "-f" ] && MODE=full
  [ $arg == "-s" ] && FILTER="tail -n50"
  [ $arg == "-a" ] && CFLAGS+=" -fsanitize=address" RECOMPILE=yes
  [ $arg == "-u" ] && CFLAGS+=" -fsanitize=undefined" RECOMPILE=yes
  [ $arg == "-v" ] && VALGRIND="yes" RECOMPILE=yes
  [ $arg == "-h" ] && { echo "Usage: $( basename $0 ) [-i] <container> [<seed>]"; echo " -i   toggle infinite mode" ; exit 0 ; }
  [ $arg == "--" ] && { break; }
done

set -- $(echo $@)

if [ "$MODE" == full ]
then
  $0 -a map
  $0 -f map
  $0 -v map
  $0 -a vector
  $0 -f vector
  $0 -v vector
  $0 -a stack
  $0 -f stack
  $0 -v stack
  exit
fi

if [ "$1" != "vector" -a "$1" != "stack" -a "$1" != "map" -a "$1" != "set" ]
then
  echo "Usage: $( basename $0 ) [-i] <container> [<seed>]" >&2
  exit 1
fi

C=$1
shift

[ ! -e .setup ] && ./setup.sh

. .setup

for i in "${INCLUDE_DIRECTORIES[@]}"
do
	CFLAGS+=" -I"$i
done

check_last_change()
{
  ret=0
  for file in $( find $INCLUDE_DIRECTORIES . -maxdepth 1 | grep '.hpp\|.hpp')
  do
    tmp="$(stat -c "%Y" $file )"
    : $(( ret = tmp > ret ? tmp : ret ))
  done
  echo $ret
}

if [ "$VALGRIND" != "yes" ] && [  ! -x bin/fifodiff ] || [ "$(stat -c %Y srcs/fifodiff.cpp)" -ge "$(stat -c %Y bin/fifodiff)" ]
then
  echo "🐒 compiling fifodiff..."
  clang++ $CFLAGS -D BEFORE_SIZE=-1 srcs/fifodiff.cpp -o bin/fifodiff || exit
fi

if [ "$VALGRIND" != "yes" ] && [ "$MODE" == "infinite_" ]
then
  CFLAGS+=" -D NTEST=-1"
elif [ "$CPP_LOGGING" == "y" ]
then
  CFLAGS+=" -D CPP_LOGGING"
fi

if [ "$VALGRIND" != "yes" ] && [ $RECOMPILE == "yes" ] || [  ! -x bin/"$MODE"std_containers_$C  ] || [ "$(stat -c %Y srcs/main.cpp)" -ge "$(stat -c %Y bin/"$MODE"std_containers_$C)" ]
then
  echo "🐒 compiling std... "
  clang++ $CFLAGS -D NAMESPACE=std -D MONKEY_$(echo $C | tr 'a-z' 'A-Z') srcs/main.cpp -o bin/"$MODE"std_containers_$C || exit
fi

if [ $RECOMPILE == "yes" ] || [ ! -x bin/"$MODE"ft_containers_$C ] || [ "$(check_last_change)" -ge "$(stat -c %Y bin/"$MODE"ft_containers_$C)" -o "$(stat -c %Y srcs/main.cpp)" -ge "$(stat -c %Y bin/"$MODE"ft_containers_$C)" ]
then
  echo "🐒 compiling ft... "
  clang++ $CFLAGS -D NAMESPACE=ft -D MONKEY_$(echo $C | tr 'a-z' 'A-Z')  srcs/main.cpp -o bin/"$MODE"ft_containers_$C || exit
fi

if [ "$VALGRIND" = "yes" ]
then
  valgrind ./bin/"$MODE"ft_containers_$C $@
  exit
fi

rm .std_$C .ft_$C 2>/dev/null
mkfifo .std_$C .ft_$C
trap "pkill -9 fifodiff; rm .std .ft 2>/dev/null" INT
echo "🐒 running... "
./bin/"$MODE"std_containers_$C $@ >> .std_$C | ./bin/"$MODE"ft_containers_$C $@ >> .ft_$C | ./bin/fifodiff .std_$C .ft_$C | $FILTER
rm .std_$C .ft_$C 2>/dev/null
