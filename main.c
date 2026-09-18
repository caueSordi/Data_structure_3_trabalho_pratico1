#include "funcoes.h"
#include <stdio.h>
#include <string.h>
#define nomeCSV_esperado "conectaPoPs.csv"
int main(){

    char nomeCSV[50], nomeBin[50];
    int func=0, RRN=0, quantBusca=0;
    scanf("%d", &func);

    switch(func)
    {case 1:
    
        scanf("%s %s", nomeCSV, nomeBin);

        if(!strcmp(nomeCSV, nomeCSV_esperado)){
            funcao_CREATE(nomeCSV, nomeBin);
        }else{
            printf("Falha no processamento do arquivo.\n");
        }
        break;     
    
    case 2:
        scanf("%s", nomeBin);
        funcao_READ_ALL(nomeBin);
        break;
   case 3:
   
        scanf("%s %d", nomeBin,  &quantBusca);
        funcao_READVALUE(quantBusca, nomeBin);
        break;
   case  4:
    break;
        scanf("%s %d", nomeBin, &RRN);
        funcao_SELECT_RRN(nomeBin, RRN);
        break;
   case 5: 
        scanf("%s", nomeBin);
        funcao_DELETE(nomeBin);
        break;
    case 6:
        scanf("%s %d", nomeBin, &quantBusca);
        funcao_INSERT(nomeBin, quantBusca);
        break;
    case 7: 
        scanf("%s %d", nomeBin, &quantBusca);
        funcao_UPDATE(nomeBin, quantBusca);

    }

return 0;
}