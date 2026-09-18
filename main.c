#include "funcoes.h"
#include <stdio.h>
#include <string.h>
#define nomeCSV_esperado "conectaPoPs.csv"
int main(){

    char nomeCSV[50], nomeBin[50];
    int func=0, RRN=0, quantBusca=0;
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

    if (func == 2)
    {
        scanf("%s", nomeBin);
        funcao_READ_ALL(nomeBin);
    }
    if(func == 3){
        scanf("%s %d", nomeBin,  &quantBusca);
        funcao_READVALUE(quantBusca, nomeBin);
    }
    if (func == 4)
    {
        scanf("%s %d", nomeBin, &RRN);
        funcao_SELECT_RRN(nomeBin, RRN);
    }

    if (func == 5)
    {
        scanf("%s", nomeBin);
        funcao_DELETE(nomeBin);
    }
}