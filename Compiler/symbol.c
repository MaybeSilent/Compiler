#include "symbol.h"
#include "types.h"
#include "error.h"

#define KEYNUM 13
#define MAXINDEX 127

FILE *instream, *outstream;

String key[KEYNUM];

Symbol sps[256];

char ch;

void setup();
void nextch();
void skip(char request);

void nextch(){
    if(DEBUG)
        printf("����nextch\n");
    ch = fgetc(instream);
    countChar ++;
    if(ch == '\n'){
        countLine ++;
        countChar = 0;
    }
    if(DEBUG)
        printf("���nextch\n");
}

void skip(char request){
    if(request == ' '){
        while(!isspace(ch) && ch != '\n') nextch(); //������ǰ��ʶ����������һ����ʶ��Ϊֹ
    } else {
        while(ch != request && ch != '\n') nextch();
    }

    nextch();
}

int isAlpha(){
    return (ch >= 'a' && ch <= 'z')||(ch >= 'A' && ch <= 'Z')||ch == '_';
}


int isNum(){
    return ch >= '0' && ch <= '9';
}

void getString()
{
    if(DEBUG)
        printf("����getString\n");
    int k = 0;
    do
    {
        id[k++] = ch;
        nextch();
    }
    while(k < MAXINDEX && (isNum()||isAlpha()));
    id[k] = '\0';

    //���ֲ������������ַ����ڱ������е�λ��
    int start = 0, end = KEYNUM - 1;
    while(start <= end)
    {
        k = (start + end)/2;
        if(strcmp(id,key[k]) == 0)
            break;
        else if(strcmp(id,key[k]) < 0)
            end = k - 1;
        else
            start = k + 1;
    }

    if(start <= end)
        curSy = (Symbol)k;
    else
        curSy = IDENTSY;
    if(DEBUG)
        printf("�˳�getString\n");
}

void getNum()
{
    if(DEBUG)
        printf("����getNum\n");
    int firstzero = 0;
    int ErrorZero = 0;
    int k = 0;
    inum = 0;
    do
    {
        inum *= 10;
        inum += ch - '0';
        if(firstzero == 1)
            ErrorZero = 1;
        if(k == 0 && ch == '0')
            firstzero = 1;
        k ++;
        nextch();
    }
    while(isNum());
    curSy = INTCON;
    if(ErrorZero)
        { error(0); }
    //ǰ����ERROR����>>>>>>>>>>>>>>>>>>>>>>>>>>
    if(DEBUG)
        printf("�˳�getNum\n");
}

void insymbol()
{
    if(DEBUG)
        printf("����insymbol\n");

    while(isspace(ch))
        nextch(); //���˿ո�

    if((ch >= 'a' && ch <= 'z')||(ch >= 'A' && ch <= 'Z')||ch == '_')  //ϵͳ�����ֻ��߱���
    {
        getString();
    }
    else if(isNum())
    {
        getNum();
    }
    else if(ch == '\'')
    {
        nextch();
        if(ch == '*'||ch == '/'||ch == '+'||ch == '-'||isNum()||isAlpha()) {
            ichar = ch;
        } else {
            ichar = '0';//�Ƿ��ַ�Ĭ��Ϊ0����������
            error(1);//�ַ������޶��ķ�Χ ERROR
            //ERROR>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        }
        curSy = CHARCON;
        nextch();
        if(ch != '\'') {
            error(1);
            skip('\''); //������һ��������λ�ü�����ȡ
        } //ERROR left
        nextch();
    }
    else if(ch == '"')
    {
        nextch();
        int k = 0;
        while(ch>=32&&ch<=126&&ch!=34) {
            iString[k++] = ch;
            nextch();
        }
        iString[k] = '\0';
        curSy = STRINGCON;
        if(ch != '"') {
            error(1);
            skip('\"');
        } //ERROR������
        nextch();
    }
    else if(ch == '=')
    {
        nextch();
        if(ch == '=') {
            curSy = EQU;
            nextch();
        } else curSy = BECOMESY;
    }
    else if(ch == '>')
    {
        nextch();
        if(ch == '=') {
            curSy = MOREEQU;
            nextch();
        } else curSy = MORE;
    }
    else if(ch == '<')
    {
        nextch();
        if(ch == '=') {
            curSy = LESSEQU;
            nextch();
        } else curSy = LESS;
    }
    else if(ch == '!')
    {
        nextch();
        curSy = NOEQU; //����ָ�����
        if(ch == '=') {
            nextch();
        } else {
            error(1);
        } ;   //ERROR �������
    }
    else if(ch == '('||ch == ')'||ch == '['||ch == ']'||ch == '{'||ch == '}'||ch == '+'
            ||ch == '-'||ch == '*'||ch == '/'||ch == ','||ch == ';')
    {
        curSy = sps[(int)ch];
        nextch();
    } else if(ch == EOF) {
        if(curSy == FINISHED){
            printf("�ļ���ǰ�������������\n");
            exit(0);
        }
        printf("�ļ���ȡ����\n");
        curSy = FINISHED;
    }

    if(DEBUG)
        printf("�˳�insymbol\n");
}


void setup()
{
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
    key[6] = "int";
    key[7] = "main";
    key[8] = "printf";
    key[9] = "return";
    key[10] = "scanf";
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

    //char filename[128];
    //printf("����������ļ���:");
    //scanf("%s",filename);
    instream = fopen("E:\\workfile\\develop\\Compiler\\Compiler\\test.txt","r");
    if(instream == NULL)
    {
        printf("�ļ�������");
        exit(0);
    }
    nextch();
}
