#include "arquivos.h"
#include <string.h>
#include <stdlib.h>

// o layout do arquivo exige estes tamanhos @
_Static_assert(sizeof(long long) == 8, "long long precisa ter 8 bytes");
_Static_assert(sizeof(int) == 4, "int precisa ter 4 bytes");

#define TAM_LINHA_CSV 1024
#define NUM_COLUNAS_CSV 4

// abertura, fechamento e posicionamento @

int arquivo_Abertura(FILE **arq, const char *nome, const char *modo)
{
    *arq = fopen(nome, modo);
    return *arq != NULL;
}

void arquivo_Close(FILE **arq)
{
    if (*arq != NULL) {
        fclose(*arq);
        *arq = NULL;
    }
}

void arquivo_inicio(FILE *arq)
{
    fseek(arq, 0, SEEK_SET);
}

void arquivo_PosCabecalho(FILE *arq)
{
    fseek(arq, TAM_CABECALHO, SEEK_SET);
}

void arquivo_Posiciona(FILE *arq, long long offset)
{
    fseek(arq, (long)offset, SEEK_SET);
}

long long arquivo_PosicaoAtual(FILE *arq)
{
    return (long long)ftell(arq);
}

// escrita @

void arquivo_EscreveChar(FILE *arq, char c)
{
    fwrite(&c, sizeof(char), 1, arq);
}

void arquivo_EscreveInt(FILE *arq, int valor)
{
    fwrite(&valor, sizeof(int), 1, arq);
}

void arquivo_EscreveLongLong(FILE *arq, long long valor)
{
    fwrite(&valor, sizeof(long long), 1, arq);
}

void arquivo_EscreveString(FILE *arq, const char *s)
{
    size_t n = strlen(s);
    if (n > 0)
        fwrite(s, sizeof(char), n, arq);
}

void arquivo_EscreveLixo(FILE *arq, int n)
{
    for (int i = 0; i < n; i++)
        arquivo_EscreveChar(arq, VALOR_NULO_CHAR);
}

// leitura @

int arquivo_LeChar(FILE *arq, char *c)
{
    return fread(c, sizeof(char), 1, arq) == 1;
}

int arquivo_LeInt(FILE *arq, int *valor)
{
    return fread(valor, sizeof(int), 1, arq) == 1;
}

int arquivo_LeLongLong(FILE *arq, long long *valor)
{
    return fread(valor, sizeof(long long), 1, arq) == 1;
}

int arquivo_LeStringAteDelim(FILE *arq, char *dest, int tamMax)
{
    int n = 0;
    int c;

    while ((c = fgetc(arq)) != EOF) {
        if (c == DELIMITADOR) {
            dest[n] = '\0';
            return 1;
        }
        // o que passar do limite e descartado @
        if (n < tamMax - 1)
            dest[n++] = (char)c;
    }

    // acabou o arquivo antes do '|' @
    dest[n] = '\0';
    return 0;
}

// csv @

// copia a string sem estourar o tamanho do destino @
static void copia_string(char *destino, const char *origem, int tamMax)
{
    strncpy(destino, origem, tamMax - 1);
    destino[tamMax - 1] = '\0';
}

int csv_LeLinha(FILE *arqCSV, Registro *reg)
{
    char linha[TAM_LINHA_CSV];

    // pula linhas em branco @
    do {
        if (fgets(linha, sizeof(linha), arqCSV) == NULL)
            return 0;
        linha[strcspn(linha, "\r\n")] = '\0';
    } while (linha[0] == '\0');

    Inicializa_registro(reg);

    // separa as colunas sem usar strtok, para nao perder as vazias @
    char *campos[NUM_COLUNAS_CSV];
    int n = 0;
    char *p = linha;

    campos[n++] = p;
    while (*p != '\0' && n < NUM_COLUNAS_CSV) {
        if (*p == ',') {
            *p = '\0';
            campos[n++] = p + 1;
        }
        p++;
    }

    // colunas que faltaram apontam para o fim da linha, ou seja, vazias @
    for (int i = n; i < NUM_COLUNAS_CSV; i++)
        campos[i] = p;

    // ordem das colunas: idPoPs, nomePoPs, nomePais, siglaPais @
    reg->idPoPs = (campos[0][0] == '\0') ? VALOR_NULO_INT : atoi(campos[0]);
    copia_string(reg->nomePoPs, campos[1], TAM_MAX_STRING);
    copia_string(reg->nomePais, campos[2], TAM_MAX_STRING);

    // sigla vazia continua "$$" (nulo) @
    if (campos[3][0] != '\0') {
        reg->siglaPais[0] = campos[3][0];
        reg->siglaPais[1] = (campos[3][1] != '\0') ? campos[3][1] : VALOR_NULO_CHAR;
    }

    return 1;
}