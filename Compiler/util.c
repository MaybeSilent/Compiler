#include "types.h"
#include "error.h"


char numstring[32];
char charstring[32];
char tempreg[32];


void enterblock(){
    blockCount ++;
    blocktabs[blockCount].last = 0;
    blocktabs[blockCount].lastparm = 0;
    blocktabs[blockCount].parmnum = 0;
}

int loc(String id){
    int j = level;
    while(j >= 0){
        int i = blocktabs[display[j]].last;
        while(i != 0){
            if(strcmp(idtabs[i].name , id) == 0) return i;
        }
    }
    error(12);
    return -1;

}

void entertab(String name, enum kindsy kind, enum typel type, int value){
    int blockindex = display[level];
    //检查前面是否已有相应命名冲突 error(4)
    int i = blocktabs[blockindex].last;
    while(i != 0){
        if(strcmp(idtabs[i].name,name) == 0){
            error(4);
            return;
        } else i = idtabs[i].link;
    }

    tabCount ++;
    strcpy(idtabs[tabCount].name,name);
    idtabs[tabCount].type = type;
    idtabs[tabCount].kind = kind;
    idtabs[tabCount].value = value;

    idtabs[tabCount].link = blocktabs[blockindex].last;
    blocktabs[blockindex].last = tabCount;
    if(kind == Parm) blocktabs[blockindex].lastparm = tabCount;

}

void emit(enum ops op, String arg1, String arg2, String result){
    codes[codeCount].op = op;
    strcpy(codes[codeCount].arg1 , arg1);
    strcpy(codes[codeCount].arg2 , arg2);
    strcpy(codes[codeCount].result , result);
    codeCount++;
}

String intToString(int inum){

    itoa(inum,numstring,10);
    return numstring;
}

String charToString(char ichar){

    charstring[0] = ichar;
    charstring[1] = '\0';
    return charstring;
}

String numToReg(int n){
    tempreg[0] = '$';
    tempreg[1] = '\0';
    strcat(tempreg,intToString(n));
    return tempreg;
}

void printtabs(){
    String kindString[] = {"const","var","array","function"};
    String typeString[] = {"int","char","array","none"};
    printf("%15s %15s %15s %15s %15s\n","name","kind","type","value","link");
    int i = 1 ;
    for( ; i <=  tabCount ; i++){
        printf("%15s %15s %15s %15d %15d\n",idtabs[i].name
               ,kindString[idtabs[i].kind],typeString[idtabs[i].type],idtabs[i].value,idtabs[i].link);
    }
}

void printcodes(){
    printf("<<<<<<<<<<<midcodes>>>>>>>>>>>>\n");
    String opstring[] = {"const","var","array","function"};
    printf("%15s %15s %15s %15s\n","opeartion","arg1","arg2","result");
    int i = 0 ;
    for( ; i <  codeCount ; i++){
        printf("%15s %15s %15s %15s\n",opstring[codes[i].op],codes[i].arg1,codes[i].arg2,codes[i].result);
    }
}
