#include "../include/net.h"
#include <stdlib.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>


void new_server() {
  int sockfd, n_sockfd, portn, n;
  socklen_t addrlen;
  struct sockaddr_in serv_addr, cli_addr;
  sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

  if (sockfd < 0) {
    fprintf(stderr, "The problems with the creat socket \n");
    exit(EXIT_FAILURE);
  }

  portn = 8080;
  /* bzero((char *) &serv_addr, sizeof(serv_addr)); */

  serv_addr.sin_family = AF_UNIX;
  serv_addr.sin_port = htons(portn);
  serv_addr.sin_addr.s_addr = INADDR_ANY;

  addrlen = sizeof(serv_addr);
  printf("bind == %d\n", bind(sockfd, (struct sockaddr *)&serv_addr, addrlen));
  
     /* int bind(int sockfd, const struct sockaddr *addr, */
     /*          socklen_t addrlen); */
  printf("%d\n", getsockname(sockfd, (struct sockaddr *) &serv_addr, &addrlen));
  exit(0);
  while (0) {
    char *input = (char *)malloc(sizeof(char) * 256);
    scanf("%255s", input);
    if (strcmp(input, "/exit") == 0) {
      printf("Success close the server\n"); /* Maybe here i need to disconnect
                                               any user connection */
      free(input);
      exit(EXIT_SUCCESS);
    }
    free(input);
  }
}

void new_client() {
     /* int connect(int sockfd, const struct sockaddr *addr, */
     /*             socklen_t addrlen); */
  
    }
