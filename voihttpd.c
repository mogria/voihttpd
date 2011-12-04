#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//network libraries
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>


#define BUFLEN 1024
#define NAME "voihttpd"

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

  socklen_t clilen;

  char buffer[BUFLEN];

  struct sockaddr_in server_addr; //Server Adress
  struct sockaddr_in cli_addr; //Server Adress

  int n;

  // check if enougth arguments are given
  if(argc < 2) {
    fprintf(stderr, "%s: no port provided\n", NAME);
    exit(1);
  }

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    error("cannot open the socket", 2);
  }

  //initialize addr struct to zero
  bzero((char *) &server_addr, sizeof(server_addr));

  port = atoi(argv[1]); //grab port number
  
  //fill data into the server addr struct
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);

  if(bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
    error("can't bind to address", 3);
  }

  listen(sockfd,5);
  clilen = sizeof(cli_addr);
  clientsocketfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
  if(clientsocketfd < 0) {
    error("can't accept", 4);
  }

  bzero(buffer, 256);

  n = read(clientsocketfd, buffer, BUFLEN - 1);
  if(n < 0) {
    error("can't read from socket", 5);
  }

  printf("%s\n", buffer);

  n = write(clientsocketfd, "Gotcha!", 7);
  if(n < 0) {
    error("can't write to socket", 6);
  }

  close(clientsocketfd);
  close(sockfd);
}
