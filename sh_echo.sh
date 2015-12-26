#!/bin/bash

#!bash file

printf "try %s function for %d time(s), \n new line is ok?" "printf()" 1

myvar="hello world"
echo $myvar
echo "$myvar"
echo '$myvar'
echo \$myvar
y='$'myvar
echo $y
echo {eval y}
eval y='$'myvar
echo $y
echo $$ # current process id
echo $0 # the name of shell script
echo $# # the number of paramters passed
echo "$*" # print the list of parameters, separated by the first character in IFS
echo $* # print the list of parameters
echo "$@" # print the list of paramters
echo $@ # print the list of paramters

# expr
num1=3
num2=$((expr $num1+2))
num3='expr $num1+2'
echo '$num2:' $num2
echo '$num3:' $num3
echo "expr $num1 | expr $num2"
echo 'expr $num2 | expr $num1'
echo 'expr 0 | expr $num1'
echo 'expr $num1 & expr $num2'
echo 'expr 0 & expr $num1'

# if...then...elif...then...fi
echo "Is this morning?"
read timeofday
if [ "$timeofday" == "yes" ]
then
  echo "good morning"
elif [ "$timeofday" == "no" ]
then
  echo "good afternoon"
else
  echo "not recognized"
fi

# for
for ff in $(ls ~/Codes/*.cpp)
do
  echo $ff
done

# while
num=1
num=$($num+2) # $($num+2) is to get ($num+2) command result, so is 1+2
echo "\$num after \$(\$num+2): $num"
num=$(($num+3)) # $num is assigned with expression result value
echo "\$num after \$((\$num+3)): $num"
count=1
while [ $count -lt 5 ]
do
  echo '$count' "is $count"
  count=$(($count + 1))
done

# case
case "$timeofday" in
  yes|y|YES|Y|Yes) echo "good morning";;
  no|n|NO|N) echo "good afternoon";;
  y*|Y*) echo "not sure morning";;
  n*|N*) echo "not sure afternoon";;
  *) echo "not recognized";;
esac

# function
echo $v_str
v_str="global variable"
local v_local_o="local variable outside"
foo()
{
  local v_str="local variable"
  v_temp="v_temp without 'local'"
  local v_local="v_local with 'local'"
  echo "inside foo()"
  echo $v_local_o
  echo $v_str
}

foo
echo "after foo()"
echo $v_str
echo $v_temp
echo $v_local

# function with paramter
goo()
{
  echo "Input paramter(s): $*"
  local v_path="$1"+"/*"+"$2"
  echo v_path
#  ls v_path | wc -l
#  count=ls v_path | wc -l
}
goo ~/Codes .h
cd ~/Codes


# trap
my_file=/tmp/my_file_$$
trap 'rm -f $my_file' INT
echo creating file $my_file
date > $my_file
echo "press CTRL-C to interrupt..."
while [ -f $my_file ]
do
  echo $my_file exists
  sleep 1
done
echo "$my_file doesn't exist"

trap INT
date > $my_file
echo "press CTRL-C to interrupt..."
while [ -f $my_file ]
do
  echo $my_file exists
  sleep 1
done
echo "this line doesn't run!"
#clear
#var="FOO"
#echo $var

#read -p "Please enter your name: " name
#read -p "Please enter your age: " age
#read -p "Please enter your sex M/F: " sex
#echo "So you're a $age old $sex called $name."
 
