#include "busca.h"
#include "arquivos.h"
#include "registro.h"
#include <stdlib.h>
#include <string.h>

// novo: compara string do registro com o valor do criterio, "NULO" ou numero vazio casa com campo vazio
static int stringBate(const char *campo, const char *valor) {
if (valor[0] == '\0' || strcmp(valor, "NULO") == 0) {
return campo[0] == '\0';
}
return strcmp(campo, valor) == 0;
}

/* Compara um campo do registro contra o valor textual do critério.
 * Campos inteiros são comparados numericamente (atoi); campos string
 * são comparados com strcmp. Adicione outros "if" aqui se o TP tiver
 * mais campos buscáveis. */
static int campoBate(const Registro *reg, const Criterio *c) {
if (strcmp(c->nomeCampo, "idPoPs") == 0) {
return reg->idPoPs == atoi(c->valor); // alterado: era IDPoPs 
}
// novo: siglaPais, nulo e "$$"
if (strcmp(c->nomeCampo, "siglaPais") == 0) {
if (c->valor[0] == '\0' || strcmp(c->valor, "NULO") == 0) {
return reg->siglaPais[0] == VALOR_NULO_CHAR;
}
return reg->siglaPais[0] == c->valor[0] && reg->siglaPais[1] == c->valor[1];
}

if (strcmp(c->nomeCampo, "nomePoPs") == 0) {
return stringBate(reg->nomePoPs, c->valor);
}
if (strcmp(c->nomeCampo, "nomePais") == 0) {
return stringBate(reg->nomePais, c->valor);
}
    /* nome de campo desconhecido: não bate com nada */
return 0;
}

int Registro_satisfaz(const Registro *reg, const Criterio *criterios, int nCriterios) {
if (reg->removido == REGISTRO_REMOVIDO) return 0;

for (int i = 0; i < nCriterios; i++) {
if (!campoBate(reg, &criterios[i])) return 0;
}
return 1; /* bateu em TODOS os critérios */
}

int Buscar_sequencial(FILE *arquivo, const Criterio *criterios, int nCriterios,
long long offsetsEncontrados[], int maxResultados) { // alterado: era int rrnEncontrados[] @
Registro reg;
int total = 0;

arquivo_PosCabecalho(arquivo); // novo: comeca sempre no primeiro registro 
long long offset = arquivo_PosicaoAtual(arquivo); 

while (Ler_registro(arquivo, &reg)) {
if (Registro_satisfaz(&reg, criterios, nCriterios)) {
if (total < maxResultados) {
offsetsEncontrados[total] = offset; // alterado: era rrnEncontrados 
}
total++;
}
offset = arquivo_PosicaoAtual(arquivo); 
}
return total;
}

/* removido:
os campos idPoPsConectado, velocidade e unidadeMedida do campoBate
a variavel rrn do Buscar_sequencial pq nao usamos mais RRN*/