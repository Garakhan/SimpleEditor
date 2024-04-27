#include <iostream>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <unistd.h>
#include <format>
#include <termios.h>
#include <stdio.h>

#include "Editor.h"
#include "utility.h"
#include "exception.h"
using namespace std;

int main(int argc, char** argv) {
    Editor e;
    termaction::clrscr(TERM_READ_FD);
    termaction::mv2beg(TERM_WRITE_FD);
    termaction::clrscr(TERM_READ_FD);
    termaction::enableRawMode(TERM_WRITE_FD);
    // termaction::twrite(STDOUT_FILENO, "\x1b[?47h", 6);
    e.refreshEditorScreen();
    e.mainLoop();
    // termaction::twrite(STDOUT_FILENO, "\x1b[?47l", 6);
}