#ifndef OPTIMIZATION_H_INCLUDED
#define OPTIMIZATION_H_INCLUDED

#define OPTIMIZE 0

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
};
struct loopStruct loopBlo[256];
int loopCount;

char inblock[1024][64];
char outblock[1024][64];
char use[1024][64];
char def[1024][64];

int functionIndex;

int bbCount;
void optimize();

void basicBlock();

#endif // OPTIMIZATION_H_INCLUDED
