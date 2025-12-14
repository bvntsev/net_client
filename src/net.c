#include <stdlib.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>
#include <netinet/in.h>
#include <netinet/ip.h> 

#include "../include/net.h"


#define EXIT_ERROR(ARG)                         \
                        fprintf(stderr, ARG);   \
                        exit(EXIT_FAILURE);

#define PORT            12312
#define MAX_LEN         255


void send_package(int *arrivalfd, struct package *send_package) {
    ssize_t bytes_send = send(*arrivalfd, (void *)send_package,
                        sizeof(*send_package), 0);
    if (bytes_send == -1) {
        EXIT_ERROR("The incorrect send_package process\n")
    }
    else {
        printf("The packages was send\n");
    }
}


struct package *get_package(int *sockfd) {

    //   ssize_t recv(size_t size;
    //                    int sockfd, void buf[size], size_t size,
    //                    int flags);
    void *buf = (void *)malloc(sizeof(struct package));
    if (recv(*sockfd, buf, sizeof(struct package), 0) == -1) {
        EXIT_ERROR("The incorrect get_package process\n")
    }
    else {
        printf("The package was arrived\n");
        return (struct package *) buf;
    }
}

extern ssize_t 
getline(char **restrict lineptr, size_t *restrict n, FILE *restrict stream);

pthread_t r_thread, w_thread;


static void *write_message(void *userfd){

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    for (;;){
        printf("> ");
        fflush(stdout);

        char *response = NULL;
                //(char *)malloc(sizeof(char)*255);
        size_t len_response = 0; 

        ssize_t bytes_read = getline(&response, &len_response, stdin); 

        if (bytes_read == -1) {
            EXIT_ERROR("Error getline\n")
        }

        if (strncmp(response, "!exit", 5) == 0) {
            printf("Exit is working\n");
            free(response);

            if (close(*(int *) userfd) == 0){
                printf("The socket was closed successful\n");
            }
            else{
                printf("The socket was closed with an error\n");
            }
            pthread_cancel(r_thread);
            pthread_exit(NULL);
        }

        if (strncmp(response, "!clear", 6) == 0) {
    #define CLEAR "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n \
                   \n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
    #define CLEAR_LEN 28

            printf("%s", CLEAR);

            ssize_t write_bytes = write(*((int *)userfd), CLEAR, CLEAR_LEN);  
            response[0] = '\0';
            if ((size_t)write_bytes > CLEAR_LEN || 
                    write_bytes == -1) goto emergency_exit;

    #undef CLEAR
    #undef CLEAR_LEN
        }

        ssize_t write_bytes = write(*((int *)userfd), response, len_response);  
        if (write_bytes > (long int)len_response || write_bytes == -1){
emergency_exit:
            free(response);
            printf("The fd was closed \n");
            pthread_exit(NULL);
        }
        free(response); 
    }
}

static void *read_message(void *userfd){

    for (;;) {
        char *message = (char *) malloc(sizeof(char) * MAX_LEN );
        if (message){

            ssize_t read_byte = read(*((int *)userfd), message, MAX_LEN);

            if ((long)read_byte == 0) {
                printf("The client just closed the session\n");
                free(message);
            
                pthread_cancel(w_thread);

                pthread_exit(NULL);
            }

            if ((long)read_byte == -1 ){
                printf("The client got critical error\n");
                printf("Emergency exit\n");
                free(message);
                pthread_exit(NULL);
            }

            if (message[0] != '\0'){
                printf("\r>> %s", message);
                fflush(stdout);
                printf("> ");
                fflush(stdout);
            }
            free(message);

        }
        else {
            EXIT_ERROR("ERROR of allocated memory\n")
        }
    }
}

static void *create_communication_pthreads(int *userfd) {

    if ( pthread_create
            ( &r_thread, NULL, read_message, (void *)userfd) 
            != 0) {
        EXIT_ERROR("Incorrect create the read thread\n")
    }

    if ( pthread_create
            ( &w_thread, NULL, write_message, (void *)userfd) 
            != 0) {
        EXIT_ERROR("Incorrect create the thread\n")
    }
    
    void *retval = NULL;
    pthread_join(r_thread, NULL);
    pthread_join(w_thread, retval);
    return retval;
}

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

    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        EXIT_ERROR("SERVER: setsockopt(SO_REUSEADDR) failed\n");
    }
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

    struct server_data * server = (struct server_data *)    
                                    malloc(sizeof(struct server_data));
    server->client_amount = 1;

    server->serverfd = n_sockfd;
    server->clients = (struct client_data **)
                        malloc(sizeof(struct client_data)*5);

    server->clients[0] = (struct client_data *)get_package(&n_sockfd);

    send_package(&n_sockfd, (void *) server);

    printf("%d\n", server->clients[0]->clientfd);

    printf("my server fd - %d %d \n", sockfd, n_sockfd);

    printf("Clients amount is %hu \n", server->client_amount);
    printf("The name of client is %s\n", server->clients[server->client_amount-1]->name);
    create_communication_pthreads(&n_sockfd);


    printf("Session is done\n");

	close(n_sockfd);
	close(sockfd);

}

void new_client(char * client_name) {

    int clientfd = socket(AF_INET, SOCK_STREAM, 0);

    if (clientfd == -1) {
        EXIT_ERROR("CLIENT: The problems with creat socket\n ")
    }
    printf("The socket is correct\n");

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    socklen_t s_addrlen;
    s_addrlen = sizeof(serv_addr); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    
    if (connect(clientfd, (struct sockaddr *)&serv_addr, s_addrlen) == -1){
        EXIT_ERROR("CLIENT: The problems with connecting\n ")
    }
    
    printf("The connection is correct\n");

    printf("my client fd - %d \n", clientfd);

    struct package *sent_pkg = (struct package *)malloc(sizeof(struct package));
    
    sent_pkg->type = client_pkg;
    struct client_data *client = (struct client_data *)
                                    malloc(sizeof(struct client_data));

    client->clientfd = clientfd;
    sprintf(client->name, "MY CLIENT NAME");
    client->type = idle;

    sent_pkg->pkg = (void *)client;

    send_package(&clientfd, sent_pkg);


    struct package *server_pkg = get_package(&client->clientfd);
    client->serverfd = (int *) server_pkg->pkg;


    create_communication_pthreads(&client->clientfd);

    printf("Session is done\n");

    close(clientfd);

    return ;
}
