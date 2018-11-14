#include "types.h"
#include "symbol.h"
/*
前导0：0
输入字符非法：1
*/
String errMessage[] = {
    "输入内容含前导零","输入字符非法"
};

void error(int n){
    printf(errMessage[n]);
    printf("：出错位置:%d行%d字符处标识符\n",countLine+1,countChar-1);
}
