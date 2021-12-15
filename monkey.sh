#!/bin/bash

if [ "$1" != "vector" -a "$1" != "stack" -a "$1" != "map" -a "$1" != "set" ]
then
  echo "Usage: $( basename $0 ) <container>" >&2
  exit 1
fi

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

if [ ! -x bin/ft_containers_$1 ] || [ "$(check_last_change)" -ge "$(stat -c %Y bin/ft_containers_$1)" -o "$(stat -c %Y srcs/main.cpp)" -ge "$(stat -c %Y bin/ft_containers_$1)" ]
then
  echo "🐒 compiling ft... "
  clang++ $CFLAGS -D NAMESPACE=ft -D MONKEY_$(echo $1 | tr 'a-z' 'A-Z') srcs/main.cpp -o bin/ft_containers_$1 || exit
fi

if [  ! -x bin/std_containers_$1  ] || [ "$(stat -c %Y srcs/main.cpp)" -ge "$(stat -c %Y bin/std_containers_$1)" ]
then
  echo "🐒 compiling std... "
  clang++ $CFLAGS -D NAMESPACE=std -D MONKEY_$(echo $1 | tr 'a-z' 'A-Z') srcs/main.cpp -o bin/std_containers_$1 || exit
fi

./bin/std_containers_$1 $@ > .stdtmp
./bin/ft_containers_$1 $@ > .fttmp

if diff .stdtmp .fttmp > diff.log
then
  echo "🐒 no diff detected."
  rm 2>/dev/null diff.log
else
  echo "🐒 output differ:"
  cat diff.log
fi

rm .stdtmp .fttmp 2>/dev/null
