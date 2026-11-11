#ifndef REGISTRO_H
#define REGISTRO_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STATUS_INCONSISTENTE '0'
#define STATUS_CONSISTENTE '1'

#define REGISTRO_NAO_REMOVIDO '0'
#define REGISTRO_REMOVIDO '1'

#define VALOR_NULO_INT -1
#define VALOR_NULO_CHAR '$'

#define LISTA_VAZIA -1LL
#define DELIMITADOR '|'

#define TAM_CABECALHO 25
#define BYTES_ANTES_TAMANHO 5 // tamanho do registro removido + tamanhoRegistro
#define BYTES_FIXOS_APOS_TAMANHO 14 // encadeamento(8) + idPoPs(4) + siglaPais(2)
// atencao: assume que tamanhoRegistro nao conta os 5 primeiros bytes

#define TAM_SIGLA 2
#define TAM_MAX_STRING 256
#define TAM_NOME_CAMPO 20


//definindo a struct cabecalho
// 25 bytes: status(1), topoLista(8), proxByteOffset(8), nroRegRem(4), nroPoPs(4) 
typedef struct  {
char status; 
long long topoLista; // byte offset do primeiro removido, -1 se nao ha 
long long proxByteOffset;

int nroRegRem;
int nroPoPs;
}Cabecalho;



typedef struct 
{
char removido;
int tamanhoRegistro;
long long encadeamento; // byte offset do proximo removido da lista
int idPoPs;
char siglaPais[TAM_SIGLA + 1]; // 2 chars + '\0' em memoria, nulo = "$$" 
char nomePoPs[TAM_MAX_STRING]; // vazio = nulo 
char nomePais[TAM_MAX_STRING]; // vazio = nulo

} Registro ; 

void Inicializa_registro(Registro *reg);
//coloca o registro em um estado vazio conhecido antes de inserir dados

int Ler_registro(FILE *arquivo, Registro *reg); 
// le registro ativo ou removido e deixa o ponteiro no proximo registro

int Ler_registro_offset(FILE *arquivo, long long offset, Registro *reg);

//le um registro do arquivo, por campo e retorna 1 se leu ou 0 se deu erro

// completa com '$' ate tamanhoRegistro, retorna 0 se os dados nao couberem
int Escrever_registro(FILE *arquivo, const Registro *reg);
//escreve um registro no arquivo com todos os campos em ordem

void Imprimir_registro(const Registro *reg);

// valorCampo precisa ter TAM_MAX_STRING colunas, por causa dos campos string
int registro_SatisfazCriterio(const Registro *reg, char nomeCampo[][TAM_NOME_CAMPO], char valorCampo[][TAM_MAX_STRING], int quantCampos);

int campoIntCMP(const char *valorTexto);

void Inicializa_cabecalho(Cabecalho *cab);
int Ler_Cabecalho(FILE *arquivo, Cabecalho *cab);
void Escrever_Cabecalho(FILE *arquivo, const Cabecalho *cab);

int calcula_tamanho_registro(const Registro *reg);
int tamanho_total_registro(const Registro *reg);


#endif