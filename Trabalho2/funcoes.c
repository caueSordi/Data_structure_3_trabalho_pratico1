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
// case 9 Create index
int funcao_CREATE_INDEX(char *nomeBin, char *nomeIndice) {
    FILE *arqBIN = fopen(nomeBin, "rb");
    if (arqBIN == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return 1;
    }

    Cabecalho cab;
    if (!Ler_Cabecalho(arqBIN, &cab) || cab.status == STATUS_INCONSISTENTE) {
        printf("Falha no processamento do arquivo.\n");
        fclose(arqBIN);
        return 1;
    }

    RegistroIndice *indices = NULL;
    if (cab.nroPoPs > 0) {
        indices = malloc(cab.nroPoPs * sizeof(RegistroIndice));
    }
    int qtdIndices = 0;

    arquivo_PosCabecalho(arqBIN);
    Registro reg;
    long long offsetAtual = arquivo_PosicaoAtual(arqBIN);
    
    // extrai dados ativos do arquivo de dados para o array
    while (Ler_registro(arqBIN, &reg)) {
        if (reg.removido != REGISTRO_REMOVIDO) {
            if (indices != NULL) {
                indices[qtdIndices].idPoPs = reg.idPoPs;
                indices[qtdIndices].byteOffset = offsetAtual;
                qtdIndices++;
            }
        }
        offsetAtual = arquivo_PosicaoAtual(arqBIN);
    }
    fclose(arqBIN);

    // ordena pelo idPoPs crescente
    if (indices != NULL && qtdIndices > 0) {
        qsort(indices, qtdIndices, sizeof(RegistroIndice), compara_indice);
    }

    FILE *arqIND = fopen(nomeIndice, "wb");
    if (arqIND == NULL) {
        printf("Falha no processamento do arquivo.\n");
        if (indices) free(indices);
        return 1;
    }

    char status = '0';
    fwrite(&status, sizeof(char), 1, arqIND);

    // escreve os 12 bytes por registro no indice
    for (int i = 0; i < qtdIndices; i++) {
        fwrite(&indices[i].idPoPs, sizeof(int), 1, arqIND);
        fwrite(&indices[i].byteOffset, sizeof(long long), 1, arqIND);
    }

    // fecha atualizando status
    status = '1';
    fseek(arqIND, 0, SEEK_SET);
    fwrite(&status, sizeof(char), 1, arqIND);

    fclose(arqIND);
    if (indices) free(indices);

    BinarioNaTela(nomeIndice);
    return 0;
}


// case 11 implementada com busca indexada e sequencial
int funcao_READVALUE(char *nomeBin, char *nomeIndice, int nBuscas) {
    FILE *arqBIN = fopen(nomeBin, "rb");
    FILE *arqIND = fopen(nomeIndice, "rb");

    if (arqBIN == NULL || arqIND == NULL) {
        printf("Falha no processamento do arquivo.\n");
        if (arqBIN) fclose(arqBIN);
        if (arqIND) fclose(arqIND);
        return 1;
    }

    Cabecalho cab;
    if (!Ler_Cabecalho(arqBIN, &cab) || cab.status == STATUS_INCONSISTENTE) {
        printf("Falha no processamento do arquivo.\n");
        fclose(arqBIN); fclose(arqIND);
        return 1;
    }

    char statusIndice;
    if (fread(&statusIndice, sizeof(char), 1, arqIND) != 1 || statusIndice == '0') {
        printf("Falha no processamento do arquivo.\n");
        fclose(arqBIN); fclose(arqIND);
        return 1;
    }

    // carrega indice inteiro para a memoria
    fseek(arqIND, 0, SEEK_END);
    long long tamIndice = ftell(arqIND) - 1; // desconta cabecalho
    int qtdIndices = tamIndice / 12;
    RegistroIndice *indices = NULL;
    
    if (qtdIndices > 0) {
        indices = malloc(qtdIndices * sizeof(RegistroIndice));
        fseek(arqIND, 1, SEEK_SET);
        for (int i = 0; i < qtdIndices; i++) {
            fread(&indices[i].idPoPs, sizeof(int), 1, arqIND);
            fread(&indices[i].byteOffset, sizeof(long long), 1, arqIND);
        }
    }
    fclose(arqIND);

    // faz as buscas
    for (int i = 0; i < nBuscas; i++) {
        int m;
        scanf("%d", &m);

        Criterio *criterios = malloc(m * sizeof(Criterio));
        int temIdPops = 0;
        int idBuscado = -1;

        for (int j = 0; j < m; j++) {
            scanf("%s", criterios[j].nomeCampo);
            ScanQuoteString(criterios[j].valor); // lida com as aspas para tratar strings
            
            // checa se possui campo indexado para decidir a estrategia
            if (strcmp(criterios[j].nomeCampo, "idPoPs") == 0) {
                temIdPops = 1;
                idBuscado = atoi(criterios[j].valor);
            }
        }

        int encontrado = 0;
        
        if (temIdPops && indices != NULL) {
            // executa busca indexada super rapida usando o idPoPs
            long long offset = busca_binaria_indice(indices, qtdIndices, idBuscado);
            if (offset != -1) {
                Registro reg;
                if (Ler_registro_offset(arqBIN, offset, &reg)) {
                    // testa se atende a TODOS os outros criterios
                    if (Registro_satisfaz(&reg, criterios, m)) {
                        Imprimir_registro(&reg);
                        encontrado = 1;
                    }
                }
            }
        } else {
            // fallback para a busca sequencial varrendo o arquivo inteiro
            arquivo_PosCabecalho(arqBIN);
            Registro reg;
            while (Ler_registro(arqBIN, &reg)) {
                if (Registro_satisfaz(&reg, criterios, m)) {
                    Imprimir_registro(&reg);
                    encontrado = 1;
                }
            }
        }

        if (!encontrado) {
            printf("Registro inexistente.\n");
        }
        free(criterios);
    }

    if (indices) free(indices);
    fclose(arqBIN);
    return 0;
}
// FEITO: funcao_READVALUE (SELECT WHERE, 11) implementada com busca indexada e sequencial

// apagado: funcao_SELECT_RRN, no TP2 nao existe RRN, a busca por byte offset vai ficar dentro do SELECT WHERE

// pendente: funcao_DELETE (12), funcao_INSERT (13) e funcao_UPDATE (14) ficam para os passos 5 e 6, vao ser reescritas com Worst Fit e indice 

// FEITO: funcao_CREATE_INDEX (9) e a funcionalidade nova do TP2, fica para o passo 4 