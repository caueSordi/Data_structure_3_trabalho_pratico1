#ifndef FUNCOES_H
#define FUNCOES_H

#include "arquivos.h"
#include "registro.h"
#include "busca.h"


int funcao_CREATE(char *nomeCSV, char *nomeBin); // modificado para agora o CREATE TABLE (8)
int funcao_READ_ALL(char *nomeBin); //agora e o SELECT (10) do TP2
 
int funcao_CREATE_INDEX(char *nomeBin, char *nomeIndice); //case 9
int funcao_READVALUE(char *nomeBin, char *nomeIndice, int nBuscas);// case 11 da busca sequencial e indexada

#endif