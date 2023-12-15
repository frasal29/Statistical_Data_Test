#ifndef _FILTER_MATRIX_H
#define _FILTER_MATRIX_H
#include <table_reader.h>

int* secondarycol(int* support,int el);

char*** filtermatrix(char*** matrix,char* categoria, int occorrenze,int colonna,int righemat,int colonnemat);


#endif
