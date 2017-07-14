#!/bin/bash

i=0
str=""
lable=("|" "/" "-" "\\")  #顺时针打印
while [ $i -le 100 ]
do
  printf "[%-100s][%d%%][%c]\r" "$str" "$i" "${lable[$index]}"
  let i++
  str='#'$str
  let index=i%4
  sleep 0.1
done
