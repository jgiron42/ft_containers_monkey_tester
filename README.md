# ft_containers_monkey_tester (personal project)

![monke](https://upload.wikimedia.org/wikipedia/commons/f/f1/Monkey-typing.jpg)

This repo is a tester based on the [fuzzing](https://en.wikipedia.org/wiki/Fuzzing) technique for the 42 ft_containers project. The tester generate two programs (std and ft) with a certain amount of random test in both then compare the two outputs

## monkey.sh

monkey.sh will launch the tester, execute a thousand test and do a diff on the two outputs

(⚠️ when correcting a ft_containers, please use the infinite_monkey.sh script which is more efficient)

## infinite_monkey.sh

infinite_monkey.sh will launch the tester, execute an inifinity of test then return. the fifodiff program present in srcs is used for the comparison of the two output streams

## usage

just clone the repo wherever you want and execute one of the scripts `./[monkey.sh|infinite_monkey.sh[] <container>` (you may have to setup your includes name in includes.hpp)
