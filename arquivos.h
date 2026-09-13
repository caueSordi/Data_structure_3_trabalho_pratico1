#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include <stdio.h>

// Abre um arquivo no modo especificado e armazena o ponteiro em *pontArquivo.
void arquivo_Abertura(FILE **pontArquivo, char *nome, char *modo);

// Fecha o arquivo apontado por *arquivo.
void arquivo_Close(FILE **arquivo);

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
















#endif 