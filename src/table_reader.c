#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <table_reader.h>

typedef char* stringa[MAX];

int numcolSup(FILE* fp)
{ // restituisce int numero colonne per l'array del file di supporto
    if(fp == NULL) {
        printf("File non trovato\n");
        return -1;
    }
    fseek(fp, 0, SEEK_SET);
    char c;
    int col = 0;
    c = fgetc(fp);
    while(c != EOF) {
        if(c == ';')
            col++;
        c = fgetc(fp);
    }
    return col + 1;
}

// restituisce il numero delle colonne del file passato in input
// il separatore delle colonne e' il carattere ";".

int numcolMat(FILE* fp)
{

    if(fp == NULL) {
        printf("File non trovato\n");
        return -1;
    }
    fseek(fp, 0, SEEK_SET); // place the cursor on the first character
    char c = fgetc(fp);     // read one character
    if(c == EOF)
        return 0;
    int col = 0; // there is at least one line
    while(c != EOF && c != '\n' && c != '\r') {
        if(c == ';') {
            col++;
            c = fgetc(fp);
        } else {
            c = fgetc(fp);
        }
    }
    if(c == '\n')
        col++;
    printf("numero colonne: %d\n", col);
    return col;
}

// restituisce il numero di righe del file passato in input

int numrow(FILE* fp)
{
    printf("chiamata a numrow()\n");
    if(fp == NULL) {
        printf("File non trovato\n");
        return -1;
    }
    fseek(fp, 0, SEEK_SET); // place the cursor on the first character
    char c = fgetc(fp);     // read one character
    int rows = 0;           // there is at least one line
    while(c != EOF) {
        if(c == '\n') { // an endline followed by an EOF should not be counted
            c = fgetc(fp);
            if(c == '\r')
                c = fgetc(fp);
            rows++;
        } else {
            c = fgetc(fp); // read another character
        }
    }
    printf("numero righe: %d\n", rows);
    return rows;
}

/* Crea e restituisce una matrice di celle contenenti
   stringhe prese dal file fp */

char*** initmat(FILE* fp)
{

    printf("chiamata a initmat()\n");
    int row = numrow(fp);    // numero di righe della tabella
    int col = numcolMat(fp); // numero colonne della tabella
    printf("le righe sono %d e le colonne sono %d\n", row, col);
    char c;
    fseek(fp, 0, SEEK_SET);
    char*** matrice = (char***)calloc(row, sizeof(char**)); // alloco la matrice per quanto righarda le righe
    for(int i = 0; i < row; i++) {                          // alloco le colonne
        matrice[i] = (char**)calloc(col, sizeof(char*));
    }

    for(int i = 0; i < row; i++) {                             // lettura righe
        for(int j = 0; j < col; j++) {                         // lettura colonne
            matrice[i][j] = (char*)malloc(MAX * sizeof(char)); // alloco la stringa riga i colonna j
            c = fgetc(fp);
            if(c == EOF)
                return matrice;
            while(c == '\n' || c == '\r')
                c = fgetc(fp);
            if(c == ';') {
                matrice[i][j] = NULL; // così perdi la memoria allocata
                continue;             // passa alla prossima cella
            }
            if(c == ' ') {
                c = fgetc(fp);
                if(c == ';' || c == '\n' || c == '\r' || c == EOF)
                    matrice[i][j] = NULL;
                continue;
            }
            for(int x = 0; x < MAX; x++) { // lettura file e inserimento in el j colonna
                matrice[i][j][x] = c;
                // printf("matrice[%d][%d][%d]=%c valore %d\n",i,j,x,c,c);
                c = fgetc(fp);
                if(c == EOF)
                    return matrice; // fine FILE termino la creazione della matrice

                if(c == ';' || c == '\n' || c == '\r') {
                    matrice[i][j][x + 1] = '\0'; // termino la stringa
                    break;
                }
            } // fine inserimento elemento in colonna
        }     // fine colonna

    } // fine riga
    return matrice;
}

int* initlist(FILE* fp)
{ // inizializza l'array del file di supporto
    int colonne = numcolSup(fp);
    fseek(fp, 0, SEEK_SET);
    int valore = -1;
    char c;
    c = fgetc(fp);
    int* list = (int*)malloc(colonne * sizeof(int));

    while(c == ' ' || c == '\n' || c == '\r')
        c = fgetc(fp);
    for(int x = 0; x < colonne; x++) {
        valore = 0;
        if(c == EOF)
            return list;
        while(c != ';' && c != '\n' && c != '\r' && c != EOF) {
            if(c == 'W') {
                valore += 8;
                // printf("colonna %d char %c valore %d\n",x+1,c,valore);
            }
            if(c == 'R') {
                valore += 4;
                // printf("colonna %d char %c valore %d\n",x+1,c,valore);
            }
            if(c == 'P') {
                valore += 2;
                // printf("colonna %d char %c valore %d\n",x+1,c,valore);
            }
            if(c == 'N') {
                valore += 0;
                // printf("colonna %d char %c valore %d\n",x+1,c,valore);
            }
            if(c == 'U') {
                valore += 0;
                // printf("colonna %d char %c valore %d\n",x+1,c,valore);
            }
            if(c == 'S') {
                valore += 0;
                // printf("colonna %d char %c valore %d\n",x+1,c,valore);
            }
            if(c == 'I') {
                valore += 0;
                // printf("colonna %d char %c valore %d\n",x+1,c,valore);
            }
            if(c == 'O') {
                valore += 1;
                // printf("colonna %d char %c valore %d\n",x+1,c,valore);
            }
            c = fgetc(fp);
        }
        list[x] = valore;
        c = fgetc(fp);
    }

    return list;
}

int isNumber(int elemento)
{
    if(elemento >= 0 && elemento <= 7)
        return 1;
    return 0;
}
int isWord(int elemento)
{
    if(elemento >= 8 && elemento <= 15)
        return 1;
    return 0;
}
int isRelevant(int elemento)
{
    if(elemento == 4 || elemento == 5 || elemento == 6 || elemento == 7 || elemento == 12 || elemento == 13 ||
        elemento == 14 || elemento == 15)
        return 1;
    return 0;
}
int isPrincipal(int elemento)
{
    if(elemento == 2 || elemento == 3 || elemento == 6 || elemento == 7 || elemento == 10 || elemento == 11 ||
        elemento == 14 || elemento == 15)
        return 1;
    return 0;
}
int isSecondary(int elemento)
{
    if(elemento == 0 || elemento == 1 || elemento == 4 || elemento == 5 || elemento == 8 || elemento == 9 ||
        elemento == 12 || elemento == 13)
        return 1;
    return 0;
}
int isInput(int elemento)
{
    if(elemento == 0 || elemento == 2 || elemento == 4 || elemento == 6 || elemento == 8 || elemento == 10 ||
        elemento == 12 || elemento == 14)
        return 1;
    return 0;
}
int isOutput(int elemento)
{
    if(elemento == 1 || elemento == 3 || elemento == 5 || elemento == 7 || elemento == 9 || elemento == 11 ||
        elemento == 13 || elemento == 15)
        return 1;
    return 0;
}

char* getDescription(int valore)
{
    if(valore == 0)
        return ("NUSI");
    if(valore == 1)
        return ("NUSO");
    if(valore == 2)
        return ("NUPI");
    if(valore == 3)
        return ("NUPO");
    if(valore == 4)
        return ("NRSI");
    if(valore == 5)
        return ("NRSO");
    if(valore == 6)
        return ("NRPI");
    if(valore == 7)
        return ("NRPO");
    if(valore == 8)
        return ("WUSI");
    if(valore == 9)
        return ("WUSO");
    if(valore == 10)
        return ("WUPI");
    if(valore == 11)
        return ("WUPO");
    if(valore == 12)
        return ("WRSI");
    if(valore == 13)
        return ("WRSO");
    if(valore == 14)
        return ("WRPI");
    if(valore == 15)
        return ("WRPO");
    return NULL;
}

// ritorna colonna principale INPUT
int getposPrincipal(int* arr, int el)
{ // ritorna colonna PRINCIPALE INPUT
    int pos = 0;
    for(int i = 0; i < el; i++) { // creo un indice per scorrere tutte le colonne
        if(isPrincipal(arr[i]) && isInput(arr[i]) && isRelevant(arr[i])) {
            return pos;
        }
        pos++;
    }
    printf("\nColonna principale di Input non trovata!\n");
    exit(1);
    return -1;
}

// ritorna colonna principale OUTPUT
int getposOutput(int* arr, int el)
{
    int pos = 0;
    for(int i = 0; i < el; i++) {
        if(isOutput(arr[i]) && isPrincipal(arr[i]) && isRelevant(arr[i])) {
            return pos;
        }
        pos++;
    }
    printf("\nColonna principale di Output non trovata!\n");
    exit(1);
    return -1;
}


void getNameColumn(FILE* fp, int pos)
{
    if(fp == NULL) {
        printf("File non trovato\n");
    }
    fseek(fp, 0, SEEK_SET);
    int p = 0;
    char c;
    c = fgetc(fp);
    if(pos == 0) { // stampa 1a colonna
        while(c != ';') {
            printf("%c", c);
            c = fgetc(fp);
        }
    } else {
        while(p < pos) {
            if(c == ';')
                p++;
            c = fgetc(fp);
        }
        while(c != ';' && c != '\n' && c != '\r' && c != EOF) {
            printf("%c", c);
            c = fgetc(fp);
            if(c == ';' || c == '\n' || c == '\r' || c == EOF)
                break;
        }
    }
    return;
}
