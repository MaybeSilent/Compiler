#ifndef PARSE_H_INCLUDED
#define PARSE_H_INCLUDED


void constDec();
void varDec();
void expression();
void factor();
void term();

void statement();
void ifstate();

void statement();
void state();
void comstate();
void callparm(int pos);
void callfunction(int pos);

#endif // PARSE_H_INCLUDED
