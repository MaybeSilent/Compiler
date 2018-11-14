#include "parse.h"
#include "types.h"

#define KEYNUM 13
#define MAXINDEX 127

FILE *instream , *outstream;

String key[KEYNUM];

Symbol sps[256];

char ch;
int countLine,countChar; //countLine , countChar ����ʱ�Ǽ�

void setup();
void nextch();


void nextch(){
    if(DEBUG) printf("����nextch\n");
    ch = fgetc(instream);
    countChar ++;
    if(ch == '\n') countLine ++;
    if(ch == EOF){
        printf("������󣺳���������������ǰ����");
        exit(0);
    }
    if(DEBUG) printf("���nextch\n");
}

int isAlpha(){
    return (ch >= 'a' && ch <= 'z')||(ch >= 'A' && ch <= 'Z')||ch == '_';
}

int isNum(){
    return ch >= '0' && ch <= '9';
}

void getString(){
    if(DEBUG) printf("����getString\n");
    int k = 0;
    do{
        id[k++] = ch;
        nextch();
    }while(k < MAXINDEX && (isNum()||isAlpha()));
    id[k] = '\0';

    //���ֲ������������ַ����ڱ������е�λ��
    int start = 0 , end = KEYNUM - 1;
    while(start <= end){
        k = (start + end)/2;
        if(strcmp(id,key[k]) == 0) break;
        else if(strcmp(id,key[k]) < 0) end = k - 1;
        else start = k + 1;
    }

    if(start <= end) curSy = (Symbol)k;
    else curSy = IDENTSY;
    if(DEBUG) printf("�˳�getString\n");
}

void getNum(){
    if(DEBUG) printf("����getNum\n");
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
    //ǰ����ERROR����>>>>>>>>>>>>>>>>>>>>>>>>>>
    if(DEBUG) printf("�˳�getNum\n");
}

void insymbol(){
    if(DEBUG) printf("����insymbol\n");

    while(isspace(ch))
        nextch(); //���˿ո�

    if((ch >= 'a' && ch <= 'z')||(ch >= 'A' && ch <= 'Z')||ch == '_'){ //ϵͳ�����ֻ��߱���
        getString();
    } else if(isNum()){
        getNum();
    } else if(ch == '\''){
        nextch();
        if(ch == '*'||ch == '/'||ch == '+'||ch == '-'||isNum()||isAlpha()){
            ichar = ch;
        } else {
            ichar = '0';
            //�Ƿ��ַ�Ĭ��Ϊ0����������
            //�ַ������޶��ķ�Χ ERROR
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
        if(ch != '"') {} //ERROR������
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
        curSy = NOEQU; //����ָ�����
        if(ch == '='){
            nextch();
        } else {} ; //ERROR �������
    } else if(ch == '('||ch == ')'||ch == '['||ch == ']'||ch == '{'||ch == '}'||ch == '+'
              ||ch == '-'||ch == '*'||ch == '/'||ch == ','||ch == ';'){
        curSy = sps[(int)ch];
        nextch();
    }

    if(DEBUG) printf("�˳�insymbol\n");
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
    printf("����������ļ���:");
    scanf("%s",filename);
    instream = fopen(filename,"r");
    if(instream == NULL){
        printf("�ļ�������");
        exit(0);
    }
    nextch();
}
