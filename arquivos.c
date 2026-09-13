#include <arquivos.h>

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

//Escreve um conjunto de dados 
int arquivo_EscreveChar(FILE *pontArquivo, char dado){
   if( fwrite(&dado, sizoef(dado), 1,  pontArquivo) != 1){
        printf("Falha no processamento do arquivo.\n");
        return 0;
    }
    return 1;
}

int arquivo_EscreveInt(FILE *pontArquivo, int dado){
   if( fwrite(&dado, sizoef(dado), 1,  pontArquivo) != 1){
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