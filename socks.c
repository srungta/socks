/*** includes ***/
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

/*** defines ***/
/*
This macro ANDs the key value with 00011111. This is similar to what Ctrl key does
when pressed before a alphabet. It clears out the first three bits and returns the rest.
*/
#define CTRL_KEY(k) ((k) & 0x1f)

/*** data ***/
// This variable stored the termios state at program init.
struct termios original_termios;

/*** terminal ***/

// Method to handle errors during program execution.
void die(const char *s){
  perror(s);
  exit(1);
}

// Resets the terminal state.
void disableRawMode(){
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios) == -1) {
    die("disableRawMode - tcsetattr");
  }
}

/*
  By default terminal runs in cooked mode, where the input is not received by the
  program till the user presses Enter key. However this is not helpful while writing
  a text editor. In raw mode, we can capture the input behavior as we want.

  The terminal behavior is controlled through many flags and these flags can be
  read using tcgetattr in a struct called termios. it can then be edited and
  written back using tcsetattr.
*/
void enableRawMode()
{
  // Variable to read the current flag status.
  struct termios raw;

  // Get the current flag status determining terminal behavior.
  if(tcgetattr(STDIN_FILENO, &raw) == -1){
      die("enableRawMode - tcgetattr");
  }

  /* Disable the parent flag using masks:
      - CS8 : CS8 is not a flag, it is a bit mask with multiple bits,
              which we set using the bitwise-OR (|) operator unlike all the flags we are turning off.
              It sets the character size (CS) to 8 bits per byte.
  */
  raw.c_cflag |= (CS8);

  /* Disable the following input flags:
      - BRKINT : When enabled, a break condition will cause a SIGINT signal to be sent to the program, like pressing Ctrl-C.
      - ICRNL : Ctrl + M is printed as newline becasue the terminal facilitates conversion of carriage return to new lines.
                This also change Enter from 10[new line] to 13[carriage return].
      - INPCK : Enables parity checking, which doesn’t seem to apply to modern terminal emulators.
      - ISTRIP : Causes the 8th bit of each input byte to be stripped, meaning it will set it to 0. This is probably already turned off.
      - IXON : Disables Ctrl + S and Ctrl + Q, which stop and resume data transmission from terminal.
  */
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

  /* Disable the following output flags:
      - OPOST : Disables output post processing like changing \n to \r\n.
  */
  raw.c_oflag &= ~(OPOST);

  /* Disable the following flags:
      - ECHO [input]: whatever you type will not be displayed on the terminal.
      - ICANON [input]: Enables the canonical mode, where input is processed byte wise instead of line wise.
      - ISIG [input]: Disable the interrupt signals like Ctrl + C[terminate] and Ctrl + Z[suspend]
      - IEXTEN: Disable Ctrl + V which lets you type another character and send it as is to terminal
                (for example, you can type Ctrl + V and then Ctrl + C , it does not interrupt the program, rather sends it as a character.)
  */
  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

  // Sets the minimum number of bytes of input needed before read() returns.
  raw.c_cc[VMIN] = 1;

  // Sets the maximum amount of time to wait before read() returns. Unit : 1/10 sec.
  // This does not seem to work in Bash on Windows.
  raw.c_cc[VTIME] = 1;

  // Write back the edited flags.
  if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1){
      die("enableRawMode - tcsetattr");
  }
}

/*
Reads one character at a time from the terminal and returns it to the calling method.
TODO : Expand to handle escape sequences.
*/
char editorReadKey(){
  // Number of characters read.
  int nread;
  // Initialize with an empty chaacter.
  char c = '\0';
  /*
    read() reads the user input from the STDIN_FILENO standard input, which in
    our case is the console. It returns the number of characters entered and returns
    0 when EOF is reached. Here we are checking till the point a character is entered.
  */
  while((nread = read(STDIN_FILENO, &c, 1)) != 1)
  {
    if (nread == -1 && errno != EAGAIN) {
      die("editorReadKey - read()");
    }
  }
  return c;
}


/*** output ***/

/*
Method clears out the area in the terminal to be used as the editor area.
  write : Writes to the output buffer.
  STDOUT_FILENO : Standard output. In our case th terminal.
  \x1b : Escape character.
  [ : Used after the escape character to specify the command to be executed.
  2 : Parameter to J which says clean the whole page.
  J : Erase in display [http://vt100.net/docs/vt100-ug/chapter3.html#ED]
  4 : Number of bytes being written to output.
*/
void editorRefreshScreen(){
  write(STDOUT_FILENO, "\x1b[2J", 4);
}

/*** input ***/

/*
Reads the key from the terminal and exits the program if the key is Ctrl Q.
TODO : Handle other editor related key commands.
*/
void editorProcessKey(){
  char c = editorReadKey();
  switch (c){
    case CTRL_KEY('q'):
      exit(0);
      break;
  }
}

/*** init ***/
/*
  Setup up the flags for the editor to work.
*/
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
  while (1)
  {
    editorRefreshScreen();
    editorProcessKey();
  }

  return 0;
}
