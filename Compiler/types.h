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
    PLUS,SUB,MULT,DIV,LESS,LESSEQU,EQU,NOEQU,BECOMESY,MORE,MOREEQU,COMMA,SEMICOLON,
    LPARENT,RPARENT,LBRACKET,RBRACKET,LBRACE,RBRACE,
    IDENTSY,INTCON,CHARCON,STRINGCON
};
typedef enum symbol Symbol;

Symbol curSy;
char id[128];
int inum;
char ichar;
char iString[1024]; //�ʷ���������ĳ����������������

struct Tab {
    char ident[128];
};

#endif // TYPES_H_INCLUDED
