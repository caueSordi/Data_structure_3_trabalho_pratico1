#include "busca.h"
#include <stdlib.h>
#include <string.h>

/* Compara um campo do registro contra o valor textual do critério.
 * Campos inteiros são comparados numericamente (atoi); unidadeMedida
 * é comparado como caractere. Adicione outros "if" aqui se o TP tiver
 * mais campos buscáveis. */
static int campoBate(const Registro *reg, const Criterio *c) {
    if (strcmp(c->nomeCampo, "idPoPs") == 0) {
        return reg->idPoPs == atoi(c->valor);
    }
    if (strcmp(c->nomeCampo, "idPoPsConectado") == 0) {
        return reg->idPoPsConectado == atoi(c->valor);
    }
    if (strcmp(c->nomeCampo, "velocidade") == 0) {
        return reg->velocidade == atoi(c->valor);
    }
    if (strcmp(c->nomeCampo, "unidadeMedida") == 0) {
        return reg->unidadeMedida == c->valor[0];
    }
    /* nome de campo desconhecido: não bate com nada */
    return 0;
}

int Registro_satisfaz(const Registro *reg, const Criterio *criterios, int nCriterios) {
    if (reg->removido == REG_REMOVIDO) return 0;

    for (int i = 0; i < nCriterios; i++) {
        if (!campoBate(reg, &criterios[i])) return 0;
    }
    return 1; /* bateu em TODOS os critérios */
}

int Buscar_sequencial(FILE *arquivo, const Criterio *criterios, int nCriterios,
                      int rrnEncontrados[], int maxResultados) {
    Registro reg;
    int rrn = 0;
    int total = 0;

    while (lerRegistro(arquivo, &reg)) {
        if (registroSatisfaz(&reg, criterios, nCriterios)) {
            if (total < maxResultados) {
                rrnEncontrados[total] = rrn;
            }
            total++;
        }
        rrn++;
    }
    return total;
}