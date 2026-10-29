#ifndef BUSCA_H
#define BUSCA_H

#include "registro.h"

#define TAM_VALOR_CAMPO TAM_MAX_STRING // alterado: era 64, agora igual ao TAM_MAX_STRING por causa dos campos string
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
// alterado: guarda byte offsets em vez de RRNs, e comeca sempre no primeiro registro @
int Buscar_sequencial(FILE *arquivo, const Criterio *criterios, int nCriterios,
long long offsetsEncontrados[], int maxResultados);

#endif

// estrutura do indice para memoria
typedef struct {
    int idPoPs;
    long long byteOffset;
} RegistroIndice;

int compara_indice(const void *a, const void *b);
long long busca_binaria_indice(RegistroIndice *indices, int qtd, int idBuscado);


// removido: @
// TAM_NOME_CAMPO, ja existe no registro.h com 20 e os dois valores davam conflito @
// o vetor rrnEncontrados, trocado por offsetsEncontrados @