#!/bin/bash
echo "compling $1"
if [[ $1 == *.c ]]
then 
	gcc -ggdb `pkg-config --cflags opencv` -o `basename $1 .c` $1 `pkg-config --libs opencv` -llua5.1;
elif [[ $1 == *.cpp ]]
then 
	g++ -std=c++11 -ggdb `pkg-config --cflags opencv` -o `basename $1 .cpp` $1 `pkg-config --libs opencv` -llua5.1;
else 
	echo "Please compile only .c or .cpp files! "
fi 
echo "Output file => ${1%.*}"
