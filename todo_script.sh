#!/bin/bash

# $1 is first arg
echo $1

foo=3

echo $foo # THIS DOESN'T WORK???

if [ "$1" != "todo.txt" ] # THIS DOESN'T WORK EITHER
then
  echo "not todo.txt"
  exit
fi

