#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

void printtabs();
void printcodes();

String intToString(int inum);
String charToString(char ichar);
int stringToInt(String in);
int charToInt(String in);

void emit(enum ops op, String arg1, String arg2, String result);
void entertab(String name, enum kindsy kind, enum typel type, int value);
void enterblock();

int loc(String id);
String numToReg(int n);
String numToLabel(int n);
void emitLabel(int codepos , int labelnum);

String getStringCon(int count);
int insertString(String instring);
void printmips();
void program();

String findbyReg(String in,int flag);
void printToFILE();
#endif // UTIL_H_INCLUDED
