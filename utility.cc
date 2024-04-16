#include "utility.h"
#include "debug.h"

#if defined(__DEBUG__)
#include <fstream>
using namespace std;
#endif

namespace termutil {
    termios original_terminal;
    int enableRawMode(int fd){
        // if (!isatty(STDIN_FILENO))  
    }

    struct winsize getWindowSize() {
        struct winsize ws;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
            perror("ioctl");
        }
        return ws;
    }

    /** Query cursor position to &Editor::cursorPos
    */
    int getCursorPosition(int ifd, int ofd, int *row, int *col) {
        int buf_size = 32;
        char buf[buf_size];
        unsigned i=0;

        if (termaction::qcurspos(ofd)!=4)
            return -1;
        
        while(i<buf_size-1){
            if (read(ifd, buf+i, 1)!=1) break;
            if (buf[i]=='R') break;
            i++;
        }
        buf[i]='\0';
        if (buf[0]!=ESC || buf[1]!='[') return -1;
        if (sscanf(buf+2, "%d;%d", row, col)!=2) return -1;
        return 0;
    }
    
    /** Querying window size of terminal
    */
    int getWindowSize(int ifd, int ofd, int *row, int *col) {
        winsize ws;
        if (ioctl(ifd, TCIFLUSH, TIOCGWINSZ)==-1) {//get with cursor
            int orig_row, orig_col;

            //get original cursor position
            if (getCursorPosition(ifd, ofd, &orig_col, &orig_row)==-1) return -1;
            //send cursor to right-bottom to get screen size
            if(write(ofd, CURSOR_C_RGHTBTM, CURSOR_L_RGHTBTM)!=12) return -1;
            //get cursor position: row col number of cursor being at right-bottom
            if(getCursorPosition(ifd, ofd, row, col)==-1) return -1;
            //restore original cursor position
            char seq[32];
            snprintf(seq, 32, "\x1b[%d;%dH", orig_row, orig_col);
            if(write(ofd, seq, strlen(seq))==-1) return -1;
            return 1;
        } else {
            *col = ws.ws_col;
            *row = ws.ws_row;
            return 0;
        }
    }

    int readKeyStroke(int ifd) {
        /** TODO: will be added more features.
        */
        char c;
        if (read(ifd, &c, 1)!=1) return -1;
        return c;
    }
}

namespace debug {
    void wtf (char* filename, char* m, char* c, ...) {
            if (filename==NULL) {
                filename = "/Users/garakhan/Desktop/wsp/git/kilo/proj/debug.txt";
            }
            if (m==NULL) {
                m = "a";
            }
            va_list args;
            va_start(args, c);
            FILE *rowFile = fopen(filename, m);
            vfprintf(rowFile, c, args);
            va_end(args);
            fclose(rowFile);
    }
}