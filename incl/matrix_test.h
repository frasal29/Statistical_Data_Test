#ifndef _MATRIX_TEST_H
#define _MATRIX_TEST_H


/*DEFINIZIONI FUNZIONI*/
int getNumCategorieInput(char*** matrice,int righe,int pos);

int getNumCategorieOutput(char*** matrice, int righe, int pos, int* arraysupporto);

char** getCategorieInput(char*** matrice, int righe,int pos,int catnum);

char** getCategorieOutput(char*** matrice, int righe, int pos, int catnum); 

int getOccurence(char*** matrice,char* nomeA, char* nomeB, int posA, int posB, int righe);

double** makeTestMatrix(char*** matrice,int righe,int colonnaI,int colonnaO,int* arrsupp);

int getCatOccurence(char*** matrice,int rows,int col,char* categoria); 

#endif
