#ifndef _TEST_H
#define _TEST_H
#include <table_reader.h>
#include <matrix_test.h>
#include <chi_square_test.h>
#include <chi_table_reader.h>
#include <filter_matrix.h>

/*DEFINIZIONI*/
void testColonneMat(FILE* fp);
void testRigheMat(FILE* fp);
void testColonneSup(FILE* fp);
void testprintMat(char*** tabella,int righe,int colonne);
void testprintArrSup(int* relations,int colonne);
void testElements(int* relations,int colonne);
void testgetNumCategorie(char*** tabella,int righe,int pos);
void testgetCategorie(char*** matrice,int righe, int pos, int catnum);
void testprintMatTest(double** matrice,int righe,int colonne);
void testgetposPrincipal(int* arr,int el);
void testgetposPrincipal(int* arr,int el);
//void testgetposPrincipal(FILE* fp);
//void testgetposOutuput(FILE* fp);
void testgetNameColumn(FILE* fp,int pos);
void testtotalElements(double** matrice, int row, int col);
void testdegfreedom(int row,int col);
void testexpectedValue(double** matrice, int row,int col);
void printrowsumsarray(double** matrice, int row,int col);
void printcolumnsumsarray(double** matrice, int row,int col);
void testchivalue(double**matrice,int row,int col);
void testgetColumnNum(FILE* fp);
void testgetRowNum(FILE* fp);
void testgetColPos(FILE* fp,double sl);
void testgetRowPos(FILE* fp, int df);
void testgetCriticalValue(FILE* fp,int sl,int df);
void testgetChiTestResult(double y1,double y2,double  x1,double x2,double chiv);
void testgetSignificanceLevel(FILE*fp,int pos);
void testchiTestCompare(FILE* fp, double chivalue,int df);
void testsecondarycol(int* support,int el);
void testgetCatOccurence(char*** matrice,int rows,int col,char* categoria);

#endif
