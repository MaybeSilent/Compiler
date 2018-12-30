#include "types.h"
#include "optimization.h"

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
                    if((codes[opblock[i].finish].op == GotoOp || j <= i) && (loopCount == 0 || j > loopBlo[loopCount].ends)){
                        loopBlo[loopCount].start = j;
                        loopBlo[loopCount].ends = i;
                        loopCount ++ ;
                    }
                    break;
                }
            }
        }
        if(codes[opblock[i].finish + 1].op != FunctionOp && i + 1 < InCount) opblock[i].next[opblock[i].nextCount++] = i + 1;
    }

}
