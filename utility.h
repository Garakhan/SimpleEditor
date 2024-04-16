#ifndef __TERMINAL_UTILITY_H__
#define __TERMINAL_UTILITY_H__

#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdarg.h>

#include "debug.h"
#include "exception.h"
#if defined(__DEBUG__)
    #include <iostream>
#endif

#define __UNUSED__ __attribute__((unused))

#define CURSOR_C_MV2BEG         "\x1b[H"
#define CURSOR_C_MV2END         "\x1b[L"
#define CURSOR_C_HIDE           "\x1b[?25l"
#define CURSOR_C_SHOW           "\x1b[?25h"
#define CURSOR_C_CLRSCR         "\x1b[1J"
#define CURSOR_C_CLRGHT         "\x1b[K"
#define CURSOR_C_TILDA          "~\x1b[0K\r\n"
#define CURSOR_C_NEWLINE        "\x1b[0K\r\n"
#define CURSOR_C_POSITION       "\x1b[6n"
#define CURSOR_C_RGHTBTM        "\x1b[999C\x1b[999B"
#define CURSOR_C_BACKSPC        "\x1b[\b"
#define CURSOR_L_MV2BEG         3
#define CURSOR_L_MV2END         3
#define CURSOR_L_HIDE           6
#define CURSOR_L_SHOW           6
#define CURSOR_L_CLRSCR         4
#define CURSOR_L_CLRGHT         3
#define CURSOR_L_TILDA          7
#define CURSOR_L_NEWLINE        6
#define CURSOR_L_POSITION       4
#define CURSOR_L_RGHTBTM        12
#define CURSOR_L_BACKSPC        3
#define SEQLEN(str)             strlen(str)

enum KEY_STROKES {
    ENTER           = 13,
    CTRL_Q          = 17,
    ESC             = 27,
    BACKSPACE       = 127,
    LEFT_SQUARE     = 91,
    UP_ARROW        = 65,
    DOWN_ARROW,
    RIGHT_ARROW,
    LEFT_ARROW,

    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    DEL_KEY,
    HOME_KEY,
    END_KEY,
    PAGE_UP,
    PAGE_DOWN
};


namespace termaction {

    inline int mv2beg (int ofd) {
        int seql = write(ofd, CURSOR_C_MV2BEG, CURSOR_L_MV2BEG);
        return seql;
    }
    inline int mv2end (int ofd) {
        int seql = write(ofd, CURSOR_C_MV2END, CURSOR_L_MV2END);
        return seql;
    }
    inline int hidecursor (int ofd) {
        int seql = write(ofd, CURSOR_C_HIDE, CURSOR_L_HIDE);
        return seql;
    }
    inline int showcursor (int ofd) {
        int seql = write(ofd, CURSOR_C_SHOW, CURSOR_L_SHOW);
        return seql;
    }
    inline int clrscr (int ofd) {
        int seql = write(ofd, CURSOR_C_CLRSCR, CURSOR_L_CLRSCR);
        return seql;
    }
    inline int clrght(int ofd) {
        int seql = write(ofd, CURSOR_C_CLRGHT, CURSOR_L_CLRGHT);
        return seql;
    }
    inline int insertnl(int ofd) {
        int seql = write(ofd, CURSOR_C_NEWLINE, CURSOR_L_NEWLINE);
        return seql;
    }
    inline int qcurspos(int ofd){
        int seql = write(ofd, CURSOR_C_POSITION, CURSOR_L_POSITION);
        return seql;
    }
    inline int backspace(int ofd){
        int seql = write(ofd, CURSOR_C_BACKSPC, CURSOR_L_BACKSPC);
        return seql;
    }

    inline int twrite(int ofd, char* s, int l){
        int seql = write(ofd, s, l);
        return seql;
    }
    inline size_t setCursorPos(int ofd, int row, int col) {
        char cursorPosSeq[80];
        sprintf(cursorPosSeq, "\x1b[%zu;%zuH", row, col);
        size_t seq = write(ofd, cursorPosSeq, strlen(cursorPosSeq));
        return seq;
    }
} 

namespace termutil {
    extern termios original_terminal;
    int enableRawMode(int fd);
    struct winsize getWindowSize();
    int getCursorPosition(int, int, int*, int*);//Editor::cursorPos will be referenced
    // int setCursorPosition(int, int, size_t*, size_t*);//send cursor to location. Editor::curosrPos will be referenced
    int getWindowSize(int, int, int*, int*);//Editor::cursorPos will be referenced
    int readKeyStroke(int);
}


namespace debug {
    void wtf(char*, char*, char*, ...);
}

#endif
