#include "types.h"
#include "parse.h"

void testParse(){
    String symbolString[] = {
        "CHARSY","CONSTSY","DOSY","ELSESY","FORSY","IFSY","INPUTSY","INTSY",
        "MAINSY","OUTPUTSY","RETURNSY","VOIDSY","WHILESY",
        "PLUS","SUB","MULT","DIV","LESS","LESSEQU","EQU","NOEQU","BECOMESY","MORE","MOREEQU","COMMA","SEMICOLON",
        "LPARENT","RPARENT","LBRACKET","RBRACKET","LBRACE","RBRACE",
        "IDENTSY","INTCON","CHARCON","STRINGCON"
    };
    String symbolOfOp[] = {"+","-","*","/","<","<=","==","!=","=",">",">=",",",";",
                            "(",")","[","]","{","}"};
    int count = 0;
    setup();
    while(1){
        insymbol();
        count ++;
        printf("%d\t",count);
        if((curSy>=0 && curSy <= 12)||curSy == IDENTSY){
            printf("%s\t%s\n",id,symbolString[curSy]);
        } else if(curSy >= 13 && curSy <= 31){
            printf("%s\t%s\n",symbolOfOp[curSy-13],symbolString[curSy]);
        } else if(curSy == INTCON){
            printf("%d\t%s\n",inum,symbolString[curSy]);
        } else if(curSy == CHARCON){
            printf("\'%c\'\t%s\n",ichar,symbolString[curSy]);
        } else if(curSy == STRINGCON){
            printf("\"%s\"\t%s\n",iString,symbolString[curSy]);
        }
    }
}
