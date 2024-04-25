#include <iostream>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <unistd.h>
#include <format>
#include <termios.h>

#include "Editor.h"
#include "utility.h"
#include "exception.h"
using namespace std;

int main(int argc, char** argv) {
    Editor e;
    // cout<<e.getNRow()<<endl;
    // return -1;
    termaction::clrscr(TERM_READ_FD);
    termaction::mv2beg(TERM_WRITE_FD);
    termaction::clrscr(TERM_READ_FD);
    termaction::enableRawMode(TERM_WRITE_FD);
    e.refreshEditorScreen();
    // termaction::mv2beg(TERM_WRITE_FD);
    e.mainLoop();
}