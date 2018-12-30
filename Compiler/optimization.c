#include "types.h"
#include "optimization.h"
void loopCallLoop(){
    int function[256];
    int funcCount;
    int i ;
    for(i = 0 ; i < codeCount ; i++){
        if(codes[i].op == FunctionOp){
            function[funcCount++] = i;
        }
    }

    for( i = 0 ; i < loopCount ; i++){
        int j ;
        int start = opblock[loopBlo[i].start].start;
        int end = opblock[loopBlo[i].ends].finish;
        int AlFlag = 0;
        for(j = start ; j < end ; j++){
            if( codes[j].op == CallOp ){
                int k ;
                int coms = 0;
                int come = 0;
                for( k = 0 ; k < funcCount ; k++ ){
                    if(strcmp(codes[function[k]].result,codes[j].arg1) == 0){
                        coms = function[k];
                        come = function[k + 1];
                        int n = 0;
                        for(n = 0 ; n < loopCount ; n++){
                            if(loopBlo[n].start >= coms && loopBlo[n].ends <= come) AlFlag = 1;
                            break;
                        }
                        if(n != loopCount) break;
                    }
                }
                if(k != funcCount) break;
            }
        }
        if(AlFlag){
            //去除当前循环的优化，保留内层循环的优化
            for(j = i ; j < loopCount ; j++){
                loopBlo[i] = loopBlo[i+1];
            }
            loopCount --;
            i --;
        }
    } //如果循环里面的函数调用也有循环，进行相应的优化，$s0寄存器会出现相应的问题，所以需要去除掉外层循环
}

void opexpre(){
    int i ;
    //同时进行循环内赋值语句的优化
    ///////////////////////////////////////////////////
    for(i = 0 ; i < loopCount ; i++){
        int j;
        int start = opblock[loopBlo[i].start].start;
        int end = opblock[loopBlo[i].ends].finish;
        for( j = start ; j < end ; j++){
            if(codes[j].op == AddOp || codes[j].op == SubOp || codes[j].op == MultOp || codes[j].op == DivOp){
                if(codes[j+1].op == BecomeOp){
                    int k = 0;
                    int AddFlag = 0;
                    for(k = 0 ; k < loopBlo[i].regNum ; k ++){
                        if(strcmp(loopBlo[i].regs[k],codes[j+1].result) == 0){ //必须是已经分配了寄存器的值
                            AddFlag = 1;
                        }
                    } //不能够是数组
                    if(strcmp(codes[j].result,codes[j+1].arg1) == 0 && strcmp(codes[j+1].arg2,"0") == 0 && AddFlag){
                        strcpy(codes[j].result , codes[j+1].result);
                        printf("%d %d >>>>>>>>>>>>>>>>>>>>>>>>>>\n",j , j +1);
                        codes[j+1].op = NullOp;
                    }
                }
            }
        }
    }
    ///////////////////////////////////////////////
    //相关窥孔优化
}

void basicBlock(){
    int i;
    int blocksIn[8192];
    int InCount = 1;
    for(i = 0 ; i < codeCount ; i++){
        if(codes[i].op == FunctionOp || codes[i].op == LabelOp){
            if(i > blocksIn[InCount - 1]) blocksIn[InCount++] = i;
        } else if(codes[i].op == CallOp || codes[i].op == RetOp || codes[i].op == TrueOp || codes[i].op == FalseOp){
            if(i + 1 < codeCount && i > blocksIn[InCount - 1]) blocksIn[InCount++] = i + 1;
        }
    }
    bbCount = InCount;
    for(i = 0 ; i < InCount ; i ++){
        opblock[i].start = blocksIn[i];
        opblock[i].finish = blocksIn[i+1] - 1;
        if(i + 1 == InCount) opblock[i].finish = codeCount - 1;

        if(codes[opblock[i].finish].op == TrueOp || codes[opblock[i].finish].op == FalseOp || codes[opblock[i].finish].op == GotoOp){
            int j = 0;
            char storeReg[32];
            strcpy(storeReg,codes[opblock[i].finish].result);
            for(j = 0 ; j < InCount ; j++){
                if(strcmp(codes[blocksIn[j]].result,storeReg) == 0){
                    opblock[i].next[opblock[i].nextCount++] = j;
                    if((j <= i) && (loopCount == 0 || j > loopBlo[loopCount-1].ends)){
                        loopBlo[loopCount].start = j;
                        loopBlo[loopCount].ends = i;
                        loopCount ++ ; //找出基本块之间有循环的部分
                    }
                    break;
                }
            }
        }
        if(codes[opblock[i].finish + 1].op != FunctionOp && i + 1 < InCount) opblock[i].next[opblock[i].nextCount++] = i + 1;
    }
    loopCallLoop();
}
//循环中全局寄存器的分配
//对于循环中的全局寄存器进行分配
int isVar(String in){
    if(in[0] == '$' || (in[0] >= '0' && in[0] <= '9')){
        return 0;
    }
    return 1;
}
void addVar(String var){
    int i = 0 ;
    for( i = 0 ; i < numsOfCounter ; i++){
        if(strcmp(counters[i].name,var) == 0 ){
            counters[i].nums++;
            return ;
        }
    }
    strcpy(counters[numsOfCounter].name,var);
    counters[numsOfCounter].nums = 1;
    numsOfCounter ++;
}
int cmp (const void *a , const void *b){
    return ((struct countOfVar *)b)->nums - ((struct countOfVar *)a) -> nums ;
}
void sortCounters(){
    qsort(counters,numsOfCounter,sizeof(struct countOfVar),cmp);
}

void loopOptimize(){
    int i = 0;
    for(i = 0 ; i < loopCount ; i ++ ){
        numsOfCounter = 0;
        int j = 0;
        int start = opblock[loopBlo[i].start].start;
        int end = opblock[loopBlo[i].ends].finish;
        for(j = start ; j <= end ; j ++){
            if((codes[j].op >= 5 && codes[j].op <= 18 && codes[j].op != 6) ){
                if(codes[j].op == BecomeOp && strcmp(codes[j].arg2,"0") != 0) continue;
                if(codes[j].op == GetArrayOp){
                    if(isVar(codes[j].arg2)) addVar(codes[j].arg2);
                    continue;
                }
                if(isVar(codes[j].arg1)) addVar(codes[j].arg1);
                if(isVar(codes[j].arg2)) addVar(codes[j].arg2);
                if(isVar(codes[j].result)) addVar(codes[j].result);
            }
        }

        sortCounters();

        int resultNum = numsOfCounter < 8 ? numsOfCounter : 8 ;
        loopBlo[i].regNum = resultNum ;
        for(j = 0 ; j < resultNum ; j++){
            strcpy(loopBlo[i].regs[j],counters[j].name);
            //printf(">>>>>>>>>>>>>>>%s %d\n",counters[j].name,counters[j].nums);
        }

    }

    opexpre();
}


//DAG图的优化


