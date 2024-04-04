#ifndef __DEBUG__H__
#define __DEBUG_H__

#include <stdarg.h>

#define __DEBUG__
#define __BREAKPOINT__ std::cout<<"==>BREAKPOINT<=="<<std::endl;
#define __PRINT__(str) std::cout<<"### "<<str<<" ###"<<std::endl;


namespace debug {
    void wtf (char* filename, char* m, char* c, ...);
}

#endif