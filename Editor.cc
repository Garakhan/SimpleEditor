// #include "utility.h"
#include "Editor.h"
#include "debug.h"
#include <cstring>

// ==================== ROW ====================

// SETTERS and GETTERS
int Row::getIdx(void){
    return idx;
}
void Row::setIdx(int idx){
    this->idx = idx;
}

char* Row::getContent(void) const{
    return content;
}
void Row::setContent(char* content){
    this->content = content;
    length = strlen(content);
}
void Row::setContent(char* content, int length){
    this->content = content;
    this->length = length;
}


int Row::getLen() const{
    return length;
}
void Row::setLen(int length){
    this->length=length;
}

// CONSTRUCTORS
Row::Row(){
    this->content = (char*)malloc(sizeof(char));
    if (this->content==NULL) {
        throw std::runtime_error("Could not allocate memory at zero constructor");
    }
    this->length=0;
    this->content[this->length]=END_STRING;//empty string of '\0'
}
Row::Row(int idx):idx(idx){
    content = (char*)malloc(sizeof(char));
    if (this->content==NULL) {
        throw std::runtime_error("Could not allocate memory at one constructor");
    }
    this->length=0;
    this->content[this->length]=END_STRING;//empty string of '\0'
}
Row::Row(int idx, char* content):idx(idx){
    int content_len = strlen(content);
    this->content = (char*)malloc(sizeof(char)*(content_len+1));
    if (this->content==NULL) {
        throw std::runtime_error("Could not allocate memory at two constructor");
    }
    memcpy(this->content, content, content_len);
    this->length = content_len;
    this->content[this->length]=END_STRING;
}
Row::Row(int idx, char* content, int length):idx(idx), length(length){
    this->length = length;
    this->content = (char*)malloc(sizeof(char)*(length+1));
    if (this->content==NULL) {
        throw std::runtime_error("Could not allocate memory at three constructor.");
    }
    memcpy(this->content, content, this->length);
    this->content[this->length] = END_STRING;
}
Row::Row(const Row& row):idx(row.idx),length(row.length){
    // if (content!=NULL) {
    //     content=NULL;
    // }
    // debug::wtf(NULL, NULL, "__BREAKPOINT_copyConstructor__\n");
    this->content = (char*)malloc(sizeof(char)*(this->length+1));
    if (this->content==NULL) {
        throw std::runtime_error("Could not allocate memory at copy constructor");
    }
    memcpy(this->content, row.content, this->length);
    this->content[this->length]=END_STRING;
}
Row& Row::operator=(const Row& row) noexcept{
    if(this!=&row){
        this->idx=row.idx;
        // this->content=row.content;
        // this->cnt=row.cnt;
        if (content!=NULL) {
            free(content);
            content=NULL;
        }
        char* newContent = (char*)malloc(sizeof(char)*(length+1));
        if(newContent==NULL) throw std::runtime_error("Cannot allocate memory for left hand side.");
        memcpy(newContent, row.content, length);
        newContent[length]=END_STRING;
        content = newContent;
    }
    return *this;
}
// Row::Row(Row&& row):idx(row.idx), content(row.content), length(row.length){
//     row.idx=0;
//     free(row.content);
//     row.content=nullptr;
//     row.length=0;
// }
// Row& Row::operator=(Row&& row) noexcept{
//     if(this!=&row){
//         this->idx=row.idx;
//         this->content=row.content;
//         this->length=row.length;
//         row.idx=0;
//         free(row.content);
//         row.content=nullptr;
//         row.length=0;
//     }
//     return *this;
// }

int Row::updateRowContent(int idx, char* add, int len){

    /** Update row content. if `add` is "" (empty string) then delete the character at idx.
     * @param int - index at which the (*add) should be added
     * @param add - char* to add to cnt
     * @param len - length of char* to be added
    */

    if(idx>this->length+1) throw std::range_error("Index to be added must be smaller than this->len");
    if (add==""){//delete the character at idx
        if (this->length==0) return 0;//if no content the row should be deleted.
        memmove(this->content+idx, this->content+idx+1, this->length-idx);//shift idx+1 one backwards
        this->content = (char*) realloc(this->content, this->length);//decrease length by one
        this->content[this->length-1]=END_STRING;
        this->length--;
        return 0;
    }
    this->content = (char*) realloc(this->content, sizeof(char)*(this->length+len+1));
    memmove(this->content+idx, this->content+idx+len, this->length-idx);
    memcpy(this->content+idx, add, len);
    this->length+=len;
    this->content[this->length]=END_STRING;
    return 0;
}
int Row::insertRowContent(int idx, char* add, int len) {
    this->content = (char*) realloc(this->content, sizeof(char)*(this->length+len+1));
    memmove(this->content+idx+len, this->content+idx, this->length-idx);
    memcpy(this->content+idx, add, len);
    // debug::wtf(NULL, NULL, "__BREAKPOINT_insertRowContent__\n");
    // debug::wtf(NULL, NULL, "RowCont: '%s'\n", this->content);
    this->length+=len;
    this->content[this->length]=END_STRING;
    return 0;
}
int Row::delRowContent(int idx, int len, int dir){//not fully as intented
    if (len > this->length) {
    }
    idx = idx+dir*(len-1)-1;
    // debug::wtf(NULL, NULL, "Part: %d\n", idx);
    // debug::wtf(NULL, NULL, "Part: %s\n", this->content+idx);
    // debug::wtf(NULL, NULL, "Part: %s\n", this->content+idx+len);
    memmove(this->content+idx, this->content+idx+len, this->length-idx-len);
    this->length -= len;
    this->content = (char*) realloc(this->content, sizeof(char)*this->length+1);
    this->content[this->length]= END_STRING;    
    return 0;
}
int Row::delRowContentRight(int idx, int len){
    if (len > this->length) {
    }
    // debug::wtf(NULL, NULL, "Part: %d\n", idx);
    // debug::wtf(NULL, NULL, "Part: %s\n", this->content+idx);
    // debug::wtf(NULL, NULL, "Part: %s\n", this->content+idx+len);
    memmove(this->content+idx, this->content+idx+len, this->length-idx-len+1);
    this->length -= len;
    this->content = (char*) realloc(this->content, sizeof(char)*(this->length+1));
    this->content[this->length]= END_STRING;    
    return 0;
}
int Row::delRowContentLeft(int idx, int len){
    if (len > this->length) {
    }
    // debug::wtf(NULL, NULL, "Part: %d\n", idx);
    // debug::wtf(NULL, NULL, "Part: %s\n", this->content+idx);
    // debug::wtf(NULL, NULL, "Part: %s\n", this->content+idx+len);
    memmove(this->content+idx-len+1, this->content+idx+1, this->length-idx-1);
    this->length -= len;
    this->content = (char*) realloc(this->content, sizeof(char)*(this->length+1));
    this->content[this->length]= END_STRING;    
    return 0;
}

Row::~Row(){
    // debug::wtf(NULL, NULL, "__BREAKPOINT_rowDestructor__\n");
    if(this->content!=NULL){
        free(this->content);
        this->content=NULL;
    }
}
// Row::~Row(){}

/*
***************************************
*************** Editor ****************
***************************************
*/
// ============== Constructors ==============
Editor::Editor(){//empty editor
    winsize ws = termaction::getWindowSize();
    screenrows = ws.ws_row-1;
    editorCnt = (char*) malloc(sizeof(char));
    if (editorCnt==NULL) {
        throw std::runtime_error("Cannot allocate memory for editor content");
    }
    lenEditorCnt = 0;//\0 does not count
    editorCnt[lenEditorCnt] = END_STRING;
    cursorPosCol=0;
    cursorPosRow=0;
    ifd = STDIN_FILENO;
    ofd = STDOUT_FILENO;
    Row row = Row(0, "", 0);
    insertRowAt(row, cursorPosRow);
    // editorFillTildas();
    // renderEditorCnt();
    refreshEditorScreen();
 }

Editor::~Editor(){
    if (editorCnt!=NULL) {
        free(editorCnt);
        editorCnt=NULL;
    }
}


// ============== EDITOR CONTENT ==============


void Editor::editorFillTildas() {
    for (int i=rows.size(); i<screenrows; i++){
        appendEditorContent("~\r\n", 3);
    }
    // debug::wtf(NULL, NULL, "%s\n", editorCnt);
    // debug::wtf(NULL, NULL, "----------------\n");
}

void Editor::renderEditorCnt(){
    // if (lenEditorCnt!=0 || editorCnt[0]!=END_STRING){
    //     lenEditorCnt=0;
    //     editorCnt[0]=END_STRING;
    // }
    free(editorCnt);
    editorCnt=NULL;
    lenEditorCnt=0;
    editorCnt=(char*)malloc(sizeof(char));
    for (auto& i : rows) {
        // debug::wtf(NULL, NULL, "%s\n", i.getContent());
        appendEditorContent(i.getContent(), i.getLen());//vector implamentation of rows
        appendEditorContent("\r\n", 2);
    }
    // debug::wtf(NULL, NULL, "----------------\n");
}

void Editor::appendEditorContent(char* cont, int len){
    char* tmp = (char*) realloc(editorCnt, lenEditorCnt+len+1);
    if (tmp==NULL){
        throw std::runtime_error("Cannot realloc memory for editor content");
    }
    memcpy(tmp+lenEditorCnt, cont, len);
    lenEditorCnt+=len;
    tmp[lenEditorCnt] = END_STRING;
    editorCnt = tmp;
}

void Editor::refreshEditorScreen(){
    // debug::wtf(NULL, NULL, "__BREAKPOINT_refreshEditorScreen__\n");
    // debug::wtf(NULL, NULL, "=======================\n");
    for (auto& i : rows) {
        debug::wtf(NULL, NULL, "Row %d: '%s'\n", i.getIdx(), i.getContent());
    }
    termaction::hidecursor(ofd);
    // termaction::setCursorPos(ofd, cursorPosRow+1, 1);
    // termaction::clrght(ofd);
    // if (lenEditorCnt>20) {
    termaction::mv2beg(ofd);
    termaction::clrscr(ofd);
    // }
    renderEditorCnt();
    // debug::wtf(NULL, NULL, "%s\n", editorCnt);
    // termaction::twrite(
    //     ofd,
    //     getRowAt(cursorPosRow)->getContent(),
    //     getRowAt(cursorPosRow)->getLen()
    // );//with lenEditorContent
    editorFillTildas();
    termaction::twrite(ofd, editorCnt, lenEditorCnt);
    // debug::wtf(NULL, NULL, "CursorPosRow: %d\n", cursorPosRow);
    // debug::wtf(NULL, NULL, "CursorPosCol: %d\n", cursorPosCol);
    // debug::wtf(NULL, NULL, "\nContent: %s\n", editorCnt);
    // adjustRowCol();

    termaction::setCursorPos(ofd, cursorPosRow+1, cursorPosCol+1);//real cursorPos is less 1 because of container idx
    termaction::showcursor(ofd);
}

// ============== ROW OPERATIONS ==============

// template<typename R>// R for Row
// void Editor::insertRowAt(R&& row) {
//     // appendEditorContent(row.getContent(), strlen(row.getContent()));//appendEditorContent is called in renderEditorCnt function
//     if (cursorPosRow==getNRow()+1){
//         rows.push_back(std::forward<R>(row));//vector implementation of rows
//     } else {
//         rows.insert(rows.begin()+cursorPosRow, std::forward<R>(row));//vector implementation of rows
//     }
//     // rows[idx]=row;//unordered_map implementation of rows
// }

// template<typename R>// R for Row
// void Editor::insertRowAt(R&& row, row_nt idx) {
//     // appendEditorContent(row.getContent(), strlen(row.getContent()));//appendEditorContent is called in renderEditorCnt function
//     if (idx==getNRow()+1){
//         rows.push_back(std::forward<R>(row));//vector implementation of rows
//     } else {
//         rows.insert(rows.begin()+idx, std::forward<R>(row));//vector implementation of rows
//     }
//     // rows[idx]=row;//unordered_map implementation of rows
// }


void Editor::insertRowAt(Row& row, int idx) {
    // appendEditorContent(row.getContent(), strlen(row.getContent()));//appendEditorContent is called in renderEditorCnt function
    if (idx==getNRow()+1){
        // rows.push_back(std::forward<Row>(row));//vector implementation of rows
        rows.push_back(row);//vector implementation of rows
    } else {
        // rows.insert(rows.begin()+idx, std::forward<Row>(row));//vector implementation of rows
        rows.insert(rows.begin()+idx, row);//vector implementation of rows
        debug::wtf(NULL, NULL, "ROW INSERT\n");
    }
    // rows[idx]=row;//unordered_map implementation of rows
}

void Editor::insertRowAt(Row& row) {
    // appendEditorContent(row.getContent(), strlen(row.getContent()));//appendEditorContent is called in renderEditorCnt function
    if (cursorPosRow==getNRow()+1){
        rows.push_back(row);//vector implementation of rows
    } else {
        rows.insert(rows.begin()+cursorPosRow, row);//vector implementation of rows
    }
    // rows[idx]=row;//unordered_map implementation of rows
}

void Editor::removeRowAt(int idx) {
    // appendEditorContent(row.getContent(), strlen(row.getContent()));//appendEditorContent is called in renderEditorCnt function
    if (idx==getNRow()-1){
        // rows.push_back(std::forward<Row>(row));//vector implementation of rows
        rows.pop_back();//vector implementation of rows
    } else {
        // rows.insert(rows.begin()+idx, std::forward<Row>(row));//vector implementation of rows
        // debug::wtf(NULL, NULL, "__BREAKPOINT_removeRowAt__\n");
        // debug::wtf(NULL, NULL, "rowslen: %d\n", getNRow());
        rows.erase(rows.begin()+idx);//vector implementation of rows
        // debug::wtf(NULL, NULL, "__BREAKPOINT_removeRowAt__\n");
    }
    // rows[idx]=row;//unordered_map implementation of rows
}

void Editor::handleRemoveRow(int idx){
    // char *tmp_cnt = getRowAt(idx)->getContent();
    Row *rowUp = getRowAt(idx-1);
    // debug::wtf(NULL, NULL, "LenUp: %d\n", rowUp->getLen());
    Row *rowDown = getRowAt(idx);
    // debug::wtf(NULL, NULL, "LenUp: %d\n", rowUp->getLen());
    // debug::wtf(NULL, NULL, "ContDown: '%s'\n", rowDown->getContent());
    // debug::wtf(NULL, NULL, "LenDown: %d\n", rowDown->getLen());
    // getRowAt(idx-1)->updateRowContent(rowUp->getLen(), rowDown->getContent(), rowDown->getLen());
    getRowAt(idx-1)->insertRowContent(rowUp->getLen(), rowDown->getContent(), rowDown->getLen());
    removeRowAt(idx);
    // debug::wtf(NULL, NULL, "__BREAKPOINT_handleRemoveRow\n");
}

void Editor::handleBreakRow(int idxr, int idxc){
    Row *row = getRowAt(idxr);
    char *tmp = row->getContent()+idxc;
    Row newRow = Row(idxr+1, tmp, row->getLen()-idxc);
    row->delRowContentRight(idxc, row->getLen()-idxc);
    // row->delRowContent(cursorPosCol-1, 2, 1);
    insertRowAt(newRow, idxr+1);
}

// template<typename R>// R for Row
// void Editor::pushBackRow(R&& row) {
//     // appendEditorContent(row.getContent(), strlen(row.getContent()));//appendEditorContent is called in renderEditorCnt function
//     rows.push_back(std::forward<R>(row));//vector implementation of rows
//     // rows[idx]=row;//unordered_map implementation of rows
// }

// ================ EDITOR EXIT ACTION ================
void Editor::editorExitAction() {
    exit(0);
}

// ================ EDITOR ACTIONS ================

int Editor::editorKeyAction(){
    int nc = 1;
    char c;
    char cursorPosSeq[80];
    size_t row = 8;
    size_t col = 10;
    if((c=termaction::readKeyStroke(ifd))!=-1) {
    // debug::wtf(NULL, NULL, "Read C: %d\n", c);
    // debug::wtf(NULL, NULL, "Pressed: %d\n", c);
        switch(int(c)){
            case CTRL_Q:
                editorExitAction();//atexit will handle post-exit setups(disableRawMode). see termaction:: at utility.h
                break;
            case ENTER:
                // termaction::setCursorPos(ofd, &row, &col);
                editorEnterAction();
                // sprintf(cursorPosSeq, "\x1b[%zu;%zuH", row, col);
                // write(ofd, cursorPosSeq, strlen(cursorPosSeq));
                break;
            case BACKSPACE:
                // termaction::backspace(ofd);
                editorBackSpaceAction();
                // debug::wtf(NULL, NULL, "__BREAKPOINT_editorKeyAction__\n");
                break;
            case ESC:
                if ((c=termaction::readKeyStroke(ifd))!=-1 && c==LEFT_SQUARE){
                    if ((c=termaction::readKeyStroke(ifd))!=-1){
                        switch(int(c)) {
                            case LEFT_ARROW:
                                cursorPosCol--;
                                adjustRowCol();
                                break;
                            case RIGHT_ARROW:
                                cursorPosCol++;
                                adjustRowCol();
                                break;
                            case UP_ARROW:
                                cursorPosRow--;
                                adjustRowCol();
                                break;
                            case DOWN_ARROW:
                                cursorPosRow++;
                                adjustRowCol();
                                break;
                        }
                    }
                }
                break;
            // case ARROW_LEFT:
            //     cursorPosCol--;
            //     debug::wtf(NULL, NULL, "CursorPosCol: %d\n", cursorPosCol);
            //     // adjustRowCol();
            //     break;
            default:
                editorTypeAction(&c, nc);
                break;
        }
        refreshEditorScreen();
    }
}

int Editor::type(int ofd, char* s, unsigned l) {//type characters to terminal
    if (termaction::twrite(ofd, s, l)!=l) return -1;
    return l;
}

int Editor::editorTypeAction(char* c, unsigned l) {//what to do when typing
    getRowAt(cursorPosRow)->insertRowContent(cursorPosCol, c, l);
    cursorPosCol++;
    adjustRowCol();
    // debug::wtf(NULL, NULL, "%d\n", cursorPosCol);
    return 0;
}

int Editor::editorEnterAction() {//what to do when ENTER pressed
    if (cursorPosCol==getRowAt(cursorPosRow)->getLen()) {
        // getRowAt(cursorPosRow)->updateRowContent(cursorPosCol, "\r\n", 2);
        // debug::wtf(NULL, NULL, "================\n");
        // debug::wtf(NULL, NULL, "VectorSize: %d\n", getNRow());
        // debug::wtf(NULL, NULL, "CursorPosRow: %d\n", cursorPosRow);
        cursorPosRow++;
        cursorPosCol=0;
        Row row = Row(cursorPosRow, " ", 0);
        insertRowAt(row, cursorPosRow);
        // debug::wtf(NULL, NULL, "-----------------\n");
        // debug::wtf(NULL, NULL, "VectorSize: %d\n", getNRow());
        // debug::wtf(NULL, NULL, "CursorPosRow: %d\n", cursorPosRow);
        // debug::wtf(NULL, NULL, "================\n");
        // debug::wtf(NULL, NULL, "VectorLastContent: %s\n", getRowAt(cursorPosRow-1)->getContent());
        // debug::wtf(NULL, NULL, "EditorContent:\n %s\n", getEditorCnt());
        return 0;
    } 
    handleBreakRow(cursorPosRow++, cursorPosCol);
    cursorPosCol=0;
    return 0;
}

int Editor::editorBackSpaceAction() {
    if (cursorPosCol-1<0) {
        if (cursorPosRow>0) {
            cursorPosCol=getRowAt(cursorPosRow-1)->getLen();//send 
            // debug::wtf(NULL, NULL, "CursorPosRow: %d\n", cursorPosRow);
            // debug::wtf(NULL, NULL, "CursorPosCol: %d\n", cursorPosCol);
            handleRemoveRow(cursorPosRow);
            cursorPosRow--;
            return 0;
        } else {
            cursorPosCol=0;
            return 0;
        }
    }
    // debug::wtf(NULL, NULL, "CursorPosRow: %d\n", cursorPosRow);
    // debug::wtf(NULL, NULL, "CursorPosCol: %d\n", cursorPosCol);
    // getRowAt(cursorPosRow)->updateRowContent(cursorPosCol, "", -1);
    getRowAt(cursorPosRow)->delRowContentLeft(--cursorPosCol, 1);
}

int Editor::adjustRowCol() {
    if (cursorPosCol<0) {
        if (cursorPosRow>0) {
            cursorPosRow--;
            cursorPosCol=getRowAt(cursorPosRow)->getLen();
        } else {
            cursorPosCol=0;
        }
    } else if (cursorPosRow>=getNRow()) {
        // cursorPosRow = getNRow()-1;
        cursorPosRow--;
    } else if (cursorPosRow < 0) {
        cursorPosRow = 0;
    } else if (cursorPosCol>getRowAt(cursorPosRow)->getLen()) {
        // debug::wtf(NULL, NULL, "CursorPosRow: %d\n", cursorPosRow);
        // debug::wtf(NULL, NULL, "CursorPosCol: %d\n", cursorPosCol);
        if (cursorPosRow<getNRow()-1) {
            cursorPosRow++;
            cursorPosCol=0;
        } else {
            cursorPosCol--;
        }
    }
    return 0;
}

// ================= Getters/Setters =================

Row* Editor::getRowAt(int idx){
    if (idx>=rows.size()) {
        std::string error_mes = "Row index [" + std::to_string(idx) + "] is out of range";
        throw std::runtime_error(error_mes);
    }
    return &(rows[idx]);
}

int Editor::getScreenRows(void){
    return screenrows;
}

void Editor::setNScreenRows(int n_screenRows){
    screenrows = n_screenRows;
}

char* Editor::getEditorCnt() {
    return editorCnt;
}

int Editor::getCursorPosition() {
    if (termaction::getCursorPosition(ifd, ofd, &cursorPosRow, &cursorPosCol)) return 1;
    else return 0;
}
int Editor::getWindowSize() {
    if (termaction::getWindowSize(ifd, ofd, &maxPosRow, &maxPosCol)) return 1;
    else return 0;
}

int Editor::getNRow(void){
    return rows.size();
}

void* Editor::mainLoop(){
    while(true) {
        editorKeyAction();
        // refreshEditorScreen();
    }
}
