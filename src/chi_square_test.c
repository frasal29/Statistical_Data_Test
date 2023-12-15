#include <stdlib.h>
#include <stdio.h>
#include <chi_square_test.h>

//RESTITUISCE SOMMA DELLE RIGHE NELLA COLONNA col

double totalrow(double** matrice, int row, int col){

  double tot=0;
  for(int i=0;i<col;i++)
    tot+= matrice[row][i];
  return tot;
}

//RESTITUISCE SOMMA DELLE COLONNE NELLA RIGA row

double totalcol(double** matrice, int row, int col){
  double tot=0;
  for(int i=0;i<row;i++)
    tot+=matrice[i][col];
  return tot;
}

//RESTITUISCE SOMMA DI TUTTI GLI ELEMENTI DELLA MATRICE

double totalElements(double** matrice, int row, int col){
  double tot=0;
  for(int i=0;i<row;i++)
    for(int j=0;j<col;j++)
      tot+=matrice[i][j];
  return tot;
}

//RESTITUISCE I GRADI DI LIBERTA' DELLA MATRICE

int degfreedom(int row,int col){
//  return (row-1)*(col-1); // non va bene nel caso in cui le colonne sono pari a 1 --> mi ritorna 0
    
    if(row > 1 && col > 1) return((row-1)*(col-1));
    if(row == 1 && col > 1) return(col-1);
    if(row > 1 && col == 1) return(row-1);
}

//RESTITUISCE ARRAY CON I VALORI DELLE SOMMATORIE DELLE RIGHE DELLA MATRICE

double* rowsums(double** matrice,int row,int col) {
  double* somme=(double*)calloc(row,sizeof(double));
  for(int i=0;i<row;i++) {
    somme[i]=totalrow(matrice,i,col);
  }
  return somme;
}

//RESTITUISCE ARRAY CON I VALORI DELLE SOMMATORIE DELLE COLONNE DELLA MATRICE

double* columnsums(double** matrice,int row,int col) {
  double* somme=(double*) calloc(col,sizeof(double));
  for(int i=0;i<col;i++) {
    somme[i]=totalcol(matrice,row,i);
  }
  return somme;
}

//RESTITUISCE VALORE DI EXPECTED VALUE

double expectedValue(double rowtot,double coltot, double eltot){
  return (rowtot*coltot)/eltot;
}

//RESTITUISCE CHI-VALUE MATRICE

double chivalue(double**matrice,int row,int col){
  double tot=0;
  double* sumrows=rowsums(matrice,row,col);
  double* sumcol=columnsums(matrice,row,col);
  double  totalElements=0;
  double temp=0;
  for(int i=0;i<row;i++){
    totalElements += sumrows[i];
  }
    for(int r=0;r<row;r++)
      for(int c=0;c<col;c++){
        //printf("\nCALCOLOCHIVALUE\n(%lf-%lf)^2/%lf\n",matrice[r][c],expectedValue(sumrows[r],sumcol[c],totalElements),expectedValue(sumrows[r],sumcol[c],totalElements));
        temp=((matrice[r][c]- expectedValue(sumrows[r],sumcol[c],totalElements)));
        tot+=temp*temp/expectedValue(sumrows[r],sumcol[c],totalElements);
        //printf("Uguale a=%lf\n",temp*temp/expectedValue(sumrows[r],sumcol[c],totalElements));
      }
    //printf("\n");

  return tot;
}