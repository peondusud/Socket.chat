#ifndef _SPLITSCREEN_H_
#define _SPLITSCREEN_H_

/**
 * Initializes split screen output
 * @param nlines the number of lines of the output area
 * @param prompt the prompt used for input
 */
void splitscreen_init( int nlines, const char *prompt);

/**
 * Display the prompt in the input area
 */
void splitscreen_prompt(void);

/**
 * Reads one character from input. This read is blocking.
 * @return the read character or -1 if error
 */
int splitscreen_input_char( void);

/**
 * Reads one line from input. This read is blocking.
 * @param buf the buffer where to copy read characters
 * @param size the maximum number of character to read
 * @return the number of read characters or -1 if error
 */
int splitscreen_input_line( char *buf, int size);

/**
 * Outputs one line in the output area
 * @param buf the characters to display
 */
void splitscreen_output( const char *buf);

/**
 * Returns the terminal to previous state.
 * Must be called before exiting.
 */
void splitscreen_quit( void);

#endif
