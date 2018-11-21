#include "types.h"
#include "symbol.h"
/*
前导0：0
输入字符非法：1
*/
int errorNum = 0;

int getErrorNum(){
    return errorNum;
}

void skipVaildSymbol(){
    //遇到非法的symbol,尝试跳过
}

String errMessage[] = {
    "输入内容含前导零","输入字符非法","常量声明缺失赋值符号","常量赋值类型不匹配","变量名重复声明",//0,1,2,3,4
    "变量名声明错误","变量数组大小值需为大于0的整数","数组声明出错","函数参数声明错误",//5,6,7,8
    "函数参数变量名称声明错误","因子内容需为相应的整数","因子内容错误","变量未定义",//9,10,11,**12**
    "数组元素调用错误","函数调用错误","函数参数数目不匹配","函数参数类型不匹配",//**13**,14,15,16
    "赋值类型必须为变量或者数组","赋值语句类型不匹配","条件语句出错","do-while语句出错",//17,18,19,20
    "scanf语句出错","scanf语句读入类型不匹配","printf语句出错","for语句出错",//21,22,23,24
    "变量类型错误",";缺失或者位置出错","函数调用参数错误","语句无法匹配","return语句出错",//25,26,27,28,29
    "}缺失或位置出错","非法的开始字符","声明语句出错","函数返回值错误","main函数格式错误",//30,31,32,33,34
    "文件结束异常"//35
};

void error(int n){
    errorNum ++ ;
    printf(errMessage[n]);
    printf("：出错位置:%d行%d字符处标识符\n",countLine+1,countChar);
}

void errorjump(int n){
    errorNum ++ ;
    printf(errMessage[n]);
    printf("：出错位置:%d行%d字符处标识符\n",countLine+1,countChar);
    while(curSy != SEMICOLON) insymbol();
    insymbol();
}
