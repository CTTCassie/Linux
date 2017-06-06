server=fifo_server
client=fifo_client
cc=gcc
ServerSrc=server.c
ClientSrc=client.c
.PHONY:all
all:$(server) $(client)
$(server):$(ServerSrc)
	$(cc) -o $@ $^
$(client):$(ClientSrc)
	$(cc) -o $@ $^
.PHONY:clean
clean:
	rm -f $(client) $(server) myfifo
