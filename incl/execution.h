#ifndef EXECUTION_H
#define EXECUTION_H
#include <table_reader.h>
#include <matrix_test.h>
#include <chi_square_test.h>
#include <chi_table_reader.h>
#include <filter_matrix.h>
#include <definitions.h>
#include <stdio.h>

void do_test(FILE* test,FILE*support,FILE*chitable);

void calculate(char*** matrice,int righematrice,int posPrincipale,int posOutput,FILE*chitable,FILE*test, int* arraysupporto);

void doprincipal(char*** matrice,int righematrice,int colonnematrice,int posPrincipale,int posOutput,FILE*chitable,FILE*test, int* arrsupp); 

char*** matriceModificata(char*** matrice, int righe, int colonne, int posI, int posO);

int colOutputIsNRPO(int posOutput, int* arrsupp);

void dofiltered(FILE*test,FILE*chitable,char*** matrice,int* arraysecondario, int elemsecondario,int righematrice,int colonnematrice,int posInout,int posOutput); 

void docombinations(FILE*test,FILE*chitable,char*** matrice,int* arraysecondario, int elemsecondario,int righematrice,int colonnematrice,int posInout,int posOutput);

void getmatrixtestvalues(double**matriced,char*** matrice,int righematchar,int righe,int colonne,int colonnaI,int colonnaO, int* arraysupporto);

#endif
