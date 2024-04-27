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
    // cout<<e.getNRow()<<endl;
    // return -1;
    termaction::clrscr(TERM_READ_FD);
    termaction::mv2beg(TERM_WRITE_FD);
    termaction::clrscr(TERM_READ_FD);
    termaction::enableRawMode(TERM_WRITE_FD);
    // termaction::twrite(STDOUT_FILENO, "asd\r\n", 3);
    // char c;
    // while(true) {
    //     if (read(STDIN_FILENO, &c, 1)==1) {
    //         if (c==CTRL_Q){
    //             termaction::twrite(STDOUT_FILENO, "qwe\r\n", 5);
    //             termaction::disableRawMode(TERM_WRITE_FD);
    //             break;
    //         }
    //         else {
    //             termaction::twrite(STDOUT_FILENO, &c, 1);
    //         }
    //    }
    // }
    e.refreshEditorScreen();
    // termaction::mv2beg(TERM_WRITE_FD);
    e.mainLoop();
}