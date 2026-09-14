#include "arquivo.h"
#include "registro.h"

void Inicializa_Cabecalho(Cabecalho *cab)
{
    cab->status = STATUS_INCONSISTENTE;
    cab->prox_RRN = 0;
    cab->topo_pilha = -1;
    cab->nmro_registro_removidos = 0;
    cab->nmro_pares = 0;

}

int Ler_Cabecalho(FILE *arquivo, Cabecalho *cab)
{
    if (fread(&cab->status, sizeof(char), 1, arquivo) != 1)
        return 0;
    if (fread(&cab->prox_RRN, sizeof(int), 1, arquivo) != 1)
        return 0;
    if (fread(&cab->topo_pilha, sizeof(int), 1, arquivo) != 1)
        return 0;
    if (fread(&cab->nmro_registro_removidos, sizeof(int), 1, arquivo) != 1)
        return 0;
    if (fread(&cab->nmro_pares, sizeof(int), 1, arquivo) != 1)
        return 0;
    return 1;
}

void Escrever_Cabecalho(FILE *arquivo, Cabecalho *cab)
{
    fwrite(&cab->status, sizeof(char), 1, arquivo);
    fwrite(&cab->prox_RRN, sizeof(int), 1, arquivo);
    fwrite(&cab->topo_pilha, sizeof(int), 1, arquivo);
    fwrite(&cab->nmro_registro_removidos, sizeof(int), 1, arquivo);
    fwrite(&cab->nmro_pares, sizeof(int), 1, arquivo);
}


//verificar depois, TAMANHO_REGISTO existe?
long RRN_posicao(int rrn)
{
    return (TAMANHO_CABECALHO + (long)rrn * TAMANHO_REGISTRO);
    
    ;
}


int Ler_registro_rrn(FILE *arquivo, int rrn, Registro *reg)
{
    if (fseek(arquivo, RRN_posicao(rrn), SEEK_SET) != 0)
        return 0;
    return Ler_registro(arquivo, reg);
}