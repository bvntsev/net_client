#include "../include/net.h"

#include <stdlib.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <fcntl.h>

#define EXIT_ERROR(ARG)\
fprintf(stderr, ARG);\
exit(EXIT_FAILURE);

#define PORT 12312
#define MAX_LEN 255

ssize_t getline(char **restrict lineptr, size_t *restrict n, FILE *restrict stream);

void new_server() {

    int sockfd, n_sockfd;
    socklen_t s_addrlen, c_addrlen;
    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        EXIT_ERROR("SERVER: The problems with the creat socket \n")
    }

    memset((void *)&serv_addr, 0, sizeof(serv_addr));
	
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    s_addrlen = sizeof(serv_addr);
	c_addrlen = sizeof(cli_addr);
    if (bind(sockfd, (struct sockaddr * ) &serv_addr, s_addrlen) == -1) {
        EXIT_ERROR("SERVER: The problems with bind \n")
    }
	printf("The bind is correct \n");

    if (listen(sockfd, 5) == -1) {
        EXIT_ERROR("SERVER: The problems with listen \n")
    }
	printf("The listen is correct \n");
	   
	n_sockfd = accept(sockfd, (struct sockaddr * ) &cli_addr, &c_addrlen);
    if (n_sockfd == -1) {
        EXIT_ERROR("SERVER: The problems with accept \n")
    }
	printf("The accept is correct \n");

    for (;;){
        char *str = NULL;
        read(n_sockfd, str, MAX_LEN);
        printf(">> %s\n", str);

    }
	close(n_sockfd);
	close(sockfd);
}

void new_client() {
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientfd == -1) {
        EXIT_ERROR("CLIENT: The problems with creat socket\n ")
    }
    printf("The socket is correct\n");
    struct sockaddr_in cli_addr, serv_addr;
    socklen_t s_addrlen, c_addrlen;
    s_addrlen = sizeof(serv_addr); 
    c_addrlen = sizeof (cli_addr);

    cli_addr.sin_family = AF_INET;
    cli_addr.sin_port = htons(PORT);
    cli_addr.sin_addr.s_addr = INADDR_ANY;
    
    if (connect(clientfd, (struct sockaddr *)&cli_addr, c_addrlen) == -1){
        EXIT_ERROR("CLIENT: The problems with connecting\n ")
    }
    printf("The connection is correct");

    for (;;){
        char *response = NULL;
        size_t len_response = 0; 

        size_t bytes_read = getline(&response, &len_response, stdin); 

        if (bytes_read == -1) {
            EXIT_ERROR("Error getline\n")
        }
        write(clientfd, response, bytes_read);  
        free(response); 
    }
    close(clientfd);

    return ;


	
}
