#!/bin/bash

set -- ""$(getopt ihs $@)
MODE=""
FILTER="cat"
for arg in $@
do
  shift
  [ $arg == "-i" ] && MODE=infinite_
  [ $arg == "-s" ] && FILTER="tail -n50"
  [ $arg == "-h" ] && { echo "Usage: $( basename $0 ) [-i] <container> [<seed>]"; echo " -i   toggle infinite mode" ; exit 0 ; }
  [ $arg == "--" ] && { shift; break; }
done

set -- ""$(echo $@)

if [ "$1" != "vector" -a "$1" != "stack" -a "$1" != "map" -a "$1" != "set" ]
then
  echo "Usage: $( basename $0 ) [-i] <container> [<seed>]" >&2
  exit 1
fi

C=$1

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

if [  ! -x bin/fifodiff ] || [ "$(stat -c %Y srcs/fifodiff.cpp)" -ge "$(stat -c %Y bin/fifodiff)" ]
then
  echo "🐒 compiling fifodiff..."
  clang++ $CFLAGS -D BEFORE_SIZE=-1 srcs/fifodiff.cpp -o bin/fifodiff || exit
fi

if [ "$MODE" == "infinite_" ]
then
  CFLAGS+=" -D NTEST=-1"
elif [ "$CPP_LOGGING" == "y" ]
then
  CFLAGS+=" -D CPP_LOGGING"
fi

if [ ! -x bin/"$MODE"ft_containers_$C ] || [ "$(check_last_change)" -ge "$(stat -c %Y bin/"$MODE"ft_containers_$C)" -o "$(stat -c %Y srcs/main.cpp)" -ge "$(stat -c %Y bin/"$MODE"ft_containers_$C)" ]
then
  echo "🐒 compiling ft... "
  clang++ $CFLAGS -D NAMESPACE=ft -D MONKEY_$(echo $C | tr 'a-z' 'A-Z')  srcs/main.cpp -o bin/"$MODE"ft_containers_$C || exit
fi

if [  ! -x bin/"$MODE"std_containers_$C  ] || [ "$(stat -c %Y srcs/main.cpp)" -ge "$(stat -c %Y bin/"$MODE"std_containers_$C)" ]
then
  echo "🐒 compiling std... "
  clang++ $CFLAGS -D NAMESPACE=std -D MONKEY_$(echo $C | tr 'a-z' 'A-Z') srcs/main.cpp -o bin/"$MODE"std_containers_$C || exit
fi

rm .std .ft 2>/dev/null
mkfifo .std .ft
trap "pkill -9 fifodiff; rm .std .ft 2>/dev/null" INT
echo "🐒 running... "
./bin/"$MODE"std_containers_$C $@ >> .std | ./bin/"$MODE"ft_containers_$C $@ >> .ft | ./bin/fifodiff .std .ft | $FILTER
rm .std .ft 2>/dev/null
