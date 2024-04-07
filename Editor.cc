// #include "utility.h"
#include "Editor.h"
#include "debug.h"

// ==================== ROW ====================
Row::row_nt Row::getIdx(void){
    return idx;
}

char* Row::getContent(void) const{
    return cnt;
}

void Row::setContent(char* content){
    cnt = content;
    len = strlen(cnt);
}
void Row::setContent(char* content, row_nt length){
    cnt = content;
    len = length;
}

void Row::setIdx(Row::row_nt idx){
    this->idx = idx;
}

Row::row_nt Row::getLen() const{
    return len;
}

void Row::setLen(Row::row_nt length){
    len=length;
}

Row::Row(){
    cnt = (char*)malloc(sizeof(char));
    cnt[len]='\0';
    len=0;
}
Row::Row(Row::row_nt idx):idx(idx){
    cnt = (char*)malloc(sizeof(char));
    cnt[len]='\0';
    len=0;
}
Row::Row(Row::row_nt idx, char* content):idx(idx){
    row_nt content_len = strlen(content);
    cnt = (char*)malloc(sizeof(char)*content_len);
    memcpy(cnt, content, strlen(content));
    cnt[content_len]='\0';
    len = strlen(cnt);
}
Row::Row(Row::row_nt idx, char* content, row_nt length):idx(idx), len(length){
    cnt = (char*)malloc(sizeof(char)*(len+1));
    memcpy(cnt, content, len);
    cnt[len] = '\0';
    // debug::wtf(NULL, NULL, "Constructor called with %s\n", cnt);
    // debug::wtf(NULL, NULL, "Length %d\n", strlen(cnt));
}

Row::Row(const Row& row):idx(row.idx),len(row.len){
    if (cnt!=NULL) {
        cnt=NULL;
    }
    cnt = (char*)malloc(sizeof(char)*(len+1));
    memcpy(cnt, row.cnt, len);
    cnt[len]='\0';
}

Row& Row::operator=(const Row& row) noexcept{
    if(this!=&row){
        this->idx=row.idx;
        this->len=row.len;
        // this->cnt=row.cnt;
        if (cnt!=NULL) {
            free(cnt);
            cnt=NULL;
        }
        cnt = (char*)malloc(sizeof(char)*(len+1));
        if(cnt==NULL) throw std::runtime_error("Cannot allocate memory for left hand side.");
        memcpy(cnt, row.cnt, len);
        cnt[len]='\0';
    }
    return *this;
}

// Row::Row(Row&& row):idx(row.idx), cnt(row.cnt), l(row.l){
//     row.idx=0;
//     // row.cnt=nullptr;
//     free(row.cnt);
//     row.l=0;
// }

// Row& Row::operator=(Row&& row) noexcept{
//     if(this!=&row){
//         this->idx=row.idx;
//         this->cnt=row.cnt;
//         this->l=row.l;
//         row.idx=0;
//         // row.cnt=nullptr;
//         free(row.cnt);
//         row.l=0;
//     }
//     return *this;
// }

int Row::updateRowContent(row_nt idx, char* add, row_nt length){
    /** 
     * @param int - index at which the (*add) should be added
     * @param add - char* to add to cnt
     * @param length - length of char* to be added
    */
    // __BREAKPOINT__

    if(idx>len+1) throw std::range_error("Index to be added must be smaller than this->len");
    char* upd = (char*) malloc(sizeof(char)*(len+length+1));
    memcpy(upd, cnt, idx);
    memcpy(upd+idx, add, length);
    memcpy(upd+idx+length, &(cnt[idx]), len-idx);
    upd[len+length]='\0';
    free(cnt);
    cnt = upd;
    len += length;
    return 0;
}

Row::~Row(){free(cnt);}

/*
***************************************
*************** Editor ****************
***************************************
*/
// ============== Constructors ==============
Editor::Editor(){//empty editor
    winsize ws = termutil::getWindowSize();
    screenrows = ws.ws_row;
    editorCnt = (char*) malloc(sizeof(char));
    editorCnt[0] = '\0';
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
    if (lenEditorCnt!=0 || editorCnt[0]!='\0'){
        lenEditorCnt=0;
        editorCnt[0]='\0';
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
    editorCnt[lenEditorCnt] = '\0';
    editorCnt = tmp;
}

void Editor::refreshEditorScreen(){
    termaction::hidecursor(ofd);
    termaction::mv2beg(ofd);
    termaction::clrght(ofd);
    renderEditorCnt();
    termaction::twrite(ofd, editorCnt, lenEditorCnt);//with lenEditorContent
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
    if((c=termutil::readKeyStroke(ifd))!=-1) {
        switch(int(c)){
            case CTRL_Q:
                return -1;
                break;
            case ENTER:
                editorEnterAction();
                break;
            case BACKSPACE:
                termaction::backspace(ofd);
                break;
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
    // debug::wtf(NULL, NULL, "Row_%d content: %s\n", cursorPosRow, getRowAt(cursorPosRow)->getContent());
    // debug::wtf(NULL, NULL, "cursorPosCol: %d\n", cursorPosCol);
    // debug::wtf(NULL, NULL, "editorCnt: %s\n", editorCnt);
    cursorPosCol++;
    return 1;
}

int Editor::editorEnterAction() {//what to do when ENTER pressed
    if (cursorPosCol==getRowAt(cursorPosRow)->getLen()) {
        getRowAt(cursorPosRow)->updateRowContent(cursorPosCol, "\r\n", 2);
        cursorPosRow++;
        cursorPosCol=0;
        Row row = Row(cursorPosRow, "", 0);
        insertRowAt(row, cursorPosRow);
        debug::wtf(NULL, NULL, "================\n");
        debug::wtf(NULL, NULL, "VectorSize: %d\n", getNRow());
        debug::wtf(NULL, NULL, "VectorLastContent: %s\n", getRowAt(cursorPosRow-1)->getContent());
        debug::wtf(NULL, NULL, "EditorContent:\n %s\n", getEditorCnt());
        return 1;
    }
    /** TODO: else statment
    */
}

int editorBackSpaceAction() {

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
    if (termutil::getCursorPosition(ifd, ofd, &cursorPosRow, &cursorPosCol)) return 1;
    else return 0;
}
int Editor::getWindowSize() {
    if (termutil::getWindowSize(ifd, ofd, &maxPosRow, &maxPosCol)) return 1;
    else return 0;
}

Editor::row_nt Editor::getNRow(void){
    return rows.size();
}