/*************************************************************************
    > File Name: client.c
    > Author: wangjx
 ************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
int bfx_sock_client(char *ip ,int port ,int family)                                
{                                                                               
    int sockfd ,ret;                                                            
    struct sockaddr_in addr;                                                    
    struct timeval ti;                                                          
                                                                                
    sockfd = socket(AF_INET ,family ,0);                                        
    if(sockfd < 0){                                                             
        goto err;                                                               
    }                                                                           
                                                                                
    addr.sin_family = AF_INET;                                                  
    addr.sin_port = htons(port);                                                
    addr.sin_addr.s_addr = inet_addr(ip);                                       
                                                                                                                                
    ti.tv_sec=5;                                                                
    ti.tv_usec=0;                                                               
    setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,&ti,sizeof(ti));                   
    setsockopt(sockfd,SOL_SOCKET,SO_SNDTIMEO,&ti,sizeof(ti));                   
                                                                                
    ret = connect(sockfd ,(struct sockaddr *)&addr ,                            
            (socklen_t)sizeof(struct sockaddr));                                
    if(ret < 0){                                                                
        goto err;                                                               
    }                                                                           
    return sockfd;                                                              
err:                                                                            
    if(sockfd > 0){                                                             
        close(sockfd);                                                          
        sockfd = -1;                                                            
    }                                                                                                                                          
    return sockfd;                                                              
}
int bfx_sock_reada(int sockfd ,char *buf ,int size)                                
{                                                                                  
    char *p = buf;                                                                 
    int retval = -1,len = size, n = 0;                                             
    for(;;) {                                                                      
        retval = read(sockfd ,p ,len);                                             
        if (retval < 0 && errno == EINTR)                                          
            continue;                                                              
        if (0 == retval) {retval = n; break;} // read not block,to eof             
        if(retval < 0){                                                            
            if (errno == EAGAIN) {retval = n; break;} //timeout                             
            goto err;                                                              
        } else {                                                                   
			printf("recv %s\n", p);
            break;                                                                 
        }                                                                          
        p += retval;                                                               
        len -= retval;                                                             
        n += retval;                                                               
    }                                                                              
                                                                                   
err:                                                                               
    return retval;                                                                 
}
int bfx_sock_write(int sockfd ,char *buf ,int size)                             
{                                                                               
    char *p = buf;                                                              
    int retval ,len = size, n = 0;                                              
                                                                                
    do {                                                                        
        retval = write(sockfd ,p ,len);                                         
        if(retval < 0){                                                         
            if (errno == EAGAIN) {retval = n; break;}
            goto err;                                                           
        }                                                                       
                                                                                
        p += retval;                                                            
        len -= retval;                                                          
        n += retval;                                                            
    }while(len);                                                                
                                                                                
err:                                                                            
    return retval;                                                              
}
int bfx_sock_set_noblock(int fd)                                                                                                               
{                                                                               
	int flags ,on = 1;                                                          

	flags = fcntl(fd, F_GETFL, 0);                                              
	setsockopt(fd , SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));                 
	return fcntl(fd, F_SETFL, flags|O_NONBLOCK);                                
}                                                                               

int bfx_sock_set_attr(int fd)                                                   
{                                                                               
	int ret , reuse = 1;                                                        
	ret = setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse));      
	ret &= setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse,sizeof(reuse));      
	return ret;                                                                 
}  
int bfx_sock_server(char *ip ,int port ,int family)                             
{                                                                               
	int sockfd ,ret;                                                            
	struct sockaddr_in addr;

	sockfd = socket(AF_INET ,family ,0);                                           
	if(sockfd < 0){                                                                
		goto err;                                                                  
	}                                                                              

	addr.sin_family = AF_INET;                                                     
	addr.sin_port = htons(port);                                                   
	if(!ip){                                                                       
		addr.sin_addr.s_addr = htonl(INADDR_ANY);                                  
	}else{                                                                         
		addr.sin_addr.s_addr = inet_addr(ip);                                      
	}                                                                              

	ret = bind(sockfd ,(struct sockaddr *)&addr ,                                  
			(socklen_t)sizeof(struct sockaddr));                                   
	if(ret < 0){                                                                   
		perror("bind failed");                                                     
		goto err;                                                                  
	}                                                                              

	if(family == SOCK_STREAM){                                                     
		ret = listen(sockfd ,5);                                                   
		if(ret < 0){                                                               
			goto err;                                                              
		}                                                                          
	}                                                                              
	if(bfx_sock_set_noblock(sockfd) < 0){                                          
		goto err;                                                                  
	}                                                                              
	if(bfx_sock_set_attr(sockfd) < 0){                                             
		goto err;                                                                  
	}                                                                              
	return sockfd; 
err:                                                                               
	if(sockfd > 0){                                                             
		close(sockfd);                                                          
		sockfd = -1;
	}                                                                           
	return sockfd;                                                              
}
void main()
{
	int sock;
	char *response = calloc(1, sizeof(100));;
	sock = bfx_sock_server("127.0.0.1", 5999, SOCK_STREAM);
	bfx_sock_reada(sock, response, 100);
	while (1) {
		sleep(1);
	}
}
