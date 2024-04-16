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
    row_nt idx; //index of the current row
    char* cnt=NULL; // content of the row
    row_nt len;

    public:

    Row();
    Row(row_nt);
    Row(row_nt, char*);
    Row(row_nt, char*, row_nt);
    // Row(Row&& row);
    Row(const Row& row);
    // Row& operator=(Row&&) noexcept;
    Row& operator=(const Row&) noexcept;
    ~Row();
    row_nt getIdx(void);
    char* getContent(void) const;
    void setIdx(row_nt);
    void setContent(char*);
    void setContent(char*, row_nt);
    row_nt getLen() const;
    void setLen(row_nt);
    int updateRowContent(row_nt, char*, row_nt);
    // int updateRowContent(row_nt, char, row_nt);

};


// =========== Editor ===============
class Editor : private Types {
    std::vector<Row> rows; // rows of the editor
    // std::unordered_map<row_nt, Row> rows;
    row_nt screenrows; // number of visible row on screen
    char* editorCnt=NULL; //editor content
    row_nt lenEditorCnt; //length editor content
    row_nt cursorPosRow;
    row_nt cursorPosCol;
    row_nt maxPosRow;
    row_nt maxPosCol;
    const std::string filename;
    int ifd, ofd;

    public:

    // method functions
    ~Editor();
    Editor();
    Row* getRowAt(row_nt);//get row at index
    row_nt getScreenRows(void);//get screenrows (number of visible(window) screen)
    void setNScreenRows(row_nt);//set screenrows (number of visible(window) screen)
    row_nt getNRow(void);//get number of rows (size of std::vector<Row> rows)
    void refreshEditorScreen();//refreshing the editor screen to update visible screen content
    void renderEditorCnt();//write contents of all rows std::vector<Row> rows to char* editorCnt
    /** TODO:use string.h append method instead of appendEditorContent
     * 
    */ 
    void appendEditorContent(char*, row_nt);//dynamically append to char* editorContent
    // template<typename R> void insertRowAt(R&&, row_nt);//insert Row
    // template<typename R> void insertRowAt(R&&);//insert Row
    void insertRowAt(Row&);//insert Row
    void insertRowAt(Row&, row_nt);//insert Row
    // template<typename R> void pushBackRow(R&&);//pushback Row
    // template<typename R> void pushBackRow(Row&&);//pushback Row
    char* getEditorCnt();//get char* Editorcontent

    // ========= Editor Key Action =========
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
    friend int termutil::getCursorPosition(int, int, int*, int*);
    friend inline size_t termaction::setCursorPos(int, int, int);
    friend int termutil::getWindowSize(int, int, int*, int*);

};


#endif