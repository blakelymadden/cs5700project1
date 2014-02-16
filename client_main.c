/**
 * Blake Madden
 * 
 * The main function for the client program.
 * TODO: arguments parsing should be cleaner and failsafe. Currently
 *       imporoper input can cause crashes and unexpected sequences
 * TODO: delegate the hello message handling to a function outside
 *       of main
 */

#include "client.h"

int main(int argc, char* argv[]) {
  if (argc != 3 && argc != 5) {
    printf("USAGE: %s [hostname] [NEU ID]\n", argv[0]);
    exit(1);
  }
  char *port = DEFAULT_PORT;
  int str_size = strlen(argv[2]);
  const char *hello_head = "cs5700spring2014 HELLO ";

  if (argc == 5) { // if we have the -p flag
    port = argv[2];
    str_size = strlen(argv[4]);
  }
  str_size += strlen(hello_head) + 1; // +1 for the \n
  char HELLO_MESSAGE[str_size];
  memset(HELLO_MESSAGE, 0, (str_size));

  if (argc == 3) { // no -p flag
    sprintf(HELLO_MESSAGE, "%s%s\n", hello_head, argv[2]);
    makeSession(argv[1], port, HELLO_MESSAGE, (str_size));
  } else { // if we have the -p flag
    sprintf(HELLO_MESSAGE, "%s%s\n", hello_head, argv[4]);
    makeSession(argv[3], port, HELLO_MESSAGE, (str_size));
  }
  return 0;
}
