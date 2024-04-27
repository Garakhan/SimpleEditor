#ifndef __TERMINAL_EDITOR_H__
#define __TERMINAL_EDITOR_H__

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <memory>
#include <stdexcept>
#include <unordered_map>

#include "utility.h"

#define END_STRING '\0'

// ========== Types ==========
class Types {
    protected:
    // typedef size_t row_nt;//row number
    typedef int row_nt;//row number
};

// ========== ROW ============
class Row : private Types {

    /** TODO: fix move constructors
    */ 

    int idx; //index of the current row
    char* content=NULL; // content of the row => literal chars
    char* rendered=NULL;//rendered content of row
    int length;

    public:

    // CONSTRUCTORS
    Row();
    Row(int);
    Row(int, char*);
    Row(int, char*, int);
    Row(const Row& row);
    Row& operator=(const Row&) noexcept;
    // Row(Row&& row);
    // Row& operator=(Row&&) noexcept;
    ~Row();

    int getIdx(void);
    void setIdx(int);
    char* getContent(void) const;
    void setContent(char*);
    void setContent(char*, int);
    void getRendered(void) const;
    int getLen() const;
    void setLen(int);
    int updateRowContent(int, char*, int);//idx, chars, len
    int insertRowContent(int, char*, int);//idx, chars, len
    int delRowContent(int, int, int);//idx, len, direc
    int delRowContentRight(int, int);//idx, len
    int delRowContentLeft(int, int);//idx, len
};


// =========== Editor ===============
class Editor : private Types {

    /** TODO:use string.h append method instead of appendEditorContent
     * TODO: fix template functions
     * TODO: fix functions for right hand referecnces for the move constructors of Row
    */ 

    std::vector<Row> rows; // rows of the editor
    // std::unordered_map<row_nt, Row> rows;
    int screenrows; // number of visible row on screen
    char* editorCnt=NULL; //editor content
    int lenEditorCnt; //length editor content
    int cursorPosRow;
    int cursorPosCol;
    int maxPosRow;
    int maxPosCol;
    const std::string filename;
    int ifd, ofd;

    public:

    // method functions
    ~Editor();
    Editor();

    Row* getRowAt(int);//get row at index
    row_nt getScreenRows(void);//get screenrows (number of visible(window) screen)
    void setNScreenRows(int);//set screenrows (number of visible(window) screen)
    row_nt getNRow(void);//get number of rows (size of std::vector<Row> rows)
    

    // ========= Editor Exit =========
    void editorExitAction();//for CTRL_Q
    friend int termaction::enableRawMode(int);
    friend int termaction::disableRawMode(int);


    // ========= Screen Update =========
    void refreshEditorScreen();//refreshing the editor screen to update visible screen content
    void renderEditorCnt();//write contents of all rows std::vector<Row> rows to char* editorCnt
    void appendEditorContent(char*, int);//dynamically append to char* editorContent
    char* getEditorCnt();//get char* Editorcontent

    // ========= Row Management =========
    void insertRowAt(Row&);//insert Row
    void insertRowAt(Row&, int);//insert Row
    void removeRowAt(int);//remove Row
    void handleRemoveRow(int);//write content of row to another, and delete
    void handleBreakRow(int, int);//break row at idx and create two rows
    // template<typename R> void pushBackRow(R&&);//pushback Row
    // template<typename R> void pushBackRow(Row&&);//pushback Row
    // template<typename R> void insertRowAt(R&&, row_nt);//insert Row
    // template<typename R> void insertRowAt(R&&);//insert Row

    // ========= Editor Key Action =========
    int editorDeleteWordAction();
    int editorBackSpaceAction();
    int editorKeyAction();
    int editorEnterAction();
    int editorTypeAction(char*, unsigned);
    int type(int, char*, unsigned);//abastraction for termaction::twrite()

    int getWindowSize();
    void editorFillTildas();

    // ========== Editor Cursor Position =========
    int getCursorPosition();
    int adjustRowCol();//adjust update Row or Col depending on one another

    //friend functions
    friend int termaction::getCursorPosition(int, int, int*, int*);
    friend inline int termaction::setCursorPos(int, int, int);
    friend int termaction::getWindowSize(int, int, int*, int*);

    //main loop
    void *mainLoop();

};

#endif