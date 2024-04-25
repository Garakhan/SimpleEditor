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
    content = (char*)malloc(sizeof(char));
    content[1]=END_STRING;//empty string of '\0'
    length=0;
}
Row::Row(int idx):idx(idx){
    content = (char*)malloc(sizeof(char));
    content[1]=END_STRING;//empty string of '\0' with idx
    length=0;
}
Row::Row(int idx, char* content):idx(idx){
    int content_len = strlen(content);
    this->content = (char*)malloc(sizeof(char)*content_len+1);
    memcpy(this->content, content, content_len);
    this->content[content_len]=END_STRING;
    this->length = content_len;
}
Row::Row(int idx, char* content, int length):idx(idx), length(length){
    this->content = (char*)malloc(sizeof(char)*(length+1));
    memcpy(this->content, content, length);
    this->content[length] = END_STRING;
}
Row::Row(const Row& row):idx(row.idx),length(row.length){
    if (content!=NULL) {
        content=NULL;
    }
    content = (char*)malloc(sizeof(char)*(length+1));
    memcpy(content, row.content, length);
    content[length]=END_STRING;
}
Row& Row::operator=(const Row& row) noexcept{
    if(this!=&row){
        this->idx=row.idx;
        this->content=row.content;
        // this->cnt=row.cnt;
        if (content!=NULL) {
            free(content);
            content=NULL;
        }
        content = (char*)malloc(sizeof(char)*(length+1));
        if(content==NULL) throw std::runtime_error("Cannot allocate memory for left hand side.");
        memcpy(content, row.content, length);
        content[length]=END_STRING;
    }
    return *this;
}
Row::Row(Row&& row):idx(row.idx), content(row.content), length(row.length){
    row.idx=0;
    free(row.content);
    row.content=nullptr;
    row.length=0;
}
Row& Row::operator=(Row&& row) noexcept{
    if(this!=&row){
        this->idx=row.idx;
        this->content=row.content;
        this->length=row.length;
        row.idx=0;
        free(row.content);
        row.content=nullptr;
        row.length=0;
    }
    return *this;
}

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
    memmove(this->content+idx, this->content+idx+len, this->length-idx);
    memcpy(this->content+idx, add, len);
    this->length+=len;
    this->content[this->length]=END_STRING;
    return 0;
}
int Row::delRowContent(int idx, int len, int dir){
    if (len > this->length) {
    
    }
    idx = idx+dir*(len-1);
    memmove(this->content+idx, this->content+idx+len, this->length-idx-len);
    this->length -= len;
    this->content = (char*) realloc(this->content, sizeof(char)*this->length+1);
    this->content[this->length]= END_STRING;    
    return 0;
}

Row::~Row(){free(this->content);}

/*
***************************************
*************** Editor ****************
***************************************
*/
// ============== Constructors ==============
Editor::Editor(){//empty editor
    winsize ws = termaction::getWindowSize();
    screenrows = ws.ws_row;
    editorCnt = (char*) malloc(sizeof(char));
    editorCnt[0] = END_STRING;
    lenEditorCnt = 0;//\0 does not count
    cursorPosCol=0;
    cursorPosRow=0;
    ifd = STDIN_FILENO;
    ofd = STDOUT_FILENO;
    Row row = Row(0, "", 0);
    insertRowAt(row, cursorPosRow);
    // for (int r=0; r<screenrows; r++){//writing tildas all along
    //     appendEditorContent(CURSOR_C_TILDA, CURSOR_L_TILDA);
    // }
    renderEditorCnt();
    editorFillTildas();
 }

Editor::~Editor(){
    free(editorCnt);
}


// ============== EDITOR CONTENT ==============

void Editor::editorFillTildas() {
    for (int i=rows.size(); i<screenrows; i++){
        appendEditorContent(CURSOR_C_TILDA, CURSOR_L_TILDA);
    }
}

void Editor::renderEditorCnt(){
    if (lenEditorCnt!=0 || editorCnt[0]!=END_STRING){
        lenEditorCnt=0;
        editorCnt[0]=END_STRING;
    }
    for (auto& i : rows) {
        appendEditorContent(i.getContent(), i.getLen());//vector implamentation of rows
    }
}

void Editor::appendEditorContent(char* cont, row_nt len){
    char* tmp = (char*) realloc(editorCnt, lenEditorCnt+len+1);
    if (tmp==NULL){
        return;
    }
    memcpy(tmp+lenEditorCnt, cont, len);
    lenEditorCnt+=len;
    editorCnt[lenEditorCnt] = END_STRING;
    editorCnt = tmp;
}

void Editor::refreshEditorScreen(){
    termaction::hidecursor(ofd);
    // termaction::setCursorPos(ofd, cursorPosRow+1, 1);
    // termaction::clrght(ofd);
    // if (lenEditorCnt>20) {
        termaction::clrscr(ofd);
        termaction::mv2beg(ofd);
    // }
    renderEditorCnt();
    debug::wtf(NULL, NULL, "%s\n", editorCnt);
    // termaction::twrite(
    //     ofd,
    //     getRowAt(cursorPosRow)->getContent(),
    //     getRowAt(cursorPosRow)->getLen()
    // );//with lenEditorContent
    termaction::twrite(ofd, editorCnt, lenEditorCnt);
    adjustRowCol();
    // debug::wtf(NULL, NULL, "CursorPosRow: %d\n", cursorPosRow);
    // debug::wtf(NULL, NULL, "CursorPosCol: %d\n", cursorPosCol);
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


void Editor::insertRowAt(Row& row, row_nt idx) {
    // appendEditorContent(row.getContent(), strlen(row.getContent()));//appendEditorContent is called in renderEditorCnt function
    if (idx==getNRow()+1){
        // rows.push_back(std::forward<Row>(row));//vector implementation of rows
        rows.push_back(row);//vector implementation of rows
    } else {
        // rows.insert(rows.begin()+idx, std::forward<Row>(row));//vector implementation of rows
        rows.insert(rows.begin()+idx, row);//vector implementation of rows
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

// template<typename R>// R for Row
// void Editor::pushBackRow(R&& row) {
//     // appendEditorContent(row.getContent(), strlen(row.getContent()));//appendEditorContent is called in renderEditorCnt function
//     rows.push_back(std::forward<R>(row));//vector implementation of rows
//     // rows[idx]=row;//unordered_map implementation of rows
// }


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
                return -1;
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
                break;
            case ESC:
                if ((c=termaction::readKeyStroke(ifd))!=-1 && c==LEFT_SQUARE){
                    if ((c=termaction::readKeyStroke(ifd))!=-1){
                        switch(int(c)) {
                            case LEFT_ARROW:
                                cursorPosCol--;
                                break;
                            case RIGHT_ARROW:
                                cursorPosCol++;
                                break;
                            case UP_ARROW:
                                cursorPosRow--;
                                break;
                            case DOWN_ARROW:
                                cursorPosRow++;
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
    getRowAt(cursorPosRow)->updateRowContent(cursorPosCol, c, l);
    cursorPosCol++;
    // debug::wtf(NULL, NULL, "%d\n", cursorPosCol);
    return 1;
}

int Editor::editorEnterAction() {//what to do when ENTER pressed
    if (cursorPosCol==getRowAt(cursorPosRow)->getLen()) {
        getRowAt(cursorPosRow)->updateRowContent(cursorPosCol, "\r\n", 2);
        cursorPosRow++;
        cursorPosCol=0;
        Row row = Row(cursorPosRow, "", 0);
        insertRowAt(row, cursorPosRow);
        // debug::wtf(NULL, NULL, "================\n");
        // debug::wtf(NULL, NULL, "VectorSize: %d\n", getNRow());
        // debug::wtf(NULL, NULL, "VectorLastContent: %s\n", getRowAt(cursorPosRow-1)->getContent());
        // debug::wtf(NULL, NULL, "EditorContent:\n %s\n", getEditorCnt());
        return 1;
    }
    /** TODO: else statment
    */
}

int Editor::editorBackSpaceAction() {
    cursorPosCol--;
    adjustRowCol();
    debug::wtf(NULL, NULL, "CursorPosRow: %d\n", cursorPosRow);
    debug::wtf(NULL, NULL, "CursorPosCol: %d\n", cursorPosCol);
    getRowAt(cursorPosRow)->updateRowContent(cursorPosCol, "", -1);
}

int Editor::adjustRowCol() {
    if (cursorPosCol<0) {
        if (cursorPosRow>0) {
            cursorPosRow--;
            cursorPosCol=getRowAt(cursorPosRow)->getLen();
        }
        cursorPosCol=0;
    } else if (cursorPosRow>=getNRow()) {
        // cursorPosRow = getNRow()-1;
        cursorPosRow--;
    } else if (cursorPosRow < 0) {
        cursorPosRow = 0;
    } else if (cursorPosCol>getRowAt(cursorPosRow)->getLen()) {
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

Row* Editor::getRowAt(row_nt idx){
    if (idx>=rows.size()) {
        std::string error_mes = "Row index [" + std::to_string(idx) + "] is out of range";
        throw std::runtime_error(error_mes);
    }
    return &(rows[idx]);
}

Editor::row_nt Editor::getScreenRows(void){
    return screenrows;
}

void Editor::setNScreenRows(row_nt n_screenRows){
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

Editor::row_nt Editor::getNRow(void){
    return rows.size();
}

void* Editor::mainLoop(){
    while(true) {

    }
}
