#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include "splitscreen.h"

static WINDOW *win_output;
static WINDOW *win_input;

static const char *default_prompt = ">";
static const char *input_prompt;

void splitscreen_init( int nlines, const char *prompt)
{
  initscr(); 
  cbreak();
  nonl();
  intrflush(stdscr, FALSE);
  keypad(stdscr, TRUE);

  win_output = newwin( nlines, 0, 0, 0);
/*   wborder( win_output, 0, 0, 0, '-', 0, 0, 0, 0); */
  scrollok( win_output, TRUE);


  win_input = newwin( 1, 0, nlines, 0);

  if (prompt == NULL)
    input_prompt = default_prompt;
  else
    input_prompt = strdup( prompt);
}

void splitscreen_prompt(void)
{
  werase( win_input);
  waddstr( win_input, input_prompt);
  waddch( win_input, ' ');
  wrefresh( win_input);
}

int splitscreen_input_char( void)
{
  int ch;

  ch = wgetch( win_input);
  if ( ch == ERR)
    return -1;

  return ch;
}

int splitscreen_input_line( char *buf, int size)
{
  if (wgetnstr( win_input, buf, size) == ERR)
    return -1;

  return strlen( buf);
}

void splitscreen_output( const char *buf)
{
    waddstr( win_output, buf);
    waddch( win_output, '\n');
    wrefresh( win_output);
    wrefresh( win_input);
}

void splitscreen_quit( void)
{
  endwin();
}
