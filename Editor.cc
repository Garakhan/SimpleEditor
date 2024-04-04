// #include "utility.h"
#include "Editor.h"
#include "debug.h"

// ==================== ROW ====================
Row::row_nt Row::getIdx(void){
    return idx;
}

char* Row::getContent(void){
    return cnt;
}

void Row::setContent(char* content){
    cnt = content;
    l = strlen(cnt);
}
void Row::setContent(char* content, row_nt length){
    cnt = content;
    l = length;
}

void Row::setIdx(Row::row_nt idx){
    this->idx = idx;
}

Row::row_nt Row::getLen(){
    return l;
}

void Row::setLen(Row::row_nt length){
    l=length;
}

Row::Row(){
    cnt = (char*)malloc(sizeof(char));
    cnt[l]='\0';
    l=0;
}
Row::Row(Row::row_nt idx):idx(idx){
    cnt = (char*)malloc(sizeof(char));
    cnt[l]='\0';
    l=0;
}
Row::Row(Row::row_nt idx, char* content):idx(idx){
    row_nt content_len = strlen(content);
    cnt = (char*)malloc(sizeof(char)*content_len);
    memcpy(cnt, content, strlen(content));
    cnt[content_len]='\0';
    l = strlen(cnt);
}
Row::Row(Row::row_nt idx, char* content, row_nt length):idx(idx), l(length){
    cnt = (char*)malloc(sizeof(char)*length);
    memcpy(cnt, content, length);
    cnt[length] = '\0';
    l += length;
}

Row::Row(Row& row):idx(row.idx), cnt(row.cnt), l(row.l){
}

Row& Row::operator=(Row& row) noexcept{
    if(this!=&row){
        this->idx=row.idx;
        this->cnt=row.cnt;
        this->l=row.l;
    }
    return *this;
}

Row::Row(Row&& row):idx(row.idx), cnt(row.cnt), l(row.l){
    row.idx=0;
    // row.cnt=nullptr;
    free(row.cnt);
    row.l=0;
}

Row& Row::operator=(Row&& row) noexcept{
    if(this!=&row){
        this->idx=row.idx;
        this->cnt=row.cnt;
        this->l=row.l;
        row.idx=0;
        // row.cnt=nullptr;
        free(row.cnt);
        row.l=0;
    }
    return *this;
}

int Row::updateRowContent(row_nt idx, char* add, row_nt len){
    // __BREAKPOINT__
    char* rlc = (char*) realloc(cnt, len+l+1);
    if (rlc==NULL) {
        return 0;
    }
    memcpy(rlc, cnt, idx);
    memcpy(rlc+idx, add, len);
    memcpy(rlc+idx+len, cnt+idx, l-idx);
    rlc[len+l+1]='\0';
    cnt = rlc;
    l += len;//updatelength
    debug::wtf(NULL, NULL, "Row len: %d\n", l);
    debug::wtf(NULL, NULL, "Content: %s\n", cnt);

    return 1;
}

Row::~Row(){}


// ==================== Editor ====================
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
    pushBackRow(Row(0, "", 0));
    for (int r=0; r<screenrows; r++){//writing tildas all along
        // insertRowAt(Row(r, CURSOR_C_TILDA, CURSOR_L_TILDA), r);
        appendEditorContent(CURSOR_C_TILDA, CURSOR_L_TILDA);
    }
}

Editor::~Editor(){
    free(editorCnt);
}

Editor::row_nt Editor::getNRow(void){
    return rows.size();
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

template<typename R>// R for Row
void Editor::insertRowAt(R&& row, row_nt idx) {
    // appendEditorContent(row.getContent(), strlen(row.getContent()));//appendEditorContent is called in renderEditorCnt function
    rows.insert(rows.begin()+idx, std::forward<R>(row));//vector implementation of rows
    // rows[idx]=row;//unordered_map implementation of rows
}
template<typename R>// R for Row
void Editor::pushBackRow(R&& row) {
    // appendEditorContent(row.getContent(), strlen(row.getContent()));//appendEditorContent is called in renderEditorCnt function
    rows.push_back(std::forward<R>(row));//vector implementation of rows
    // rows[idx]=row;//unordered_map implementation of rows
}

void Editor::refreshEditorScreen(){
    termaction::hidecursor(ofd);
    termaction::mv2beg(ofd);
    termaction::clrght(ofd);
    renderEditorCnt();
    // __PRINT__(editorCnt);
    termaction::twrite(ofd, editorCnt, lenEditorCnt);//with lenEditorContent
    termaction::showcursor(ofd);

    // termaction::twrite(ofd, editorCnt, strlen(editorCnt));
}

void Editor::editorCursorAction(){
    int nc = 1;
    char c;
    if((c=termutil::readKeyStroke(ifd))!=-1) {
        switch(int(c)){
            case CTRL_Q:
                // debug::wtf(NULL, NULL, "cursorPosCol: %d\n", cursorPosCol);
                // sleep(1);
                exit(1);
                break;
            case ENTER:
                editorEnterAction();
                break;
            case BACKSPACE:
                termaction::backspace(ofd);
                break;
            default:
                // termaction::twrite(ofd, &c, nc);
                // type(ofd, &c, nc);
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
    // if (type(ofd, c, l)!=l) return -1;
    getRowAt(cursorPosRow)->updateRowContent(cursorPosCol, c, l);
    cursorPosCol++;
    return 1;
    // if (cursorPosRow<getNRow())
    // cursorPosCol++;
    // return 1;
}

int Editor::editorEnterAction() {//what to do when ENTER pressed
    debug::wtf(NULL, NULL, "getLen: %d\n", getRowAt(cursorPosRow)->getLen());
    if (cursorPosCol==getRowAt(cursorPosRow)->getLen()) {
        cursorPosRow++;
        insertRowAt(Row(cursorPosRow), cursorPosRow);
        return 1;
    }
}

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