#include "types.h"
#include "util.h"

int nowCode;

char globalVariable[4096][128];
int globalpos[4096];
int globalCount;
char localVariable[4096][128];
int localpos[4096];
int localCount;

char mipsCon[64];

void Init(){
    text = (MipsPtr)malloc(sizeof(MipsCode));
    memset(text,0,sizeof(MipsCode));

    strcpy(dataVariable[dataCount++] , ".data");
    strcpy(text -> code , ".text");
}

void fillData(){
    int i = 0;
    char string_con[128];
    for(i = 0 ; i < countprint ; i++){
        sprintf(string_con,"%s: .asciiz \"%s\"",getStringCon(i),printreg[i]);//sprintf黑科技
        strcpy(dataVariable[dataCount++] , string_con);
    }
}

void addVariable(int size , int globalFlag){
    if(globalFlag){
        strcpy(globalVariable[globalCount++],codes[nowCode].result);
        globalpos[globalCount] = globalpos[globalCount-1]+size;
    } else {
        strcpy(localVariable[localCount++],codes[nowCode].result);
        localpos[localCount] = localpos[localCount-1]+size;
    }
}

MipsPtr declarMidCodes(MipsPtr ptr , int globalFlag){
    while(codes[nowCode].op == ConstOp){

        addVariable(4,globalFlag);

        if(strcmp(codes[nowCode].arg1,"int") == 0) sprintf(mipsCon,"li $t1 %d",stringToInt(codes[nowCode].arg2)); //为const变量赋值
        else sprintf(mipsCon,"li $t1 %d",charToInt(codes[nowCode].arg2));
        ptr = addNext(ptr,mipsCon);
        sprintf(mipsCon,"sw $t1 %d($sp)",globalpos[globalCount-1]);
        ptr = addNext(ptr,mipsCon);
        nowCode++;
    }
    while(codes[nowCode].op == VarOp || codes[nowCode].op == ArrayOp){
        if(codes[nowCode].op == VarOp){
            addVariable(4,globalFlag);
        } else {
            addVariable(stringToInt(codes[nowCode].arg2)*4,globalFlag);
        }
        nowCode++;
    }
    sprintf(mipsCon,"subi $sp $sp %d",globalpos[globalCount]);
    ptr = addNext(ptr,mipsCon);
    ptr = addNext(ptr,"#~~~变量声明部分结束~~~~");
    return ptr;
}

void fillText(){
    MipsPtr textcopy = text;
    textcopy = addNext(textcopy,"move $gp $sp");
    textcopy = declarMidCodes(textcopy,1);
    textcopy = addNext(textcopy,"j main");

}


void midToMips(){
    Init();
    fillData();
    fillText();
}
