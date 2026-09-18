#ifndef REGISTRO_H
#define REGISTRO_H
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TAMANHO_REMOVIDO    1
#define TAMANHO_ENCADEAMENTO 4
#define TAMANHO_ID_POPS     4
#define TAMANHO_ID_POPS_CONECTADO   4
#define TAMANHO_VELOCIDADE 4
#define TAMANHO_UNIDADE_MEDIDA 1

#define TAMANHO_REGISTRO (TAMANHO_REMOVIDO + TAMANHO_ENCADEAMENTO + TAMANHO_ID_POPS + TAMANHO_ID_POPS_CONECTADO + TAMANHO_VELOCIDADE + TAMANHO_UNIDADE_MEDIDA)

#define REGISTRO_NAO_REMOVIDO '0'
#define REGISTRO_REMOVIDO '1'

#define VALOR_NULO_INT -1

#define VALOR_NULO_CHAR '$'


//definindo a struct cabecalho
//POssui 17 bytes, status(1), topo(4), prox(4), nroRem(4), nroPares(4)
typedef struct  {
    char status; 
    int topo_Pilha;
    int proxRNN;
    int nroRegRem;
    int nroPares;
}Cabecalho;


//struct do registro

typedef struct 
{
    char removido;
    char unidade_medida;
    int velocidade;
    int encadeamento_pilha;
    int IDPoPs;
    int IDPoPs_Conectado;

} Registro ; 

void inicializar_registro(Registro *reg);
 //coloca o registro em um estado vazio conhecido antes de inserir dados

int Ler_registro(FILE *arquivo, Registro *reg); 
//le um registro do arquivo, por campo e retorna 1 se leu ou 0 se deu erro ou EOF

void Escrever_registro(FILE *arquivo, Registro *reg);
//escreve um registro no arquivo com todos os campos em ordem

void Imprimir_registro(const Registro *reg);

int registro_SatisfazCriterio(Registro *reg, char nomeCampo[][20], char valorCampo[][20], int quantCampos);

int campoIntCMP(char *valorTexto);

char campoCharCMP(char *valorTexto);


#endif
