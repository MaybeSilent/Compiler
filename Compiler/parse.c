#include "symbol.h"
#include "types.h"
#include "parse.h"
#include "error.h"
#include "util.h"

int tempregNum;

enum typel expreType = Int; //xpression ����ֵ���ͣ���������ת���ж�����

char retfactor[32];
char retterm[32];
char retexpre[32];

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

String expression(){

    return retexpre;
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
        }//emit�������ͣ���������������ջ�л��߼Ĵ���֮��
    }
}

//�﷨����ִ�����Ҫ�����Ӧ�ı������Ƿ����
void factor(){
    insymbol();
    if(curSy == IDENTSY){
        int pos = loc(id);//IDENTSY����Ϊ����������Ϊ������Ҳ����Ϊ����
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
        //////////////////////////////ǿ������ת��expreType
    } else if(curSy == PLUS || curSy == SUB){
        int sign = 1;
        if(curSy == SUB) sign = -1;
        insymbol(); //��ȡ��Ӧ������
        if(curSy != INTCON) {
            inum = inum * sign;
            strcpy(retfactor,intToString(inum));
        } else error(10);
    } else if(curSy == CHARSY) {
        strcpy(retfactor,charToString(ichar));
        ///////////////////////////////
        expreType = Char;
        ///////////////////////////////ǿ������ת��expreType
    } else if(curSy == LPARENT){
        strcpy(retfactor,expression());//���ñ��ʽ��Ӧ����
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
        //�������
    }
    //////////////////
    //�ǵ�None����ֵ�ĸ�ֵ����
}


void synanalysis(){
    codeCount = 0;
    tabCount = 0;
}
