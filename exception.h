#ifndef __TERMINAL_EXCEPTION_H__
#define __TERMINAL_EXCEPTION_H__

#include <stdexcept>
#include <string>
#include <unistd.h>
#include <stdlib.h>

class TerminalHandleException {
    public:

    static void TerminalConnectionError() {
        if(!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO)){
            throw std::runtime_error("STDIO is not a terminal.");
        }
    }
    static void TerminalIOCorruption() {
            throw std::runtime_error("Cannot query cursor position.");
    }
};

#endif