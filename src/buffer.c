#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "termwidth.h"
#include "font.h"
#include "config.h"
#include "utils.h"


// print color or not, depending on the command line parameters
extern char *color_string;

char *contentbuf;
char *windowbuf;

int column_content;
int column_window;
int space_number;


void initialize_contentbuf()
{
  // if failed to get the column number, assume terminal columns to be 80
  if (get_terminal_width(NULL, &column_window) == -1)
    column_window = 80;

  column_content = NUMBER_WIDTH * 8;

  space_number = (column_window - column_content) / 2;
  if (space_number < 0)
    space_number = 0;

  if ((contentbuf = malloc(9 * column_content)) == NULL)
    p_exit("failed to malloc the contentbuf\n");

  memset(contentbuf, ' ', 9 * column_content);
//  memset(contentbuf, '*', 9 * column_content);

  if ((windowbuf = malloc(9 * column_window)) == NULL)
    p_exit("failed to malloc the windowbuf\n");

  memset(windowbuf, ' ', 9 * column_window);
}


void fill_contentbuf_rect(int offset, char *buf)
{
  int l, i;

  for (l = 0; l < 9; l++)
  {
    for (i = 0; i < 8; i++)
      contentbuf[l * column_content + offset + i] = buf[l * 8 + i];
  }
}


/**
 * blink the separator between hour, minute and second
 */
void reverse_separator()
{
  // blink have a font in the global font array(index 10 and 11)
  static int blink_idx = 10;

  fill_contentbuf_rect(NUMBER_WIDTH * 2, &font[getfont_idx(blink_idx)]);
  fill_contentbuf_rect(NUMBER_WIDTH * 5, &font[getfont_idx(blink_idx)]);

  if (blink_idx == 10)
    blink_idx = 11;
  else
    blink_idx = 10;
}


void update_contentbuf(int hour, int minute, int second)
{
  int t1, t2;
  static int prevsecond = 0;

  t1 = hour / 10;
  t2 = hour % 10;
  fill_contentbuf_rect(NUMBER_WIDTH * 0, &font[getfont_idx(t1)]);
  fill_contentbuf_rect(NUMBER_WIDTH * 1, &font[getfont_idx(t2)]);

  t1 = minute / 10;
  t2 = minute % 10;
  fill_contentbuf_rect(NUMBER_WIDTH * 3, &font[getfont_idx(t1)]);
  fill_contentbuf_rect(NUMBER_WIDTH * 4, &font[getfont_idx(t2)]);

  t1 = second / 10;
  t2 = second % 10;
  fill_contentbuf_rect(NUMBER_WIDTH * 6, &font[getfont_idx(t1)]);
  fill_contentbuf_rect(NUMBER_WIDTH * 7, &font[getfont_idx(t2)]);

  if (prevsecond != second)
  {
    reverse_separator();
    prevsecond = second;
  }
}



void my_putchar(char c)
{
  if (color_string != NULL && c != ' ')
    printf("%s%c\033[0m", color_string, ' ');
  else
    putchar(c);
}


/** THIS IS FOR STATIC DISPLAY
 * show time on the terminal, the terminal should be vt100 compatible,
 * or the output might be a mess
 * this function will simply show what is in contentbuf, but can set a
 * offset for contentbuf, this is why this function is written
 */
void flush_contentbuf()
{
  int row, col;
  int i;

  // restore to the left top position
  printf("\033[u");

  for (row = 0; row < 9; row++)
  {
    for (i = 0; i < space_number; i++)
      putchar(' ');

    for (col = 0; col < column_content; col++)
      my_putchar(*(contentbuf + row * column_content + col));

    putchar('\n');
  }
}


/**
 * shift the contentbuf to windowbuf
 */
void update_windowbuf()
{
  int row, col;
  static int offset = 0;

  if (offset + column_content < 0)
    offset = column_window;

  for (col = 0; col < column_window; col++)
  {
    for (row = 0; row < 9; row++)
    {
      if (col >= offset && col - offset <= column_content - 1)
        *(windowbuf + row * column_window + col) =
          *(contentbuf + row * column_content + col - offset);
      else
        *(windowbuf + row * column_window + col) = ' ';
    }
  }

  offset -= 1;
}


/** THIS IS FOR DYNAMIC DISPLAY
 */
void flush_windowbuf()
{
  int row, col;

  // restore to the left top position
  printf("\033[u");

  for (row = 0; row < 9; row++)
  {
    for (col = 0; col < column_window; col++)
      my_putchar(*(windowbuf + row * column_window + col));
    putchar('\n');
  }
}
