#include "symbol.h"
#include "types.h"
#include "parse.h"
#include "error.h"
#include "util.h"

int tempregNum;
int labelNum;

enum typel expreType; //xpression 返回值类型，用于类型转换判断问题

enum typel retType; //返回语句的返回值类型

char printreg[32][1024];
int countprint;

char retfactor[32];
char retterm[32];
char retexpre[32];

char termarg1[32];
char termarg2[32];

char exprearg1[32];
char exprereg2[32];

char judgereg[32];

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
            emit(PushParmOp,"0","0",id);
        } else if(curSy == INTCON){
            if(params[i] != Int) error(16);
            emit(PushParmOp,"0","0",intToString(inum));
        } else if(curSy == CHARCON){
            if(params[i] != Char) error(16);
            emit(PushParmOp,"0","0",charToString(ichar));
        } else {
            error(15);
            return ;
        }//emit不用类型，函数参数都放在栈中或者寄存器之中
    }
}

//语法分析执行语句要检查相应的变量名是否存在
void factor(){
    //insymbol(); 解析因子之时，可以默认为已经读入相应的内容
    if(curSy == IDENTSY){
        int pos = loc(id);//IDENTSY可能为变量，可能为函数，也可能为数组
        if(pos == -1) strcpy(retfactor,"NULL");
        if(idtabs[pos].kind == Var){
            strcpy(retfactor,id);
        } else if(idtabs[pos].kind == Array){
            insymbol();
            if(curSy == LPARENT){
                insymbol();
                expression();
                emit(GetArrayOp,idtabs[pos].name,retexpre,numToReg(tempregNum));//此处要进行相应的处理
                strcpy(retfactor,numToReg(tempregNum++));
            }else error(13);
            insymbol();
            if(curSy != RPARENT) error(13);
        } else if(idtabs[pos].kind == Function && idtabs[pos].type != None){ //调用的函数必须要有相应的返回值
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
    } else if(curSy == INTCON){
        strcpy(retfactor,intToString(inum));
    } else if(curSy == CHARCON) {
        strcpy(retfactor,charToString(ichar));
        ///////////////////////////////
        expreType = Char;
        ///////////////////////////////强制类型转换expreType
    } else if(curSy == LPARENT){
        insymbol();
        expression();
        strcpy(retfactor,retexpre);//调用表达式相应代码
        insymbol();
        if(curSy == RPARENT){
            expreType = Int;
        } else error(11);
    } else error(11);
}

void term(){
    factor();
    insymbol();
    while(curSy == MULT || curSy == DIV){
        /////////////////////
        expreType = Int;//表达式返回类型
        /////////////////////
        int multFlag = curSy == MULT ? 1 : 0;
        expreType = Int; // 遇到乘号就进行相应的类型转换
        strcpy(termarg1,retfactor); // retfactor中始终应为最新的return寄存器值
        //////////////// 读取第二位因子
        factor();
        strcpy(termarg2,retfactor);
        ////////////////
        if(multFlag) emit(MultOp,termarg1,termarg2,numToReg(tempregNum));
        else emit(MultOp,termarg1,termarg2,numToReg(tempregNum));
        strcpy(retfactor,numToReg(tempregNum++));

        insymbol(); //term 预读取了下一位用于判断
    }
    strcpy(retterm,retfactor);
}


void expression(){
    //insymbol();
    expreType = Int;
    if(curSy == PLUS || curSy == SUB){ //表达式第一项内容可以为+或-
        /////////////////////
        expreType = Int;//表达式返回类型
        /////////////////////
        if(curSy == SUB){
            term(); //term 已经预读取了一位进行相应的判断
            emit(MultOp,"-1",retterm,numToReg(tempregNum));
            strcpy(retexpre,numToReg(tempregNum++));
        } //如果为-，则需要进行计算
        else{
            term();
            strcpy(retexpre,retterm);
        }
    }

    while(curSy == PLUS || curSy == SUB){
        /////////////////////
        expreType = Int;//表达式返回类型
        /////////////////////
        int addFlag = (curSy == PLUS) ? 1 : 0;
        insymbol();
        strcpy(exprearg1,retexpre);
        term(); //此处term已经预读取了下一位内容
        strcpy(exprereg2,retterm);
        if(addFlag) emit(AddOp,exprearg1,exprereg2,numToReg(tempregNum));
        else emit(SubOp,exprearg1,exprereg2,numToReg(tempregNum));

        strcpy(retexpre,numToReg(tempregNum++));//表达式返回值更新
    }
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

//语句 -> 赋值语句/函数调用语句（callparm，传入相应的pos）
//条件语句 if 开头
//assign 要进行类型检查
void assignstate(int pos){ //赋值语句预读入相应的标识符
    if(idtabs[pos].kind == Array){
        insymbol();
        if(curSy == LBRACKET){
            insymbol();
            expression(); //预读取了一位
            if(expreType != idtabs[pos].type) error(18);
            emit(BecomeOp,retexpre,"0",idtabs[pos].name); //分号丢到外面检查
        } else error(13);
    } else if(idtabs[pos].kind == Var) {
        insymbol();
        expression();
        if(expreType != idtabs[pos].type) error(18); //类型检查语句
        emit(BecomeOp,retexpre,"0",idtabs[pos].name);
    } else error(17);
}
//生成相应的label，并对label之前的标号位置进行反填
void genBackLabel(int codepos){
    int labelpos = labelNum;
    emit(LabelOp,"0","0",numToLabel(labelNum++));
    emitLabel(codepos,labelpos);
}

void judgestate(){
    insymbol();
    expression();
    strcpy(judgereg,retexpre);

    if(curSy == RPARENT){
        emit(NoequOp,judgereg,"0","0");
    } else if(curSy >= 17 && curSy <= 22){
        insymbol();
        expression();
        emit((enum ops)(curSy-5),judgereg,retexpre,"0");
        insymbol();
        if(curSy != RPARENT) error(19);
        else insymbol();
    } else error(19);
}

void ifstate(){
    insymbol();
    if(curSy == LPARENT){
        //////////////////////
        judgestate();//条件判断语句的插入
        //////////////////////
        int ifcodepos = codeCount; //用于标志的反填
        emit(FalseOp,"0","0","0");

        statement();

        insymbol();
        if(curSy == ELSESY){
            int elsecodepos = codeCount;
            emit(GotoOp,"0","0","0"); //  <--------------------------------
            genBackLabel(ifcodepos);  //                                  |
            statement();              //                                  |
            genBackLabel(elsecodepos);//  ---------------------------------
        } else {
            genBackLabel(ifcodepos);
        }

    } else error(19);
}

void dowhilestate(){
    //do,while语句label不用进行相应的反填
    int dowhilelabel = labelNum;
    emit(LabelOp,"0","0",numToLabel(labelNum++));
    statement();
    insymbol();
    if(curSy == WHILESY){
        insymbol();
        judgestate();
        emit(TrueOp,"0","0",numToLabel(dowhilelabel));
    } else error(20);
}

void forstate(){
    insymbol();
    if(curSy == IDENTSY){
        insymbol();
        if(curSy == BECOMESY){
            int pos = loc(id);
            if(pos == -1) error(12);
            insymbol();
            expression();
        }
    } else error(24);
}

void readstate(){
    insymbol();
    if(curSy == LPARENT){
        insymbol();
        while(curSy == IDENTSY){
            int pos = loc(id);
            if(pos == -1) error(12);
            if(idtabs[pos].kind != Var) error(22);
            insymbol();
            if(curSy == COMMA) insymbol();
        }
        if(curSy != RPARENT) error(21);
        else insymbol();
    } else error(21);
}

void writestate(){
    insymbol();
    if(curSy == LPARENT){
        insymbol();
        if(curSy == STRINGCON){
            strcpy(printreg[countprint],iString);
            emit(PrintfOp,"string","0",intToString(countprint++));
            insymbol();
            if(curSy != COMMA){
                insymbol();
                return ;
            }
        }

        expression(); //expression 的前置insymbol已经在前面了
        if(expreType == Int) emit(PrintfOp,"int","0",retexpre);
        else if(expreType == Char) emit(PrintfOp,"char","0",retexpre);

        if(curSy != RPARENT) error(23);
        else insymbol();
    }
}

void returnstate(){
    insymbol();
    if(curSy == LPARENT){
        insymbol();
        expression();
        retType = expreType;
        emit(RetOp,"0","0",retexpre);
    } else {
        emit(RetOp,"0","0","0");
    }
}

void statement(){

}

