#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

int main(){
  int sockfd = 0, n = 0;
  char* ip = "127.0.0.1";
  char recvBuff[1024];
  struct sockaddr_in serv_addr;

  memset(recvBuff, '0', sizeof(recvBuff));\

  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    printf("\n socket error");
    return 1;
  }

  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(1234);

  if(inet_pton(AF_INET, ip, &serv_addr.sin_addr)<=0){
    printf("\n inet pton error");
    return 1;
  }

  if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
    printf("\n connect error");
    return 1;
  }

  while( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0){
    recvBuff[n] = 0;
    if(fputs(recvBuff, stdout) == EOF){
      printf("\n fputs error");
    }
  }

  if( n < 0){
    printf("\n read error");
  }

  return 0;

}
