#include "TCPTimeTask.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>

#define BUFSIZE 1024

static const char* hostName = "www.google.com";
static const int portNo = 80;

void error(char *msg) {
    perror(msg);
}

std::map<int, double> TCPTimeTask::operation() const {
  std::map<int, double> result;
  int sockfd, n;
  struct sockaddr_in serveraddr;
  struct hostent *server;

  /* socket: create the socket */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error("ERROR opening socket");

  /* gethostbyname: get the server's DNS entry */
  server = gethostbyname(hostName);
  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host as %s\n", hostName);
  }

  /* build the server's Internet address */
  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  bcopy((char *)server->h_addr,
      (char *)&serveraddr.sin_addr.s_addr, server->h_length);
  serveraddr.sin_port = htons(portNo);
  clock_t t;
  t = clock();
  if (connect(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
    error("ERROR connecting");

  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC;
  close(sockfd);
  result[0] = time_taken;
  return result;
}

const std::string TCPTimeTask::id = "30c55933-a5ff-46ec-a3ee-4c77f421102c";

const std::string TCPTimeTask::m_description = "Task to collects connect time to www.google.com:80. ";

const int TCPTimeTask::m_numberof_metrices = 1;
