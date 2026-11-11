#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include <stdio.h>
#include "registro.h"

// abertura, fechamento e posicionamento @

// abre o arquivo, retorna 1 se abriu e 0 se falhou @
int       arquivo_Abertura(FILE **arq, const char *nome, const char *modo);

// fecha o arquivo e zera o ponteiro @
void      arquivo_Close(FILE **arq);

// vai para o byte 0 @
void      arquivo_inicio(FILE *arq);

// vai para logo depois do cabecalho @
void      arquivo_PosCabecalho(FILE *arq);

// vai para um byte offset @
void      arquivo_Posiciona(FILE *arq, long long offset);

// posicao atual do arquivo @
long long arquivo_PosicaoAtual(FILE *arq);

// escrita, um campo por chamada @

void arquivo_EscreveChar(FILE *arq, char c);
void arquivo_EscreveInt(FILE *arq, int valor);
void arquivo_EscreveLongLong(FILE *arq, long long valor);

// escreve a string sem o '\0' e sem o '|' @
void arquivo_EscreveString(FILE *arq, const char *s);

// escreve n bytes '$' @
void arquivo_EscreveLixo(FILE *arq, int n);

// leitura, retorna 1 se leu e 0 se deu erro ou fim de arquivo @

int arquivo_LeChar(FILE *arq, char *c);
int arquivo_LeInt(FILE *arq, int *valor);
int arquivo_LeLongLong(FILE *arq, long long *valor);

// le ate o '|' (consumido e nao copiado), trunca se passar de tamMax @
int arquivo_LeStringAteDelim(FILE *arq, char *dest, int tamMax);

// csv @

// le a proxima linha do CSV e preenche o registro, retorna 0 no fim do arquivo @
// colunas assumidas: idPoPs,nomePoPs,nomePais,siglaPais, confira no seu .csv @
int csv_LeLinha(FILE *arqCSV, Registro *reg);

#endif