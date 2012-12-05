#include <stdlib.h>
#include "splitscreen.h"

#define OUTPUT_WINDOW_LINES 20
#define N 128

int main( int argc, char **argv)
{
  char line[N];

  splitscreen_init( OUTPUT_WINDOW_LINES, NULL);

  splitscreen_output( "hello");

  for( ;;) {
    splitscreen_prompt();
    if (splitscreen_input_line( line, N) < 0)
      break;

    splitscreen_output( line);
  }

  splitscreen_quit();
}
