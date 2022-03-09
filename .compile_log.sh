#!/bin/bash

if [ "$1" != "ft" -a "$1" != "std"  ]
then
  echo "Usage: $( basename $0 ) <namespace>" >&2
  echo "example: $( basename $0 ) ft" >&2
  exit 1
fi

if [ ! -e log/log.cpp ]
then
  echo "no log to compile" >&2
fi

. .setup

for i in "${INCLUDE_DIRECTORIES[@]}"
do
	CFLAGS+=" -I"$i
done

echo "🐒 compiling..."
clang++ $CFLAGS -D NAMESPACE=$1 -I. -I srcs log/log.cpp -o bin/log_$1 || exit
echo "🐒 bin/log_$1 generated"