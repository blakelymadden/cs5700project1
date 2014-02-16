/*
 * Written by Blake Madden
 * 
 * This program is designed to follow a specific protocol described
 * in the assignment documentation for CS5700 Network Fundamentals
 * at Northeastern University.
 */

#include "client.h"

/**
 * gets the struct addrinfo for the designated node
 */
struct addrinfo *setupAddrList(char *node, char *port) {
  struct addrinfo hints;
  struct addrinfo *res;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  int failure = getaddrinfo(node, port, &hints, &res);
  if (failure) {
    fprintf(stderr, "FAILED: getaddrinfo: %s\n", gai_strerror(failure));
    exit(2);
  }
  return res;
}

/**
 * Initializes the socket and connects it to the provided node
 */
int openConnection(char *node, char *port) {
  struct addrinfo *res, *temp;
  int sockfd;
  res = setupAddrList(node, port);
  for (temp = res; temp != NULL; temp = temp->ai_next) {
    sockfd = socket(temp->ai_family, temp->ai_socktype, temp->ai_protocol);
    if (sockfd == -1)
      continue;

    if (connect(sockfd, temp->ai_addr, temp->ai_addrlen) != -1) {
      break;
    }
    close(sockfd);
  }

  if (temp == NULL) {
    fprintf(stderr, "FAILED: could not connect\n");
    exit(3);
  }
  freeaddrinfo(res);
  return sockfd;
}

/**
 * This function parses the response from the server and performs
 * a simple mathematical operation based on the response.
 *
 * TODO: This function is ridiculously verbose, and I need to
 *       heavily refactor it or perhaps start from scratch. C is not
 *       particularly ammenable to string parsing, but I can surely
 *       do better than this.
 *
 * Note: The function does not handle bad input. It strictly follows
 *       the protocol specified in the assignment.
 */
char *doMath(char *buf) {
  char *solution = (char*)malloc(100);
  while (*buf != ' ') { // disregard everything up to the first space;
    buf++;
  }
  buf++; // move to the 'S' in STATUS
  /* if we have a STATUS message */
  if (buf[0] == 'S' && buf[1] == 'T' && buf[2] == 'A'
      && buf[3] == 'T' && buf[4] == 'U' && buf[5] == 'S') {
    char first[5];
    char operator[2];
    char second[5];
    int first_num;
    int second_num;
    int ans;
    int i = 0;
    buf = buf + 7;

    while (*buf != ' ') {
      first[i] = *buf;
      i++;
      buf++;
    }
    first[i] = '\0';
    first_num = atoi(first);
    i = 0;

    buf++;
    operator[0] = buf[0];
    operator[1] = '\0';

    buf = buf + 2;

    while (*buf != '\n') {
      second[i] = *buf;
      i++;
      buf++;
    }
    first[i] = '\0';
    second_num = atoi(second);

    if (operator[0] == '+') {
      ans = first_num + second_num;
    } else if (operator[0] == '-') {
      ans = first_num - second_num;
    } else if (operator[0] == '*') {
      ans = first_num * second_num; 
    } else if (operator[0] == '/') {
      ans = first_num / second_num;
    }
    sprintf(solution, "cs5700spring2014 %d\n", ans);
    return solution;
  } else { // making the bold assumption that the BYE message is the only other message
    printf("%s", buf);
    free(solution);
  }
  return NULL;
}

/*
 * Sets everything up for the while(1) loop that accepts requests until
 * it receives the final BYE message
 */
void makeSession(char *node, char *port, char *HELLO_MESSAGE, int str_size) {
  int sockfd = openConnection(node, port);
  char buffer[MAX_BUFFER];
  char *solution = NULL;
  int read_size;
  if ((read_size = send(sockfd, HELLO_MESSAGE, str_size, 0)) == -1) {
    fprintf(stderr, "FAILED: write\n");
    exit(4);
  }
  while (1) {
    if ((read_size = recv(sockfd, buffer, MAX_BUFFER, 0)) == -1) {
      perror("FAILED: recv");
      exit(7);
    }
    buffer[read_size] = '\0';
    if (read_size == -1) {
      perror("read");
      exit(5);
    }
    solution = doMath(buffer);
    if (solution == NULL)
      break;
    if (send(sockfd, solution, strlen(solution), 0) == -1) {
      fprintf(stderr, "FAILED: send solution\n");
      exit(6);
    }
    memset(buffer, 0, MAX_BUFFER);
    free(solution);
  }
  close(sockfd);
}
