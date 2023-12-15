#ifndef _CHI_SQUARE_TEST_H
#define _CHI_SQUARE_TEST_H

double totalrow(double** matrice, int row, int col); 
double totalcol(double** matrice, int row, int col); 
double totalElements(double** matrice, int row, int col); 
int degfreedom(int row,int col); 
double* rowsums(double** matrice,int row,int col); 
double* columnsums(double** matrice,int row,int col); 
double expectedValue(double rowtot,double coltot, double eltot); 
double chivalue(double**matrice,int row,int col); 

#endif
