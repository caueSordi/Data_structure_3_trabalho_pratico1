#include "registro.h"

void Inicializa_registro(Registro *reg)
{
    reg->removido = REGISTRO_NAO_REMOVIDO;
    reg->encadeamento_pilha = -1;
    reg->IdPoPs = VALOR_NULO_INT;
    reg->IdPosPs = VALOR_NULO_INT;
    red->velocidade = VALOR_NULO_INT;
    red->unidade_medida = VALOR_NULO_CHAR;


}

int Ler_registro(FILE *arquivo, Registro *reg) 
{
    if (fread(&reg->removido,          sizeof(char), 1, arquivo) != 1) return 0;
    if (fread(&reg->encadeamentoPilha, sizeof(int),  1, arquivo) != 1) return 0;
    if (fread(&reg->idPoPs,            sizeof(int),  1, arquivo) != 1) return 0;
    if (fread(&reg->idPoPsConectado,   sizeof(int),  1, arquivo) != 1) return 0;
    if (fread(&reg->velocidade,        sizeof(int),  1, arquivo) != 1) return 0;
    if (fread(&reg->unidadeMedida,     sizeof(char), 1, arquivo) != 1) return 0;
    return 1;
}

void Escrever_registro(FILE *arquivo, const Registro *reg) 
{
    fwrite(&reg->removido,          sizeof(char), 1, arquivo);
    fwrite(&reg->encadeamentoPilha, sizeof(int),  1, arquivo);
    fwrite(&reg->idPoPs,            sizeof(int),  1, arquivo);
    fwrite(&reg->idPoPsConectado,   sizeof(int),  1, arquivo);
    fwrite(&reg->velocidade,        sizeof(int),  1, arquivo);
    fwrite(&reg->unidadeMedida,     sizeof(char), 1, arquivo);
}
 
void Imprimir_registro( const Registro *reg)
{
  
    printf("idPoPs: ");
    if (reg->idPoPs == VALOR_NULO_INT) printf("NULO\n");
    else                               printf("%d\n", reg->idPoPs);
 
    printf("idPoPsConectado: ");
    if (reg->idPoPsConectado == VALOR_NULO_INT) printf("NULO\n");
    else                                        printf("%d\n", reg->idPoPsConectado);
 
    printf("velocidade: ");
    if (reg->velocidade == VALOR_NULO_INT) printf("NULO\n");
    else                                   printf("%d\n", reg->velocidade);
 
    printf("unidadeMedida: ");
    if (reg->unidadeMedida == VALOR_NULO_CHAR) printf("NULO\n");
    else                                       printf("%c\n", reg->unidadeMedida);
}
 