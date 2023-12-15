#ifndef ANOVA_TEST_H
#define ANOVA_TEST_H

#include <definitions.h>
#include <stdio.h>
#include <stdlib.h>


void calculate_ANOVA_test(char*** matrice, int righematrice, int posPrincipale, int posOutput);

int* getOccorrenzeCategorieInput(char*** matrice, int righematrice, int posPrincipale, int numCategorieInput, char** categorieInput);

double* arrayDiSommaDiValoriOutput(char*** matrice, int righematrice, int posPrincipale, int posOutput, int numCategorieInput, char** categorieInput);

double* arrayDiCalcoloMediaValoriOutput(double* arrayDelleSommeOutput, int* arrayDelleOccorrenzeInput, int numDelleCategorieInput);

double* arrayDiErroriQuadratici(char*** matrice, int righematrice, int posPrincipale, int posOutput, int numCategorieInput, char** categorieInput, double* arrayMedie);

double* arrayCalcoloQuadrati(double grandeMedia, double* arrayMedie, int numDelleCategorieInput);

double* arrayCalcoloQuadratiPerLeOsservazioni(double* arrayQuadrati, int* occorrenzeOsservazioni, int numDelleCategorieInput);


#endif