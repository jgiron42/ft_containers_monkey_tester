#!/bin/bash

INCLUDE_DIRECTORIES="-I.."

CFLAGS="-Wall -Werror -Wextra -std=c++98 $INCLUDE_DIRECTORIES"

trap "pkill fifodiff; pkill infinite_ft_containers; pkill infinite_std_containers; rm .stdpipe .ftpipe 2>/dev/null" INT

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

if [  ! -x fifodiff ] || [ "$(stat -c %Y fifodiff.cpp)" -ge "$(stat -c %Y fifodiff)" ]
then
  echo "compiling fifodiff..."
  clang++ $CFLAGS -D BEFORE_SIZE=-1 fifodiff.cpp -o fifodiff || exit
fi
if [ ! -x infinite_ft_containers ] || [ "$(check_last_change)" -ge "$(stat -c %Y infinite_ft_containers)" -o "$(stat -c %Y main.cpp)" -ge "$(stat -c %Y infinite_ft_containers)" ]
then
  echo "compiling ft... "
  clang++ $CFLAGS -D NTEST=-1 -D NAMESPACE=ft main.cpp -o infinite_ft_containers || exit
fi
if [  ! -x infinite_std_containers  ] || [ "$(stat -c %Y main.cpp)" -ge "$(stat -c %Y infinite_std_containers)" ]
then
  echo "compiling std... "
  clang++ $CFLAGS -D NTEST=-1 -D NAMESPACE=std main.cpp -o infinite_std_containers || exit
fi

rm .stdpipe .ftpipe 2>/dev/null
mkfifo .stdpipe .ftpipe

./fifodiff .stdpipe .ftpipe &
./infinite_std_containers $@ >> .stdpipe | ./infinite_ft_containers $@ >> .ftpipe &
cat << EOF""
...............................:*FV$V*:...........
..............................*V$$$$****:.........
.............................:***I$$III***:.......
..............................***I$$V***V$$V*:....
.............................***:**F$$$$$$$$$V*...
...........................:::*:::*F$$$$$$$$$$$V:.
...........................:**::::*$$$$$$$$$$$$$$:
.................:***********II****$$$$$$$$$$$$$$I
...............*FI**V$$$$$$$$$$$$V$$$$$$$$$$$$$$$V
.............:III***:::**V$$$$$$$$$$$$$$$$$$$$$$$*
.............:***II*:...*V$$V$$$$$$$$$$$$$$$$$$$$:
............:**..::*....**:*$$$$$$$$$$$$$$$$$$$$$:
:I$$:......:*:..........:****$$$$$$$$$$$$$$$$$$$V.
.FNNM:...................:***I$$$$$$$$$$$$$$$$$$*.
..*MNM*....................:.:$$$$$$$$$$$$$$$$$V..
...*MN$::****:........::******$$$$$$$$$$$$$$$$$:..
....*$**FVVV$*::::.:***********IV$$$$IF$$$$$$$*:..
...:::::*****::::::..::::************:::*****:....
MONKEY IS TESTING YOUR PROJECT...
EOF
wait
rm .stdpipe .ftpipe 2>/dev/null
pkill fifodiff


