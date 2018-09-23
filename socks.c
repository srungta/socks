#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

// This variable stored the termios state at program init.
struct termios original_termios;

// Resets the terminal state.
void disableRawMode(){
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
}

/*
  By default terminal runs in cooked mode, where the input is not received by the
  program till the user presses Enter key. Howvere this is not helpful while writing
  a text editor. In raw mode, we can capture the input behavior as we want.

  The terminal behavior is controlled through many flags and these flags can be
  read using tcgetattr in a struct called termios. it can then be edited and
  written back using tcsetattr.
*/
void enableRawMode()
{
  // Variable to read the current flag status.
  struct termios raw;

  // Get the current flag status detrermining terminal behavior.
  tcgetattr(STDIN_FILENO, &raw);

  // Disable the ECHO feature. This means whatever you type will not be displayed
  // on the terminal.
  raw.c_lflag &= ~(ECHO);

  // Write back the edited flags.
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void init(){
  // Save the original value in a global variable.
  tcgetattr(STDIN_FILENO, &original_termios);

  // Disable the raw mode when exiting the program.
  atexit(disableRawMode);

  // Enable the raw mode.
  enableRawMode();
}
/*
  Entry point of the program.
*/
int main()
{
  init();
  char c;

  /*
    read() reads the user input from the STDIN_FILENO standard input, which in
    our case is the console. It returns the number of characters entered and returns
    0 when EOF is reached. Here we are checking till the point where either the alphabet
    'q' is reached or the user explicitly exits the program.
    Any text enetered after the letter 'q' is discarded by this program and passed on
    to the terminal.
  */
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q')
  {
  }

  return 0;
}
