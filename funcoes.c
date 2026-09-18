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
    
    // VERIFICAÇÃO ADICIONADA AQUI:
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
//Funcao 5: remocao logica, percorre arquivo e marca REGISTRO_REMOVIDO
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

    //marca arquivo como inconsistente durante a modificacao
    cab.status = '0';
    fseek(arqBIN, 0, SEEK_SET);
    Escrever_Cabecalho(arqBIN, &cab);

    //Leitura das buscas
    for (int i = 0; i < n; i++) {
        int nCriterios;
        scanf("%d", &nCriterios);

        char nomeCampo[5][20];
        char valorCampo[5][50];

        // Lendo os criterios desta busca
        for (int j = 0; j < nCriterios; j++) {
            scanf("%s", nomeCampo[j]);
            if (strcmp(nomeCampo[j], "unidadeMedida") == 0)
                ScanQuoteString(valorCampo[j]);
            else
                scanf("%s", valorCampo[j]);
        }
        
        // Loop para encontrar e remover os registros
        for (int rrn = 0; rrn < cab.proxRNN; rrn++) {
            Registro reg;
            if (!Ler_registro_rrn(arqBIN, rrn, &reg))
                continue;
            
            if (reg.removido == REGISTRO_REMOVIDO)
                continue;

            // --- CHECAGEM EXATA E BLINDADA ---
            int match = 1;
            for (int j = 0; j < nCriterios; j++) {
                if (strcmp(nomeCampo[j], "idPoPs") == 0) {
                    if (reg.IDPoPs != atoi(valorCampo[j])) match = 0;
                } 
                else if (strcmp(nomeCampo[j], "idPoPsConectado") == 0) {
                    if (reg.IDPoPs_Conectado != atoi(valorCampo[j])) match = 0;
                } 
                else if (strcmp(nomeCampo[j], "velocidade") == 0) {
                    if (reg.velocidade != atoi(valorCampo[j])) match = 0;
                } 
                else if (strcmp(nomeCampo[j], "unidadeMedida") == 0) {
                    if (reg.unidade_medida != valorCampo[j][0]) match = 0;
                }
            }

            if (!match)
                continue; // Nao passou 100% nos criterios
            
            // --- REMOCAO ---
            reg.removido = REGISTRO_REMOVIDO;
            reg.encadeamento_pilha = cab.topo_Pilha; // empilha 
            cab.topo_Pilha = rrn;
            cab.nroRegRem++;
            cab.nroPares--; // diminui a contagem de ativos

            fseek(arqBIN, RRN_posicao(rrn), SEEK_SET);
            Escrever_registro(arqBIN, &reg);
        }
    }

    // finalizou remocoes: arquivo volta a ficar consistente
    cab.status = '1';
    fseek(arqBIN, 0, SEEK_SET);
    Escrever_Cabecalho(arqBIN, &cab);
    
    fclose(arqBIN);
    BinarioNaTela(nomeBin);
    return 0;
}
//Funcao 6: insercao com reaproveitamento de espacos removidos
//(pilha) ou no final do arquivo, quando a pilha esta vazia
int funcao_INSERT(char *nomeBin, int quantBusca){
    FILE *arqBin;
    /* "rb+" preserva o conteudo existente; "wb" apagaria o
     * arquivo inteiro */
    arquivo_Abertura(&arqBin, nomeBin, "rb+");

    Cabecalho cab;
    if(!Ler_Cabecalho(arqBin, &cab) || cab.status == STATUS_INCONSISTENTE){
        printf("Falha no processamento do arquivo.\n");
        fclose(arqBin);
        return 1;
    }

    for(int i = 0; i < quantBusca; i++){
        char strId[32], strConect[32], strVelo[32], strUnid[32];

        /* le como texto pra poder tratar "NULO" nos campos
         * numericos tambem, igual csv_LeLinha ja faz na [1] */
        scanf("%s %s %s", strId, strConect, strVelo);
        ScanQuoteString(strUnid);

        Registro reg;
        reg.removido = REGISTRO_NAO_REMOVIDO;
        reg.encadeamento_pilha = -1;

        reg.IDPoPs           = (strcmp(strId, "NULO") == 0)     ? VALOR_NULO_INT  : atoi(strId);
        reg.IDPoPs_Conectado = (strcmp(strConect, "NULO") == 0) ? VALOR_NULO_INT  : atoi(strConect);
        reg.velocidade       = (strcmp(strVelo, "NULO") == 0)   ? VALOR_NULO_INT  : atoi(strVelo);
        reg.unidade_medida   = (strUnid[0] == '\0')             ? VALOR_NULO_CHAR : strUnid[0];

        int rrnDestino;

        if (cab.topo_Pilha != -1) {
            /* tem espaco removido: reaproveita o TOPO da pilha */
            rrnDestino = cab.topo_Pilha;

            Registro removidoAntigo;
            Ler_registro_rrn(arqBin, rrnDestino, &removidoAntigo);
            cab.topo_Pilha = removidoAntigo.encadeamento_pilha; /* desempilha */

            cab.nroRegRem--;
        } else {
            /* pilha vazia: escreve no final do arquivo */
            rrnDestino = cab.proxRNN;
            cab.proxRNN++;
            //cab.nroPares++;
        }

        arquivo_posicSeek(arqBin, rrnDestino);
        Escrever_registro(arqBin, &reg);

        // cab.nroPares++;
    }

    fseek(arqBin, 0, SEEK_SET);
    Escrever_Cabecalho(arqBin, &cab);

    fclose(arqBin);
    BinarioNaTela(nomeBin);

    return 0;
}

  int funcao_UPDATE(char *nomeBin, int quantBusca) {
      printf("Falha no processamento do arquivo.\n");
      return 1;
  }