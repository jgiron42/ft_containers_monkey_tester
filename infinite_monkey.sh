#!/bin/bash

echo "This command is deprecated! please use \"./monkey.sh -i\"" >&2
echo "(press enter...)" >&2
read VOID
./monkey.sh -i $@