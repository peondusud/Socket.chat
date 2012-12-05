#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "splitscreen.h"

#define OUTPUT_WINDOW_LINES 20
#define N 128


static void test()
{
  char line[N];
  int i = 0;

  splitscreen_prompt();
  for( ;;) {
    fd_set rfds;
    struct timeval timeout;
    int retval;
    int ch;

    FD_ZERO( &rfds);
    FD_SET( 0, &rfds);
    timeout.tv_sec = (rand()%3) + 1;
    timeout.tv_usec = 0;

    retval = select( 1, &rfds, NULL, NULL, &timeout) ;

    if (retval < 0) {
      perror( "select()");
      return;
    }
    if (retval == 0) {
      splitscreen_output( "timeout");
      continue;
    }
      
    if ((ch = splitscreen_input_char()) < 0)
      return;

    /* fprintf( stderr, "got char %d %c\n", ch, (char)ch); */

    if (ch == '\n' || ch == '\r') {
      line[i] = '\0';
      i = 0;
      splitscreen_output( line);
      splitscreen_prompt();
    } else {
      if (i < N-1) 
	line[i++] = (char)ch;
    }
  }
}


int main( int argc, char **argv)
{
  splitscreen_init( OUTPUT_WINDOW_LINES, NULL);

  splitscreen_output( "welcome");

  test();

  splitscreen_quit();
}
