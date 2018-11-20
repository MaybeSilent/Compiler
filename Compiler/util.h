#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

void printtabs();
void printcodes();
String intToString(int inum);
String charToString(char ichar);
void emit(enum ops op, String arg1, String arg2, String result);
void entertab(String name, enum kindsy kind, enum typel type, int value);
void enterblock();

int loc(String id);
String numToReg(int n);
String numToLabel(int n);
void emitLabel(int codepos , int labelnum);

void program();
#endif // UTIL_H_INCLUDED
