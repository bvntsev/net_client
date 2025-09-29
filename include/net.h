#ifndef NET_H
#define NET_H

#include <sys/socket.h>

typedef  unsigned char      u8;
typedef  unsigned short     u16;
typedef  unsigned int       u32;
typedef  unsigned long      u64;

enum pkg_type {
    server_pkg,
    client_pkg
};

struct package {
    enum pkg_type           type;
    void*                   pkg;

};

enum status_type {
    idle,
    chatting,
    leave
};

struct server_data {
    int                         serverfd;
    struct client_data**        clients;
    unsigned short              client_amount;
};

struct client_data {
    char                    name[256];
    int                     clientfd;
    enum status_type        type;
    int*                    serverfd;
};

void new_server();

void new_client(char * client_name);

#endif /* NET_H */
