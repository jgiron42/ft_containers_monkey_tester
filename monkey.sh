#!/bin/bash

INCLUDE_DIRECTORIES="-I.."

CFLAGS="-Wall -Werror -Wextra -std=c++98 $INCLUDE_DIRECTORIES"

trap "pkill fifodiff; pkill std_containers; pkill ft_containers" INT


check_last_change()
{
  ret=0
  for file in $(find ../srcs . | grep .hpp)
  do
    tmp="$(stat -c "%Y" $file )"
    : $(( ret = tmp > ret ? tmp : ret ))
  done
  echo $ret
}

if [ ! -x ft_containers ] || [ "$(check_last_change)" -ge "$(stat -c %Y ft_containers)" -o "$(stat -c %Y main.cpp)" -ge "$(stat -c %Y ft_containers)" ]
then
  echo "compiling ft... "
  clang++ $CFLAGS -D NAMESPACE=ft main.cpp -o ft_containers || exit
fi
if [  ! -x std_containers  ] || [ "$(stat -c %Y main.cpp)" -ge "$(stat -c %Y std_containers)" ]
then
  echo "compiling std... "
  clang++ $CFLAGS -D NAMESPACE=std main.cpp -o std_containers || exit
fi

./std_containers $@ > .stdtmp
./ft_containers $@ > .fttmp

echo "diff:"
diff .stdtmp .fttmp
rm .stdtmp .fttmp 2>/dev/null


