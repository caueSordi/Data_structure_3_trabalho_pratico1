#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <stdio.h>
#include "registro.h"

// definicao dos cabecalhos
#define TAMANHO_STATUS  1
#define TAMANHO_TOPO_PILHA 4
#define TAMANHO_PROXIMO_RRN 4
#define TAMANHO_NMRO_REGREM 4
#define TAMANHO_NMRO_PARES 4

#define TAMANHO_CABECALHO (TAMANHO_STATUS + TAMANHO_TOPO_PILHA + \
                            TAMANHO_PROXIMO_RRN + TAMANHO_NMRO_REGREM + TAMANHO_NMRO_PARES)

#define STATUS_CONSISTENTE '1'
#define STATUS_INCONSISTENTE '0'

typedef struct 
{
    char status;
    int prox_RRN;
    int topo_pilha;
    int nmro_registro_removidos;
    int nmro_pares;
} Cabecalho;

void Inicializa_Cabecalho(Cabecalho *cab);

int Ler_Cabecalho(FILE *arquivo, Cabecalho *cab);

void Escrever_Cabecalho(FILE *arquivo, Cabecalho *cab);

long RRN_posicao(int rrn);

int Ler_registro_rrn(FILE *arquivo, int rrn, Registro *reg);


#endif