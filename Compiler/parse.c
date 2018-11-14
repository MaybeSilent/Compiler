#include "parse.h"
#include "types.h"

#define KEYNUM 13
#define MAXINDEX 127

FILE *instream , *outstream;

String key[KEYNUM];

Symbol sps[256];

char ch;
int countLine,countChar; //countLine , countChar 出错时登记

void setup();
void nextch();


void nextch(){
    if(DEBUG) printf("进入nextch\n");
    ch = fgetc(instream);
    countChar ++;
    if(ch == '\n') countLine ++;
    if(ch == EOF){
        printf("编译错误：程序不完整，编译提前结束");
        exit(0);
    }
    if(DEBUG) printf("完成nextch\n");
}

int isAlpha(){
    return (ch >= 'a' && ch <= 'z')||(ch >= 'A' && ch <= 'Z')||ch == '_';
}

int isNum(){
    return ch >= '0' && ch <= '9';
}

void getString(){
    if(DEBUG) printf("进入getString\n");
    int k = 0;
    do{
        id[k++] = ch;
        nextch();
    }while(k < MAXINDEX && (isNum()||isAlpha()));
    id[k] = '\0';

    //二分查找所读到的字符串在保留字中的位置
    int start = 0 , end = KEYNUM - 1;
    while(start <= end){
        k = (start + end)/2;
        if(strcmp(id,key[k]) == 0) break;
        else if(strcmp(id,key[k]) < 0) end = k - 1;
        else start = k + 1;
    }

    if(start <= end) curSy = (Symbol)k;
    else curSy = IDENTSY;
    if(DEBUG) printf("退出getString\n");
}

void getNum(){
    if(DEBUG) printf("进入getNum\n");
    int firstzero = 0;
    int ErrorZero = 0;
    int k = 0;
    inum = 0;
    do{
        inum *= 10;
        inum += ch - '0';
        k ++;
        if(k == 0 && ch == '0') firstzero = 1;
        if(k == 1 && ch == '0' && firstzero == 1) ErrorZero = 1;
        nextch();
    }while(isNum());
    curSy = INTCON;
    if(ErrorZero) {}
    //前导零ERROR处理>>>>>>>>>>>>>>>>>>>>>>>>>>
    if(DEBUG) printf("退出getNum\n");
}

void insymbol(){
    if(DEBUG) printf("进入insymbol\n");

    while(isspace(ch))
        nextch(); //过滤空格

    if((ch >= 'a' && ch <= 'z')||(ch >= 'A' && ch <= 'Z')||ch == '_'){ //系统保留字或者变量
        getString();
    } else if(isNum()){
        getNum();
    } else if(ch == '\''){
        nextch();
        if(ch == '*'||ch == '/'||ch == '+'||ch == '-'||isNum()||isAlpha()){
            ichar = ch;
        } else {
            ichar = '0';
            //非法字符默认为0，继续编译
            //字符超出限定的范围 ERROR
            //ERROR>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        }
        curSy = CHARCON;
        nextch();
        if(ch != '\'') {} //ERROR left
        nextch();
    } else if(ch == '"'){
        nextch();
        int k = 0;
        while(ch>=32&&ch<=126&&ch!=34){
            iString[k++] = ch;
            nextch();
        }
        iString[k] = '\0';
        curSy = STRINGCON;
        nextch();
        if(ch != '"') {} //ERROR错误处理
        nextch();
    } else if(ch == '='){
        nextch();
        if(ch == '='){
            curSy = EQU;
            nextch();
        } else curSy = BECOMESY;
    } else if(ch == '>'){
        nextch();
        if(ch == '='){
            curSy = MOREEQU;
            nextch();
        } else curSy = MORE;
    } else if(ch == '<'){
        nextch();
        if(ch == '='){
            curSy = LESSEQU;
            nextch();
        } else curSy = LESS;
    } else if(ch == '!'){
        nextch();
        curSy = NOEQU; //错误恢复处理
        if(ch == '='){
            nextch();
        } else {} ; //ERROR 处理程序
    } else if(ch == '('||ch == ')'||ch == '['||ch == ']'||ch == '{'||ch == '}'||ch == '+'
              ||ch == '-'||ch == '*'||ch == '/'||ch == ','||ch == ';'){
        curSy = sps[(int)ch];
        nextch();
    }

    if(DEBUG) printf("退出insymbol\n");
}


void setup(){
    /*
    CHARSY,CONSTSY,DOSY,ELSESY,FORSY,IFSY,INPUTSY,INTSY,
    MAINSY,OUTPUTSY,RETURNSY,VOIDSY,WHILESY,
    */
    key[0] = "char";
    key[1] = "const";
    key[2] = "do";
    key[3] = "else";
    key[4] = "for";
    key[5] = "if";
    key[6] = "input";
    key[7] = "int";
    key[8] = "main";
    key[9] = "output";
    key[10] = "return";
    key[11] = "void";
    key[12] = "while";

    sps['+'] = PLUS;
    sps['-'] = SUB;
    sps['*'] = MULT;
    sps['/'] = DIV;
    sps['<'] = LESS;
    sps['='] = EQU;
    sps['>'] = MORE;
    sps[','] = COMMA;
    sps[';'] = SEMICOLON;
    sps['('] = LPARENT;
    sps[')'] = RPARENT;
    sps['['] = LBRACKET;
    sps[']'] = RBRACKET;
    sps['{'] = LBRACE;
    sps['}'] = RBRACE;

    char filename[128];
    printf("请输入编译文件名:");
    scanf("%s",filename);
    instream = fopen(filename,"r");
    if(instream == NULL){
        printf("文件不存在");
        exit(0);
    }
    nextch();
}
