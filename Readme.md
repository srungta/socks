## References
The editor starts with reference to this [tutorial](https://viewsourcecode.org/snaptoken/kilo/01.setup.html).


## Using make file.
To build the file, run `make` and then run `.\socks`.

### Details of `Makefile`
> We have added a few things to the compilation command:
>
> - `$(CC)` is a variable that make expands to cc by default.
> - `-Wall` stands for **all Warnings**, and gets the compiler to warn you when it sees code in your program that might not technically be wrong, but is considered bad or questionable usage of the C language, like using variables before initializing them.
> - `-Wextra` and `-pedantic` turn on even more warnings. For each step in this tutorial, if your program compiles, it shouldn’t produce any warnings except for **unused variable** warnings in some cases. If you get any other warnings, check to make sure your code exactly matches the code in that step.
> - `-std=c99` specifies the exact version of the C language standard we’re using, which is C99. C99 allows us to declare variables anywhere within a function, whereas ANSI C requires all variables to be declared at the top of a function or block.

### Order of things
[x] Get terminal state, edit and restore it at program exit.
[x] Add canonical mode to show character definitions.
[x] Disable interrupt signals `Ctrl C` and `Ctrl Z`.
[x] Disable IEXTEN flag for `Ctrl V`.
[x] Disable `carriage return` and other miscellaneous flags.
[x] Add error handling.
[x] Enable `Ctrl Q` to as exit key.
[x] Add erase screen to make space for editor.
[x] Refresh the screen when program exits.
[x] Add `tilde` to the leftmost column
[x] Get window size to show correct number of tildes.
[ ] Add fallback when unable to get window size.
[ ] Hide the cursor when repainting.
[ ] Clear lines one at a time.
[ ] Display a welcome message.
[ ] Move the cursor around.
[ ] Move the cursor with arrow keys.
[ ] Prevent moving the cursor off screen.
[ ] Handle `Page Up` and `Page Down`.
[ ] Handle `Home` and `End` keys.
[ ] Handle `Delete` key.
[ ] Open a file.
[ ] vertical scrolling.
[ ] Horizontal scrolling.
[ ] 
