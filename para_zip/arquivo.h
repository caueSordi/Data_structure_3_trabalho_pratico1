#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <stdio.h>
#include "registro.h"

#define TAMANHO_STATUS 1
#define TAMANHO_TOPO_PILHA 4
#define TAMANHO_PROXIMO_RRN 4
#define TAMANHO_NMRO_REGREM 4
#define TAMANHO_NMRO_PARES 4

#define TAMANHO_CABECALHO (TAMANHO_STATUS + TAMANHO_TOPO_PILHA + \
                           TAMANHO_PROXIMO_RRN + TAMANHO_NMRO_REGREM + \
                           TAMANHO_NMRO_PARES)

#define STATUS_CONSISTENTE '1'
#define STATUS_INCONSISTENTE '0'

void arquivo_Abertura(FILE **pontArquivo, char *nome, char *modo);
void arquivo_Close(FILE **arquivo);
void arquivo_inicio(FILE *arquivo);

int arquivo_EscreveChar(FILE *pontArquivo, char dado);
int arquivo_EscreveInt(FILE *pontArquivo, int dado);

int arquivo_LeChar(FILE *pontArquivo, char *dado);
int arquivo_LeInt(FILE *pontArquivo, int *dado);

int csv_LeLinha(FILE *arqCSV, int *idPoPs,
                int *idPoPsConectado,
                int *velocidade,
                char *unidadeMedida);

void Inicializa_Cabecalho(Cabecalho *cab);
int Ler_Cabecalho(FILE *arquivo, Cabecalho *cab);
void Escrever_Cabecalho(FILE *arquivo, Cabecalho *cab);

long RRN_posicao(int rrn);
int Ler_registro_rrn(FILE *arquivo, int rrn, Registro *reg);

void ScanQuoteString(char *str);
void BinarioNaTela(char *arquivo);

#endif