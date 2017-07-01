.PHONY:all
all:tcp_server tcp_client

tcp_server:tcp_server.c
	gcc -o $@ $^

tcp_client:tcp_client.c
	gcc -o $@ $^

.PHONY:clean
clean:
	rm -f tcp_server tcp_client
