### Left
- Window size change (not implemented).
- Syntax highlight by mouse.
- Copy to system clipboard.
### Refactor
- refactor return operations; 0 for true, -1 for false;
- add extensive documentation.
### std::string
- Translate from C-style malloc/realloc char* to std::string C++ style.
### Implementing rvalue references for constructors
- Right now rvalue references are removed. insertRowAt are implemented with lvalue copy constructors. This version debugged (see draft/vector-Row.cc). Need to understand more about std::forward (see commented-out implementation.)