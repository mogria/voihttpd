#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

//network libraries
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include "lib/assoc_array.h"
#include "lib/config.h"


#define BUFLEN 1024
#define NAME "voihttpd"

void handleHTTPRequest(int client_socket);
void error(const char *msg, int nr);
struct assoc_array *defaultConfig();

struct assoc_array *getConfig() {
  static struct assoc_array default_config;
  assoc_array(&default_config);
  assoc_set(&default_config, "port", "80");
  return &default_config;
}

void handleHTTPRequest(int client_socket) {
  char buffer[BUFLEN];

  bzero(buffer, 256);

  int n;

  n = read(client_socket, buffer, BUFLEN - 1);
  if(n < 0) {
    error("can't read from socket", 6);
  }

  printf("%s\n", buffer);

  n = write(client_socket, "Gotcha!", 7);
  if(n < 0) {
    error("can't write to socket", 7);
  }
}

void error(const char *msg, int nr) {
  char errorbuffer[256];
  char formatnr[] = "%s: %i: %s";
  char formatnonr[] = "%s: %s";
  if(nr == 0) {
    snprintf(errorbuffer, 256, formatnonr, NAME, msg);
    perror(errorbuffer);
  } else {
    snprintf(errorbuffer, 256, formatnr, NAME, msg);
    perror(errorbuffer);
    exit(nr);
  }
}

int main(int argc, char *argv[]) {

  int sockfd; //socket file descriptor
  int port; //port number
  int clientsocketfd; //socket file descriptor of the client
  int pid;
  void (*handle)(int) = handleHTTPRequest;
  struct assoc_array *config = getConfig();
  struct assoc_array *additional_config = read_config("config");

  // Copy over the things needed from the additional config
  for(assoc_rewind(config); assoc_valid(config); assoc_next(config)) {
    void *value = assoc_get(additional_config, assoc_current(config));
    if(value != NULL) {
      assoc_set(config, assoc_key(config), value);
    }
  }


  socklen_t clilen;

  struct sockaddr_in server_addr; //Server Adress
  struct sockaddr_in cli_addr; //Server Adress

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    error("cannot open the socket", 2);
  }

  //initialize addr struct to zero
  bzero((char *) &server_addr, sizeof(server_addr));

  port = atoi((char *)assoc_get(config, "port")); //grab port number
  
  //fill data into the server addr struct
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);

  if(bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
    error("can't bind to address", 3);
  }

  listen(sockfd,5);
  clilen = sizeof(cli_addr);
  while(1) {
    clientsocketfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if(clientsocketfd < 0) {
      error("can't accept", 4);
    }

    pid = fork();
    if(pid < 0) {
      error("can't fork", 5);
    }
    
    if(pid == 0) { //child
      close(sockfd);
      handle(clientsocketfd);
      exit(0);
    } else { //parent
      close(clientsocketfd);
    }

  }
  return 0;
}
