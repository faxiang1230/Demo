/*************************************************************************
    > File Name: test.c
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
    > Created Time: 2019年01月25日 星期五 07时18分15秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>   
#include <linux/un.h>   
#include <arpa/inet.h>   

#define DSET_IP_ADDRESS "192.168.100.100"
#define DEST_PORT 8000

int socket_test()
{
    int sockfd, ret, len, send_num;
    static struct sockaddr_un srv_addr;
    socklen_t addrlen;
    pid_t pid;
    struct sockaddr_in addr_serv;  
    char send_buf[20] = "hey, who are you?";

    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd<0){
        return -1; 
    }

    pid = fcntl(sockfd, F_GETOWN);
    printf("before SETOWN fown struct pid %d\n", pid);
    fcntl(sockfd, F_SETOWN, getpid());

    memset(&addr_serv, 0, sizeof(addr_serv));  
    addr_serv.sin_family = AF_INET;  
    addr_serv.sin_addr.s_addr = inet_addr(DSET_IP_ADDRESS);  
    addr_serv.sin_port = htons(DEST_PORT);  
    len = sizeof(addr_serv);  

    printf("client send: %s\n", send_buf);  
    while (1) {
        pid = fcntl(sockfd, F_GETOWN);
        printf("own pid :%d\n", pid);
        send_num = sendto(sockfd, send_buf, strlen(send_buf), 0, (struct sockaddr *)&addr_serv, len);  
        sleep(5);
    }
    return 0;
}
void sighandler(int signum) {
    printf("recv sig %d\n", signum);
}
void main()
{
    signal(SIGIO, sighandler);
    socket_test();
}
