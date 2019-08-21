#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "sighandle.h"
#include "font.h"
#include "buffer.h"
#include "utils.h"


/**
 * slide is a flag that indicate the working mode of the clock program
 */
int slide = 0;

/**
 * show with color
 */
char *color_string = NULL;


void
parse_parameters (int argc, const char **argv)
{
  const char *s;
  int i;
  for (i = 0; i < argc; i++)
    {
      s = argv[i];
      if (strcmp (s, "slide") == 0)
	slide = 1;

      else if (strcmp (s, "red") == 0)
	color_string = "\033[41m";

      else if (strcmp (s, "green") == 0)
	color_string = "\033[42m";

      else if (strcmp (s, "yellow") == 0)
	color_string = "\033[43m";

      else if (strcmp (s, "blue") == 0)
	color_string = "\033[44m";
    }
}


int
main (int argc, const char **argv)
{
  time_t time_now;
  struct tm *t;

  parse_parameters (argc, argv);

  initialize_signal ();

  initialize_font ();

  initialize_contentbuf ();

  printf ("\033[2J\033[0;0H\033[s\033[?25l");

  /* no clear version */
//  printf("\n\n\n\n\033[4A\033[s\033[?25l");

  while (1)
    {
      /* use `time' and `localtime' to get hour, minute, second */
      if (time (&time_now) == -1)
	p_exit ("failed calling time\n");

      if ((t = localtime (&time_now)) == NULL)
	p_exit ("failed calling localtime\n");

      update_contentbuf (t->tm_hour, t->tm_min, t->tm_sec);

      if (slide)
	{
	  // dynamic display(shift)
	  update_windowbuf ();
	  flush_windowbuf ();
	}
      else
	{
	  // static display
	  flush_contentbuf ();
	}

      usleep (50000);
    }

  return 0;
}
