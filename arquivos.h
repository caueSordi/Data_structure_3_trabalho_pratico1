#ifndef ARQUIVOS_H
#define ARQUIVOS_H

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


// Abre um arquivo no modo especificado e armazena o ponteiro em *pontArquivo.
void arquivo_Abertura(FILE **pontArquivo, char *nome, char *modo);

// Fecha o arquivo apontado por *arquivo.
void arquivo_Close(FILE **arquivo);

//volta para o inicio do arquivo
void arquivo_inicio(FILE *arquivo);

// Escreve um campo do tipo char (1 byte) no arquivo, na posição atual do cursor.
// Retorna 1 em sucesso, 0 em falha.
int arquivo_EscreveChar(FILE *pontArquivo, char dado);

// Escreve um campo do tipo int (4 bytes) no arquivo, na posição atual do cursor.
// Retorna 1 em sucesso, 0 em falha.
int arquivo_EscreveInt(FILE *pontArquivo, int dado);

// Lê um campo do tipo char (1 byte) do arquivo, na posição atual do cursor,
// armazenando o valor lido em *dado.
// Retorna 1 em sucesso, 0 em falha ou fim de arquivo (EOF).
int arquivo_LeChar(FILE *pontArquivo, char *dado);

// Lê um campo do tipo int (4 bytes) do arquivo, na posição atual do cursor,
// armazenando o valor lido em *dado.
// Retorna 1 em sucesso, 0 em falha ou fim de arquivo (EOF).
int arquivo_LeInt(FILE *pontArquivo, int *dado);


int csv_LeLinha(FILE *arqCSV, int *idPoPs, int *idPoPsConectado, int *velocidade, char *unidadeMedida);

void Inicializa_Cabecalho(Cabecalho *cab);

int Ler_Cabecalho(FILE *arquivo, Cabecalho *cab);

void Escrever_Cabecalho(FILE *arquivo, Cabecalho *cab);

long RRN_posicao(int rrn);

int Ler_registro_rrn(FILE *arquivo, int rrn, Registro *reg);

void ScanQuoteString(char *str);
void BinarioNaTela(char *arquivo);











#endif 