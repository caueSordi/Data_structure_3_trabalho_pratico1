#ifndef BUSCA_H
#define BUSCA_H

#include "registro.h"

#define TAM_NOME_CAMPO  32
#define TAM_VALOR_CAMPO 64
#define MAX_CRITERIOS   10

/*Critério de busca: guarda nomeDoCampo valor como texto
 a conversão para int ou char ocorre apenas na comparação, em busca.c. */
typedef struct {
    char nomeCampo[TAM_NOME_CAMPO];
    char valor[TAM_VALOR_CAMPO];
} Criterio;

/* testa se um registro ativo atende todos os critérios (AND lógico)
 É reutilizada nas buscas, remoções e atualizações.. */
int Registro_satisfaz(const Registro *reg, const Criterio *criterios, int nCriterios);

/* percorre os registros, ignora removidos e armazena os RRNs encontrados no vetor, retornando o total de resultados. */
int Buscar_sequencial(FILE *arquivo, const Criterio *criterios, int nCriterios,
                      int rrnEncontrados[], int maxResultados);

#endif