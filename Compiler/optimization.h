#ifndef OPTIMIZATION_H_INCLUDED
#define OPTIMIZATION_H_INCLUDED

#define OPTIMIZE 1

struct basicblock{
    int start ;  //基本块起点
    int finish ; //基本块终点
    int nextCount   ;
    int next[8]    ; //后继基本快
};
struct basicblock opblock[8192];

struct loopStruct{
    int start;
    int ends;
    char regs[8][128]; //存放在s寄存器中的变量
    int regNum;
    //利用计数法进行相关分配
};
struct loopStruct loopBlo[256];
int loopCount;

struct countOfVar{
    char name[32];
    int nums; //出现一次+1
};

struct countOfVar counters[256];
int numsOfCounter;

int bbCount;
void optimize();

void basicBlock();
void loopOptimize();
int IsInS();

#endif // OPTIMIZATION_H_INCLUDED
