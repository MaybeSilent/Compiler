#include "symbol.h"
#include "types.h"
#include "parse.h"
#include "error.h"
#include "util.h"

int tempregNum;
int labelNum;

enum typel expreType; //xpression ����ֵ���ͣ���������ת���ж�����

enum typel retType; //�������ķ���ֵ����

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
    if(DEBUG) printf("����consDec\n");

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


void callparm(int pos){
    enum typel params[128];
    int blockIndex = idtabs[pos].value;
    int n = blocktabs[blockIndex].parmnum - 1;
    int tabIndex = blocktabs[blockIndex].lastparm;
    for( ; n >= 0 ; n --){//��ȷ��parmList�Ĳ�������
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
        }//emit�������ͣ���������������ջ�л��߼Ĵ���֮��
    }
}

//�﷨����ִ�����Ҫ�����Ӧ�ı������Ƿ����
void factor(){
    //insymbol(); ��������֮ʱ������Ĭ��Ϊ�Ѿ�������Ӧ������
    if(curSy == IDENTSY){
        int pos = loc(id);//IDENTSY����Ϊ����������Ϊ������Ҳ����Ϊ����
        if(pos == -1) strcpy(retfactor,"NULL");
        if(idtabs[pos].kind == Var){
            strcpy(retfactor,id);
        } else if(idtabs[pos].kind == Array){
            insymbol();
            if(curSy == LPARENT){
                insymbol();
                expression();
                emit(GetArrayOp,idtabs[pos].name,retexpre,numToReg(tempregNum));//�˴�Ҫ������Ӧ�Ĵ���
                strcpy(retfactor,numToReg(tempregNum++));
            }else error(13);
            insymbol();
            if(curSy != RPARENT) error(13);
        } else if(idtabs[pos].kind == Function && idtabs[pos].type != None){ //���õĺ�������Ҫ����Ӧ�ķ���ֵ
            insymbol();
            if(curSy == LPARENT){
                callparm(pos);
                insymbol();
                if(curSy != RPARENT) error(14);
            } else error(14);
        } else error(11);
        //////////////////////////////
        if(idtabs[pos].type == Char) expreType = Char;
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
        strcpy(retfactor,intToString(inum));
    } else if(curSy == CHARCON) {
        strcpy(retfactor,charToString(ichar));
        ///////////////////////////////
        expreType = Char;
        ///////////////////////////////ǿ������ת��expreType
    } else if(curSy == LPARENT){
        insymbol();
        expression();
        strcpy(retfactor,retexpre);//���ñ��ʽ��Ӧ����
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
        expreType = Int;//���ʽ��������
        /////////////////////
        int multFlag = curSy == MULT ? 1 : 0;
        expreType = Int; // �����˺žͽ�����Ӧ������ת��
        strcpy(termarg1,retfactor); // retfactor��ʼ��ӦΪ���µ�return�Ĵ���ֵ
        //////////////// ��ȡ�ڶ�λ����
        factor();
        strcpy(termarg2,retfactor);
        ////////////////
        if(multFlag) emit(MultOp,termarg1,termarg2,numToReg(tempregNum));
        else emit(MultOp,termarg1,termarg2,numToReg(tempregNum));
        strcpy(retfactor,numToReg(tempregNum++));

        insymbol(); //term Ԥ��ȡ����һλ�����ж�
    }
    strcpy(retterm,retfactor);
}


void expression(){
    //insymbol();
    expreType = Int;
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
    }

    while(curSy == PLUS || curSy == SUB){
        /////////////////////
        expreType = Int;//���ʽ��������
        /////////////////////
        int addFlag = (curSy == PLUS) ? 1 : 0;
        insymbol();
        strcpy(exprearg1,retexpre);
        term(); //�˴�term�Ѿ�Ԥ��ȡ����һλ����
        strcpy(exprereg2,retterm);
        if(addFlag) emit(AddOp,exprearg1,exprereg2,numToReg(tempregNum));
        else emit(SubOp,exprearg1,exprereg2,numToReg(tempregNum));

        strcpy(retexpre,numToReg(tempregNum++));//���ʽ����ֵ����
    }
}

void funcInsert(int intFlag){
    paramList();
    insymbol();
    if(curSy == '{'){
        //�������
    }
    //////////////////
    //�ǵ�None����ֵ�ĸ�ֵ����
}


void synanalysis(){
    codeCount = 0;
    tabCount = 0;
}

//��� -> ��ֵ���/����������䣨callparm��������Ӧ��pos��
//������� if ��ͷ
//assign Ҫ�������ͼ��
void assignstate(int pos){ //��ֵ���Ԥ������Ӧ�ı�ʶ��
    if(idtabs[pos].kind == Array){
        insymbol();
        if(curSy == LBRACKET){
            insymbol();
            expression(); //Ԥ��ȡ��һλ
            if(expreType != idtabs[pos].type) error(18);
            emit(BecomeOp,retexpre,"0",idtabs[pos].name); //�ֺŶ���������
        } else error(13);
    } else if(idtabs[pos].kind == Var) {
        insymbol();
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
        judgestate();//�����ж����Ĳ���
        //////////////////////
        int ifcodepos = codeCount; //���ڱ�־�ķ���
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
    //do,while���label���ý�����Ӧ�ķ���
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

        expression(); //expression ��ǰ��insymbol�Ѿ���ǰ����
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

