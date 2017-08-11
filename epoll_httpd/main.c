#include"httpd.h"

static void Usage(const char *proc)
{
    printf("Usage:%s [local_ip] [local_port]\n",proc);
}

static epbuf_p alloc_epbuf(int fd)
{
    epbuf_p ptr=(epbuf_p)malloc(sizeof(epbuf_t));
    if(ptr == NULL){
        perror("malloc");
        exit(6);
    }
    ptr->fd=fd;
    return ptr;
}

static void del_epbuf(epbuf_p ptr)
{
    if(ptr != NULL){
        free(ptr);
        ptr=NULL;
    }
}

void set_fd_nonblock(int sock)  //设置成非阻塞的
{
    int old_opt=fcntl(sock,F_GETFL);
    int new_opt=old_opt | O_NONBLOCK;
    fcntl(sock,F_SETFL,new_opt);
    return old_opt;
}

int main(int argc,char *argv[])
{
    if(argc != 3){
        Usage(argv[0]);
        return 1;
    }
    int listenfd=StartUp(argv[1],atoi(argv[2]));

    int epfd=epoll_create(256); //创建一个epoll模型

    if(epfd < 0){
        perror("epoll_create error");
        return 5;
    }

    struct epoll_event ev;
    ev.events=EPOLLIN | EPOLLET;   //将工作模式设置成ET模式

    ev.data.ptr=alloc_epbuf(listenfd);

    set_fd_nonblock(listenfd);    //如果工作模式是ET模式则需要设置成非阻塞的

    epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev); //将监听套接字添加到epoll模型中去

    while(1)
    {
        struct epoll_event evs[32];
        int max_evs=32;
        int timeout=1000;   //设置超时时间
        int nums=0;         //是就绪文件描述符的个数
        switch(nums=epoll_wait(epfd,evs,max_evs,timeout))
        {
            case -1:   //epoll_wait失败
               perror("epoll_wait error");
               break;
            case 0:  //等待超时
               printf("timeout...\n");
               break;
            default:
            {
                int i=0;
                for(;i<nums;i++)
                {
                    int sock=((epbuf_p)(evs[i].data.ptr))->fd;
                    if(sock == listenfd && (evs[i].events & EPOLLIN))   //是监听套接字并且发生了EPOLLIN事件
                    {
                        struct sockaddr_in client;
                        socklen_t len=sizeof(client);
                        int new_sock=accept(sock,(struct sockaddr *)&client,&len);
                        if(new_sock < 0)
                        {
                            perror("accept arror");
                            exit(6);
                        }
                        //此时说明已经有链接到达了
                        ev.events=EPOLLIN | EPOLLET;  //ET模式则需要设置成非阻塞的

                        set_fd_nonblock(new_sock);    //如果工作模式是ET模式则需要设置成非阻塞的

                        ev.data.ptr=alloc_epbuf(new_sock);
                        epoll_ctl(epfd,EPOLL_CTL_ADD,new_sock,&ev);
                    }
                    else if(sock != listenfd && (evs[i].events & EPOLLIN)) //不是监听套接字并且发生了EPOLLIN事件
                    {
                        handler_sock(sock);
                    }
                    else  //其他事件不做处理
                    {}
                }
            }
               break;
        }
    }
}
