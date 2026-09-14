<<<<<<< Updated upstream
=======
#include "funcoes.h"
#include <stdio.h>
#include <string.h>
#define nomeCSV_esperado "conectaPoPs.csv"
int main(){

    char nomeCSV[50], nomeBin[50];
    int func=0;
    scanf("%d", &func);

    if(func == 1)
    {
        scanf("%s %s", nomeCSV, nomeBin);

        if(!strcmp(nomeCSV, nomeCSV_esperado)){
            funcao_CREATE(nomeCSV, nomeBin);
        }else{
            printf("Falha no processamento do arquivo.\n");
        }     
    }
}
>>>>>>> Stashed changes
