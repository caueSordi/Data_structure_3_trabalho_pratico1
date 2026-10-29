#include "funcoes.h"
#include <stdio.h>
#include <string.h>
 
int main(){
 
char nomeCSV[50], nomeBin[50], nomeIndice[50]; //nomeIndice adicionado
int func = 0, n = 0; // n para quantidade de buscas
scanf("%d", &func);
 
switch(func) {
case 8: // alterado: era case 1 
scanf("%s %s", nomeCSV, nomeBin);
funcao_CREATE(nomeCSV, nomeBin);
break;

//funcionalidade 9 Create_index
case 9: 
scanf("%s %s", nomeBin, nomeIndice);
funcao_CREATE_INDEX(nomeBin, nomeIndice);
break;

// case 2 modificado para 10
case 10: 
scanf("%s", nomeBin);
funcao_READ_ALL(nomeBin);
break;

//case 11
case 11: 
scanf("%s %s %d", nomeBin, nomeIndice, &n);
funcao_READVALUE(nomeBin, nomeIndice, n);
break;
}
 
return 0;
}