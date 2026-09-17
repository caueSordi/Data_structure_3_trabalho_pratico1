#include "funcoes.h"

//Função 1: Leitura do CSV, Criação e Escrita no .bin
int funcao_CREATE(char *nomeCSV, char *nomeBin){
    FILE *arqCSV, *arqBIN;

    int idPoPs, idPoPsConectado, velocidade;
    char unidade;
    Registro *registro;
    Cabecalho cabecalho;

    //manipulação do arquivo CSV e BIN
    //abertura
    arquivo_Abertura(&arqCSV, nomeCSV, "rb");
    arquivo_Abertura(&arqBIN, nomeBin, "wb");

    //escrita do cabecalho 1
    char status = '0';
    int topoPilha = -1;
    int proxRNN = 0;
    int nroRegRem = 0;
    int nroPares = 0;

    arquivo_EscreveChar(arqBIN, status);
    arquivo_EscreveInt(arqBIN, topoPilha);
    arquivo_EscreveInt(arqBIN, proxRNN);
    arquivo_EscreveInt(arqBIN, nroRegRem);
    arquivo_EscreveInt(arqBIN, nroPares);


    //verificação de integridade
    if(arqCSV == NULL){
        printf("Falha no processamento do arquivo.\n");
        return 1;

    }   
    
    //leitura do arquivo

    char removido = '0';
    int encadeamento_pilha = -1;
    int rnn = 0;

    char linhaCabecalho[100];
    fgets(linhaCabecalho, sizeof(linhaCabecalho), arqCSV);

    while(csv_LeLinha(arqCSV, &idPoPs, &idPoPsConectado, &velocidade, &unidade)){
        
        //escrita no binario
        arquivo_EscreveChar(arqBIN, removido);
        arquivo_EscreveInt(arqBIN, encadeamento_pilha);
        arquivo_EscreveInt(arqBIN, idPoPs);
        arquivo_EscreveInt(arqBIN, idPoPsConectado);
        arquivo_EscreveInt(arqBIN, velocidade);
        arquivo_EscreveChar(arqBIN, unidade);

        proxRNN++;
        nroPares++;
    }

    //reescrita do cabecalho
    arquivo_inicio(arqBIN);

    status = '1';
    arquivo_EscreveChar(arqBIN, status);
    arquivo_EscreveInt(arqBIN, topoPilha);
    arquivo_EscreveInt(arqBIN, proxRNN);
    arquivo_EscreveInt(arqBIN, nroRegRem);
    arquivo_EscreveInt(arqBIN, nroPares);

    arquivo_Close(&arqBIN);
    BinarioNaTela(nomeBin);
}


//Funcao 2:lista todos os registros (todos registros nao excluidos) 

int funcao_READ_ALL(char *nomeBin){
   //USAR AS FUNCOES DO ARQUVIO.H
    FILE *arqBIN = fopen(nomeBin, "rb");

    //mensagem de erro
    if(arqBIN == NULL){
        printf("Falha no processamento do arquivo.\n");
        return 1;
    }

    //lê cabecalho 
    Cabecalho cab ;
    if(!Ler_Cabecalho(arqBIN, &cab)){
        printf("Falha no processamento do arquivo.\n");
        return 1;
    }

    if(cab.status == STATUS_INCONSISTENTE){
        printf("Falha no processamento do arquivo.\n");
        fclose (arqBIN);
        return 1;

    }

    //Ler_Cabecalho já deixouo ponteiro para o inicio ro RRN 0, mas o fseek  deixa isso explicito e protege contra mudanças na leitura do cabecalho

    fseek(arqBIN, TAMANHO_CABECALHO, SEEK_SET);

    Registro reg;
    int encontrado = 0;

    while (Ler_registro (arqBIN, &reg)){
        if(reg.removido == REGISTRO_REMOVIDO)
            continue;

        Imprimir_registro(&reg);
        encontrado = 1;

    }
    // Fim da leitura dos registros
    
    if (!encontrado) {
        printf("Registro inexistente\n");
    }

    fclose(arqBIN);
    return 0;
}

//funcão 3: busca filtrada sequencial

void funcao_READVALUE(char *nomeBin, char *campoChar, int campoInt, int valor, char *valorChar){
    //abertura do arquivo para leitura
    FILE *arqBIN;
    arquivo_Abertura(&arqBIN, nomeBin, "rb");



}