#!/bin/bash

if [ "$1" != "vector" -a "$1" != "stack" -a "$1" != "map" -a "$1" != "set" ]
then
  echo "Usage: $( basename $0 ) <container>" >&2
  exit 1
fi
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


if [  ! -x bin/fifodiff ] || [ "$(stat -c %Y srcs/fifodiff.cpp)" -ge "$(stat -c %Y bin/fifodiff)" ]
then
  echo "🐒 compiling fifodiff..."
  clang++ $CFLAGS -D BEFORE_SIZE=-1 srcs/fifodiff.cpp -o bin/fifodiff || exit
fi
if [ ! -x bin/infinite_ft_containers_$1 ] || [ "$(check_last_change)" -ge "$(stat -c %Y bin/infinite_ft_containers_$1)" -o "$(stat -c %Y srcs/main.cpp)" -ge "$(stat -c %Y bin/infinite_ft_containers_$1)" ]
then
  echo "🐒 compiling ft... "
  clang++ $CFLAGS -D NTEST=-1 -D NAMESPACE=ft -D MONKEY_$(echo $1 | tr 'a-z' 'A-Z') srcs/main.cpp -o bin/infinite_ft_containers_$1 || exit
fi
if [  ! -x bin/infinite_std_containers_$1  ] || [ "$(stat -c %Y srcs/main.cpp)" -ge "$(stat -c %Y bin/infinite_std_containers_$1)" ]
then
  echo "🐒 compiling std... "
  clang++ $CFLAGS -D NTEST=-1 -D NAMESPACE=std -D MONKEY_$(echo $1 | tr 'a-z' 'A-Z') srcs/main.cpp -o bin/infinite_std_containers_$1 || exit
fi

rm .std .ft 2>/dev/null
mkfifo .std .ft
trap "pkill -9 fifodiff; rm .std .ft 2>/dev/null" INT
echo "🐒 running... "
./bin/infinite_std_containers_$1 $@ >> .std | ./bin/infinite_ft_containers_$1 $@ >> .ft | ./bin/fifodiff .std .ft
rm .std .ft 2>/dev/null
