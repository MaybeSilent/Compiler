#include "preprocess.h"
#include "types.h"
#include "util.h"

void addVar(int size,String name){
    int count = runtab[runCount].varCount;
    strcpy(runtab[runCount].varName[count],name);
    runtab[runCount].varCount++;
    runtab[runCount].varpos[count+1] = runtab[runCount].varpos[count] + size;
}

void processVar(){
    runCount = 0;
    strcpy(runtab[0].funcName,"#init");
    int preindex = 0;
    while(preindex != codeCount){
        if(codes[preindex].op == FunctionOp){
            runCount ++;
            strcpy(runtab[runCount].funcName,codes[preindex].result);
            runtab[runCount].varpos[0] = 8;
        } else {
            if(codes[preindex].op == ConstOp || codes[preindex].op == VarOp){
                addVar(4,codes[preindex].result);
            } else if(codes[preindex].op == ArrayOp) {
                addVar(stringToInt(codes[preindex].arg2)*4,codes[preindex].result);
            } else {
                ///////////////////////////////
                //加入临时寄存器部分
                ///////////////////////////////
                if(codes[preindex].result[0] == '$'){
                    addVar(4,codes[preindex].result);
                }
            }
        }
        preindex++;
    }
}

void freshRunPos(String name){ //读到函数的时候，需要将runpos更新一下
    int i = 0;
    for(i = 0 ; i < runCount ; i++){
        if(strcmp(runtab[i].funcName,name) == 0){
            runpos = i;
        }
    }
}


int search(int runindex,String name){
    int i = 0;
    int count = runtab[runindex].varCount;
    for(i = 0 ; i < count; i++){
        if(strcmp(runtab[runindex].varName[i],name) == 0){
            return runtab[runindex].varpos[i];
        }
    }
    return -1;
}

void find(String name, int index){
    int pos = search(runpos,name);

    if(pos == -1){
        pos = search(0,name);
        sprintf(findreg,"-%d($gp)",pos+index*4);
    } else {
        sprintf(findreg,"-%d($fp)",pos+index*4);
    }
}

