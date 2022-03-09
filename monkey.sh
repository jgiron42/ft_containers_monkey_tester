#!/bin/bash

if [ "$1" != "vector" -a "$1" != "stack" -a "$1" != "map" -a "$1" != "set" ]
then
  echo "Usage: $( basename $0 ) <container>" >&2
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

if [ ! -x bin/ft_containers_$C ] || [ "$(check_last_change)" -ge "$(stat -c %Y bin/ft_containers_$C)" -o "$(stat -c %Y srcs/main.cpp)" -ge "$(stat -c %Y bin/ft_containers_$C)" ]
then
  echo "🐒 compiling ft... "
  clang++ $CFLAGS -D NAMESPACE=ft -D MONKEY_$(echo $C | tr 'a-z' 'A-Z')  srcs/main.cpp -o bin/ft_containers_$C || exit
fi

if [  ! -x bin/std_containers_$C  ] || [ "$(stat -c %Y srcs/main.cpp)" -ge "$(stat -c %Y bin/std_containers_$C)" ]
then
  echo "🐒 compiling std... "
  clang++ $CFLAGS -D NAMESPACE=std -D MONKEY_$(echo $C | tr 'a-z' 'A-Z') srcs/main.cpp -o bin/std_containers_$C || exit
fi

./bin/std_containers_$C $@ > .stdtmp
./bin/ft_containers_$C $@ > .fttmp

if diff -y  .stdtmp .fttmp > diff.log
then
  echo "🐒 no diff detected."
  rm 2>/dev/null diff.log
else
  echo "🐒 output differ:"
  cat diff.log | grep -B50 -A10 '  <\|  >'
fi

rm .stdtmp .fttmp 2>/dev/null
