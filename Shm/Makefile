cli=client
ser=server
cliSrc=client.c comm.c
serSrc=server.c comm.c
cc=gcc

.PHONY:all
all:$(cli) $(ser)

$(cli):$(cliSrc)
	$(cc) -o $@ $^ 
$(ser):$(serSrc)
	$(cc) -o $@ $^  
.PHONY:clean
clean:
	rm -f $(cli) $(ser)
