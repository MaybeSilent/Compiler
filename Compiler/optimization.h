#ifndef OPTIMIZATION_H_INCLUDED
#define OPTIMIZATION_H_INCLUDED

#define OPTIMIZE 1

struct basicblock{
    int start ;  //���������
    int finish ; //�������յ�
    int nextCount   ;
    int next[8]    ; //��̻�����
};
struct basicblock opblock[8192];

struct loopStruct{
    int start;
    int ends;
    char regs[8][128]; //�����s�Ĵ����еı���
    int regNum;
    //���ü�����������ط���
};
struct loopStruct loopBlo[256];
int loopCount;

struct countOfVar{
    char name[32];
    int nums; //����һ��+1
};

struct countOfVar counters[256];
int numsOfCounter;

int bbCount;
void optimize();

void basicBlock();
void loopOptimize();
int IsInS();

#endif // OPTIMIZATION_H_INCLUDED
