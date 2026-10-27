#ifndef FUNCOES_H
#define FUNCOES_H

#include "arquivos.h"
#include "registro.h"
#include "busca.h"


int funcao_CREATE(char *nomeCSV, char *nomeBin); // alterado: agora e o CREATE TABLE (8) do TP2
int funcao_READ_ALL(char *nomeBin); // alterado: agora e o SELECT (10) do TP2
 

#endif