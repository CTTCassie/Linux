ROOT_PATH=$(shell pwd)
CLIENT_PATH=$(ROOT_PATH)/client
SERVER_PATH=$(ROOT_PATH)/server
CONF_PATH=$(ROOT_PATH)/conf
LIB_PATH=$(ROOT_PATH)/lib
LOG_PATH=$(ROOT_PATH)/log
WINDOW_PATH=$(ROOT_PATH)/window
DATAPOOL_PATH=$(ROOT_PATH)/pool
COMM_PATH=$(ROOT_PATH)/comm

CC=g++

LIB=-lpthread -ljson 
L_PATH=-L$(LIB_PATH)/lib

INCLUDE=-I$(DATAPOOL_PATH) -I$(LOG_PATH) -I$(WINDOW_PATH) -I$(COMM_PATH) -I$(LIB_PATH)/include -I$(COMM_PATH)

SERVER=udpServer
CLIENT=udpClient

SERVER_SRC=$(shell ls $(SERVER_PATH) $(DATAPOOL_PATH) $(COMM_PATH) $(LOG_PATH) | egrep -v '.*\.h' | egrep '.*\.cpp')
CLIENT_SRC=$(shell ls $(CLIENT_PATH) $(LOG_PATH) $(COMM_PATH) $(WINDOW_PATH) | egrep -v '.*\.h' | egrep '.*\.cpp')

SERVER_OBJ=$(shell echo $(SERVER_SRC) | sed 's/\.cpp/\.o/g')
CLIENT_OBJ=$(shell echo $(CLIENT_SRC) | sed 's/\.cpp/\.o/g')

.PHONY:all
all:$(SERVER) $(CLIENT)

$(SERVER):$(SERVER_OBJ)
	@echo Link [$^] to [$@] ...	
	$(CC) -o $@ $^ $(LIB)  $(L_PATH)
	@echo Link done...

$(CLIENT):$(CLIENT_OBJ)
	@echo Link [$^] to [$@] ...
	$(CC) -o $@ $^ $(LIB) -lncurses $(L_PATH)
	@echo Link done...

%.o:$(SERVER_PATH)/%.cpp
	@echo Compling [$^] to [$@] ...
	$(CC) -c $< $(INCLUDE) 

%.o:$(CLIENT_PATH)/%.cpp
	@echo Compling [$^] to [$@] ... 
	$(CC) -c $< $(INCLUDE)

%.o:$(DATAPOOL_PATH)/%.cpp
	@echo Compling [$^] to [$@] ...
	$(CC) -c $<
%.o:$(LOG_PATH)/%.cpp
	@echo Compling [$^] to [$@] ...
	$(CC) -c $<
%.o:$(WINDOW_PATH)/%.cpp
	@echo Compling [$^] to [$@] ...
	$(CC) -c $<
%.o:$(COMM_PATH)/%.cpp
	@echo Compling [$^] to [$@] ...
	$(CC) -c $< $(INCLUDE) -D_FAST_


.PHONY:output
output:
	mkdir -p output/log
	cp -rf conf output/
	cp server_ctl.sh output/
	cp client_ctl.sh output/
	cp $(SERVER) output/
	cp $(CLIENT) output/

.PHONY:clean
clean:
	rm -rf $(SERVER) $(CLIENT) *.o output/

.PHONY:debug
debug:
	@echo $(SERVER_SRC)
	@echo $(CLIENT_SRC)
	@echo $(SERVER_OBJ)
	@echo $(CLIENT_OBJ)
