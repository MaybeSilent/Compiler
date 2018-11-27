#include "symbol.h"
#include "types.h"
#include "parse.h"
#include "error.h"
#include "util.h"

int tempregNum;
int labelNum;

enum typel expreType; //xpression ����ֵ���ͣ���������ת���ж�����

enum typel retType; //�������ķ���ֵ����

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
    if(DEBUG) printf("����consDec\n");
    if(grammer) printf("����һ����������\n");
    insymbol();

    if(curSy == INTSY || curSy == CHARSY){
        int intFlag = 0;
        if(curSy == INTSY) intFlag = 1;
        insymbol();
        //���뵽��Ӧ�ķ��ű�֮��
        while(curSy == IDENTSY){
            insymbol();
            if(curSy == BECOMESY){
                insymbol();
                if((curSy == INTCON||curSy == SUB||curSy == PLUS) && intFlag){
                    //����Ӧ���ݲ�����ű�
                    if(curSy == PLUS || curSy == SUB){
                        int sign = 1;
                        if(curSy == SUB) sign = -1 ;
                        insymbol();
                        inum = sign*inum;
                    }
                    if(curSy != INTCON) error(3);
                    entertab(id,Const,Int,inum); //�����������ݼ��뵽���ű���
                    emit(ConstOp,"int",intToString(inum),id);//������Ӧ�����嶯��
                    insymbol();
                } else if(curSy == CHARCON && (!intFlag)){
                    entertab(id,Const,Char,ichar);
                    emit(ConstOp,"char",charToString(ichar),id);
                    insymbol();
                } else error(3);
            } else error(2);

            if(curSy == COMMA) insymbol() ;
            else return ; //�ֺ�
        }
    }

    if(DEBUG) printf("�˳�consDec\n");
}

void varInsert(int intFlag){

    if(grammer) printf("����һ����������\n");

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
            emit(VarOp,"int","0",id);//����һ�������ı���������ű�֮��
        } else{
            entertab(id,Var,Char,0);
            emit(VarOp,"char","0",id);
        }
    }


    if(curSy == COMMA){
        insymbol();
    } else return ; //�������ֱ��return �������ú�����������Ϊ�ֺţ��������൱�ڽ��쳣throw��ȥ
}

void varDec(){
    if(DEBUG) printf("����varDec\n");

    if(curSy == INTSY || curSy == CHARSY){
        int intFlag = 0;
        if(curSy == INTSY) intFlag = 1;

        insymbol();

        while(curSy == IDENTSY){
            insymbol();
            varInsert(intFlag);
        }
    } else error(5);

    if(DEBUG) printf("�˳�varDec\n");
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
    blocktabs[blockCount].parmnum = count; //����������д
    ////////////////////////
    if(curSy != RPARENT) error(8);
}

void funcInsert(int intFlag){
    if(grammer) printf("����һ����������\n");
    /////////////////////////////
    enterblock();    //enterblock
    /////////////////////////////
    if(intFlag == 1) entertab(id,Function,Int,blockCount);//���Ƚ�function������ű�֮��
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
        comstate();               //�������
        if(retFlag != 1 && (intFlag == 1 || intFlag == 0)) error(33);
    }
    //////////////////////////��������ֵ���
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
            if(curSy == LPARENT){ // ��������
                funcInsert(intFlag);
                break;
            } else {
                varInsert(intFlag);
                while(curSy == IDENTSY){
                    insymbol();
                    varInsert(intFlag);
                }
                if(curSy != SEMICOLON) error(32); //���������
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
            if(curSy == LPARENT){ // ��������
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
                        break; //�������
                    } else error(34);
                } else error(34);
            } else error(34);
        } else error(32);


        while(!(curSy == INTSY || curSy == CHARSY || curSy == VOIDSY)){
                error(31);
                insymbol();
        }
    }

    printf("���������(%d)����\n",getErrorNum());

}

void comstate(){
    if(grammer) printf("�����������\n");

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
    insymbol(); // �����Ҵ����ź�Ԥ��һλ

}

void callparm(int pos){
    if(grammer) printf("���Ǻ�����������\n");

    enum typel params[128];
    int blockIndex = idtabs[pos].value;
    int n = blocktabs[blockIndex].parmnum - 1;
    int tabIndex = blocktabs[blockIndex].lastparm;
    int i = n;
    for( ; i >= 0 ; i --){//��ȷ��parmList�Ĳ�������
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
        //emit�������ͣ���������������ջ�л��߼Ĵ���֮��
    }
    if(curSy == LPARENT) insymbol(); //�޲�����������
}

//�﷨����ִ�����Ҫ�����Ӧ�ı������Ƿ����
void factor(){
    //insymbol(); ��������֮ʱ������Ĭ��Ϊ�Ѿ�������Ӧ������
    if(grammer) printf("�������ʽ����\n");
    int constsign = (curSy == INTCON)||(curSy==CHARCON);
    /////�����Ż���ʱ����
    if(curSy == IDENTSY){
        int pos = loc(id);//IDENTSY����Ϊ����������Ϊ������Ҳ����Ϊ����
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
                emit(GetArrayOp,idtabs[pos].name,retexpre,numToReg(tempregNum));//�˴�Ҫ������Ӧ�Ĵ���
                strcpy(retfactor,numToReg(tempregNum++));
            }else error(13);
            if(curSy != RBRACKET) error(13);
        } else if(idtabs[pos].kind == Function && idtabs[pos].type != None){ //���õĺ�������Ҫ����Ӧ�ķ���ֵ
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
        //////////////////////////////ǿ������ת��expreType
    } else if(curSy == PLUS || curSy == SUB){
        int sign = 1;
        if(curSy == SUB) sign = -1;
        insymbol(); //��ȡ��Ӧ������
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
        ///////////////////////////////ǿ������ת��expreType
    } else if(curSy == LPARENT){
        insymbol();
        expression();
        strcpy(retfactor,retexpre);//���ñ��ʽ��Ӧ����
        if(curSy == RPARENT){
            expreType = Int;
        } else error(11);
    } else error(11);
    factorConst = constsign;
}

void term(){
    if(grammer) printf("�������ʽ��\n");
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
        expreType = Int;//���ʽ��������
        /////////////////////
        int multFlag = curSy == MULT ? 1 : 0;
        strcpy(termarg1,retterm); // retfactor��ʼ��ӦΪ���µ�return�Ĵ���ֵ
        //////////////// ��ȡ�ڶ�λ����
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

        insymbol(); //term Ԥ��ȡ����һλ�����ж�
    }
    termConst = termsign;
}


void expression(){
    char exprearg1[32] = {0},exprereg2[32]={0};
    if(grammer) printf("�������ʽ\n");

    expreType = None;
    if(curSy == PLUS || curSy == SUB){ //���ʽ��һ�����ݿ���Ϊ+��-
        /////////////////////
        expreType = Int;//���ʽ��������
        /////////////////////
        if(curSy == SUB){
            term(); //term �Ѿ�Ԥ��ȡ��һλ������Ӧ���ж�
            emit(MultOp,"-1",retterm,numToReg(tempregNum));
            strcpy(retexpre,numToReg(tempregNum++));
        } //���Ϊ-������Ҫ���м���
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
        expreType = Int;//���ʽ��������
        /////////////////////
        int addFlag = (curSy == PLUS) ? 1 : 0;
        insymbol();
        strcpy(exprearg1,retexpre);
        term(); //�˴�term�Ѿ�Ԥ��ȡ����һλ����
        if(!termConst) constsign = 0;

        if(constsign){
            if(addFlag) expreValue += termValue;
            else expreValue -= termValue;
            strcpy(retexpre,intToString(expreValue));
        } else {
            strcpy(exprereg2,retterm);
            if(addFlag) emit(AddOp,exprearg1,exprereg2,numToReg(tempregNum));
            else emit(SubOp,exprearg1,exprereg2,numToReg(tempregNum));

            strcpy(retexpre,numToReg(tempregNum++));//���ʽ����ֵ����
        }

    }

    if(expreType == None) expreType = Int;
}


void synanalysis(){
    codeCount = 0;
    tabCount = 0;
}

//��� -> ��ֵ���/����������䣨callparm��������Ӧ��pos��
//������� if ��ͷ
//assign Ҫ�������ͼ��
void assignstate(int pos){ //��ֵ���Ԥ������Ӧ�ı�ʶ��

    if(grammer) printf("������ֵ���\n");

    if(idtabs[pos].kind == Array){
        char indexreg[32];
        insymbol();
        if(curSy == LBRACKET){
            insymbol();
            expression(); //Ԥ��ȡ��һλ
            strcpy(indexreg,retexpre);

            if(curSy != RBRACKET) error(13);
            else insymbol();
            if(curSy != BECOMESY) error(17);
            else insymbol();
            expression();
            if(expreType != idtabs[pos].type) error(18);

            emit(BecomeOp,retexpre,indexreg,idtabs[pos].name); //�ֺŶ���������
        } else error(13);
    } else if(idtabs[pos].kind == Var) {
        insymbol();
        if(curSy != BECOMESY) error(17);
        else insymbol();
        expression();
        if(expreType != idtabs[pos].type) error(18); //���ͼ�����
        emit(BecomeOp,retexpre,"0",idtabs[pos].name);
    } else error(17);

}
//������Ӧ��label������label֮ǰ�ı��λ�ý��з���
void genBackLabel(int codepos){
    int labelpos = labelNum;
    emit(LabelOp,"0","0",numToLabel(labelNum++));
    emitLabel(codepos,labelpos);
}

void judgestate(){

    if(grammer) printf("�����������\n");

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

    if(grammer) printf("����IF���\n");

    insymbol();
    if(curSy == LPARENT){
        //////////////////////
        judgestate();//�����ж����Ĳ���
        //////////////////////
        int ifcodepos = codeCount; //���ڱ�־�ķ���
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
    //do,while���label���ý�����Ӧ�ķ���
    if(grammer) printf("����dowhile���\n");

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
    if(grammer) printf("����for���\n");

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
                //insymbol(); expression�Ѿ�����Ӧ��Ԥ��ȡ
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
    if(grammer) printf("���������\n");

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

    if(grammer) printf("����д���\n");

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

        expression(); //expression ��ǰ��insymbol�Ѿ���ǰ����
        if(expreType == Int) emit(PrintfOp,"int","0",retexpre);
        else if(expreType == Char) emit(PrintfOp,"char","0",retexpre);

        if(curSy != RPARENT) error(23);
        else insymbol();
    }
}

void returnstate(){

    if(grammer) printf("�����������\n");
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
        //�����
    } else {
        error(28);
        //ֱ����ת����һ��
        while(curSy != SEMICOLON) insymbol();
        insymbol();
    }
}

void statement(){
    insymbol();
    state();
}

