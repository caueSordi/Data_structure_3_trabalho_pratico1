// Aluno 1: Caue Sordi Paulino - 14564520
//Aluna 2: Luiza Rodrigues Cardoso - 14593332
#include "funcoes.h"
#include <stdio.h>
#include <string.h>
 
int main(){
 
char nomeCSV[50], nomeBin[50];
int func = 0;
scanf("%d", &func);
 
switch(func) {
case 8: // alterado: era case 1 @
scanf("%s %s", nomeCSV, nomeBin);
funcao_CREATE(nomeCSV, nomeBin);
break;     
case 10: // alterado: era case 2 @
scanf("%s", nomeBin);
funcao_READ_ALL(nomeBin);
break;
}
 
return 0;
}
