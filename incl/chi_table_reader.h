#ifndef _CHI_TABLE_READER_H
#define _CHI_TABLE_READER_H
#include <table_reader.h>
#include <definitions.h>
#include <stdio.h>

#define MAXNUM 10
int getColumnNum(FILE* fp); 
int getRowNum(FILE* fp); 
int getColPos(FILE* fp,double sl); 
int getRowPos(FILE* fp,int df); 
double getCriticalValue(FILE* fp,int df,int sl); 
void chiTestCompare(FILE* fp, double chivalue,int df, char*** matrice, int posInput, int posOutput); 
void getChiTestResult(double y1,double y2,double  x1,double x2,double chiv, double df, char*** matrice, int posInput, int posOutput );
double getSignificanceLevel(FILE*fp,int pos);


#endif
