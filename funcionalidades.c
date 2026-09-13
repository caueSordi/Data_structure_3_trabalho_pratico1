#include <busca.h>
#include <registro.h>
#include <arquivos.h>


//Função 1: Leitura do CSV, Criação e Escrita no .bin
int funcao_CREATE(char *nomeCSV, char *nomeBin){
    FILE *arqCSV, *arqBIN;

    //manipulação do arquivo CSV e BIN
    //abertura
    arquivo_Abertura(&arqCSV, nomeCSV, "rb");
    arquivo_Abertura(&arqBIN, nomeBin, "wb");

    //verificação de integridade
    if(arqCSV == NULL){
        printf("Falha no processamento do arquivo.\n");
        return 1;

    }   
    
    //leitura do arquivo
    int idPoPs, idPoPsConectado, velocidade;
    char unidade;
    while(csv_LeLinha(arqCSV, &idPoPs, &idPoPsConectado, &velocidade, &unidade)){

    }



}