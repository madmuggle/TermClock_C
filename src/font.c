#include <stdio.h>
#include <stdlib.h>
#include "utils.h"


/**
 * Global variable
 * the font cache, including font for numbers and blink chars
 */
char font[9 * 8 * 12];


/**
 * load a font set of number 0-9 (and other characters) from a font file,
 * the font have to be in 9 * 8 format
 */
int load_numberfont(char *fontfile, char *fontcache)
{
  FILE *fp;
  int i, j;

  if ((fp = fopen(fontfile, "rb")) == NULL)
    return -1;

  for (i = 0; i < 12; i++)
  {
    for (j = 0; j < 9; j++)
    {
      if (fread(fontcache + 8 * 9 * i + j * 8, 1, 8, fp) != 8) {
        printf("failed read font for number: %d\n", i);
        return -1;
      }
      // ignore the '\n' in a number
      if (fgetc(fp) == -1) {
        printf("failed remove the '\\n' in number: %d\n", i);
        return -1;
      }
    }
    // ignore the '\n' between two number font
    if (fgetc(fp) == -1) {
      printf("failed remove the '\\n' between %d and %d\n", i, i + 1);
      return -1;
    }
  }

  if (fclose(fp) != 0)
    p_exit("failed close the font file\n");

  return 0;
}


/**
 * get the font index of number
 */
int getfont_idx(int number)
{
  return number * 8 * 9;
}


/**
 * this function is just for test
 */
int print_number(int number)
{
  int i, j;

  if (number > 9 || number < 0)
  {
    fprintf(stderr, "number should be >=0 and <=9\n");
    return 1;
  }

  for (i = 0; i < 9; i++)
  {
    for (j = 0; j < 8; j++)
      putchar(font[getfont_idx(number) + i * 8 + j]);
    putchar('\n');
  }
  putchar('\n');

  return 0;
}


/**
 * load font from the disk file to memory
 */
void initialize_font()
{
  if ((load_numberfont("./font/number.txt", font) != 0) &&
      (load_numberfont("/usr/local/share/number.txt", font) != 0))
    p_exit("failed to open the font file\n");
}


/*
int main(int argc, const char **argv)
{
  int i;

  initialize_font();

  for (i = 0; i < 10; i++)
  {
    if (print_number(i) != 0)
      p_exit("failed to print number\n");
  }

  return 0;
}
*/
