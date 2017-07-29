#!/usr/bin/python
# -*- coding: UTF-8 -*-

import random
s=(int)(random.uniform(1,100))   #产生一个1~100之间的数字
#print(s)
count=5  #控制猜数字的次数，只能猜count次
while 1:
    n=int(input("Please input num:"))
    if count <= 0:
	   print "sorry,您的次数已经用完"
	   break
    if n > s:
	   print('large')
	   count -= 1
    if n < s:
	   print('small')
	   count -= 1
    if n == s:
	   print('you are clever')
	   break
