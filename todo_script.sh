#!/bin/bash

# $1 is first arg
echo $1

foo=3

echo $foo

if [ "$1" != "todo.txt" ]
then
  echo "not todo.txt"
  exit
fi

