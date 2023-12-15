#ifndef _TABLE_READER_H
#define _TABLE_READER_H
#include <definitions.h>
#include <stdio.h>

int numcolMat(FILE*fp); //n colonne

int numrow(FILE* fp); //n righe

int numcolSup(FILE*fp); //n colonne file supporto

char*** initmat(FILE*fp); //crea e restituisce una matrice 2x2 di array

int* initlist(FILE*fp); //crea e restituisce array che racchiude i gradi di importanza dei vari attributi

int isNumber(int elemento);

int isWord(int elemento);

int isRelevant(int elemento);

int isPrincipal(int elemento);

int isSecondary(int elemento);

int isInput(int elemento);

int isOutput(int elemento);

char* getDescription(int elemento);

int getposPrincipal(int* arr,int el); //ritorna colonna principale INPUT

int getposOutput(int* arr,int el); //ritorna colonna principale OUTPUT

void getNameColumn(FILE* fp,int pos);//Ritorna nome colonna in pos

#endif
