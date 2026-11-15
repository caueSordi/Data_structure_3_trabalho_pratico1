#ifndef FUNCOES_H
#define FUNCOES_H

#include "busca.h"

int funcao_CREATE(char *nomeCSV, char *nomeBin); // modificado para agora o CREATE TABLE (8)

int funcao_CREATE_INDEX(char *nomeBin, char *nomeIndice); //case 9

int funcao_READ_ALL(char *nomeBin); //agora e o SELECT (10) do TP2
 
int funcao_READVALUE(char *nomeBin, char *nomeIndice, int nBuscas);// case 11 da busca sequencial e indexada

void remove_chaveIdx(RegistroIndice *Idx, int *qtdIdx, int idRem);
void remocao_logica(FILE *ArqBin, RegistroIndice *Idx, int *qtdIdx, Cabecalho *cab, long long posReg, int tamReg, int id);
void funcao_DELETE(char *nomeBin, char *nomeIndice, int n); //funcao 12: delete campos

void insere_chaveIdx(RegistroIndice **Idx, int *qtdIdx, int id, long long offset);
void funcao_INSERT(char *nomeBin, char *nomeIdx, int n);


#endif

