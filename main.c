#include "funcoes.h"
#include <stdio.h>

int main(){

    char nomeCSV[50], nomeBin[50];
    int func=0;
    scanf("%d", &func);

    if(func == 1)
    {
        scanf("%s %s", nomeCSV, nomeBin);

        funcao_CREATE(nomeCSV, nomeBin);
        
    }
}