
.PHONY:all
all:httpd cgi

httpd:httpd.o main.o 
		gcc -o $@ $^ -lpthread
%.o:%.c
		gcc -c $<

.PHONY:cgi
cgi:
		cd cgi;make;make output;cd -;
		cp -rf httpd sql/;

.PHONY:clean
clean:
		rm -rf httpd *.o output;cd cgi;make clean;cd -
		rm -rf /home/ctt/Item/httpd/http_server/output/wwwroot/cgi_bin

.PHONY:output
output:all
		mkdir output
		cp -rf httpd output/
		cp -rf log output/
		cp -rf conf output/
		cp -rf wwwroot output/
		mkdir -p output/wwwroot/cgi_bin/
		cp -f cgi_math output/wwwroot/cgi_bin/
		cp -r httpd_ctl.sh output/

