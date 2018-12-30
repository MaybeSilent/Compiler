#include "types.h"
#include "util.h"
#include "optimization.h"

int nowCode;

char globalVariable[4096][128];
int globalpos[4096];
int globalCount;
char localVariable[4096][128];
int localpos[4096];
int localCount;

char funcname[256][128];
int funcnum[256];
int funcCount = 0;

int parmcount = 0;

void printVariable(){
    int i = 0;
    for(i = 0 ; i < globalCount ; i++){
        printf("%s %d    ",globalVariable[i],globalpos[i]);
    }
    printf("\n");
    for(i = 0 ; i < localCount ; i++){
        printf("%s %d    ",localVariable[i],localpos[i]);
    }
    printf("\n");
}

char mipsCon[64];

char findreg[32];
char regChar[8];

char arg1reg[32],arg2reg[32];

struct regState{
    int states; //0代表空闲
    char name[128]; //所存放的值名字
};
String regName[] = {"$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7"};
struct regState Regpool[8];

int rollIndex = 0;

String find(String name,String pos){
    int i = 0;
    int globalFlag = -1;
    int nameindex = 0;
    for(i = 0 ; i < localCount ; i++){
        if(strcmp(localVariable[i],name) == 0){
            globalFlag = 0;
            nameindex = i;
            break;
        }
    }
    if(globalFlag == -1){
        for(i = 0 ; i < globalCount ; i++){
            if(strcmp(globalVariable[i],name) == 0){
                globalFlag = 1;
                nameindex = i;
                break;
            }
        }
        if(globalFlag == -1){
            printf("%s\n",name);
            //printVariable();
            printf("%s %s %s\n",codes[nowCode].arg1,codes[nowCode].arg2,codes[nowCode].result);
            printf("server error happen\n");
        }
    }
    if(pos[0] >= '0' && pos[0] <= '9'){
        int index = atoi(pos);
        if(globalFlag) sprintf(findreg,"-%d($gp)",globalpos[nameindex]+index*4);
        else sprintf(findreg,"-%d($fp)",localpos[nameindex]+index*4);
    } else {
        String ans;
        if(pos[0] == '$') ans = findbyReg(pos,1);
        else{
            sprintf(resultMips[ansCount++],"lw $t8 %s",find(pos,"0"));
            ans = "$t8";
        }
        sprintf(resultMips[ansCount++],"sll $t8 %s 2",ans);
        if(globalFlag){
            sprintf(resultMips[ansCount++],"subu $t8 $gp $t8");
            sprintf(findreg,"-%d($t8)",globalpos[nameindex]);
        }
        else{
            sprintf(resultMips[ansCount++],"subu $t8 $fp $t8");
            sprintf(findreg,"-%d($t8)",localpos[nameindex]);
        }

    }
    return findreg;
}

void saveToMem(int index){
    if(Regpool[index].states == 0) return;
    else {
        Regpool[index].states = 0;
        sprintf(resultMips[ansCount++],"sw %s %s",regName[index],find(Regpool[index].name,"0"));
    }
}

String getReg(String in){
    int i = 0;
    for(i = 0 ; i < 8 ; i++){
        if(Regpool[i].states == 0){
            strcpy(Regpool[i].name,in);
            Regpool[i].states = 1;
            return regName[i];
        }
    }
    rollIndex = rollIndex % 7;
    saveToMem(rollIndex++);
    Regpool[rollIndex-1].states = 1;
    strcpy(Regpool[rollIndex-1].name,in);
    return regName[rollIndex-1];
}

String findbyReg(String in,int flag){ //寄存器是否需要lw
    int i = 0;
    for(i = 0 ; i < 8 ; i++){
        //printf("%s %s %d\n",Regpool[i].name,in,Regpool[i].states);
        if(Regpool[i].states == 1 && strcmp(Regpool[i].name,in) == 0){
            return regName[i];
        }
    }
    String ansreg = getReg(in);
    if(flag){
        sprintf(resultMips[ansCount++],"lw %s %s",ansreg,find(in,"0"));
    }
    return ansreg;
}

void saveall(){
    int i = 0;
    for(i = 0; i < 8 ; i++){
        saveToMem(i);
    }
}

void clearall(){
    int i = 0;
    for(i = 0 ; i < 8 ; i++) Regpool[i].states = 0;
}

void Init(){
    strcpy(dataVariable[dataCount++] , ".data");
    strcpy(resultMips[ansCount++] , ".text");
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

void RegforTemp(){
    int codecache = nowCode;
    while(codes[nowCode].op != FunctionOp && nowCode < codeCount){
        if(codes[nowCode].result[0] == '$'){
            int flag = 1;
            int i = 0;
            for(i = 0 ; i < localCount ; i++){
                if(strcmp(localVariable[i],codes[nowCode].result) == 0){
                    flag = 0;
                    break;
                }
            }
            if(flag) addVariable(4,0);
        }
        nowCode++;
    }
    nowCode = codecache;
}

void declarMidCodes(int globalFlag){
    while(codes[nowCode].op == ConstOp && nowCode < codeCount){

        addVariable(4,globalFlag);

        if(strcmp(codes[nowCode].arg1,"int") == 0) sprintf(resultMips[ansCount++],"li $t8 %d",stringToInt(codes[nowCode].arg2)); //为const变量赋值
        else sprintf(resultMips[ansCount++],"li $t8 %d",stringToInt(codes[nowCode].arg2));
        if(globalFlag) sprintf(resultMips[ansCount++],"sw $t8 %d($sp)",-1*globalpos[globalCount-1]);
        else sprintf(resultMips[ansCount++],"sw $t8 %d($fp)",-1*localpos[localCount-1]);
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
    RegforTemp();
    if(globalFlag) sprintf(resultMips[ansCount++],"subiu $sp $sp %d",globalpos[globalCount]);
    else sprintf(resultMips[ansCount++],"subiu $sp $sp %d",localpos[localCount]);
    strcpy(resultMips[ansCount++],"#~~~变量声明部分结束~~~~");
    //printVariable();
}

int judgekind(String in,String reg){
    if(in[0] == '$'){
        return 0;
    }
    else if((in[0] >= '0' && in[0] <= '9') || in[0] == '-'){
        sprintf(resultMips[ansCount++],"li %s %s",reg,in);
        return 1;
    } else{
        sprintf(resultMips[ansCount++],"lw %s %s",reg,find(in,"0"));
        return -1;
    }
}

void getargs(String arg1 , String arg2){
    int kind = judgekind(arg1,"$t8");
    if(kind == 0){
        strcpy(arg1reg,findbyReg(arg1,1));
    }
    else {
        strcpy(arg1reg,"$t8");
    }
    kind = judgekind(arg2,"$t9");
    if(kind == 0) strcpy(arg2reg,findbyReg(arg2,1));
    else {
        strcpy(arg2reg,"$t9");
    }
}

void sentences(){
    if(codes[nowCode].op == BecomeOp){
        char resultReg[32];
        strcpy(resultReg,find(codes[nowCode].result,codes[nowCode].arg2));
        int kind = judgekind(codes[nowCode].arg1,"$t9");
        if(kind == 0){
            sprintf(resultMips[ansCount++],"sw %s %s",findbyReg(codes[nowCode].arg1,1),resultReg);
        } else {
            sprintf(resultMips[ansCount++],"sw $t9 %s",resultReg);
        }
    } else if(codes[nowCode].op == AddOp || codes[nowCode].op == SubOp || codes[nowCode].op == MultOp || codes[nowCode].op == DivOp){
        //printf("%s %s\n",codes[nowCode].arg1,codes[nowCode].arg2);
        getargs(codes[nowCode].arg1,codes[nowCode].arg2);
        //获取参数的形式
        if(codes[nowCode].op == AddOp) sprintf(resultMips[ansCount++],"addu %s %s %s",findbyReg(codes[nowCode].result,0),arg1reg,arg2reg);
        else if(codes[nowCode].op == SubOp) sprintf(resultMips[ansCount++],"subu %s %s %s",findbyReg(codes[nowCode].result,0),arg1reg,arg2reg);
        else if(codes[nowCode].op == MultOp) {
            sprintf(resultMips[ansCount++],"mult %s %s",arg1reg,arg2reg);
            sprintf(resultMips[ansCount++],"mflo %s",findbyReg(codes[nowCode].result,0));
        } else if(codes[nowCode].op == DivOp) {
            //sprintf(resultMips[ansCount++],"%s %s %s","!!!!!",codes[nowCode].arg1,codes[nowCode].arg2);
            sprintf(resultMips[ansCount++],"div %s %s",arg1reg,arg2reg);
            sprintf(resultMips[ansCount++],"mflo %s",findbyReg(codes[nowCode].result,0));
        }
    } else if(codes[nowCode].op == GetArrayOp){
        strcpy(arg1reg,findbyReg(codes[nowCode].result,0));
        strcpy(arg2reg,find(codes[nowCode].arg1,codes[nowCode].arg2));
        //getargs(codes[nowCode].arg1,codes[nowCode].arg2);
        sprintf(resultMips[ansCount++],"lw %s %s",findbyReg(codes[nowCode].result,0),find(codes[nowCode].arg1,codes[nowCode].arg2));
    } else if(codes[nowCode].op == PushParmOp){
        char pushreg[32];
        while(codes[nowCode].op == PushParmOp){
            int kind = judgekind(codes[nowCode].result,"$t8");
            if(kind == 0) strcpy(pushreg,findbyReg(codes[nowCode].result,1));
            else {
                strcpy(pushreg,"$t8");
            }
            sprintf(resultMips[ansCount++],"sw %s %d($sp)",pushreg,parmcount);
            nowCode ++;
            parmcount -= 4;
        }
        nowCode --;
    } else if(codes[nowCode].op == LabelOp){
        sprintf(resultMips[ansCount++],"%s:",codes[nowCode].result);
    } else if(codes[nowCode].op >= 13 && codes[nowCode].op <= 18){
        //">=", "sge", ">", "sgt", "<=", "sle", "<", "slt", "==", "seq", "!=", "sne"
        //LessOp,LessequOp,EquOp,NoequOp,MoreOp,MoreequOp  比较的结果保留在$t8寄存器之中
        String comops[6] = {"slt","sle","seq","sne","sgt","sge"};
        getargs(codes[nowCode].arg1,codes[nowCode].arg2);
        sprintf(resultMips[ansCount++],"%s $t8 %s %s",comops[codes[nowCode].op-13],arg1reg,arg2reg);
    } else if(codes[nowCode].op == GotoOp) {
        sprintf(resultMips[ansCount++],"j %s",codes[nowCode].result);
    } else if(codes[nowCode].op == FalseOp || codes[nowCode].op == TrueOp){
        sprintf(resultMips[ansCount++],"beq $t8 %d %s",codes[nowCode].op == TrueOp,codes[nowCode].result);
    } else if(codes[nowCode].op == CallOp){
        //检索函数参数内容//
        int index = 0;
        for(index = 0 ; index < funcCount ; index++){
            if(strcmp(funcname[index],codes[nowCode].arg1) == 0) break;
        }
        int count = funcnum[index];
        int offset = 0;
        offset = ( count * (-4)) - parmcount;
        if(!OPTIMIZE) saveall();
        /////////////////////////////////
        sprintf(resultMips[ansCount++],"sw $fp %d($sp)",parmcount);
        sprintf(resultMips[ansCount++],"sw $ra %d($sp)",parmcount - 4);
        /////////////////////////////////
        if(offset != 0) sprintf(resultMips[ansCount++],"subiu $sp $sp %d",offset);
        sprintf(resultMips[ansCount++],"move $fp $sp");
        sprintf(resultMips[ansCount++],"jal %s",codes[nowCode].arg1);
        sprintf(resultMips[ansCount++],"move $sp $fp");
        if(offset != 0) sprintf(resultMips[ansCount++],"addiu $sp $sp %d",offset);
        //////////////////////////////////
        sprintf(resultMips[ansCount++],"lw $fp %d($sp)",parmcount);
        sprintf(resultMips[ansCount++],"lw $ra %d($sp)",parmcount - 4);
        if(offset != 0){
            parmcount = parmcount + count * 4;
        } else parmcount = 0;
        if(strcmp(codes[nowCode].result,"0") != 0) sprintf(resultMips[ansCount++],"move %s $v0",findbyReg(codes[nowCode].result,0));
    } else if(codes[nowCode].op == ScanfOp){
        int scanftype = strcmp(codes[nowCode].arg1,"int") == 0 ? 5 : 12 ;
        sprintf(resultMips[ansCount++],"li $v0 %d",scanftype);
        sprintf(resultMips[ansCount++],"syscall");
        sprintf(resultMips[ansCount++],"sw $v0 %s",find(codes[nowCode].result,"0"));
    } else if(codes[nowCode].op == PrintfOp){
        int printftype = strcmp(codes[nowCode].arg1,"int") == 0 ? 1 : strcmp(codes[nowCode].arg1,"char") == 0 ? 11 : 4;
        if(printftype == 4) {
            sprintf(resultMips[ansCount++],"la $a0 %s",codes[nowCode].result);
        } else {
            char outreg[32];
            int kind = judgekind(codes[nowCode].result,"$t8");
            if(kind == 0) strcpy(outreg,findbyReg(codes[nowCode].result,1));
            else {
                strcpy(outreg,"$t8");
            }
            sprintf(resultMips[ansCount++],"move $a0 %s",outreg);
        }
        sprintf(resultMips[ansCount++],"li $v0 %d",printftype);
        sprintf(resultMips[ansCount++],"syscall");
        sprintf(resultMips[ansCount++],"li $a0 \'\\n\'");
        sprintf(resultMips[ansCount++],"li $v0 11");
        sprintf(resultMips[ansCount++],"syscall");

    }else if(codes[nowCode].op == RetOp){
        if(strcmp(codes[nowCode].result,"NULL") !=0 ){
            char outreg[32];
            int kind = judgekind(codes[nowCode].result,"$t8");
            if(kind == 0) strcpy(outreg,findbyReg(codes[nowCode].result,1));
            else {
                strcpy(outreg,"$t8");
            }
            sprintf(resultMips[ansCount++],"move $v0 %s",outreg);
        }
        sprintf(resultMips[ansCount++],"jr $ra");
    }
    nowCode ++;
}

void funcMidCodes(){
    //函数跳转保存栈，参数声明变量
    localCount = 0;
    localpos[0] = 0;
    int func_count = 0;
    while(codes[nowCode].op == ParmOp){
        addVariable(4,0);
        nowCode ++;
        func_count ++;
    }
    funcnum[funcCount++] = func_count;
    ///////////////////////////////插入fp与ra区域
    strcpy(localVariable[localCount++],"$fp");
    localpos[localCount] = localpos[localCount-1]+4;
    strcpy(localVariable[localCount++],"$ra");
    localpos[localCount] = localpos[localCount-1]+4;
    ////////////////////////////////插入fp与ra区域

    declarMidCodes(0); //局部声明变量
    while(codes[nowCode].op != FunctionOp && nowCode < codeCount){
        sentences();
    }
    if(strcmp(resultMips[ansCount-1],"jr $ra")!=0 && nowCode < codeCount){
            sprintf(resultMips[ansCount++],"jr $ra");
    }
}


void fillText(){
    strcpy(resultMips[ansCount++],"move $gp $sp");
    declarMidCodes(1); //add global value
    strcpy(resultMips[ansCount++],"j main");
    while(codes[nowCode].op == FunctionOp){
        strcpy(funcname[funcCount],codes[nowCode].result);//将函数名加入到函数表
        clearall();
        sprintf(resultMips[ansCount++],"%s:",codes[nowCode++].result);
        funcMidCodes();
    }

    if(strcmp(resultMips[ansCount-1],"jr $ra") == 0) ansCount --;

}


void midToMips(){
    Init();
    fillData();
    fillText();
}
