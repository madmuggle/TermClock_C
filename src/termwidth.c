#include <sys/ioctl.h>
#include <stdio.h>


/**
 * get the terminal width, the environment variable COLUMNS does not work
 * with getenv, because it's not exported by bash
 */
int
get_terminal_width (int *rows, int *cols)
{
  struct winsize ws;

  if (ioctl (0, TIOCGWINSZ, &ws) == -1)
    {
      perror ("ioctl, TIOCGWINSZ");
      return -1;
    }

  if (rows != NULL)
    *rows = ws.ws_row;

  if (cols != NULL)
    *cols = ws.ws_col;

  return 0;
}
