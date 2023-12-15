#include <ANOVA_test.h>
#include <gsl/gsl_cdf.h>
#include <matrix_test.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void calculate_ANOVA_test(char*** matrice, int righematrice, int posPrincipale, int posOutput) {
    
    int numCategorieInput = getNumCategorieInput(matrice, righematrice, posPrincipale);
    char** categorieInput = getCategorieInput(matrice, righematrice, posPrincipale, numCategorieInput);
    int* occorrenzeInput =
        getOccorrenzeCategorieInput(matrice, righematrice, posPrincipale, numCategorieInput, categorieInput);
    double* arraySommeOutput =
        arrayDiSommaDiValoriOutput(matrice, righematrice, posPrincipale, posOutput, numCategorieInput, categorieInput);
    double* arrayMediaOutput = arrayDiCalcoloMediaValoriOutput(arraySommeOutput, occorrenzeInput, numCategorieInput);
    double* arrayErroriQuadraticiOutput = arrayDiErroriQuadratici(
        matrice, righematrice, posPrincipale, posOutput, numCategorieInput, categorieInput, arrayMediaOutput);

    double sommaDegliErroriQuadratici = 0;
    double grandeMediaTutteOsservazioni = 0;
    double sommaTuttiValori = 0;
    int sommaOccorrenze = 0;
    for(int i = 0; i < numCategorieInput; i++) {
        sommaDegliErroriQuadratici += arrayErroriQuadraticiOutput[i];
        sommaTuttiValori += arraySommeOutput[i];
        sommaOccorrenze += occorrenzeInput[i];
    }
    grandeMediaTutteOsservazioni = sommaTuttiValori / sommaOccorrenze;

    double* arrayQuadrati = arrayCalcoloQuadrati(grandeMediaTutteOsservazioni, arrayMediaOutput, numCategorieInput);
    double* arrayQuadratiPerOsservazioni =
        arrayCalcoloQuadratiPerLeOsservazioni(arrayQuadrati, occorrenzeInput, numCategorieInput);

    double SSb = 0; // somma dei quadrati between
    for(int i = 0; i < numCategorieInput; i++) {
        SSb += arrayQuadratiPerOsservazioni[i];
    }

    int df1 = numCategorieInput - 1;               // DF between
    int df2 = sommaOccorrenze - numCategorieInput; // DF within

    double MSe = sommaDegliErroriQuadratici / df2; // media quadrata dell'errore
    double MSb = SSb / df1;                        // media quadrata between

    double F = MSb / MSe; // valore F

    // p value
    double p = gsl_cdf_fdist_P(F, df1, df2);
    double q = gsl_cdf_fdist_Q(F, df1, df2); // il p value è questo

    printf("\n\n-----ESEGUO TEST ANOVA A UNA VIA-----\n\n");
    printf("COLONNA INPUT: %s\n", matrice[0][posPrincipale]);
    printf("COLONNA OUTPUT: %s\n", matrice[0][posOutput]);

    printf("\nNumero di categorie di Principal Input: %d\n\n", numCategorieInput);

    // stampo le categorie di principal input
    printf("Categorie di Principal Input:\n");
    for(int i = 0; i < numCategorieInput; i++) {
        printf("%d: %s\n", i, categorieInput[i]);
    }

    // stampo il numero di occorrenze per ogni categoria
    printf("\nNumero di occorrenze per ogni categoria:\n");
    for(int i = 0; i < numCategorieInput; i++) {
        printf("%d: %s --> %d\n", i, categorieInput[i], occorrenzeInput[i]);
    }

    // stampo l'array di somma di valori dell'ouput ordinati secondo categoria input
    printf("\nTotale output per ogni categoria:\n");
    for(int i = 0; i < numCategorieInput; i++) {
        printf("%d: %s -->totale: %lf\n", i, categorieInput[i], arraySommeOutput[i]);
    }

    // stampo l'array delle medie dei valori dell'ouput ordinati secondo categoria input
    printf("\nMedie per ogni categoria:\n");
    for(int i = 0; i < numCategorieInput; i++) {
        printf("%d: %s -->media: %lf\n", i, categorieInput[i], arrayMediaOutput[i]);
    }

    // stampo l'array delle somme degli errori quadratici medi ordinati secondo categoria input
    printf("\nErrori quadratici per ogni categoria:\n");
    for(int i = 0; i < numCategorieInput; i++) {
        printf("%d: %s -->somma degli errori quadratici: %lf\n", i, categorieInput[i], arrayErroriQuadraticiOutput[i]);
    }

    // stampo la somma degli errori quadratici di tutte le categorie
    printf("\nSomma degli errori quadratici di tutte le categorie (SSE within): %lf\n", sommaDegliErroriQuadratici);

    // stampo la grande media di tutte le osservazioni
    printf("\nGrande media di tutte le osservazioni %lf\n", grandeMediaTutteOsservazioni);

    // stampo l'array dei quadrati ottenuti dalla formula (grande media - media)^2 ordinati secondo categoria input
    printf("\nQuadrati di ogni categoria ottenuti dalla formula (Grande media - media)^2:\n");
    for(int i = 0; i < numCategorieInput; i++) {
        printf("%d: %s -->Quadrato: %lf\n", i, categorieInput[i], arrayQuadrati[i]);
    }

    // stampo l'array dei quadrati ottenuti dalla formula numeroDiOsservazioni * (grande media - media)^2 ordinati
    // secondo categoria input
    printf("\nQuadrati di ogni categoria ottenuti dalla formula NumeroDiOsservazioni * (Grande media - media)^2:\n");
    for(int i = 0; i < numCategorieInput; i++) {
        printf("%d: %s   Numero Osservazioni: %d-->Valore: %lf\n", i, categorieInput[i], occorrenzeInput[i],
            arrayQuadratiPerOsservazioni[i]);
    }

    // stampo la somma degli elementi dell'array dei quadrati ottenuti dalla formula numeroDiOsservazioni * (grande
    // media - media)^2 ordinati secondo categoria input
    printf("\nSomma dei quadrati (SST between): %lf\n", SSb);

    // stampo i Degrees of Freedom
    printf("\nDF1 = %d      DF2 = %d\n", df1, df2);

    // stampo le medie quadrate
    printf("\nMedia quadrata dell'errore (MSE within): %lf\n", MSe);
    printf("\nMedia quadrata between (MST between): %lf\n", MSb);

    // stampo il valore di F
    printf("\nValore di F : %lf\n", F);

    // stampo il valore del p-value
    printf("\n\nIl valore preciso del p-value calcolato\ntramite la funzione gsl_cdf_fdist_Q e': %lf\n", q);
    printf("P-VALUE COMPLEMENTARE: %lf\n", p);
    if(q <= 0.05) { // rifiuto dell'ipotesi nulla
    printf("\n\nPosso rifiutare l'ipotesi nulla e affermare con una confidenza del 95%% che\nESISTE UNA DIFFERENZA STATISTICAMENTE SIGNIFICATIVA ");
    printf("TRA l'INPUT: %s E l'OUTPUT: %s\n\n", matrice[0][posPrincipale], matrice[0][posOutput]);
    }
    if(q > 0.05) { // accetto dell'ipotesi nulla
    printf("\n\nNon ho sufficienti prove per rifiutare l'ipotesi nulla, percio' posso affermare con una confidenza del 95%% che\n");
    printf("NON ESISTE UNA DIFFERENZA STATISTICAMENTE SIGNIFICATIVA TRA L'INPUT: %s E L'OUTPUT: %s\n\n", matrice[0][posPrincipale], matrice[0][posOutput]);
    }

    // fine anova
}

// funzione che svolge questo calcolo: osservazioni * (grande media - media)^2 per ogni categoria
double*
arrayCalcoloQuadratiPerLeOsservazioni(double* arrayQuadrati, int* occorrenzeOsservazioni, int numDelleCategorieInput)
{
    double* array = (double*)calloc(numDelleCategorieInput, sizeof(double));
    for(int i = 0; i < numDelleCategorieInput; i++) {
        array[i] = arrayQuadrati[i] * occorrenzeOsservazioni[i];
    }
    return array;
}

// funzione che svolge questo calcolo: (grande media - media)^2 per ogni categoria
double* arrayCalcoloQuadrati(double grandeMedia, double* arrayMedie, int numDelleCategorieInput)
{
    double* array = (double*)calloc(numDelleCategorieInput, sizeof(double));
    for(int i = 0; i < numDelleCategorieInput; i++) {
        array[i] = (grandeMedia - arrayMedie[i]) * (grandeMedia - arrayMedie[i]);
    }
    return array;
}

// funzione che mi restituisce un array di valori della media per ogni categoria
double* arrayDiCalcoloMediaValoriOutput(double* arrayDelleSommeOutput,
    int* arrayDelleOccorrenzeInput,
    int numDelleCategorieInput)
{
    // creo un array delle medie
    double* array = (double*)calloc(numDelleCategorieInput, sizeof(double));
    for(int i = 0; i < numDelleCategorieInput; i++) {
        array[i] = arrayDelleSommeOutput[i] / arrayDelleOccorrenzeInput[i];
    }
    return array;
}

// funzione che crea un array dove metto tutti gli errori quadratici per ogni categoria
double* arrayDiErroriQuadratici(char*** matrice,
    int righematrice,
    int posPrincipale,
    int posOutput,
    int numCategorieInput,
    char** categorieInput,
    double* arrayMedie)
{
    double* array = (double*)calloc(numCategorieInput, sizeof(double));
    for(int i = 0; i < numCategorieInput; i++) {
        double sommaDiValori = 0;
        for(int j = 0; j < righematrice; j++) { // la prima riga è l'intestazione cioè quella con j = 0
            if(strcmp(categorieInput[i], matrice[j][posPrincipale]) == 0) {
                sommaDiValori +=
                    ((atof(matrice[j][posOutput]) - arrayMedie[i]) * (atof(matrice[j][posOutput]) - arrayMedie[i]));
                array[i] = sommaDiValori;
            }
        }
    }
    return array;
}

// funzione che crea un array dove metto tutte le somme di valori per ogni categoria
double* arrayDiSommaDiValoriOutput(char*** matrice,
    int righematrice,
    int posPrincipale,
    int posOutput,
    int numCategorieInput,
    char** categorieInput)
{
    double* array = (double*)calloc(numCategorieInput, sizeof(double));
    for(int i = 0; i < numCategorieInput; i++) {
        double sommaDiValori = 0;
        for(int j = 0; j < righematrice; j++) { // la prima riga è l'intestazione cioè quella con j = 0
            if(strcmp(categorieInput[i], matrice[j][posPrincipale]) == 0) {
                sommaDiValori += atof(matrice[j][posOutput]);
                array[i] = sommaDiValori;
            }
        }
    }
    return array;
}

// questa funzione mi restituisce un array con il numero di occorrenze per ciascuna categoria
int* getOccorrenzeCategorieInput(char*** matrice,
    int righematrice,
    int posPrincipale,
    int numCategorieInput,
    char** categorieInput)
{
    int* occorrenze = (int*)calloc(numCategorieInput, sizeof(int));
    for(int i = 0; i < numCategorieInput; i++) {
        int occorrenza = 0;
        for(int j = 0; j < righematrice; j++) {
            if((strcmp(categorieInput[i], matrice[j][posPrincipale])) == 0) { // se sono uguali le stringhe
                occorrenza++;
                occorrenze[i] = occorrenza;
            }
        }
    }
    return occorrenze;
}
