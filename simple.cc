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

struct termios orig_termios;

void disableRawMode(int fd) {
    /* Don't even check the return value as it's too late. */
        tcsetattr(fd,TCSAFLUSH,&orig_termios);
}

/* Called at exit to avoid remaining in raw mode. */
void editorAtExit(void) {
    disableRawMode(STDIN_FILENO);
}
/* Raw mode: 1960 magic shit. */
int enableRawMode(int fd) {
    struct termios raw;

    if (!isatty(STDIN_FILENO)) goto fatal;
    atexit(editorAtExit);
    if (tcgetattr(fd,&orig_termios) == -1) goto fatal;

    raw = orig_termios;  /* modify the original mode */
    /* input modes: no break, no CR to NL, no parity check, no strip char,
     * no start/stop output control. */
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    /* output modes - disable post processing */
    raw.c_oflag &= ~(OPOST);
    /* control modes - set 8 bit chars */
    raw.c_cflag |= (CS8);
    /* local modes - choing off, canonical off, no extended functions,
     * no signal chars (^Z,^C) */
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    /* control chars - set return condition: min number of bytes and timer. */
    raw.c_cc[VMIN] = 0; /* Return each byte, or zero for timeout. */
    raw.c_cc[VTIME] = 1; /* 100 ms timeout (unit is tens of second). */

    /* put terminal in raw mode after flushing */
    if (tcsetattr(fd,TCSAFLUSH,&raw) < 0) goto fatal;
    return 0;

fatal:
    errno = ENOTTY;
    return -1;
}

void editorQuit(int inp=STDIN_FILENO){
}

int main(int argc, char** argv) {
    TerminalHandleException::TerminalConnectionError();
    Editor e;
    // e.renderEditorCnt();
    auto scrnCnt = e.getEditorCnt();
    // cout<<scrnCnt<<endl;
    termaction::hidecursor(STDOUT_FILENO);
    termaction::clrscr(STDOUT_FILENO);
    termaction::mv2beg(STDOUT_FILENO);
    termaction::clrght(STDOUT_FILENO);
    write(STDOUT_FILENO, scrnCnt, strlen(scrnCnt));
    termaction::mv2beg(STDOUT_FILENO);
    termaction::showcursor(STDOUT_FILENO);
    enableRawMode(STDIN_FILENO);
    // termutil::enableRawMode();
    while(1){
        e.editorCursorAction();
    }
    return 0;

}