#include "registro.h"
#include "arquivos.h"

void Inicializa_registro(Registro *reg)
{
reg->removido = REGISTRO_NAO_REMOVIDO;
reg->encadeamento = LISTA_VAZIA; // alterado: era encadeamento_pilha = -1 @
reg->tamanhoRegistro = 0; // novo @
reg->idPoPs = VALOR_NULO_INT; // alterado: era IDPoPs @
reg->siglaPais[0] = VALOR_NULO_CHAR; // novo: sigla nula e "$$" @
reg->siglaPais[1] = VALOR_NULO_CHAR;
reg->siglaPais[2] = '\0';
reg->nomePoPs[0] = '\0'; // novo: string vazia e nulo @
reg->nomePais[0] = '\0';
// apagado: velocidade e unidade_medida @


}

int Ler_registro(FILE *arquivo, Registro *reg) 
{
long inicio = ftell(arquivo); // novo: guarda onde o registro comeca @
Inicializa_registro(reg); // novo: limpa o registro antes de ler @

if (fread(&reg->removido,          sizeof(char), 1, arquivo) != 1) return 0;
if (fread(&reg->tamanhoRegistro,   sizeof(int),  1, arquivo) != 1) return 0; // novo @
if (reg->tamanhoRegistro < BYTES_FIXOS_APOS_TAMANHO) return 0; // novo: tamanho invalido @
long fimRegistro = inicio + BYTES_ANTES_TAMANHO + reg->tamanhoRegistro; // novo: onde comeca o proximo registro @
if (fread(&reg->encadeamento, sizeof(long long), 1, arquivo) != 1) return 0; // alterado: era int encadeamento_pilha @

// novo: se esta removido o resto e lixo, so pula para o proximo registro @
if (reg->removido == REGISTRO_REMOVIDO) {
fseek(arquivo, fimRegistro, SEEK_SET);
return 1;
}

if (fread(&reg->idPoPs,            sizeof(int),  1, arquivo) != 1) return 0; // alterado: era IDPoPs @
// apagado: leitura de IDPoPs_Conectado e velocidade @
if (fread(&reg->siglaPais[0],      sizeof(char), 1, arquivo) != 1) return 0; // novo: sigla tem 2 chars @
if (fread(&reg->siglaPais[1],      sizeof(char), 1, arquivo) != 1) return 0;
reg->siglaPais[2] = '\0';
// novo: strings variaveis, lidas ate o '|' @
if (!arquivo_LeStringAteDelim(arquivo, reg->nomePoPs, TAM_MAX_STRING)) return 0;
if (!arquivo_LeStringAteDelim(arquivo, reg->nomePais, TAM_MAX_STRING)) return 0;
// apagado: leitura de unidade_medida @

// novo: pula a sobra de '$' se o espaco era maior que o registro @
fseek(arquivo, fimRegistro, SEEK_SET);
return 1;
}



// apagado: Ler_registro_rrn, no TP2 nao existe RRN @
// novo: vai ate o byte offset e le o registro @
int Ler_registro_offset(FILE *arquivo, long long offset, Registro *reg)
{
if (fseek(arquivo, (long)offset, SEEK_SET) != 0)
return 0;
return Ler_registro(arquivo, reg);
}


int Escrever_registro(FILE *arquivo,  const Registro *reg) // alterado: agora retorna int e reg e const @
{
int usado = calcula_tamanho_registro(reg); // novo: bytes usados pelos dados @
char delimitador = DELIMITADOR; // novo @

if (usado > reg->tamanhoRegistro) return 0; // novo: dados nao cabem no espaco reservado @

fwrite(&reg->removido,          sizeof(char), 1, arquivo);
fwrite(&reg->tamanhoRegistro,   sizeof(int),  1, arquivo); // novo @
fwrite(&reg->encadeamento, sizeof(long long), 1, arquivo); // alterado: era int encadeamento_pilha @
fwrite(&reg->idPoPs,            sizeof(int),  1, arquivo); // alterado: era IDPoPs @
fwrite(reg->siglaPais, sizeof(char), TAM_SIGLA, arquivo); // novo: 2 chars da sigla @
// apagado: escrita de IDPoPs_Conectado, velocidade e unidade_medida @

// novo: strings variaveis, cada uma seguida de '|' @
fwrite(reg->nomePoPs, sizeof(char), strlen(reg->nomePoPs), arquivo);
fwrite(&delimitador, sizeof(char), 1, arquivo);
fwrite(reg->nomePais, sizeof(char), strlen(reg->nomePais), arquivo);
fwrite(&delimitador, sizeof(char), 1, arquivo);

arquivo_EscreveLixo(arquivo, reg->tamanhoRegistro - usado); // novo: completa a sobra com '$' @
return 1; // novo @
}
void Imprimir_registro(const Registro *reg)
{
if (reg->removido == REGISTRO_REMOVIDO)
return; /* nunca imprime removidos */

// alterado: campos novos e formato provisorio, confira os rotulos no PDF @
if (reg->idPoPs == VALOR_NULO_INT)
printf("Identificador do ponto: NULO\n");
else
printf("Identificador do ponto: %d\n", reg->idPoPs);

if (reg->nomePoPs[0] == '\0')
printf("Nome do ponto: NULO\n");
else
printf("Nome do ponto: %s\n", reg->nomePoPs);

if (reg->nomePais[0] == '\0')
printf("Pais de localizacao: NULO\n");
else
printf("Pais de localizacao: %s\n", reg->nomePais);

if (reg->siglaPais[0] == VALOR_NULO_CHAR)
printf("Sigla do pais: NULO\n");
else
printf("Sigla do pais: %c%c\n", reg->siglaPais[0], reg->siglaPais[1]);

printf("\n");
// apagado: impressao de IDPoPs_Conectado, velocidade e unidade_medida @
}

// novo: compara string do registro com o valor da busca, "NULO" ou vazio casa com campo vazio @
static int string_igual(const char *campo, const char *valor)
{
if (valor[0] == '\0' || !strcmp(valor, "NULO"))
return campo[0] == '\0';
return !strcmp(campo, valor);
}

int registro_SatisfazCriterio(const Registro *reg, char nomeCampo[][TAM_NOME_CAMPO], char valorCampo[][TAM_MAX_STRING], int quantCampos){ // alterado: reg const e valorCampo maior, pois agora ha strings @
for(int i=0; i< quantCampos; i++)
{
if(!strcmp(nomeCampo[i], "idPoPs")){
int valor = campoIntCMP(valorCampo[i]);
if(reg->idPoPs != valor) // alterado: era IDPoPs @
return 0;
}
// apagado: criterios idPoPsConectado, velocidade e unidadeMedida @
// novo: criterio siglaPais, nulo e "$$" @
else if(!strcmp(nomeCampo[i], "siglaPais")){
const char *valor = valorCampo[i];
if(valor[0] == '\0' || !strcmp(valor, "NULO")){
if(reg->siglaPais[0] != VALOR_NULO_CHAR)
return 0;
}
else if(reg->siglaPais[0] != valor[0] || reg->siglaPais[1] != valor[1]){
return 0;
}
}
// novo: criterios nomePoPs e nomePais @
else if(!strcmp(nomeCampo[i], "nomePoPs")){
if(!string_igual(reg->nomePoPs, valorCampo[i]))
return 0;
}
else if(!strcmp(nomeCampo[i], "nomePais")){
if(!string_igual(reg->nomePais, valorCampo[i]))
return 0;
}
}

return 1;//passou por todos e nao achou
}

int campoIntCMP(const char *valorTexto){ // alterado: parametro const @
if(!strcmp(valorTexto, "NULO")){
return -1;
}
int aux = atoi(valorTexto);
return aux;
}

// apagado: campoCharCMP, nao existe mais campo char @


// novo: cabecalho, inicializa com lista vazia e proxByteOffset logo depois do cabecalho @
void Inicializa_cabecalho(Cabecalho *cab)
{
cab->status = STATUS_INCONSISTENTE;
cab->topoLista = LISTA_VAZIA;
cab->proxByteOffset = TAM_CABECALHO;
cab->nroRegRem = 0;
cab->nroPoPs = 0;
}

// novo: le o cabecalho do inicio do arquivo, retorna 1 se leu tudo @
int Ler_Cabecalho(FILE *arquivo, Cabecalho *cab)
{
arquivo_inicio(arquivo);
if (fread(&cab->status,         sizeof(char),      1, arquivo) != 1) return 0;
if (fread(&cab->topoLista,      sizeof(long long), 1, arquivo) != 1) return 0;
if (fread(&cab->proxByteOffset, sizeof(long long), 1, arquivo) != 1) return 0;
if (fread(&cab->nroRegRem,      sizeof(int),       1, arquivo) != 1) return 0;
if (fread(&cab->nroPoPs,        sizeof(int),       1, arquivo) != 1) return 0;
return 1;
}

// novo: escreve o cabecalho no inicio do arquivo, campo a campo @
void Escrever_Cabecalho(FILE *arquivo, const Cabecalho *cab)
{
arquivo_inicio(arquivo);
fwrite(&cab->status,         sizeof(char),      1, arquivo);
fwrite(&cab->topoLista,      sizeof(long long), 1, arquivo);
fwrite(&cab->proxByteOffset, sizeof(long long), 1, arquivo);
fwrite(&cab->nroRegRem,      sizeof(int),       1, arquivo);
fwrite(&cab->nroPoPs,        sizeof(int),       1, arquivo);
}

// novo: tamanhoRegistro de um registro sem sobra, campos fixos + nomePoPs e nomePais com o '|' @
int calcula_tamanho_registro(const Registro *reg)
{
return BYTES_FIXOS_APOS_TAMANHO + (int)strlen(reg->nomePoPs) + 1 + (int)strlen(reg->nomePais) + 1;
}

// novo: bytes que o registro ocupa no arquivo @
int tamanho_total_registro(const Registro *reg)
{
return BYTES_ANTES_TAMANHO + reg->tamanhoRegistro;
}