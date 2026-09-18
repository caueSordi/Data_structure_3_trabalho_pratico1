#include "funcoes.h"
#include <string.h> 
#include <stdio.h>
#include <stdlib.h>

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
    arquivo_PosCabecalho(arqBIN);

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

int funcao_READVALUE(int quantBusca, char *nomeBin){
    int quantCampos =0;
    char aux[20];
    Registro reg;
   


    //abertura do arquivo para leitura
    FILE *arqBIN;
    arquivo_Abertura(&arqBIN, nomeBin, "rb");

    
    for(int i=0; i<quantBusca; i++)
    {
        int encontrado = 0;
        scanf("%d", &quantCampos);

        char nomeCampo[quantCampos][20];
        char valorCampo[quantCampos][20];

        //lendo os dados de pesquisa
        for(int j =0; j<quantCampos; j++){
            scanf("%s", nomeCampo[j]);
            if(!strcmp(nomeCampo[j], "unidadeMedida")){
                ScanQuoteString(valorCampo[j]);
            }
            else{
                scanf("%s", valorCampo[j]);
            }
        }

        //posicionado o seek apos o cabecalho
        arquivo_PosCabecalho(arqBIN);

        while (Ler_registro (arqBIN, &reg)){
        if(reg.removido == REGISTRO_REMOVIDO)
            continue;

        if(registro_SatisfazCriterio(&reg, nomeCampo, valorCampo, quantCampos)){
            Imprimir_registro(&reg);
            
            encontrado = 1;
        }
    }
    printf("\n");
    if(!encontrado){
        printf("Registro inexistente.\n");
    }
    
}

    arquivo_Close(&arqBIN);
    return 0;
}

//Funcao 4: Busca por RRN, recebe o RRN e calcula a posicao com RRN_posicao

int funcao_SELECT_RRN(char *nomeBin, int RRN){
    //abre arquivo
    FILE *arqBIN = fopen(nomeBin, "rb");

    if (arqBIN == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return 1;
    }

    Cabecalho cab;
    if(!Ler_Cabecalho(arqBIN, &cab) || cab.status == STATUS_INCONSISTENTE){
        printf("Falha no processamento do arquivo.\n");
        fclose(arqBIN);
        return 1;
    }

    if(RRN < 0 || RRN >= cab.proxRNN){
        printf("Registro inexistente.\n");
        fclose(arqBIN);
        return 0;
    }

    Registro reg;
    if(!Ler_registro_rrn(arqBIN, RRN, &reg)){
        printf("Falha no processamento do arquivo.\n");
        fclose(arqBIN);
        return 1;
    }

    if(reg.removido == REGISTRO_REMOVIDO){
        printf("Registro inexistente.\n");
        fclose(arqBIN);
        return 0;
    }

    Imprimir_registro(&reg);
    fclose(arqBIN);
    return 0;
}

//Funcao 5: remocao logica, le o conjunto de criterios igual a 3, percorre  arquivo e marca REGISTRO_REMOVIDO, coloca os registros na pilha
int funcao_DELETE(char *nomeBin){
    int n;
    scanf("%d", &n);

    FILE *arqBIN = fopen(nomeBin, "rb+");
    if (arqBIN == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return 1;
    }

    Cabecalho cab;
    if(!Ler_Cabecalho(arqBIN, &cab) || cab.status == STATUS_INCONSISTENTE){
        printf("Falha no processamento do arquivo.\n");
        fclose(arqBIN);
        return 1;
    }

    //Leitura dos Criterios
    for (int i = 0; i < n; i++) {
        int nCriterios;
        scanf("%d", &nCriterios);

        Criterio criterios[MAX_CRITERIOS];
        for (int j = 0; j < nCriterios; j++) {
            scanf("%s", criterios[j].nomeCampo);
            // unidade medida esta entre aspas
            if (strcmp(criterios[j].nomeCampo, "unidadeMedida") == 0)
                ScanQuoteString(criterios[j].valor);
            else
                scanf("%s", criterios[j].valor);
        }
        //loop para ir removendo os registros que batem o criterio
        for (int rrn = 0; rrn < cab.proxRNN; rrn++) {
            Registro reg;
            if (!Ler_registro_rrn(arqBIN, rrn, &reg))
                continue;
            if (!Registro_satisfaz(&reg, criterios, nCriterios))
                continue;
            //muda status do registro e atualiza o topo_pilha
            reg.removido = REGISTRO_REMOVIDO;
            reg.encadeamento_pilha = cab.topo_Pilha; // empilha 
            cab.topo_Pilha = rrn;
            cab.nroRegRem++;
            //cab.nroPares--; // comentario temporario

            fseek(arqBIN, RRN_posicao(rrn), SEEK_SET);
            Escrever_registro(arqBIN, &reg);
        }
    }

    // cabecalho mudou (topoPilha, nroRegRem, nroPares): reescreve 
    fseek(arqBIN, 0, SEEK_SET);
    Escrever_Cabecalho(arqBIN, &cab);
    fclose(arqBIN);
    BinarioNaTela(nomeBin);
    return 0;
}