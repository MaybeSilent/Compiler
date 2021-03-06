#ifndef OPTIMIZATION_H_INCLUDED
#define OPTIMIZATION_H_INCLUDED


int OPTIMIZE;

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


//DAG图相关结构
enum DagOp{
    ADDTWO , MULTTWO , DIVTWO , SUBTWO ,
    NUMOP
};
struct Node{
    struct Node* parent[32];
    enum DagOp op;
    char varname[128][32];
};

struct Node nodeList[256];

struct expre{
    int start ;
    int ends;
};

struct expre expres[256];
int expreNum;

void findexpres();


#endif // OPTIMIZATION_H_INCLUDED
