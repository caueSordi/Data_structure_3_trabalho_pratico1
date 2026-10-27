#include "funcoes.h"
#include <string.h> 
#include <stdio.h>
#include <stdlib.h>

//Função 1: Leitura do CSV, Criação e Escrita no .bin
// alterado: agora e a funcionalidade 8 (CREATE TABLE) do trabalho 2 
int funcao_CREATE(char *nomeCSV, char *nomeBin){
    FILE *arqCSV, *arqBIN;

    // apagado: idPoPs, idPoPsConectado, velocidade e unidade, o csv_LeLinha agora preenche o Registro direto 
    Registro registro; // alterado: era o ponteiro Registro *registro, sem uso, agora recebe cada linha do CSV
    Cabecalho cabecalho;

    //manipulação do arquivo CSV e BIN
    arquivo_Abertura(&arqCSV, nomeCSV, "rb");

    //verificação de integridade

    if(arqCSV == NULL){
        printf("Falha no processamento do arquivo.\n");
        return 1;

    }   

    arquivo_Abertura(&arqBIN, nomeBin, "wb");
    // novo: verifica tambem se o bin abriu
    if(arqBIN == NULL){
        printf("Falha no processamento do arquivo.\n");
        arquivo_Close(&arqCSV);
        return 1;
    }

    //escrita do cabecalho 1
    Inicializa_cabecalho(&cabecalho); // alterado: o cabecalho usa a struct, tem 25 bytes, status 0 e lista vazia
    Escrever_Cabecalho(arqBIN, &cabecalho);

    //leitura do arquivo

    // apagado: removido, encadeamento_pilha e rnn, o Registro ja comeca com esses valores 

    char linhaCabecalho[1024]; // alterado: era 100, uma linha maior viraria um registro 
    fgets(linhaCabecalho, sizeof(linhaCabecalho), arqCSV);

    while(csv_LeLinha(arqCSV, &registro)){ // alterado: preenche o Registro direto, sem as variaveis soltas 
        
        //escrita no binario
        registro.tamanhoRegistro = calcula_tamanho_registro(&registro); // novo: registro de tamanho variavel 
        Escrever_registro(arqBIN, &registro);
        // apagado: as escritas de removido, encadeamento_pilha, idPoPs, idPoPsConectado, velocidade e unidade 

        cabecalho.proxByteOffset += tamanho_total_registro(&registro); 
        cabecalho.nroPoPs++; 
    }

  
    cabecalho.status = STATUS_CONSISTENTE; 
    Escrever_Cabecalho(arqBIN, &cabecalho); 
    // apagado: arquivo_inicio e a escrita campo a campo do cabecalho antigo 

    arquivo_Close(&arqCSV); 
    arquivo_Close(&arqBIN);
    BinarioNaTela(nomeBin);
    return 0;
}


//Funcao 2:lista todos os registros (todos registros nao excluidos) 
// alterado: agora e a funcionalidade 10 (SELECT) do Trabalho 2

int funcao_READ_ALL(char *nomeBin){
   //USAR AS FUNCOES DO ARQUVIO.H
    FILE *arqBIN = fopen(nomeBin, "rb");

    //mensagem de erro
    if(arqBIN == NULL){
        printf("Falha no processamento do arquivo.\n");
        return 1;
    }


    Cabecalho cab ;
    if(!Ler_Cabecalho(arqBIN, &cab)){
        printf("Falha no processamento do arquivo.\n");
        fclose(arqBIN); // novo: faltava fechar o arquivo
        return 1;
    }

    if(cab.status == STATUS_INCONSISTENTE){
        printf("Falha no processamento do arquivo.\n");
        fclose (arqBIN);
        return 1;

    }

    //Ler_Cabecalho já deixouo ponteiro para o inicio ro RRN 0, mas o fseek  deixa isso explicito e protege contra mudanças na leitura do cabecalho
    // alterado: agora o ponteiro fica no primeiro registro, logo depois dos 25 bytes do cabecalho, nao ha mais RRN @
    arquivo_PosCabecalho(arqBIN);

    Registro reg;
    int encontrado = 0;

    while (Ler_registro (arqBIN, &reg)){ // alterado: Ler_registro agora le registro variavel e pula para o proximo 
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

// pendente: funcao_READVALUE (SELECT WHERE, 11) fica para o passo 4, vai usar o indice quando o criterio for idPoPs

// apagado: funcao_SELECT_RRN, no TP2 nao existe RRN, a busca por byte offset vai ficar dentro do SELECT WHERE

// pendente: funcao_DELETE (12), funcao_INSERT (13) e funcao_UPDATE (14) ficam para os passos 5 e 6, vao ser reescritas com Worst Fit e indice 

// pendente: funcao_CREATE_INDEX (9) e a funcionalidade nova do TP2, fica para o passo 4 