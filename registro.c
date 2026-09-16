#include "registro.h"

void Inicializa_registro(Registro *reg)
{
    reg->removido = REGISTRO_NAO_REMOVIDO;
    reg->encadeamento_pilha = -1;
    reg->IDPoPs = VALOR_NULO_INT;
    reg->velocidade = VALOR_NULO_INT;
    reg->unidade_medida = VALOR_NULO_CHAR;


}

int Ler_registro(FILE *arquivo, Registro *reg) 
{
    if (fread(&reg->removido,          sizeof(char), 1, arquivo) != 1) return 0;
    if (fread(&reg->encadeamento_pilha, sizeof(int),  1, arquivo) != 1) return 0;
    if (fread(&reg->IDPoPs,            sizeof(int),  1, arquivo) != 1) return 0;
    if (fread(&reg->IDPoPs_Conectado,   sizeof(int),  1, arquivo) != 1) return 0;
    if (fread(&reg->velocidade,        sizeof(int),  1, arquivo) != 1) return 0;
    if (fread(&reg->unidade_medida,     sizeof(char), 1, arquivo) != 1) return 0;
    return 1;
}

void Escrever_registro(FILE *arquivo,  Registro *reg) 
{
    fwrite(&reg->removido,          sizeof(char), 1, arquivo);
    fwrite(&reg->encadeamento_pilha, sizeof(int),  1, arquivo);
    fwrite(&reg->IDPoPs,            sizeof(int),  1, arquivo);
    fwrite(&reg->IDPoPs_Conectado,   sizeof(int),  1, arquivo);
    fwrite(&reg->velocidade,        sizeof(int),  1, arquivo);
    fwrite(&reg->unidade_medida,     sizeof(char), 1, arquivo);
}
 
void Imprimir_registro(const Registro *reg)
{
    if (reg->removido == REGISTRO_REMOVIDO)
        return; /* nunca imprime removidos */

    if (reg->IDPoPs == VALOR_NULO_INT)
        printf("NULO ");
    else
        printf("%d ", reg->IDPoPs);

    if (reg->IDPoPs_Conectado == VALOR_NULO_INT)
        printf("NULO ");
    else
        printf("%d ", reg->IDPoPs_Conectado);

    if (reg->velocidade == VALOR_NULO_INT)
        printf("NULO ");
    else
        printf("%d ", reg->velocidade);

    if (reg->unidade_medida == VALOR_NULO_CHAR)
        printf("NULO\n");
    else
        printf("\"%c\"\n", reg->unidade_medida);
}