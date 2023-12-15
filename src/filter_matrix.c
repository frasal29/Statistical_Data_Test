#include <filter_matrix.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// PRENDE IN INPUT L'ARRAY INT DEL FILE SUPPORTO E RITORNA UN ARRAY CON LE VARIE COLONNE SECONDARY RELEVANT
int* secondarycol(int* support, int el)
{
    int* secrel = (int*)calloc(el, sizeof(int));
    for(int i = 0; i < el; i++) {
        if(isRelevant(support[i]) == 1 && isSecondary(support[i]) == 1)
            secrel[i] = 1;
        else
            secrel[i] = 0;
    }
    return secrel;
}

/*PARTE DALLA MATRICE INIALE
 E CREA UNA MATRICE FILTRANDO SOLO LE RIGHE AVENTI LA CATEGORIA SPECIFICATA IN COLONNA */

char*** filtermatrix(char*** matrix, char* categoria, int occorrenze, int colonna, int righemat, int colonnemat) {
    int occ = occorrenze + 1;
    char*** matrixfiltered = (char***)calloc(occ, sizeof(char**));
    matrixfiltered[0] = (char**)calloc(colonnemat, sizeof(char*));
    for(int n = 0; n < colonnemat; n++) {
                matrixfiltered[0][n] = (char*)malloc(MAX * sizeof(char));
                strcpy(matrixfiltered[0][n], matrix[0][n]);
    }
    
    int pos = 1;
    for(int i = 0; i < righemat; i++){ // righe della matrice non filtrata
        if(strcmp(matrix[i][colonna], categoria) == 0) {
            matrixfiltered[pos] = (char**)calloc(colonnemat, sizeof(char*));
            
            for(int n = 0;n < colonnemat; n++) {
                matrixfiltered[pos][n] = (char*)malloc(MAX * sizeof(char));
                strcpy(matrixfiltered[pos][n], matrix[i][n]);
            }
            pos++;
        }
    }
    return matrixfiltered;
}



