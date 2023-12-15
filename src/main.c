#include <execution.h>
#include <definitions.h>
#include <stdio.h>
#include <stdlib.h>
#include <test.h>

int verbose; // is an "extern" variable in definition.h

void usage(int argc, char** argv)
{

    printf("Usage: %s <test file> <description file> <verbose flag>\n\n", argv[0]);
    printf("Where: \n");
    printf("       <test file>   The name of a comma separated file containing\n");
    printf("                     the data to be analyzed. The first line of the\n");
    printf("                     file must contain the names of the columns.\n");
    printf("                     The only admitted separator is a semicolon \";\".\n");
    printf("<description file>   Contains a single row and a column for each column of the <test file>.\n");
    printf("                     Describes the role of each column.\n");
    printf("    <verbose flag>   May be 0 or 1. 0 stands for \"non-verbose\"\n");
    printf("                     1 stands for \"verbose\"\n");
}

int main(int argc, char** argv)
{

    if(argc < 4) {                                   // se metto fino a 3 stringhe
        printf("%s: Too few parameters\n", argv[0]); // argv[0] sarebbe rome-test-analyzer
        usage(argc, argv);                           // messaggio iniziale
        exit(1); // uscita negativa dal programma (invece exit(0) indica una uscita positiva)
    }

    if(argc > 4) {                           // se metto piu' di 4 stringhe
        printf("%s: Too many parameters\n"); // ho troppi parametri
        usage(argc, argv);                   // messaggio iniziale
        exit(1);                             // uscita
    }

    /* qui sono nella condizione in cui ho esattamente 4 parametri ( che e' corretto ai fini del programma)*/
    verbose = atoi(argv[3]);                   // la funzione atoi mi converte la stringa in un intero
    if(verbose != 1 && verbose != 0) {         // se questo numero non e' nè 0 nè 1
        printf("%d: Invalid flag\n", verbose); // il flag inserito non è valido
        usage(argc, argv);
        exit(1);
    }

    FILE* chitable = fopen("chitablemax.txt", "r"); // chitable è un puntatore ad un oggetto di tipo file che trovo nel
                                                    // percorso indicato e mi serve solo in read(lettura)
    if(chitable == NULL) {                          // se è nullo
        printf("%s: File \"chitablemax.txt\" inesistente\n", argv[0]);
        usage(argc, argv);
        exit(1);
    }

    FILE* filetest = fopen(argv[1], "r"); // sto aprendo ad esempio il file fumatori.csv in lettura
    if(filetest == NULL) {
        printf("%s: File test inesistente\n", argv[0]);
        usage(argc, argv);
        exit(1);
    }

    FILE* filesupport = fopen(argv[2], "r"); // sto aprendo il file fumatorisupporto.csv
    if(filesupport == NULL) {
        printf("%s: File supporto inesistente\n", argv[0]);
        usage(argc, argv);
        exit(1);
    }

    int verbose = atoi(argv[3]); // converto in intero il quarto parametro che sarebbe il flag 0 o 1

    do_test(filetest, filesupport, chitable); // LANCIO LA FUNZIONE PRINCIPALE IN CUI:
                                              // IL PRIMO PARAMETRO E' fumatori.csv
                                              // IL SECONDO PARAMETRO E' fumatorisupporto.csv
                                              // IL TERZO PARAMETRO E' chitablemax.txt
                                              /* CHIUDO TUTTI I FILE*/
    fclose(chitable);
    fclose(filetest);
    fclose(filesupport);

    return 0;
}
