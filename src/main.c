#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>

#include "../include/net.h"

int main(int argc, const char *argv[]) {

  if (argc != 2) {
    fprintf(stderr, "The invalid amount of input arguments\n");
    exit(EXIT_FAILURE);
  }

  if (strcmp(argv[1], "server") == 0) {
    printf("Find server arg\n");
    new_server();
  } else if (strcmp(argv[1], "client") == 0) {
    printf("Find client arg\n");
    return 0;
  } else {
    fprintf(stderr, "The incorrect user command.\nUse --help\n");
    exit(EXIT_FAILURE);
  }


  return EXIT_SUCCESS;
}
