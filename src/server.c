#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX 1
#define PORT 6666

struct Sockinfo
{
    int fd;
    pthread_t tid;
    struct sockaddr_in addr;
};

struct Sockinfo sockinfo[128];


void* working(void* arg)
{
    while(1)
    {
        struct Sockinfo* info = (struct Sockinfo*)arg;
        char buf[MAX];
        //通过文件描述符读取对应内存空间中的通信数据到buf中
        int ret = read(info->fd,buf,sizeof(buf));
        if(ret==0)
        {
            printf("客户端已关闭连接...\n");
            info->fd=-1;
            break;
        }
        else if(ret==-1)
        {
            printf("接收数据失败...\n");
            info->fd=-1;
            break;
        }
        else
        {
            //打印buf的内容
            printf("%s\n",buf);
            //do something
        }
    }
    return NULL;

}


int main()
{
    //创建监听套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }

    printf("socket successfully created..\n");

    struct sockaddr_in addr;
    // assign IP, PORT
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    // binding newly created socket to given IP and verification
    int ret = bind(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    if (ret != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }

    printf("socket successfully binded..\n");


    // now server is ready to listen and verification
    ret = listen(sockfd,100);
    if (ret != 0) {
        printf("Listen failed...\n");
        exit(0);
    }

    printf("server listening...\n");

    socklen_t len = sizeof(struct sockaddr);
    int max = sizeof(sockinfo)/sizeof(sockinfo[0]);
    for(int i=0;i<max;i++)
    {
        bzero(&sockinfo[i],sizeof(sockinfo[i]));
        sockinfo[i].fd=-1;
        sockinfo[i].tid=-1;
    }

    while(1)
    {
        // 遍历存储参数的数组，当fd为-1时，说明此文件描述符未被使用
        struct Sockinfo* pinfo;
        for(int i=0; i<max; ++i)
        {
            if(sockinfo[i].fd == -1)
            {
                pinfo = &sockinfo[i];
                break;
            }
            if(i == max-1)
            {
                sleep(1);
                i--;
            }
        }
        //被动监听客户端的握手请求，三次握手成功则返回一个通信文件描述符，并将客户端的ip地址存进addr
        //当无客户端进行握手请求时，代码会阻塞在accept，直到下一个客户端连接
        int connfd = accept(sockfd, (struct sockaddr*)&pinfo->addr, &len);
        printf("parent thread, connfd: %d\n", connfd);
        if(connfd == -1)
        {
            perror("accept");
            exit(0);
        }
        pinfo->fd = connfd;
        //创建子线程，pinfo作为参数传入working函数，会将创建的新线程的线程id赋给pinfo的tid
        pthread_create(&pinfo->tid, NULL, working, pinfo);
        //将线程设置为分离模式，当线程结束后系统会自动回收线程资源
        pthread_detach(pinfo->tid);
    }

    // 释放资源
    close(sockfd);  // 监听
    return 0;

}
