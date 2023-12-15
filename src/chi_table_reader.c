#include <chi_table_reader.h>
#include <gsl/gsl_cdf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// RITORNA NUMERO DI COLONNE CHAR TABLE

int getColumnNum(FILE* fp)
{
    if(fp == NULL) {
        printf("File non trovato\n");
        return -1;
    }
    int num = 0;
    fseek(fp, 0, SEEK_SET);
    char el;
    el = fgetc(fp);
    while(el != EOF) {
        if(el == 9)
            num++;
        if(el == '\n' || el == '\r')
            break;
        el = fgetc(fp);
    }
    return num + 1;
}

// RITORNA NUMERO DI RIGHE CHAR TABLE

int getRowNum(FILE* fp)
{
    if(fp == NULL) {
        printf("File non trovato\n");
        return -1;
    }
    int num = 0;
    fseek(fp, 0, SEEK_SET);
    char el;
    el = fgetc(fp);
    while(el != EOF) {
        if(el == '\n')
            num++;
        el = fgetc(fp);
    }
    return num + 1;
}

// RITORNA POSIZIONE IN COLONNA DI VALORE SL

int getColPos(FILE* fp, double sl)
{ // RITORNA POSIZIONE COLONNA DELL'ELEMENTO CON VALORE SL, -1 IN CASO DI ERRORE
    if(fp == NULL) {
        printf("File non trovato\n");
        return -1;
    }
    fseek(fp, 0, SEEK_SET);
    int pos = 0;
    int arrpos = 0;
    char temp[20];
    char c;
    c = fgetc(fp);
    while(c != EOF) {
        if(c != '\n' && c != '\r')
            while(c != 9) { // CICLO PER COPIARE IL NUMERO IN I-COLONNA IN ARRAY
                if(arrpos < 20) {
                    temp[arrpos] = c;
                    arrpos += 1;
                }
                c = fgetc(fp);
            }
        if(c == 9) {
            if(atof(temp) == sl)
                return pos + 1;
            arrpos = 0;
            memset(temp, 0, sizeof(temp));
            c = fgetc(fp);
            pos++;
        }
        if(c == '\n' || c == '\r')
            break;
    }
    printf("Valore non trovato\n");
    return -1;
}

// RITORNA POSIZIONE IN RIGA DI DF

int getRowPos(FILE* fp, int df)
{ // RITORNA POSIZIONE RIGA DELL'ELEMENTO CON VALORE D, -1 IN CASO DI ERRORE
    if(fp == NULL) {
        printf("File non trovato\n");
        return -1;
    }
    fseek(fp, 0, SEEK_SET);
    int pos = 0;
    int arrpos = 0;
    char temp[20];
    char c;
    c = fgetc(fp);
    while(c != EOF) {
        if(c != 9) {
            if(arrpos < 20) {
                temp[arrpos] = c;
                arrpos++;
            }
            c = fgetc(fp);
        }
        if(c == 9) {
            if(atof(temp) == df)
                return pos + 1;
            else {
                arrpos = 0;
                memset(temp, 0, sizeof(temp));
                while(c != '\r' && c != EOF)
                    c = fgetc(fp);
                pos++;
            }
        }
        if(c == '\n') {
            pos++;
            arrpos = 0;
            memset(temp, 0, sizeof(temp));
            c = fgetc(fp);
        }
        if(c == '\r') {
            c = fgetc(fp);
        }
    }
    printf("Valore %d non trovato\n", df);
    return -1;
}

// RITORNA CRITICAL VALUE DALLA CHI TABLE

double getCriticalValue(FILE* fp, int df, int sl)
{
    if(fp == NULL) {
        printf("File non trovato\n");
        return -1;
    }
    fseek(fp, 0, SEEK_SET);
    char temp[20];
    int tempos = 0;
    char c;
    int row = 0;
    int col = 0;
    c = fgetc(fp);

    if(df == 0 || df < 0 || df > 51) {
        printf("Valore Degrees of Freedom DF invalido");
        return -1;
    }
    if(sl == 0 || sl < 0 || sl > 12) {
        printf("Valore Degrees of Freedom DF invalido");
        return -1;
    }
    while(row < df || c != EOF) {
        c = fgetc(fp);
        if(c == '\n')
            row += 1;
        if(row == df - 1)
            break;
    }
    while(col < sl || c != EOF || c != '\n' || c != '\r') {
        if(c == 9)
            col++;
        c = fgetc(fp);
        if(col == sl - 1)
            break;
    }

    while(c != 9 || c != '\n' || c != '\r' || c != EOF) {
        if(tempos < 20) {
            temp[tempos] = c;
            tempos++;
        }
        c = fgetc(fp);
        if(c == 9 || c == EOF)
            return (atof(temp));
    }
    printf("Errore valore non trovato\n");
    return -1;
}

// STAMPA RISULTATO TEST

void getChiTestResult(double y1, double y2, double x1, double x2, double chiv, double df, char*** matrice, int posInput, int posOutput)
{ // gli passo in input il valore df, fondamentale per il calcolo del p-value esatto

    //    // con questi valori in input posso solamente dire tra quali valori è compreso il p-value
    //    double result = y1 + ((y2 - y1) * (chiv - x1) / (x2 - x1));
    //
    //    if(verbose == 1)
    //        printf("\np-value ottenuto= %lf\nLa probabilità ottenuta dal Chi Test è del: %lf%c\n", result,
    //            100 - result * 100, '%');
    //    else
    //        printf("%.2lf%c\n", 100 - result * 100, '%');
    //    return;

 if(verbose == 1) {
        double result = gsl_cdf_chisq_Q(chiv, df);
        printf("\nLeggendo la tabella posso solamente dire tra quali valori e' compreso il p-value");
        printf("\n%lf <= p-value <= %lf", y2, y1);
        printf("\n\nIl valore preciso del p-value calcolato\ntramite la funzione gsl_cdf_chisq_Q e': %lf", result);
        printf("\nIl valore complementare del p-value e' : %lf", gsl_cdf_chisq_P(chiv, df));
        if(result <= 0.05) { // rifiuto dell'ipotesi nulla
            printf("\n\nPosso rifiutare l'ipotesi nulla e\naffermare con una confidenza del 95%% che\nESISTE UNA "
                   "DIFFERENZA STATISTICAMENTE SIGNIFICATIVA ");
            printf("TRA l'INPUT: %s E l'OUTPUT: %s\n\n", matrice[0][posInput], matrice[0][posOutput]);
        }
        if(result > 0.05) { // accetto dell'ipotesi nulla
            printf("\n\nNon ho sufficienti prove per rifiutare l'ipotesi nulla,\npercio' posso affermare con una confidenza "
                   "del 95%% che\n");
            printf("NON ESISTE UNA DIFFERENZA STATISTICAMENTE SIGNIFICATIVA TRA L'INPUT: %s E L'OUTPUT: %s\n\n",
                matrice[0][posInput], matrice[0][posOutput]);
        }
    } else {
        printf("\n%lf <= p-value <= %lf", y2, y1);
    }
    return;
}



double getSignificanceLevel(FILE* fp, int pos)
{
    if(fp == NULL) {
        printf("File non trovato\n");
        return -1;
    }
    fseek(fp, 0, SEEK_SET);
    char temp[20];
    int tempos = 0;
    char c;
    int p = 1;
    if(pos < 1 || pos > 12) {
        printf("posizione SL invalida\n");
        return -1;
    }
    while(p < pos) {
        c = fgetc(fp);
        if(c == 9)
            p++;
    }
    c = fgetc(fp); // Mi sposto in posizione scelta con pos sul primo numero
    while(c != 9 || c == '\r' || c == '\n' || c != EOF) {
        if(tempos < 20) {
            temp[tempos] = c;
            tempos++;
        }
        c = fgetc(fp);
        if(c == '\r' || c == '\n' || c == EOF)
            break;
    }
    return atof(temp);
}

void chiTestCompare(FILE* fp, double chivalue, int df, char*** matrice, int posInput, int posOutput)
{
    if(fp == NULL) {
        printf("File non trovato\n");
        return;
    }
    fseek(fp, 0, SEEK_SET);
    char temp1[20];
    char temp2[20];
    int pos1 = 0;
    int pos2 = 0;
    int tempos = 0;
    char c;
    int row = 0;
    int stop = 0;

    if(df == 0 || df < 0 || df > 250) {
        printf("Valore Degrees of Freedom DF invalido.Valore= %d non ammesso\n", df);
        return;
    }
    c = fgetc(fp);
    while(row < df || c != EOF) { // mi sposto sulla riga pari a DF

        c = fgetc(fp);
        if(c == '\n')
            row += 1;
        if(row == df)
            break;
    }

    while(c == '\n' || c == '\r') // mi sposto sul primo numero della riga
        c = fgetc(fp);
    while(c != 9) { // Deve superare il numero in prima colonna che indica il df
        c = fgetc(fp);
    }
    c = fgetc(fp);
    while(c != 9) { // salvo primo numero e effettuo la prima comparazione. Se ha successo mi fermo altrimenti continuo

        if(tempos < 20) {
            temp1[tempos] = c;
            tempos++;
        }
        c = fgetc(fp);
    }
    if(atof(temp1) == chivalue) { // caso in cui chi value < del valore della prima colonna
        pos1 = 2;
        pos2 = -2;
    } else if(atof(temp1) > chivalue) { // caso in cui chi value < del valore della prima colonna
        pos1 = 2;
        pos2 = -1;
    } else {
        pos1 = 2;
        pos2 = 1;

        while(stop != 1) {

            if(c == '\n' || c == '\r') {
                pos1 = -1;
                pos2 = -1;
                break;
            }

            c = fgetc(fp);

            for(int i = 0; i < 20; i++) // copio i valori di temp1 in temp2
                temp2[i] = temp1[i];
            tempos = 0;
            memset(temp1, 0, sizeof(temp1)); // resetto temp1

            while(c != 9 && c != EOF && c != '\n' && c != '\r') { // salvo prossimo numero in temp1
                if(tempos < 20) {
                    temp1[tempos] = c;
                    tempos++;
                }
                c = fgetc(fp);
            }
            pos1++;
            pos2++;
            if(atof(temp1) > chivalue && atof(temp2) < chivalue)
                break;
            if(atof(temp1) < chivalue &&
                c == '\r') { // Caso in cui ultimo numero in colonna 12 è minore di chi value (pos1=12 pos2=-1)
                pos2 = -1;
                break;
            }
            if(atof(temp1) == chivalue) { // Caso in cui chi value==valore in tabella
                pos2 = -2;
                break;
            }
        }
    }
    // FINE ELSE. ORA SI ANALIZZANO I CASI DEI VARI VALORI DI POS1 E POS2

    if(pos1 == -1 && pos2 == -1) {
        if(verbose == 1)
            printf("Caso1:Non contenuto\n"); // DA TOGLIERE
        printf("Errore nel calcolo della probabilità:Risultato impossibile\n");
        return;
    }
    if(pos1 == 2 && pos2 == -1) { // QUANDO CHI VALUE = EL IN COLONNA
        if(verbose == 1) {
            printf("Caso2:Valore<colonna 1\n"); // DA TOGLIERE
            printf("DF=%d ", df);               // DA TOGLIERE
            printf("Y2=%lf - X1=%lf - CHI VALUE=%lf\n", getSignificanceLevel(fp, pos1), atof(temp1), chivalue);
        }
        getChiTestResult(0, getSignificanceLevel(fp, pos1), 0, atof(temp1), chivalue, df, matrice, posInput, posOutput);
        return;
    }
    if(pos2 == -2) { // QUANDO CHI VALUE = EL IN COLONNA
        if(verbose == 1) {
            printf("Caso3:Valore==colonna\n"); // DA TOGLIERE
            printf("DF=%d ", df);              // DA TOGLIERE
            printf("Y2=%lf - X1=%lf - CHI VALUE=%lf\n", getSignificanceLevel(fp, pos1), atof(temp1), chivalue);
        }
        getChiTestResult(0, getSignificanceLevel(fp, pos1), 0, atof(temp1), chivalue, df, matrice, posInput, posOutput);
        return;
    }
    if(pos1 == 12 && pos2 == -1) { // QUANDO CHI VALUE > EL IN 12A COLONNA (ultima colonna)
        if(verbose == 1) {
            printf("Caso4:Valore>Ultima colonna\n"); // DA TOGLIERE
            printf("DF=%d ", df);                    // DA TOGLIERE
            printf("Y2=%lf - X1=%lf - CHI VALUE=%lf\n", getSignificanceLevel(fp, pos1), atof(temp1), chivalue);
        }
        getChiTestResult(0, getSignificanceLevel(fp, pos1), 0, atof(temp1), chivalue, df, matrice, posInput, posOutput);
        return;
    }
    /*caso in cui il chi value è compreso tra due valori*/
    if(pos1 < 13 && pos2 > 1) {
        if(verbose == 1) {
            printf("Caso5:Valori compresi tra prima e ultima colonna\n"); // DA TOGLIERE
            printf("DF=%d ", df);                                         // DA TOGLIERE
            printf("Y1=%lf - Y2=%lf - X2=%lf - X1=%lf - CHIV=%lf\n", getSignificanceLevel(fp, pos2),
                getSignificanceLevel(fp, pos1), atof(temp1), atof(temp2), chivalue);
        }
        getChiTestResult(
            getSignificanceLevel(fp, pos2), getSignificanceLevel(fp, pos1), atof(temp2), atof(temp1), chivalue, df, matrice, posInput, posOutput);
    }
}
