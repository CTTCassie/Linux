ROOT_PATH=$(shell pwd)
INCLUDE_PATH=-I$(ROOT_PATH)/../lib/include
LIB_PATH=-L$(ROOT_PATH)/../lib/lib
LIB=-lmysqlclient

CC=g++

.PHONY:all
all:cgi_insert cgi_select cgi_delete cgi_update

cgi_insert:cgi_insert.cpp sql_api.cpp
	$(CC) -o $@ $^ $(INCLUDE_PATH) $(LIB_PATH) $(LIB)

cgi_select:cgi_select.cpp sql_api.cpp
	$(CC) -o $@ $^ $(INCLUDE_PATH) $(LIB_PATH) $(LIB)

cgi_delete:cgi_delete.cpp sql_api.cpp
	$(CC) -o $@ $^ $(INCLUDE_PATH) $(LIB_PATH) $(LIB)

cgi_update:cgi_update.cpp sql_api.cpp
	$(CC) -o $@ $^ $(INCLUDE_PATH) $(LIB_PATH) $(LIB)

.PHONY:clean
clean:
	rm -rf cgi_insert cgi_select cgi_delete cgi_update output/

.PHONY:output
output:
	mkdir output
	cp -rf $(ROOT_PATH)/httpd output/
	cp -rf $(ROOT_PATH)/wwwroot output/
	mkdir -p output/wwwroot/cgi_bin/
	cp -rf ../cgi/cgi_math $(ROOT_PATH)/output/wwwroot/cgi_bin/
	cp -rf cgi_insert $(ROOT_PATH)/output/wwwroot/cgi_bin/
	cp -rf cgi_select $(ROOT_PATH)/output/wwwroot/cgi_bin/
	cp -rf cgi_delete $(ROOT_PATH)/output/wwwroot/cgi_bin/
	cp -rf cgi_update $(ROOT_PATH)/output/wwwroot/cgi_bin/
