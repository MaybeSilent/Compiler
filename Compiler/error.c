#include "types.h"
#include "symbol.h"
/*
ǰ��0��0
�����ַ��Ƿ���1
*/
String errMessage[] = {
    "�������ݺ�ǰ����","�����ַ��Ƿ�"
};

void error(int n){
    printf(errMessage[n]);
    printf("������λ��:%d��%d�ַ�����ʶ��\n",countLine+1,countChar-1);
}
