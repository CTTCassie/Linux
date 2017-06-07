semset=SemSet
SemSetSrc=comm.c SemSet.c
cc=gcc
$(semset):$(SemSetSrc)
	$(cc) -o $@ $^
.PHONY:clean
clean:
	rm -f $(semset)
