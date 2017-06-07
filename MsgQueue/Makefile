cli=client
ser=server
cc=gcc
serSrc=server.c comm.c
cliSrc=client.c comm.c
.PHONY:all
all:$(cli) $(ser)

$(cli):$(cliSrc)
	$(cc) -o $@ $^
$(ser):$(serSrc)
	$(cc) -o $@ $^
.PHONY:clean
clean:
	rm -f $(cli) $(ser)
