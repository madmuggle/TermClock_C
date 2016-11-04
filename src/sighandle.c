#include <signal.h>
#include <stdio.h>
#include <stdlib.h>


void sigint_handler(int signum)
{
  // show cursor, clear the screen, and reset the cursor to left top
  printf("\033[?25h\033[2J\033[0;0H");
  exit(0);
}


void initialize_signal()
{
  struct sigaction sig;

  sig.sa_handler = sigint_handler;
  sig.sa_flags = 0;

  sigaction(SIGINT, &sig, NULL);
}
