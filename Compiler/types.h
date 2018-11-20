#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define DEBUG 0

typedef char* String;

enum symbol{
    CHARSY,CONSTSY,DOSY,ELSESY,FORSY,IFSY,INPUTSY,INTSY,
    MAINSY,OUTPUTSY,RETURNSY,VOIDSY,WHILESY,
    PLUS,SUB,MULT,DIV,LESS,LESSEQU,EQU,NOEQU,MORE,MOREEQU,BECOMESY,COMMA,SEMICOLON,
    LPARENT,RPARENT,LBRACKET,RBRACKET,LBRACE,RBRACE,
    IDENTSY,INTCON,CHARCON,STRINGCON
};
typedef enum symbol Symbol;

enum kindsy{Const,Var,Array,Function,Parm};
enum typel{Int,Char,None};

Symbol curSy;
char id[128];
int inum;
char ichar;
char iString[1024]; //�ʷ���������ĳ����������������

struct Tab {
    char name[128];
    enum kindsy kind;
    enum typel type; //���ں�����˵type��ʾ����ֵ����
    int value; // ���ں�������value��ʾ�ú�����block���е�����

    int link; //linkΪǰһ�������ڷ��ű��е�λ��
};
typedef struct Tab tab;
tab idtabs[10240];
int tabCount;

//������c0�ķ������Ϊһά����
//struct Atab {
//    int arraySize;
//    int n; //ά��
//    enum typel type;
//    int sizeList[32];
//};
//typedef struct Atab atab;
//atab arraytabs[1024];
//int atabCount;

//��������ʱ������������������{֮ʱ��������Ӧ��block
struct Btab {
    int last;
    int lastparm;
    int parmnum;
};
typedef struct Btab btab; //��¼block�м�¼�Ĳ���λ�����������������λ��
btab blocktabs[4096];
int blockCount;

enum ops{
    ConstOp,VarOp,ArrayOp,FunctionOp,ParmOp,PushParmOp,GetArrayOp,MultOp,DivOp,AddOp,SubOp,
    BecomeOp,LessOp,LessequOp,EquOp,NoequOp,MoreOp,MoreequOp,
    FalseOp,TrueOp,GotoOp,LabelOp,ScanfOp,PrintfOp,RetOp,MainOp
    //LESS,LESSEQU,EQU,NOEQU,MORE,MOREEQU,17-22 ->  12-17 -> -5
};
struct MidCode {
    enum ops op;
    char arg1[32];
    char arg2[32];
    char result[32];
};
typedef struct MidCode midcode;
midcode codes[10240];
int codeCount;

int level;
int display[128]; //display����¼����¼����һ��block��btab��λ��


#endif // TYPES_H_INCLUDED
