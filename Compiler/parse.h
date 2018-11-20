#ifndef PARSE_H_INCLUDED
#define PARSE_H_INCLUDED


void constDec();
void varDec();
void expression();
void factor();
void term();

void statement();
void ifstate();

//内部函数声明
void statement();
void state();
void comstate();

#endif // PARSE_H_INCLUDED
