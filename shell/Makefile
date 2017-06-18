src=myshell.c
dest=myshell
cc=gcc
$(dest):$(src)
	$(cc) -o $@ $^
.PHONY:clean
clean:
	rm -f $(dest)
