ROOT=$(shell pwd)
INCLUDE=-I $(ROOT)/../lib/include
LIB=-L $(ROOT)/../lib/lib

Test:dataType.cpp jsonApi.cpp
	g++ -o $@ $^ $(INCLUDE) $(LIB) -ljson
.PHONY:clean
clean:
	rm -f Test
