#include "symbol.h"
#include "types.h"
#include "parse.h"
#include "error.h"
#include "util.h"

int tempregNum;

enum typel expreType = Int; //xpression 返回值类型，用于类型转换判断问题

char retfactor[32];
char retterm[32];
char retexpre[32];

char paramName[32];


void constDec(){
    if(DEBUG) printf("进入consDec\n");

    insymbol();
    if(curSy == INTSY || curSy == CHARSY){
        int intFlag = 0;
        if(curSy == INTSY) intFlag = 1;
        insymbol();
        //插入到相应的符号表之中
        while(curSy == IDENTSY){
            insymbol();
            if(curSy == BECOMESY){
                insymbol();
                if((curSy == INTCON||curSy == SUB||curSy == PLUS) && intFlag){
                    //将相应内容插入符号表
                    if(curSy == PLUS || curSy == SUB){
                        int sign = 1;
                        if(curSy == SUB) sign = -1 ;
                        insymbol();
                        inum = sign*inum;
                    }
                    if(curSy != INTCON) error(3);
                    entertab(id,Const,Int,inum); //将读到的内容加入到符号表中
                    emit(ConstOp,"int",intToString(inum),id);//加入相应的语义动作
                    insymbol();
                } else if(curSy == CHARCON && (!intFlag)){
                    entertab(id,Const,Char,ichar);
                    emit(ConstOp,"char",charToString(ichar),id);
                    insymbol();
                } else error(3);
            } else error(2);

            if(curSy == COMMA) insymbol() ;
            else return ; //分号
        }
    }

    if(DEBUG) printf("退出consDec\n");
}

void varInsert(int intFlag){
    if(curSy == LBRACKET){
        insymbol();
        if(curSy == INTCON && inum != 0){
            if(intFlag){
                entertab(id,Array,Int,inum);
                emit(ArrayOp,"int",intToString(inum),id);
            } else {
                entertab(id,Array,Char,inum);
                emit(ArrayOp,"char",intToString(inum),id);
            }
        }else error(6);
        insymbol();
        if(curSy == RBRACKET) insymbol();
        else error(7);
    } else {
        if(intFlag){
            entertab(id,Var,Int,0);
            emit(VarOp,"int","0",id);//将上一个读到的变量插入符号表之中
        } else{
            entertab(id,Var,Char,0);
            emit(VarOp,"char","0",id);
        }
    }


    if(curSy == COMMA){
        insymbol();
    } else return ; //其余符号直接return 到外层调用函数处理，正常为分号，不正常相当于将异常throw出去
}

void varDec(){
    if(DEBUG) printf("进入varDec\n");

    if(curSy == INTSY || curSy == CHARSY){
        int intFlag = 0;
        if(curSy == INTSY) intFlag = 1;

        insymbol();

        while(curSy == IDENTSY){
            insymbol();
            varInsert(intFlag);
        }
    } else error(5);

    if(DEBUG) printf("退出varDec\n");
}

void paramList(){
    insymbol();
    int count = 0;
    while(curSy == INTSY || curSy == CHARSY){
        count ++;
        int intFlag = 0;
        if(curSy == INTSY) intFlag = 1;
        insymbol();
        if(curSy == IDENTSY){
            if(intFlag){
                entertab(id,Parm,Int,0);
                emit(ParmOp,"int","0",id);
            } else {
                entertab(id,Parm,Char,0);
                emit(ParmOp,"char","0",id);
            }
            insymbol();
            if(curSy == COMMA) insymbol();
        } else error(9);
    }
    ////////////////////////
    blocktabs[blockCount].parmnum = count; //参数数量填写
    ////////////////////////
    if(curSy != RPARENT) error(8);
}

String expression(){

    return retexpre;
}

void callparm(int pos){
    enum typel params[128];
    int blockIndex = idtabs[pos].value;
    int n = blocktabs[blockIndex].parmnum - 1;
    int tabIndex = blocktabs[blockIndex].lastparm;
    for( ; n >= 0 ; n --){//先确定parmList的参数类型
        params[n] = idtabs[tabIndex].type;
        tabIndex = idtabs[tabIndex].link;
    }
    ///////////////////
    int i = 0;
    for(i = 0 ; i < n ; i++){
        insymbol();
        if(curSy == IDENTSY){
            int idpos = loc(id);
            if(idpos == -1) continue ;
            if(idtabs[idpos].type != params[i]) error(16);
            emit(PushParm,"0","0",id);
        } else if(curSy == INTCON){
            if(params[i] != Int) error(16);
            emit(PushParm,"0","0",intToString(inum));
        } else if(curSy == CHARCON){
            if(params[i] != Char) error(16);
            emit(PushParm,"0","0",charToString(ichar));
        } else {
            error(15);
            return ;
        }//emit不用类型，函数参数都放在栈中或者寄存器之中
    }
}

//语法分析执行语句要检查相应的变量名是否存在
void factor(){
    insymbol();
    if(curSy == IDENTSY){
        int pos = loc(id);//IDENTSY可能为变量，可能为函数，也可能为数组
        if(pos == -1) strcpy(retfactor,"NULL");
        if(idtabs[pos].kind == Var){
            strcpy(retfactor,id);
        } else if(idtabs[pos].kind == Array){
            insymbol();
            if(curSy == LPARENT){
                strcpy(retfactor,expression());
            }else error(13);
        } else if(idtabs[pos].kind == Function){
            insymbol();
            if(curSy == LPARENT){
                callparm(pos);
                insymbol();
                if(curSy != RPARENT) error(14);
            } else error(14);
        } else error(11);
        //////////////////////////////
        if(idtabs[pos].type == Char) expreType = Char;
        //////////////////////////////强制类型转换expreType
    } else if(curSy == PLUS || curSy == SUB){
        int sign = 1;
        if(curSy == SUB) sign = -1;
        insymbol(); //读取相应的整数
        if(curSy != INTCON) {
            inum = inum * sign;
            strcpy(retfactor,intToString(inum));
        } else error(10);
    } else if(curSy == CHARSY) {
        strcpy(retfactor,charToString(ichar));
        ///////////////////////////////
        expreType = Char;
        ///////////////////////////////强制类型转换expreType
    } else if(curSy == LPARENT){
        strcpy(retfactor,expression());//调用表达式相应代码
    } else error(11);
}

void term(){
    factor();
    insymbol();
}

void funcInsert(int intFlag){
    paramList();
    insymbol();
    if(curSy == '{'){
        //复合语句
    }
    //////////////////
    //记得None返回值的赋值操作
}


void synanalysis(){
    codeCount = 0;
    tabCount = 0;
}
