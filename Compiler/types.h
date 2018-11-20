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
char iString[1024]; //词法分析读入的常量与变量保存内容

struct Tab {
    char name[128];
    enum kindsy kind;
    enum typel type; //对于函数来说type表示返回值类型
    int value; // 对于函数名的value表示该函数在block表中的索引

    int link; //link为前一个变量在符号表中的位置
};
typedef struct Tab tab;
tab idtabs[10240];
int tabCount;

//！！！c0文法数组仅为一维数组
//struct Atab {
//    int arraySize;
//    int n; //维数
//    enum typel type;
//    int sizeList[32];
//};
//typedef struct Atab atab;
//atab arraytabs[1024];
//int atabCount;

//声明函数时，或者在语句块中遇到{之时，插入相应的block
struct Btab {
    int last;
    int lastparm;
    int parmnum;
};
typedef struct Btab btab; //记录block中记录的参数位置与最后声明变量的位置
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
int display[128]; //display区记录，记录了上一层block在btab的位置


#endif // TYPES_H_INCLUDED
