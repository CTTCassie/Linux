ROOT_PATH=$(shell pwd)
OUTPUT=${ROOT_PATH}/../output
TITLE=${ROOT_PATH}/../title
BIN=crawler
TEST_BIN=crawler_test
SRC=$(shell ls *.cc | grep -v crawler_main.cc | grep -v crawler_test.cc)
HEADER=$(shell ls *.h)

LIB=-lcurl -lboost_regex
TEST_FLAG=-I ~/third_part/include/ -L ~/third_part/lib/ -lgtest -lgtest_main -lpthread -lcurl -lboost_regex

.PHONY:all
all:${BIN} ${TEST_BIN}

${BIN}:crawler_main.cc ${SRC} ${HEADER}
	g++ crawler_main.cc ${SRC} -o ${OUTPUT}/${BIN} ${LIB}

${TEST_BIN}:crawler_test.cc ${SRC} ${HEADER}
	g++ crawler_test.cc ${SRC} -o ${OUTPUT}/${TEST_BIN} ${TEST_FLAG}

.PHONY:clean
clean:
	rm -rf ${OUTPUT}/${BIN} ${OUTPUT}/${TEST_BIN} ${TITLE}/*
