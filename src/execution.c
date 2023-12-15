#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <execution.h>
#include <ANOVA_test.h>
#include <matrix_test.h>
#include <gsl/gsl_statistics.h> // buona parte delle funzioni statistiche
#include <gsl/gsl_sort.h> // per ordinare l'array



// Esegue tutti i test partendo da principale

void do_test(FILE* test, FILE* support, FILE* chitable) { // IL PRIMO PARAMETRO E' fumatori.csv
                                          // IL SECONDO PARAMETRO E' fumatorisupporto.csv
                                          // IL TERZO PARAMETRO E' chitablemax.txt

  if(verbose) printf("------ESEGUO TEST------\n");

  char*** matrice=initmat(test); // Crea e restituisce una matrice di celle contenenti stringhe prese dal file test
  int* arrsupporto=initlist(support); //inizializza l'array del file di supporto trasformato in interi
  int elementisupporto=numcolSup(support); // numero di colonne del file di supporto
  int righematrice=numrow(test); // numero di righe della matrice del file test
  int colonnematrice=numcolMat(test); // numero di colonne della matrice del file test
  int posPrincipale=getposPrincipal(arrsupporto,elementisupporto); // intero che indica la posizione dell'array di supporto dove si trova la colonna che contiene Principal Input
  int posOutput=getposOutput(arrsupporto,elementisupporto); // intero che indica la posizione dell'array di supporto dove si trova la colonna che contiene Principal OUTPUT
  //int posPrincipale=getposPrincipal(support);
  //int posOutput=getposOutput(support);
  if(verbose) printf("\n-----------INIZIO PRINCIPALE-----------\n");

  doprincipal(matrice,righematrice,colonnematrice,posPrincipale,posOutput,chitable,test,arrsupporto);

  if(verbose) printf("\n-----------FINE PRINCIPALE-----------\n");
  
  if(verbose) printf("\n------------------------------------------------------------------------------------------\n");
  
  if(verbose) printf("\n\n-----------INIZIO FILTRAGGIO-----------\n\n");

  dofiltered(test, chitable, matrice, arrsupporto, elementisupporto, righematrice, colonnematrice, posPrincipale, posOutput);

  if(verbose) printf("\n-----------FINE FILTRAGGIO-----------\n");
  if(verbose) printf("\n------------------------------------------------------------------------------------------\n");
  if(verbose) printf("\n\n-----------INIZIO COMBINAZIONI-----------\n\n");

  docombinations(test, chitable, matrice, arrsupporto, elementisupporto, 
                 righematrice, colonnematrice, posPrincipale, posOutput);

  if(verbose) printf("\n\n-----------FINE COMBINAZIONI-----------\n");
  if(verbose) printf("\n\n-----------FINE DELL'ESECUZIONE-----------\n");
  if(verbose) printf("\n------------------------------------------------------------------------------------------\n");

}

// Esegue tutti i calcoli del chi test

void calculate(char*** matrice, int righematrice, int posPrincipale, int posOutput, FILE* chitable, FILE* test, int* arraysupporto) {
  double** matricetest = makeTestMatrix(matrice, righematrice, posPrincipale, posOutput, arraysupporto);
  int righe = getNumCategorieInput(matrice, righematrice, posPrincipale);
  int colonne = getNumCategorieOutput(matrice, righematrice, posOutput, arraysupporto);
  char** categorieInput = getCategorieInput(matrice, righematrice, posPrincipale, righe);
  char** categorieOutput = getCategorieOutput(matrice, righematrice, posOutput, colonne);
    
  if(colonne == 1)
    colonne++;
  
  if(verbose) {
    printf("\n\n\n-----ESEGUO IL CHI TEST-----\n\n");
    printf("COLONNA INPUT: %s\n", matrice[0][posPrincipale]);
    printf("COLONNA OUTPUT: %s\n", matrice[0][posOutput]);
    
    printf("\n\nTABELLA DI CONTINGENZA:\n");
    for(int i = 0; i < righe; i++) {
        for(int c = 0; c < colonne; c++) {
            printf("%s %s: %g   ", categorieInput[i], categorieOutput[c], matricetest[i][c]);
        }
        printf("\n");
    }
  }
  
  if(verbose==1) printf("\nRIGHE %d COLONNE %d\n\n",righe,colonne);
  getmatrixtestvalues(matricetest,matrice,righematrice,righe,colonne,posPrincipale,posOutput, arraysupporto);
  double chiv=chivalue(matricetest,righe,colonne);
  int df=degfreedom(righe,colonne);
  if(verbose==0) {
    printf("Prob("); 
    getNameColumn(test,posPrincipale); 
    printf("=>"); 
    getNameColumn(test,posOutput); 
    printf(") = ");
  }
  chiTestCompare(chitable, chiv, df, matrice, posPrincipale, posOutput);
}

//Esegue il test per la colonna principale e di output

void doprincipal(char*** matrice,int righematrice,int colonnematrice,int posPrincipale,int posOutput,FILE*chitable,FILE*test, int* arrsupp){
  if(verbose==1){ // se il parametro che ho messo come ultimo è 1
    printf("\nChiamata a doprincipal()\n");
    printf("Stampo la matrice di input (%d,%d):\n\n",righematrice, colonnematrice);
    for(int i = 0; i < righematrice; i++){ // per ogni riga
      printf("riga %d: ", i);
      for(int j = 0;j < colonnematrice; j++) // per ogni colonna
        printf("%s ", matrice[i][j]);
      printf("\n");
    }
    printf("\nLa colonna principale input e\' in posizione %d\n",posPrincipale);
    printf("La colonna principale output e\' in posizione %d\n",posOutput);
  }
  // qui devo verificare se la mia matrice ha in output numeri o lettere
  if(colOutputIsNRPO(posOutput, arrsupp)) {  // NRPO: Number Relevant Principal Output  
// per anova
    calculate_ANOVA_test(matrice, righematrice, posPrincipale, posOutput);

 // inizio Chi test con matrice modificata
  char*** matriceNuova = matriceModificata(matrice, righematrice, colonnematrice, posPrincipale, posOutput);
  calculate(matriceNuova, righematrice, posPrincipale, posOutput, chitable, test, arrsupp); // per il chi test
  // fine chi test
  }
  
  else {
// eseguo il chi test
    calculate(matrice,righematrice,posPrincipale,posOutput,chitable,test, arrsupp);
  }

}




// Questa funzione prende la matrice e restituisce la stessa matrice ma modificata nel caso in cui ho una NRPO
char*** matriceModificata(char*** matrice, int righe, int colonne, int posI, int posO) {
//    char*** matriceDaRitornare = matrice;

    char*** matriceDaRitornare = (char***)calloc(righe, sizeof(char**));
    for(int i = 0; i < righe ; i++) {
        matriceDaRitornare[i] = (char**)calloc(colonne, sizeof(char**));
    }

    for(int i = 0; i < righe; i++) {
        for(int j = 0; j < colonne; j++) {
            matriceDaRitornare[i][j] = matrice[i][j];
        }
    }    
// ora devo sostituire tutti i numeri della colonna corrispondente all'output con dei letterali  che sono i quartili corrispondenti

// creo un array con tutti i valori numerici in modo da calcolare i quartili
    double* arrayDiValoriOutput = (double*)calloc(righe - 1, sizeof(double)); // il numero di elementi è pari al numero di righe
    for(int i = 1; i < righe; i++) {          // per tutte le righe della matrice
        arrayDiValoriOutput[i - 1] = atof(matriceDaRitornare[i][posO]); // ora questo array contiene tutti i valori che erano in char in forma double della colonna OUTPUT
                                                                        // la dimensione di questo array è righe -1
    }
    gsl_sort(arrayDiValoriOutput, 1, righe-1); // ordino gli elementi in ordine crescente
       
       // DIVIDO IN QUARTILI
    printf("\n----------------------------------------------------------------------------------------");
    printf("\n\n-----RAGGRUPPO L'OUTPUT DELLA MATRICE IN QUARTILI PER ESEGUIRE IL CHI TEST-----");
    double minPrimoQuartile = gsl_stats_quantile_from_sorted_data(arrayDiValoriOutput, 1, righe-1, 0);
    double maxPrimoQuartile = gsl_stats_quantile_from_sorted_data(arrayDiValoriOutput, 1, righe-1, 0.25);
    double minSecondoQuartile = maxPrimoQuartile + 0.001;
    double maxSecondoQuartile = gsl_stats_quantile_from_sorted_data(arrayDiValoriOutput, 1, righe-1, 0.50);
    double minTerzoQuartile = maxSecondoQuartile + 0.001;
    double maxTerzoQuartile = gsl_stats_quantile_from_sorted_data(arrayDiValoriOutput, 1, righe-1, 0.75);
    double minQuartoQuartile = maxTerzoQuartile + 0.001;
    double maxQuartoQuartile = gsl_stats_quantile_from_sorted_data(arrayDiValoriOutput, 1, righe-1, 1);
    
    printf("\n\nIl primo quartile e' compreso tra : %g e %g\n", minPrimoQuartile, maxPrimoQuartile);
    printf("Il secondo quartile e' compreso tra : %g e %g\n", minSecondoQuartile, maxSecondoQuartile);
    
    printf("Il terzo quartile e' compreso tra : %g e %g\n", minTerzoQuartile, maxTerzoQuartile);
    printf("Il quarto quartile e' compreso tra : %g e %g\n", minQuartoQuartile, maxQuartoQuartile);
    
    for(int i = 1; i < righe; i++) { // per tutte le righe della matrice
            if((atof(matriceDaRitornare[i][posO]) >= minPrimoQuartile) && (atof(matriceDaRitornare[i][posO]) <= maxPrimoQuartile)) { // se sono sul primo quartile
                matriceDaRitornare[i][posO] = "PrimoQuartile";
            }
            if((atof(matriceDaRitornare[i][posO]) >= minSecondoQuartile) && (atof(matriceDaRitornare[i][posO]) <= maxSecondoQuartile)) { // se sono sul secondo quartile      
                matriceDaRitornare[i][posO] = "SecondoQuartile";
            }
            if((atof(matriceDaRitornare[i][posO]) >= minTerzoQuartile) && (atof(matriceDaRitornare[i][posO]) <= maxTerzoQuartile)) { // se sono sul terzo quartile      
                matriceDaRitornare[i][posO] = "TerzoQuartile";
            }
            if((atof(matriceDaRitornare[i][posO]) >= minQuartoQuartile) && (atof(matriceDaRitornare[i][posO]) <= maxQuartoQuartile)) { // se sono sul quarto quartile      
                matriceDaRitornare[i][posO] = "QuartoQuartile";
            }
    }

    printf("\n\nMatrice con OUTPUT raggruppato in quartili:\n\n");
    for(int i = 0; i < righe; i++){ // per ogni riga
      printf("riga %d: ", i);
      for(int j = 0;j < colonne; j++) // per ogni colonna
        printf("%s ", matriceDaRitornare[i][j]);
      printf("\n");
    }    
    return matriceDaRitornare;
}



// questa funzione mi dice se ho in OUTPUT una NRPO
int colOutputIsNRPO(int posOutput, int* arrsupp) {
    int verificato = 0;
    if(isNumber(arrsupp[posOutput]) && 
       isRelevant(arrsupp[posOutput]) && 
       isPrincipal(arrsupp[posOutput]) && 
       isOutput(arrsupp[posOutput])) {
         verificato = 1;
    }
    return verificato;
}


// Esegue il test filtrando per ogni categoria di ogni colonna secondaria

void dofiltered(FILE*test, FILE*chitable, char*** matrice, int* arraysecondario, int elemsecondario, int righematrice, int colonnematrice, int posInput, int posOutput) {

    int* occorrenzesupport = secondarycol(arraysecondario,elemsecondario);
    
    for(int i = 0; i < elemsecondario; i++) {
        if(occorrenzesupport[i] == 0) continue;
        if(occorrenzesupport[i] == 1) {
        
            int numcategorie = getNumCategorieInput(matrice, righematrice, i);
            char** categorie = getCategorieInput(matrice, righematrice, i, numcategorie);

            for(int n = 0; n < numcategorie; n++) {
                printf("------------------------------------------------------------------------------------------\n");
                printf("\nFiltro per colonna %d: ", i); getNameColumn(test, i); printf("\n");
                printf("Per "); getNameColumn(test, i); printf(" = %s\n\n", categorie[n]);
                int catoccorrenze = getCatOccurence(matrice, righematrice, i, categorie[n]);
                char*** matricefiltrata = filtermatrix(matrice, categorie[n], catoccorrenze, i, righematrice, colonnematrice);
                if(verbose == 1) {
                    for(int r = 0; r < catoccorrenze + 1; r++) { // stampo la matrice filtrata
                        printf("riga %d: ", r);
                        for(int c = 0; c < colonnematrice; c++)
                            printf("%s ", matricefiltrata[r][c]);
                            printf("\n");
                            
                    }
                }

                if(colOutputIsNRPO(posOutput, arraysecondario)) {
                    // faccio anova con la matrice filtrata
                    calculate_ANOVA_test(matricefiltrata, catoccorrenze + 1, posInput, posOutput);
                
                    // faccio chi test con matrice filtrata e raggruppata in quartili
                    char*** matriceFiltrataInQuartili = matriceModificata(matricefiltrata, catoccorrenze + 1, colonnematrice, posInput, posOutput);
                    calculate(matriceFiltrataInQuartili, catoccorrenze + 1, posInput, posOutput, chitable, test, arraysecondario);
                }
                else {
                    calculate(matricefiltrata, catoccorrenze + 1, posInput, posOutput, chitable, test, arraysecondario);
                }
                printf("\n\n\n");
            }
        }
    }
}


//Esegue il test combinando tutte le colonne secondarie

void docombinations(FILE*test,FILE*chitable,char*** matrice,int* arraysecondario, int elemsecondario,int righematrice,int colonnematrice,int posInput,int posOutput){
  int* occorrenzesupport=secondarycol(arraysecondario,elemsecondario);
  int ultimosecondario=-1;
  for(int i=0;i<elemsecondario;i++) {
    if(occorrenzesupport[i]==1)
      ultimosecondario=i;
  }

  for(int i=0;i<elemsecondario;i++) {
    if(i==ultimosecondario)
      return; //PRIMA ERA BREAK
    if(occorrenzesupport[i]==0) continue;
    if(occorrenzesupport[i]==1){

      int numcategorie=getNumCategorieInput(matrice,righematrice,i);
      char** categorie= getCategorieInput(matrice,righematrice,i,numcategorie);

      for(int n=0;n<numcategorie;n++){
        int catoccorrenze= getCatOccurence(matrice,righematrice,i,categorie[n]);
        char*** matricefiltrata= filtermatrix(matrice,categorie[n],catoccorrenze,i,righematrice,colonnematrice);

      for(int x=i+1;x<elemsecondario;x++){
        if(occorrenzesupport[x]==0) continue;
        if(occorrenzesupport[x]==1){
          if(verbose==1){
            printf("------------------------------------------------------------------------------------------\n");
            printf("Filtro per colonna %d:",i); getNameColumn(test,i); printf("\n");
            printf("Filtro per colonna %d:",x); getNameColumn(test,x); printf("\n");
          }

          int numcategorief=getNumCategorieInput(matricefiltrata,catoccorrenze,x);
          char** categorief= getCategorieInput(matricefiltrata,catoccorrenze,x,numcategorief);

          for(int y=0;y<numcategorief;y++){
            int catoccorrenzef = getCatOccurence(matricefiltrata,catoccorrenze+1,x,categorief[y]);
            char*** matricefiltrata2 = filtermatrix(matricefiltrata,categorief[y],catoccorrenzef,x,catoccorrenze + 1,colonnematrice);

          printf("\nFiltro: ");getNameColumn(test,i); printf(" = %s, ",categorie[n]); getNameColumn(test,x); printf(" = %s\n",categorief[y]);

          if(verbose==1){ //STAMPA ELEMENTI MATRICE
                        printf("Righe = %d   Colonne = %d\n", catoccorrenzef+1, colonnematrice);
                        printf("\n");
                        for(int rf = 0; rf < catoccorrenzef + 1; rf++) {
                            printf("riga %d: ", rf); 
                          for(int cf = 0; cf < colonnematrice; cf++)
                            printf("%s ", matricefiltrata2[rf][cf]);
                          printf("\n");
                            }
                      }
          if(catoccorrenzef > 5) {
                        if(colOutputIsNRPO(posOutput, arraysecondario)) {
                    // faccio anova con la matrice filtrata
                    calculate_ANOVA_test(matricefiltrata2, catoccorrenzef + 1, posInput, posOutput);
                
                    // faccio chi test con matrice filtrata e raggruppata in quartili
                    char*** matriceFiltrataInQuartili = matriceModificata(matricefiltrata2, catoccorrenzef + 1, colonnematrice, posInput, posOutput);
                    calculate(matriceFiltrataInQuartili, catoccorrenzef + 1, posInput, posOutput, chitable, test, arraysecondario);
                }
                else {
                    calculate(matricefiltrata2, catoccorrenzef + 1, posInput, posOutput, chitable, test, arraysecondario);
                }
                
          } else
            printf("\nOccorrenze insufficienti!\n");
          printf("\n\n\n");
      }

    }
  }


      }
    }
  }
}

//Stampa in output % delle categorie Principale e Output partecipanti al test
void getmatrixtestvalues(double**matriced,char*** matrice,int righematchar,int righe,int colonne,int colonnaI,int colonnaO, int* arraysupporto) {
  int numInput=getNumCategorieInput(matrice,righematchar,colonnaI); // numero di categorie della colonna Principal Input
  int numOutput=getNumCategorieOutput(matrice,righematchar,colonnaO, arraysupporto); // numero di categorie della colonna Principal Output
  char** catI= getCategorieInput(matrice,righematchar,colonnaI,numInput); //array con le categorie nella colonna Input
  char** catO= getCategorieOutput(matrice,righematchar,colonnaO,numOutput); //array col le categorie nella colonna Output
  double totrow;
  
  printf("\n");
  for(int i=0;i<numInput;i++) { // per tutte le occorrenze diverse di categorie di Principal Input
    totrow=totalrow(matriced,i,numOutput); // matriced sarebbe la matrice ottenuta dalla funzione maketestMatrix
    for(int j=0;j<numOutput;j++){
      int occ=getOccurence(matrice,catI[i],catO[j],colonnaI,colonnaO,righematchar);
      if(verbose==1){
        printf("(%s+%s)/%s=%lf%%\n",catI[i],catO[j],catI[i],(occ/totrow)*100);
      }
      else{
        printf("(%s+%s)/%s=%.2lf%%\n",catI[i],catO[j],catI[i],(occ/totrow)*100);

      }
      }
    }
    printf("\n");
}