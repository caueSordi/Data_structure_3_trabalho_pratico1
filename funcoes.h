#ifndef FUNCOES_H
#define FUNCOES_H

#include "arquivos.h"
#include "registro.h"
#include "busca.h"

int funcao_CREATE(char *nomeCSV, char *nomeBin);
int funcao_READ_ALL(char *nomeBin);
int funcao_SELECT_RRN(char *nomeBin, int RRN);

#endif