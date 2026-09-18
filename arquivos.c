#include "arquivos.h"
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

void arquivo_PosCabecalho(FILE *arquivo){
    fseek(arquivo, TAMANHO_CABECALHO, SEEK_SET); //volta para o inicio do arquivo
}

void arquivo_posicSeek(FILE *arquivo, int rnn){
    int offset =  17 + rnn * 18;
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

int arquivo_LeChar(FILE *pontArquivo, char *dado ){
    if(fread(&dado, sizeof(*dado), 1, pontArquivo) != 1){
        return 0; // EOF ou erro 
    }
    return 1;
}

int arquivo_LeInt(FILE *pontArquivo, int *dado ){
    if(fread(&dado, sizeof(*dado), 1, pontArquivo) != 1){
        return 0; // EOF ou erro 
    }
    return 1;
}

//leitura do arquivo csv e separação entre 4 campos
int csv_LeLinha(FILE *arqCSV, int *idPoPs, int *idPoPsConectado, int *velocidade, char *unidadeMedida){
    char linha[500];
    
    if(fgets(linha, sizeof(linha), arqCSV) == NULL){
        return 0; //final do arquivo
    }

    char campoVeloc[10] = "";
    char campoUnidade[10] = "";

    sscanf(linha, "%d, %d, %[^,],%[^\n]", idPoPs, idPoPsConectado, campoVeloc, campoUnidade);

    //caso velocidade seja nula
    if(campoVeloc[0] == '\0' || campoVeloc[0] == ' '){
        *velocidade = -1;
    }else{
        *velocidade = atoi(campoVeloc);
    }
    //caso unidadeMedida seja nula
    if(campoUnidade[0] == '\0' || campoUnidade[0] == ' '){
        *unidadeMedida = '$';
    } else{
        *unidadeMedida = campoUnidade[0];
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