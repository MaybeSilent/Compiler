#include "symbol.h"
#include "types.h"
#include "parse.h"
#include "error.h"
#include "util.h"

int tempregNum;
int labelNum;

enum typel expreType; //xpression 返回值类型，用于类型转换判断问题

enum typel retType; //返回语句的返回值类型

int retFlag;

int factorConst;
int factorValue;

int termConst;
int termValue;

char retfactor[32];
char retterm[32];
char retexpre[32];

char judgereg[32];

char paramName[32];

void constDec(){
    if(DEBUG) printf("进入consDec\n");
    if(grammer) printf("这是一条常量声明\n");
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

    if(grammer) printf("这是一条变量声明\n");

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

void funcInsert(int intFlag){
    if(grammer) printf("这是一条函数声明\n");
    /////////////////////////////
    enterblock();    //enterblock
    /////////////////////////////
    if(intFlag == 1) entertab(id,Function,Int,blockCount);//首先将function插入符号表之中
    else if(intFlag == 0) entertab(id,Function,Char,blockCount);
    else if(intFlag == -1) entertab(id,Function,None,blockCount);
    //////////////////////////////
    level = level + 1;          //
    display[level] = blockCount;//
    //////////////////////////////
    emit(FunctionOp,"0","0",id);
    paramList();
    insymbol();
    if(curSy == LBRACE) {
        retFlag = 0;
        comstate();               //复合语句
        if(retFlag != 1 && (intFlag == 1 || intFlag == 0)) error(33);
    }
    //////////////////////////函数返回值检查
    level = level - 1;
}

void program(){
    insymbol();

    while(curSy == CONSTSY){
        constDec();
        if(curSy != SEMICOLON) error(26);
        insymbol();
        while(!(curSy == INTSY || curSy == CHARSY || curSy == VOIDSY || curSy == CONSTSY)){
            error(31);
            insymbol();
        }
    }
    while(curSy == INTSY || curSy == CHARSY){
        int intFlag = (curSy == INTSY);
        insymbol();
        if(curSy == IDENTSY){
            insymbol();
            if(curSy == LPARENT){ // 函数声明
                funcInsert(intFlag);
                break;
            } else {
                varInsert(intFlag);
                while(curSy == IDENTSY){
                    insymbol();
                    varInsert(intFlag);
                }
                if(curSy != SEMICOLON) error(32); //这里出错啦
            }
        } else error(32);

        insymbol();
        while(!(curSy == INTSY || curSy == CHARSY || curSy == VOIDSY)){
            error(31);
            insymbol();
        }
    }
    while(curSy == INTSY || curSy == CHARSY || curSy == VOIDSY){
        int intFlag = (curSy == INTSY);
        if(curSy == VOIDSY) intFlag = -1;
        insymbol();
        if(curSy == IDENTSY){
            insymbol();
            if(curSy == LPARENT){ // 函数声明
                funcInsert(intFlag);
            } else error(32);
        } else if(curSy == MAINSY){
            emit(Function,"0","0","main");
            enterblock();
            //////////////////////////////
            level = level + 1;          //
            display[level] = blockCount;//
            //////////////////////////////
            insymbol();
            if(curSy == LPARENT){
                insymbol();
                if(curSy == RPARENT){
                    insymbol();
                    if(curSy == LBRACE){
                        comstate();
                        if(curSy != FINISHED) error(35);
                        break; //编译结束
                    } else error(34);
                } else error(34);
            } else error(34);
        } else error(32);


        while(!(curSy == INTSY || curSy == CHARSY || curSy == VOIDSY)){
                error(31);
                insymbol();
        }
    }

    printf("编译结束，(%d)错误\n",getErrorNum());

}

void comstate(){
    if(grammer) printf("处理到复合语句\n");

    insymbol();
    while(curSy == CONSTSY){
        constDec();
        if(curSy != SEMICOLON) error(26);
        else insymbol();
    }
    while(curSy == INTSY || curSy == CHARSY){
        varDec();
        if(curSy != SEMICOLON) error(26);
        else insymbol();
    }
    while(curSy != RBRACE){
        state();
    }
    insymbol(); // 读到右大括号后预读一位

}

void callparm(int pos){
    if(grammer) printf("这是函数调用声明\n");

    enum typel params[128];
    int blockIndex = idtabs[pos].value;
    int n = blocktabs[blockIndex].parmnum - 1;
    int tabIndex = blocktabs[blockIndex].lastparm;
    int i = n;
    for( ; i >= 0 ; i --){//先确定parmList的参数类型
        params[i] = idtabs[tabIndex].type;
        tabIndex = idtabs[tabIndex].link;
    }
    ///////////////////
    for(i = 0 ; i <= n ; i++){
        insymbol();
        expression();
        if(expreType == Char) emit(PushParmOp,"0","0",intToString(charToInt(retexpre)));
        else emit(PushParmOp,"0","0",retexpre);
        if(expreType == Int){
            if(params[i] != Int) error(16);
        } else if(expreType == Char){
            if(params[i] != Char) error(16);
        } else {
            error(15);
            return ;
        }
        if(curSy != COMMA && i != n) error(27);
        if(curSy != RPARENT && i == n) error(15);
        //emit不用类型，函数参数都放在栈中或者寄存器之中
    }
    if(curSy == LPARENT) insymbol(); //无参数函数调用
}

//语法分析执行语句要检查相应的变量名是否存在
void factor(){
    //insymbol(); 解析因子之时，可以默认为已经读入相应的内容
    if(grammer) printf("处理到表达式因子\n");
    int constsign = (curSy == INTCON)||(curSy==CHARCON);
    /////常数优化临时变量
    if(curSy == IDENTSY){
        int pos = loc(id);//IDENTSY可能为变量，可能为函数，也可能为数组
        if(pos == -1){
            error(12);
            return ;
        }
        if(idtabs[pos].kind == Var || idtabs[pos].kind == Const || idtabs[pos].kind == Parm){
            strcpy(retfactor,id);
            if(idtabs[pos].kind == Const){
                constsign = 1;
                factorValue = idtabs[pos].value;
            }
        } else if(idtabs[pos].kind == Array){
            insymbol();
            if(curSy == LBRACKET){
                insymbol();
                expression();
                emit(GetArrayOp,idtabs[pos].name,retexpre,numToReg(tempregNum));//此处要进行相应的处理
                strcpy(retfactor,numToReg(tempregNum++));
            }else error(13);
            if(curSy != RBRACKET) error(13);
        } else if(idtabs[pos].kind == Function && idtabs[pos].type != None){ //调用的函数必须要有相应的返回值
            insymbol();
            if(curSy == LPARENT){
                callparm(pos);
                emit(CallOp,idtabs[pos].name,"0",numToReg(tempregNum));
                strcpy(retfactor,numToReg(tempregNum++));
                if(curSy != RPARENT) error(14);
            } else error(14);
        } else error(11);
        //////////////////////////////
        if(idtabs[pos].type == Char && expreType == None) expreType = Char;
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
        factorValue = inum;
        strcpy(retfactor,intToString(inum));
    } else if(curSy == CHARCON) {
        factorValue = (int)ichar;
        strcpy(retfactor,charToString(ichar));
        ///////////////////////////////
        if(expreType == None) expreType = Char;
        ///////////////////////////////强制类型转换expreType
    } else if(curSy == LPARENT){
        insymbol();
        expression();
        strcpy(retfactor,retexpre);//调用表达式相应代码
        if(curSy == RPARENT){
            expreType = Int;
        } else error(11);
    } else error(11);
    factorConst = constsign;
}

void term(){
    if(grammer) printf("处理到表达式项\n");
    char termarg1[32] = {0},termarg2[32] = {0};

    int termsign = 0;
    factor();
    if(factorConst){
        termsign = 1;
        termValue = factorValue;
    }

    strcpy(retterm,retfactor);
    insymbol();
    while(curSy == MULT || curSy == DIV){
        /////////////////////
        expreType = Int;//表达式返回类型
        /////////////////////
        int multFlag = curSy == MULT ? 1 : 0;
        strcpy(termarg1,retterm); // retfactor中始终应为最新的return寄存器值
        //////////////// 读取第二位因子
        insymbol();
        factor();
        if(!factorConst) termsign = 0;

        if(termsign){
            if(multFlag) termValue *= factorValue;
            else termValue /= factorValue;
            strcpy(retterm,intToString(termValue));
        } else {
            strcpy(termarg2,retfactor);
            if(multFlag) emit(MultOp,termarg1,termarg2,numToReg(tempregNum));
            else emit(DivOp,termarg1,termarg2,numToReg(tempregNum));
            strcpy(retterm,numToReg(tempregNum++));
        }

        insymbol(); //term 预读取了下一位用于判断
    }
    termConst = termsign;
}


void expression(){
    char exprearg1[32] = {0},exprereg2[32]={0};
    if(grammer) printf("处理到表达式\n");

    expreType = None;
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
    } else {
        term();
        strcpy(retexpre,retterm);
    }
    int constsign = termConst;
    int expreValue = termValue;
    while(curSy == PLUS || curSy == SUB){
        /////////////////////
        expreType = Int;//表达式返回类型
        /////////////////////
        int addFlag = (curSy == PLUS) ? 1 : 0;
        insymbol();
        strcpy(exprearg1,retexpre);
        term(); //此处term已经预读取了下一位内容
        if(!termConst) constsign = 0;

        if(constsign){
            if(addFlag) expreValue += termValue;
            else expreValue -= termValue;
            strcpy(retexpre,intToString(expreValue));
        } else {
            strcpy(exprereg2,retterm);
            if(addFlag) emit(AddOp,exprearg1,exprereg2,numToReg(tempregNum));
            else emit(SubOp,exprearg1,exprereg2,numToReg(tempregNum));

            strcpy(retexpre,numToReg(tempregNum++));//表达式返回值更新
        }

    }

    if(expreType == None) expreType = Int;
}


void synanalysis(){
    codeCount = 0;
    tabCount = 0;
}

//语句 -> 赋值语句/函数调用语句（callparm，传入相应的pos）
//条件语句 if 开头
//assign 要进行类型检查
void assignstate(int pos){ //赋值语句预读入相应的标识符

    if(grammer) printf("处理到赋值语句\n");

    if(idtabs[pos].kind == Array){
        char indexreg[32];
        insymbol();
        if(curSy == LBRACKET){
            insymbol();
            expression(); //预读取了一位
            strcpy(indexreg,retexpre);

            if(curSy != RBRACKET) error(13);
            else insymbol();
            if(curSy != BECOMESY) error(17);
            else insymbol();
            expression();
            if(expreType != idtabs[pos].type) error(18);

            emit(BecomeOp,retexpre,indexreg,idtabs[pos].name); //分号丢到外面检查
        } else error(13);
    } else if(idtabs[pos].kind == Var) {
        insymbol();
        if(curSy != BECOMESY) error(17);
        else insymbol();
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

    if(grammer) printf("处理到条件语句\n");

    insymbol();
    expression();
    strcpy(judgereg,retexpre);

    if(curSy == RPARENT){
        emit(NoequOp,judgereg,"0","0");
    } else if(curSy >= 17 && curSy <= 22){
        enum ops ComOp = (enum ops)(curSy-5);
        insymbol();
        expression();
        emit(ComOp,judgereg,retexpre,"0");
        if(!(curSy == RPARENT || curSy == SEMICOLON)) error(19);
    } else error(19);
}

void ifstate(){

    if(grammer) printf("处理到IF语句\n");

    insymbol();
    if(curSy == LPARENT){
        //////////////////////
        judgestate();//条件判断语句的插入
        //////////////////////
        int ifcodepos = codeCount; //用于标志的反填
        emit(FalseOp,"0","0","0");

        statement();

        //insymbol();
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
    if(grammer) printf("处理到dowhile语句\n");

    int dowhilelabel = labelNum;
    emit(LabelOp,"0","0",numToLabel(labelNum++));
    statement();
    //insymbol();
    if(curSy == WHILESY){
        insymbol();
        if(curSy != LPARENT) error(20);
        judgestate();
        emit(TrueOp,"0","0",numToLabel(dowhilelabel));
    } else error(20);
}

void forstate(){
    if(grammer) printf("处理到for语句\n");

    int beforepos, afterpos, addFlag, step;
    insymbol();
    if(curSy == LPARENT){
        insymbol();
        if(curSy == IDENTSY){
            insymbol();
            if(curSy == BECOMESY){
                int pos = loc(id);
                if(pos == -1){
                    error(12);
                    return ;
                }
                insymbol();
                expression();
                emit(BecomeOp,retexpre,"0",idtabs[pos].name);
                //insymbol(); expression已经有相应的预读取
                if(curSy == SEMICOLON){
                    int forjudgeLabel = labelNum;
                    emit(LabelOp,"0","0",numToLabel(labelNum++));
                    judgestate();
                    int falseforLabel = codeCount;
                    emit(FalseOp,"0","0","0");
                    if(curSy == SEMICOLON){
                        insymbol();
                        if(curSy == IDENTSY){
                            beforepos = loc(id);
                            if(beforepos == -1) error(12);
                            insymbol();
                            if(curSy == BECOMESY){
                                insymbol();
                                if(curSy == IDENTSY){
                                    afterpos = loc(id);
                                    if(afterpos == -1) error(12);
                                    insymbol();
                                    if(curSy == PLUS || curSy == SUB){
                                        addFlag = (curSy == PLUS);
                                        insymbol();
                                        if(curSy == INTCON){
                                            step = inum;
                                            insymbol();
                                            if(curSy == RPARENT){
                                                statement();

                                                if(addFlag) emit(AddOp,idtabs[afterpos].name,intToString(step),idtabs[beforepos].name);
                                                else emit(SubOp,idtabs[afterpos].name,intToString(step),idtabs[beforepos].name);
                                                emit(GotoOp,"0","0",numToLabel(forjudgeLabel));
                                                genBackLabel(falseforLabel);
                                            } else error(24);
                                        }else error(24);
                                    } else error(24);
                                } else error(24);
                            } else error(24);
                        }
                    } else error(24);

                } else error(24);
            }
        } else error(24);
    } else error(24);
}

void readstate(){
    if(grammer) printf("处理到读语句\n");

    insymbol();
    if(curSy == LPARENT){
        insymbol();
        while(curSy == IDENTSY){
            int pos = loc(id);
            if(pos == -1){
                error(12);
                return ;
            }
            if(!(idtabs[pos].kind == Var||idtabs[pos].kind == Parm)) error(22);
            if(idtabs[pos].type == Int) emit(ScanfOp,"int","0",idtabs[pos].name);
            else if(idtabs[pos].type == Char) emit(ScanfOp,"char","0",idtabs[pos].name);
            insymbol();
            if(curSy == COMMA) insymbol();
        }
        if(curSy != RPARENT) error(21);
        else insymbol();
    } else error(21);
}


void writestate(){

    if(grammer) printf("处理到写语句\n");

    insymbol();
    if(curSy == LPARENT){
        insymbol();
        if(curSy == STRINGCON){
            int index = insertString(iString);
            emit(PrintfOp,"string","0",getStringCon(index));
            insymbol();
        }
        if(curSy == RPARENT){
            insymbol();
            return ;
        } else if(curSy == COMMA){
            insymbol();
        }

        expression(); //expression 的前置insymbol已经在前面了
        if(expreType == Int) emit(PrintfOp,"int","0",retexpre);
        else if(expreType == Char) emit(PrintfOp,"char","0",retexpre);

        if(curSy != RPARENT) error(23);
        else insymbol();
    }
}

void returnstate(){

    if(grammer) printf("处理到返回语句\n");
    retFlag = 1;
    insymbol();
    if(curSy == LPARENT){
        insymbol();
        expression();
        retType = expreType;
        emit(RetOp,"0","0",retexpre);
        if(curSy != RPARENT) error(29);
        else insymbol();
    } else {
        emit(RetOp,"0","0","0");
    }
}

void checkSem(){
    if(curSy != SEMICOLON) error(26);
    else insymbol();
}


void state(){
    if(curSy == IFSY){
        ifstate();
    } else if(curSy == DOSY){
        dowhilestate();
        insymbol();
    } else if(curSy == FORSY){
        forstate();
    } else if(curSy == LBRACE){
        statement();
        //insymbol();
        while(curSy != RBRACE){
            state();
            //insymbol();
        }
        insymbol();
    } else if(curSy == IDENTSY){
        int pos = loc(id);
        if(pos == -1){
            errorjump(12);
            return ;
        }
        if(idtabs[pos].kind == Var || idtabs[pos].kind == Array){
            assignstate(pos);
            checkSem();
        } else if(idtabs[pos].kind == Function) {
            insymbol();
            if(curSy != LPARENT) error(14);
            callparm(pos);
            emit(CallOp,idtabs[pos].name,"0","0");
            insymbol();
            checkSem();
        } else error(25);
    } else if(curSy == INPUTSY){
        readstate();
        checkSem();
    } else if(curSy == OUTPUTSY){
        writestate();
        checkSem();
    } else if(curSy == RETURNSY){
        retFlag = 1;
        returnstate();
        checkSem();
    } else if(curSy == SEMICOLON){
        //空语句
    } else {
        error(28);
        //直接跳转到下一句
        while(curSy != SEMICOLON) insymbol();
        insymbol();
    }
}

void statement(){
    insymbol();
    state();
}

