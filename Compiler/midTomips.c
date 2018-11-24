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

char findreg[32];
char regChar[8];

int used[7];

void getregChar(String in){
    if(strcmp(in,"$RegRet") == 0) strcpy(regChar,"$v0");
    else{
        strcpy(regChar,"$t");
        strcat(regChar,in+4);
    }
}

void find(String name,int index){
    int i = 0;
    for(i = 0 ; i < localCount ; i++){
        if(strcmp(localVariable[i],name) == 0){
            sprintf(findreg,"%d($fp)",-1*(localpos[i]+4*index));
            return ;
        }
    }
    for(i = 0 ; i < globalCount ; i++){
        if(strcmp(globalVariable[i],name) == 0){
            sprintf(findreg,"%d($gp)",-1*(localpos[i]+4*index));
            return ;
        }
    }
}

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
        if(globalFlag) sprintf(mipsCon,"sw $t1 %d($sp)",-1*globalpos[globalCount-1]);
        else sprintf(mipsCon,"sw $t1 %d($fp)",-1*localpos[localCount-1]);
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
    if(globalFlag) sprintf(mipsCon,"subi $sp $sp %d",globalpos[globalCount]);
    else sprintf(mipsCon,"subi $sp $sp %d",localpos[localCount]);
    ptr = addNext(ptr,mipsCon);
    ptr = addNext(ptr,"#~~~变量声明部分结束~~~~");
    return ptr;
}

int judgekind(String in,String reg){
    if(in[0] == '$') return 0;
    else if(in[0] >= '0' && in[0] <= '9'){
        sprintf(mipsCon,"li %s %s",reg,in);
        return 1;
    } else{
        find(in,0);
        sprintf(mipsCon,"lw %s %s",reg,findreg);
        return -1;
    }
}

MipsPtr sentences(MipsPtr ptr){
    if(codes[nowCode].op == BecomeOp){
        char resultReg[32];
        find(codes[nowCode].result,stringToInt(codes[nowCode].arg2));
        strcpy(resultReg,findreg);

        int kind = judgekind(codes[nowCode].arg1,"$t8");
        if(kind == 0){
            sprintf(mipsCon,"sw %s %s",codes[nowCode].arg1,resultReg);
        } else {
            ptr = addNext(ptr,mipsCon);
            sprintf(mipsCon,"sw $t8 %s",resultReg);
        }
        ptr = addNext(ptr,mipsCon);
    } else if(codes[nowCode].op == AddOp || codes[nowCode].op == SubOp || codes[nowCode].op == MultOp || codes[nowCode].op == DivOp){
        char arg1reg[32],arg2reg[32];
        int kind = judgekind(codes[nowCode].arg1,"$t8");
        if(kind == 0) strcpy(arg1reg,codes[nowCode].arg1);
        else {
            ptr = addNext(ptr,mipsCon);
            strcpy(arg1reg,"$t8");
        }
        kind = judgekind(codes[nowCode].arg2,"$t9");
        if(kind == 0) strcpy(arg2reg,codes[nowCode].arg2);
        else {
            ptr = addNext(ptr,mipsCon);
            strcpy(arg2reg,"$t9");
        }
        if(codes[nowCode].op == AddOp) sprintf(mipsCon,"add %s %s %s",arg1reg,arg2reg,codes[nowCode].result);
        else if(codes[nowCode].op == SubOp) sprintf(mipsCon,"sub %s %s %s",arg1reg,arg2reg,codes[nowCode].result);
        else if(codes[nowCode].op == MultOp) {
            sprintf(mipsCon,"mult %s %s",arg1reg,arg2reg);
            ptr = addNext(ptr,mipsCon);
            sprintf(mipsCon,"move %s $lo",codes[nowCode].result);
        } else if(codes[nowCode].op == DivOp) {
            sprintf(mipsCon,"div %s %s",arg1reg,arg2reg);
            ptr = addNext(ptr,mipsCon);
            sprintf(mipsCon,"move %s $lo",codes[nowCode].result);
        }
        ptr = addNext(ptr,mipsCon);
    } else if(codes[nowCode].op == GetArrayOp){
        find(codes[nowCode].arg1,stringToInt(codes[nowCode].arg2));
        sprintf(mipsCon,"lw %s %s",codes[nowCode].result,findreg);
    } else if(codes[nowCode].op == PushParmOp){
        int count = -8;
        char pushreg[32];
        while(codes[nowCode].op == PushParmOp){
            int kind = judgekind(codes[nowCode].result,"$t8");
            if(kind == 0) strcpy(pushreg,codes[nowCode].result);
            else {
                ptr = addNext(ptr,mipsCon);
                strcpy(pushreg,"$t8");
            }
            sprintf(mipsCon,"sw %s %d($sp)",pushreg,count);
            ptr = addNext(ptr,mipsCon);
            nowCode ++;
            count -= 4;
        }
    } else if(codes[nowCode].op == LabelOp){
        sprintf(mipsCon,"%s:",codes[nowCode].result);
        ptr = addNext(ptr,mipsCon);
    } else if(codes[nowCode].op >= 13 && codes[nowCode].op <= 18){
        //">=", "sge", ">", "sgt", "<=", "sle", "<", "slt", "==", "seq", "!=", "sne"
    }

    nowCode ++;
    return ptr;
}

MipsPtr funcMidCodes(MipsPtr ptr){
    //函数跳转保存栈，参数声明变量
    localCount = 0;
    localpos[0] = 8;
    ptr = addNext(ptr,"sw $fp 0($sp)");
    ptr = addNext(ptr,"move $fp $sp");
    ptr = addNext(ptr,"sw $ra -4($fp)");
    while(codes[nowCode].op == ParmOp){
        addVariable(4,0);
        nowCode ++;
    }
    declarMidCodes(ptr,0);
    return ptr;
}

void fillText(){
    MipsPtr textcopy = text;
    textcopy = addNext(textcopy,"move $gp $sp");
    textcopy = declarMidCodes(textcopy,1);
    textcopy = addNext(textcopy,"j main");
    while(codes[nowCode].op == FunctionOp){
        sprintf(mipsCon,"%s:",codes[nowCode++].result);
        textcopy = addNext(textcopy,mipsCon);
        textcopy = funcMidCodes(textcopy);
    }

}


void midToMips(){
    Init();
    fillData();
    fillText();

}
