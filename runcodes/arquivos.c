#include "arquivos.h"
#include "registro.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//abertura de arquivo sob um determinado modo e armazena o ponteiro
void arquivo_Abertura(FILE **pontArquivo, char *nome, char *modo){
    *pontArquivo = fopen(nome, modo);
    if(*pontArquivo == NULL){
        printf("Falha no  processamento do arquivo. \n");
    }
}

//fecha o arquivo
void arquivo_Close(FILE **arquivo){
    int flag =  fclose(*arquivo);
    if(flag != 0){
        printf("Falha no  processamento do arquivo. \n");
    }
}

void arquivo_inicio(FILE *arquivo){
    fseek(arquivo, 0, SEEK_SET); //volta para o inicio do arquivo
}

//corrigido para usar as macros de registro.h em vez de numeros fixos @
//assim se o tamanho do cabecalho ou do registro mudar, esta funcao @
//acompanha automaticamente, igual RRN_posicao ja fazia @
void arquivo_posicSeek(FILE *arquivo, int rnn)
{
    long offset = RRN_posicao(rnn);
    fseek(arquivo, offset, SEEK_SET);
}

//Escreve um conjunto de dados 
int arquivo_EscreveChar(FILE *pontArquivo, char dado){
   if( fwrite(&dado, sizeof(dado), 1,  pontArquivo) != 1){
        printf("Falha no processamento do arquivo.\n");
        return 0;
    }
    return 1;
}

int arquivo_EscreveInt(FILE *pontArquivo, int dado){
   if( fwrite(&dado, sizeof(dado), 1,  pontArquivo) != 1){
        printf("Falha no processamento do arquivo.\n");
        return 0;
    }
    return 1;
}

//corrigido: dado ja e um ponteiro (char *dado), entao o fread tem @
//que escrever em "dado", nao em "&dado" (que era o endereco do @
//proprio ponteiro, nao do char apontado por ele) @
int arquivo_LeChar(FILE *pontArquivo, char *dado ){
    if(fread(dado, sizeof(*dado), 1, pontArquivo) != 1){
        return 0; // EOF ou erro 
    }
    return 1;
}

//mesma correcao de arquivo_LeChar: dado ja e ponteiro, sem & aqui @
int arquivo_LeInt(FILE *pontArquivo, int *dado ){
    if(fread(dado, sizeof(*dado), 1, pontArquivo) != 1){
        return 0; // EOF ou erro 
    }
    return 1;
}

//leitura do arquivo csv e separação entre 4 campos
int csv_LeLinha(FILE *arqCSV, int *idPoPs, int *idPoPsConectado, int *velocidade, char *unidadeMedida){
    char linha[500];
    if(fgets(linha, sizeof(linha), arqCSV) == NULL){
        return 0; // EOF
    }

    // Remove a quebra de linha do final
    linha[strcspn(linha, "\r\n")] = '\0';

    char buffer[50];
    int pos = 0;
    int col = 0;
    char *ptr = linha;

    // Resetando os valores em todas as linhas para não herdar lixo!
    *idPoPs = -1;
    *idPoPsConectado = -1;
    *velocidade = -1;
    *unidadeMedida = '$';

    // Varredura manual (muito mais segura que sscanf para CSV)
    while (*ptr != '\0' && col < 4) {
        pos = 0;
        // Lê os caracteres até achar uma vírgula ou o fim da linha
        while (*ptr != ',' && *ptr != '\0') {
            buffer[pos++] = *ptr++;
        }
        buffer[pos] = '\0';

        // Se encontrou algo no buffer, salva no campo certo
        if (pos > 0) {
            if (col == 0) *idPoPs = atoi(buffer);
            else if (col == 1) *idPoPsConectado = atoi(buffer);
            else if (col == 2) *velocidade = atoi(buffer);
            else if (col == 3) *unidadeMedida = buffer[0];
        }

        // Pula a vírgula para ir para a próxima coluna
        if (*ptr == ',') ptr++; 
        col++;
    }

    return 1;
}
void Inicializa_Cabecalho(Cabecalho *cab)
{
    cab->status = STATUS_INCONSISTENTE;
    cab->proxRNN = 0;
    cab->topo_Pilha = -1;
    cab->nroRegRem = 0;
    cab->nroPares = 0;

}

int Ler_Cabecalho(FILE *arquivo, Cabecalho *cab)
{
    if (fread(&cab->status, sizeof(char), 1, arquivo) != 1)
        return 0;
   
    if (fread(&cab->topo_Pilha, sizeof(int), 1, arquivo) != 1)
        return 0;
     if (fread(&cab->proxRNN, sizeof(int), 1, arquivo) != 1)
        return 0;
    if (fread(&cab->nroRegRem, sizeof(int), 1, arquivo) != 1)
        return 0;
    if (fread(&cab->nroPares, sizeof(int), 1, arquivo) != 1)
        return 0;
    return 1;
}

void Escrever_Cabecalho(FILE *arquivo, Cabecalho *cab)
{
    fwrite(&cab->status, sizeof(char), 1, arquivo);
    fwrite(&cab->topo_Pilha, sizeof(int), 1, arquivo);
    fwrite(&cab->proxRNN, sizeof(int),1, arquivo);
    fwrite(&cab->nroRegRem, sizeof(int), 1, arquivo);
    fwrite(&cab->nroPares, sizeof(int), 1, arquivo);
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














////FUNCOES FORNECIDAS


/*
 * Você não precisa entender o código dessa função.
 *
 * Use essa função para comparação no run.codes.
 * Lembre-se de ter fechado (fclose) o arquivo anteriormente.
 *
 * Ela vai abrir de novo para leitura e depois fechar
 * (você não vai perder pontos por isso se usar ela).
 */
void BinarioNaTela(char *arquivo) {
    FILE *fs;
    if (arquivo == NULL || !(fs = fopen(arquivo, "rb"))) {
        fprintf(stderr,
                "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): "
                "não foi possível abrir o arquivo que me passou para leitura. "
                "Ele existe e você tá passando o nome certo? Você lembrou de "
                "fechar ele com fclose depois de usar?\n");
        return;
    }

    fseek(fs, 0, SEEK_END);
    size_t fl = ftell(fs);

    fseek(fs, 0, SEEK_SET);
    unsigned char *mb = (unsigned char *)malloc(fl);
    fread(mb, 1, fl, fs);

    unsigned long cs = 0;
    for (unsigned long i = 0; i < fl; i++) {
        cs += (unsigned long)mb[i];
    }

    printf("%lf\n", (cs / (double)100));

    free(mb);
    fclose(fs);
}

/*
 *	Use essa função para ler um campo string delimitado entre aspas (").
 *	Chame ela na hora que for ler tal campo. Por exemplo:
 *
 *	A entrada está da seguinte forma:
 *		nomeDoCampo "MARIA DA SILVA"
 *
 *	Para ler isso para as strings já alocadas str1 e str2 do seu programa,
 * você faz: scanf("%s", str1); // Vai salvar nomeDoCampo em str1
 *		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2
 * (sem as aspas)
 *
 */
void ScanQuoteString(char *str) {
    char R;

    while ((R = getchar()) != EOF && isspace(R))
        ; // ignorar espaços, \r, \n...

    if (R == 'N' || R == 'n') { // campo NULO
        getchar();
        getchar();
        getchar();       // ignorar o "ULO" de NULO.
        strcpy(str, ""); // copia string vazia
    } else if (R == '\"') {
        if (scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
            strcpy(str, "");
        }
        getchar();         // ignorar aspas fechando
    } else if (R != EOF) { // vc tá tentando ler uma string que não tá entre
                           // aspas! Fazer leitura normal %s então, pois deve
                           // ser algum inteiro ou algo assim...
        str[0] = R;
        scanf("%s", &str[1]);
    } else { // EOF
        strcpy(str, "");
    }
}