# Info
SimpleEditor - a very simple editor implemented in C++. It simply enables raw mode, clears the screen and at each key stroke it clears the screen and redraw the whole content.
Since it does not implement any efficient data structure handling, or buffer management, depending on the ligthness of the terminal emulator, you may see flickering.
SimpleEditor also does not handle window resizing, and maximum area of the editor is considered to be the current window area.
