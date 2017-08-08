#!/bin/bash

ROOT_PATH=$(pwd)
INCLUDE=$ROOT_PATH
SRC=$(ls *.c | tr '\n' ' ')
OBJ=$(ls *.c | sed 's/\.c/\.o/g' | tr '\n' ' ')
SERVER_NAME=httpd
CC=gcc
LINK=-lpthread

#low Makefile
CGI_PATH=$ROOT_PATH/cgi
MATH_SRC=$(ls $CGI_PATH | grep 'math' | grep -E '.c$')
MATH_CGI_BIN=cgi_math
cat << EOF >${CGI_PATH}/Makefile
${MATH_CGI_BIN}:${MATH_SRC}
		${CC} -o \$@ \$^

.PHONY:clean
clean:
		rm -f ${MATH_CGI_BIN}

.PHONY:output
output:
		cp ${MATH_CGI_BIN} ..
EOF

#http Makefile
cat << EOF >Makefile

.PHONY:all
all:${SERVER_NAME} cgi

${SERVER_NAME}:${OBJ}
		${CC} -o \$@ \$^ ${LINK}
%.o:%.c
		$CC -c \$<

.PHONY:cgi
cgi:
		cd cgi;make;make output;cd -;
		cp -rf ${SERVER_NAME} sql/;

.PHONY:clean
clean:
		rm -rf ${SERVER_NAME} *.o output;cd cgi;make clean;cd -
		rm -rf ${ROOT_PATH}/output/wwwroot/cgi_bin

.PHONY:output
output:all
		mkdir output
		cp -rf ${SERVER_NAME} output/
		cp -rf log output/
		cp -rf conf output/
		cp -rf wwwroot output/
		mkdir -p output/wwwroot/cgi_bin/
		cp -f cgi_math output/wwwroot/cgi_bin/
		cp -r httpd_ctl.sh output/

EOF
