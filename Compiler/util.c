#include "types.h"
#include "error.h"
#include "util.h"

char numstring[32];
char charstring[32];
char tempreg[32];
char labelreg[32];
char strconreg[32];

int stringToInt(String in){
    return atoi(in);
}

String intToString(int inum){
    itoa(inum,numstring,10);
    return numstring;
}

String getStringCon(int count){
    strcpy(strconreg,"string_");
    strcat(strconreg,intToString(count));
    return strconreg;
}


String numToLabel(int n){
    strcpy(labelreg,"label_");
    strcat(labelreg,intToString(n));
    return labelreg;
}

String processString(String instring){
    char cache[1024];
    strcpy(cache,"\\");
    int length = strlen(instring);
    int i = 0;
    for(i = 0 ; i < length ; i++){
        if(instring[i] == '\\'){
            strcat(cache,instring+i);
            strcpy(instring+i,cache);
            strcpy(cache,"\\");
            length++;
            i++;
        }
    }
    return instring;
}

int insertString(String instring){
    processString(instring);
    int i = 0;
    for(i = 0 ; i < countprint ; i ++){
        if(strcmp(printreg[i],instring) == 0){
            return i;
        }
    }
    strcpy(printreg[countprint],instring);
    return countprint++;
}

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
            i = idtabs[i].link;
        }
        j --;
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

void emitLabel(int codepos , int labelnum){
    strcpy(codes[codepos].result,numToLabel(labelnum));
}

void emit(enum ops op, String arg1, String arg2, String result){
    codes[codeCount].op = op;
    strcpy(codes[codeCount].arg1 , arg1);
    strcpy(codes[codeCount].arg2 , arg2);
    strcpy(codes[codeCount].result , result);
    codeCount++;
}


String numToReg(int n){
    strcpy(tempreg,"$Reg");
    strcat(tempreg,intToString(n));
    return tempreg;
}



void printtabs(){
    //Const,Var,Array,Function,Parm
    String kindString[] = {"const","var","array","function","parm"};
    String typeString[] = {"int","char","none"};
    printf("%15s %15s %15s %15s %15s\n","name","kind","type","value","link");
    int i = 1 ;
    for( ; i <=  tabCount ; i++){
        printf("%15s %15s %15s %15d %15d\n",idtabs[i].name
               ,kindString[idtabs[i].kind],typeString[idtabs[i].type],idtabs[i].value,idtabs[i].link);
    }
}

/*
enum ops{
    ConstOp,VarOp,ArrayOp,FunctionOp,ParmOp,PushParmOp,GetArrayOp,MultOp,DivOp,AddOp,SubOp,
    BecomeOp,LessOp,LessequOp,EquOp,NoequOp,MoreOp,MoreequOp,
    FalseOp,TrueOp,GotoOp,LabelOp,ScanfOp,PrintfOp,RetOp,MainOp
    //LESS,LESSEQU,EQU,NOEQU,MORE,MOREEQU,17-22 ->  12-17 -> -5
};
*/

void printcodes(){
    printf("<<<<<<<<<<<midcodes>>>>>>>>>>>>\n");
    String opstring[] = {"const","var","array","function","param","push","call","getArray","*","/","+","-",
                        "=","<","<=","==","!=",">",">=",
                        "falsegoto","truegoto","goto","label","scanf","printf","ret","main"};
    printf("%15s %15s %15s %15s\n","opeartion","arg1","arg2","result");
    int i = 0 ;
    for( ; i <  codeCount ; i++){
        printf("%15s %15s %15s %15s\n",opstring[codes[i].op],codes[i].arg1,codes[i].arg2,codes[i].result);
    }
}

void printstring(){
    printf("<<<<<<<<<<<strings>>>>>>>>>>>>\n");
    int i = 0 ;
    for( i = 0 ; i < countprint ; i++){
        printf("%d:%s\n",i,printreg[i]);
    }
}

void printmips(){
    printf("<<<<<<<<<<<<<mips>>>>>>>>>>>>>>\n");
    int i = 0 ;
    for(i = 0 ; i < dataCount ; i++){
        printf("%s\n",dataVariable[i]);
    }
    i = 0;
    while(i != ansCount){
        printf("%s\n",resultMips[i]);
        i++;
    }
}
