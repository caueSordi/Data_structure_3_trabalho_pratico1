#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fornecidas.h"
#include "registro.h"
#include "arquivos.h"
#include "busca.h"
#include "funcoes.h"

// Função 8: Leitura do CSV, Criação e Escrita no .bin
//  alterado: agora e a funcionalidade 8 (CREATE TABLE) do trabalho 2
void funcao_CREATE(char *nomeCSV, char *nomeBin)
{
    FILE *arqCSV, *arqBIN;

    // apagado: idPoPs, idPoPsConectado, velocidade e unidade, o csv_LeLinha agora preenche o Registro direto
    Registro registro; // alterado: era o ponteiro Registro *registro, sem uso, agora recebe cada linha do CSV
    Cabecalho cabecalho;

    // manipulação do arquivo CSV e BIN
    arquivo_Abertura(&arqCSV, nomeCSV, "rb");

    // verificação de integridade

    if (arqCSV == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    arquivo_Abertura(&arqBIN, nomeBin, "wb");
    // novo: verifica tambem se o bin abriu
    if (arqBIN == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        arquivo_Close(&arqCSV);
        return;
    }

    // escrita do cabecalho 1
    Inicializa_cabecalho(&cabecalho); // alterado: o cabecalho usa a struct, tem 25 bytes, status 0 e lista vazia
    Escrever_Cabecalho(arqBIN, &cabecalho);

    // leitura do arquivo

    // apagado: removido, encadeamento_pilha e rnn, o Registro ja comeca com esses valores

    char linhaCabecalho[1024]; // alterado: era 100, uma linha maior viraria um registro
    fgets(linhaCabecalho, sizeof(linhaCabecalho), arqCSV);

    while (csv_LeLinha(arqCSV, &registro))
    { // alterado: preenche o Registro direto, sem as variaveis soltas

        // escrita no binario
        registro.tamanhoRegistro = calcula_tamanho_registro(&registro); // novo: registro de tamanho variavel
        Escrever_registro(arqBIN, &registro);
        // apagado: as escritas de removido, encadeamento_pilha, idPoPs, idPoPsConectado, velocidade e unidade

        cabecalho.proxByteOffset += tamanho_total_registro(&registro);
        cabecalho.nroPoPs++;
    }

    cabecalho.status = STATUS_CONSISTENTE;
    arquivo_inicio(arqBIN);
    Escrever_Cabecalho(arqBIN, &cabecalho);
    // apagado: arquivo_inicio e a escrita campo a campo do cabecalho antigo

    arquivo_Close(&arqCSV);
    arquivo_Close(&arqBIN);
    BinarioNaTela(nomeBin);
    return;
}

// case 9 Create index
void funcao_CREATE_INDEX(char *nomeBin, char *nomeIndice)
{
    FILE *arqBIN = fopen(nomeBin, "rb");
    if (arqBIN == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    Cabecalho cab;
    if (!Ler_Cabecalho(arqBIN, &cab) || cab.status == STATUS_INCONSISTENTE)
    {
        printf("Falha no processamento do arquivo.\n");
        fclose(arqBIN);
        return;
    }

    RegistroIndice *indices = NULL;
    if (cab.nroPoPs > 0)
    {
        indices = malloc(cab.nroPoPs * sizeof(RegistroIndice));
    }
    int qtdIndices = 0;

    arquivo_PosCabecalho(arqBIN);
    Registro reg;
    long long offsetAtual = arquivo_PosicaoAtual(arqBIN);

    // extrai dados ativos do arquivo de dados para o array
    while (Ler_registro(arqBIN, &reg))
    {
        if (reg.removido != REGISTRO_REMOVIDO && reg.idPoPs != VALOR_NULO_INT)
        {
            if (indices != NULL)
            {
                indices[qtdIndices].idPoPs = reg.idPoPs;
                indices[qtdIndices].byteOffset = offsetAtual;
                qtdIndices++;
            }
        }
        offsetAtual = arquivo_PosicaoAtual(arqBIN);
    }
    fclose(arqBIN);

    // ordena pelo idPoPs crescente
    if (indices != NULL && qtdIndices > 0)
    {
        qsort(indices, qtdIndices, sizeof(RegistroIndice), compara_indice);
    }

    FILE *arqIND = fopen(nomeIndice, "wb");
    if (arqIND == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        if (indices)
            free(indices);
        return;
    }

    char status = '0';
    fwrite(&status, sizeof(char), 1, arqIND);

    // escreve os 12 bytes por registro no indice
    for (int i = 0; i < qtdIndices; i++)
    {
        fwrite(&indices[i].idPoPs, sizeof(int), 1, arqIND);
        fwrite(&indices[i].byteOffset, sizeof(long long), 1, arqIND);
    }

    // fecha atualizando status
    status = '1';
    fseek(arqIND, 0, SEEK_SET);
    fwrite(&status, sizeof(char), 1, arqIND);

    fclose(arqIND);
    if (indices)
        free(indices);

    BinarioNaTela(nomeIndice);
    return;
}

// Funcao 10:lista todos os registros (todos registros nao excluidos)
//  alterado: agora e a funcionalidade 10 (SELECT) do Trabalho 2

void funcao_READ_ALL(char *nomeBin)
{
    // USAR AS FUNCOES DO ARQUVIO.H
    FILE *arqBIN = fopen(nomeBin, "rb");

    // mensagem de erro
    if (arqBIN == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    Cabecalho cab;
    if (!Ler_Cabecalho(arqBIN, &cab))
    {
        printf("Falha no processamento do arquivo.\n");
        fclose(arqBIN); // novo: faltava fechar o arquivo
        return;
    }

    if (cab.status != STATUS_CONSISTENTE)
    {
        printf("Falha no processamento do arquivo.\n");
        fclose(arqBIN);
        return;
    }

    // Ler_Cabecalho já deixouo ponteiro para o inicio ro RRN 0, mas o fseek  deixa isso explicito e protege contra mudanças na leitura do cabecalho
    //  alterado: agora o ponteiro fica no primeiro registro, logo depois dos 25 bytes do cabecalho, nao ha mais RRN @
    arquivo_PosCabecalho(arqBIN);

    Registro reg;
    int encontrado = 0;

    while (Ler_registro(arqBIN, &reg))
    { // alterado: Ler_registro agora le registro variavel e pula para o proximo
        if (reg.removido == REGISTRO_REMOVIDO)
            continue;

        Imprimir_registro(&reg);
        encontrado = 1;
    }
    // Fim da leitura dos registros

    if (!encontrado)
    {
        printf("Registro inexistente\n");
    }

    fclose(arqBIN);
    return;
}

// case 11 implementada com busca indexada e sequencial
void funcao_READVALUE(char *nomeBin, char *nomeIndice, int nBuscas)
{
    FILE *arqBIN = fopen(nomeBin, "rb");
    FILE *arqIND = fopen(nomeIndice, "rb");

    if (arqBIN == NULL || arqIND == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        if (arqBIN)
            fclose(arqBIN);
        if (arqIND)
            fclose(arqIND);
        return;
    }

    Cabecalho cab;
    if (!Ler_Cabecalho(arqBIN, &cab) || cab.status == STATUS_INCONSISTENTE)
    {
        printf("Falha no processamento do arquivo.\n");
        fclose(arqBIN);
        fclose(arqIND);
        return;
    }

    char statusIndice;
    if (fread(&statusIndice, sizeof(char), 1, arqIND) != 1 || statusIndice == '0')
    {
        printf("Falha no processamento do arquivo.\n");
        fclose(arqBIN);
        fclose(arqIND);
        return;
    }

    // carrega indice inteiro para a memoria
    fseek(arqIND, 0, SEEK_END);
    long long tamIndice = ftell(arqIND) - 1; // desconta cabecalho
    int qtdIndices = tamIndice / 12;
    RegistroIndice *indices = NULL;

    if (qtdIndices > 0)
    {
        indices = malloc(qtdIndices * sizeof(RegistroIndice));
        fseek(arqIND, 1, SEEK_SET);
        for (int i = 0; i < qtdIndices; i++)
        {
            fread(&indices[i].idPoPs, sizeof(int), 1, arqIND);
            fread(&indices[i].byteOffset, sizeof(long long), 1, arqIND);
        }
    }
    fclose(arqIND);

    // faz as buscas
    for (int i = 0; i < nBuscas; i++)
    {
        int m;
        scanf("%d", &m);

        Criterio *criterios = malloc(m * sizeof(Criterio));
        int temIdPops = 0;
        int idBuscado = -1;

        for (int j = 0; j < m; j++)
        {
            scanf("%s", criterios[j].nomeCampo);
            ScanQuoteString(criterios[j].valor); // lida com as aspas para tratar strings

            // checa se possui campo indexado para decidir a estrategia
            if (strcmp(criterios[j].nomeCampo, "idPoPs") == 0)
            {
                temIdPops = 1;
                idBuscado = atoi(criterios[j].valor);
            }
        }

        int encontrado = 0;

        if (temIdPops && indices != NULL)
        {
            // executa busca indexada super rapida usando o idPoPs
            long long offset = busca_binaria_indice(indices, qtdIndices, idBuscado);
            if (offset != -1)
            {
                Registro reg;
                if (Ler_registro_offset(arqBIN, offset, &reg))
                {
                    // testa se atende a TODOS os outros criterios
                    if (Registro_satisfaz(&reg, criterios, m))
                    {
                        Imprimir_registro(&reg);
                        encontrado = 1;
                    }
                }
            }
        }
        else
        {
            // fallback para a busca sequencial varrendo o arquivo inteiro
            arquivo_PosCabecalho(arqBIN);
            Registro reg;
            while (Ler_registro(arqBIN, &reg))
            {
                if (Registro_satisfaz(&reg, criterios, m))
                {
                    Imprimir_registro(&reg);
                    encontrado = 1;
                }
            }
        }

        if (!encontrado)
        {
            printf("Registro inexistente.\n");
        }
        printf("\n");
        free(criterios);
    }

    if (indices)
        free(indices);
    fclose(arqBIN);
    return;
}

// Funcao 12: remocao logica, percorre arquivo e marca REGISTRO_REMOVIDO
void remove_chaveIdx(RegistroIndice *Idx, int *qtdIdx, int idRem)
{
    int esquerda = 0;
    int direita = *qtdIdx - 1;
    int posicao = -1;

    while (esquerda <= direita)
    {
        int meio = esquerda + (direita - esquerda) / 2;
        if (Idx[meio].idPoPs == idRem)
        {
            posicao = meio;
            break;
        }
        else if (Idx[meio].idPoPs < idRem)
        {
            esquerda = meio + 1;
        }
        else
        {
            direita = meio - 1;
        }
    }

    if (posicao != -1)
    {
        for (int i = posicao; i < (*qtdIdx) - 1; i++)
        {
            Idx[i] = Idx[i + 1];
        }
        (*qtdIdx)--;
    }
}

void remocao_logica(FILE *ArqBin, RegistroIndice *Idx, int *qtdIdx, Cabecalho *cab, long long posReg, int tamReg, int id)
{
    long long anterior = -1;
    long long atual = cab->topoLista;

    // percorre a lista de reomovidos (worst fit)
    while (atual != -1)
    {
        char remAtual;
        int tamAtual;
        long long proxAtual;

        // leitura somentes dos dados necessarios
        arquivo_Posiciona(ArqBin, atual);
        arquivo_LeChar(ArqBin, &remAtual);
        arquivo_LeInt(ArqBin, &tamAtual);
        arquivo_LeLongLong(ArqBin, &proxAtual);

        if (tamAtual <= tamReg)
            break;

        anterior = atual;
        atual = proxAtual;
    }

    // gravacao dos dados
    arquivo_Posiciona(ArqBin, posReg);
    arquivo_EscreveChar(ArqBin, REGISTRO_REMOVIDO);
    arquivo_EscreveInt(ArqBin, tamReg);
    arquivo_EscreveLongLong(ArqBin, atual);
    arquivo_EscreveLixo(ArqBin, tamReg - 13);

    // up no encadeamento
    if (anterior == -1)
    {
        cab->topoLista = posReg;
    }
    else
    {
        arquivo_Posiciona(ArqBin, anterior + 5);
        arquivo_EscreveLongLong(ArqBin, posReg);
    }

    cab->nroPoPs--;
    cab->nroRegRem++;

    // remocao da chave correspond no idx primario
    if (id != VALOR_NULO_INT && Idx != NULL)
    {
        remove_chaveIdx(Idx, qtdIdx, id);
    }
}
void funcao_DELETE(char *nomeBin, char *nomeIndice, int n)
{

    FILE *arqBIN = fopen(nomeBin, "rb+");
    FILE *arqIdx = fopen(nomeIndice, "rb+");

    if (arqBIN == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    if (arqIdx == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        fclose(arqBIN);
        return;
    }

    Cabecalho cab;
    char statusIdx;
    if (!Ler_Cabecalho(arqBIN, &cab) || cab.status == STATUS_INCONSISTENTE || !arquivo_LeChar(arqIdx, &statusIdx) || statusIdx == STATUS_INCONSISTENTE)
    {
        printf("Falha no processamento do arquivo.\n");
        fclose(arqBIN);
        fclose(arqIdx);
        return;
    }

    // carregando o arquido de indice
    fseek(arqIdx, 0, SEEK_END);
    long long tamArqIdx = arquivo_PosicaoAtual(arqIdx);
    int qtdIdx = (int)((tamArqIdx - 1) / (sizeof(int) + sizeof(long long)));
    RegistroIndice *Idxs = NULL;

    if (qtdIdx > 0)
    {
        Idxs = (RegistroIndice *)malloc(qtdIdx * sizeof(RegistroIndice));
        arquivo_Posiciona(arqIdx, 1);

        // leitura do arquivo
        for (int i = 0; i < qtdIdx; i++)
        {
            arquivo_LeInt(arqIdx, &Idxs[i].idPoPs);
            arquivo_LeLongLong(arqIdx, &Idxs[i].byteOffset);
        }
    }

    // marca arquivo como inconsistente durante a modificacao
    cab.status = STATUS_INCONSISTENTE;
    arquivo_inicio(arqBIN);
    Escrever_Cabecalho(arqBIN, &cab);

    arquivo_inicio(arqIdx);
    arquivo_EscreveChar(arqIdx, STATUS_INCONSISTENTE);
    fclose(arqIdx);

    // realizando as N buscas
    for (int i = 0; i < n; i++)
    {
        int nCriterios;
        scanf("%d", &nCriterios);

        Criterio criterios[MAX_CRITERIOS];
        for (int j = 0; j < nCriterios; j++)
        {
            scanf("%s", criterios[j].nomeCampo);
            ScanQuoteString(criterios[j].valor);
        }

        long long posicao = TAM_CABECALHO;
        while (posicao < cab.proxByteOffset)
        {
            long long posReg = posicao;
            Registro reg;
            arquivo_Posiciona(arqBIN, posReg);
            if (!Ler_registro(arqBIN, &reg))
                break;

            posicao = posReg + reg.tamanhoRegistro;

            if (reg.removido == REGISTRO_REMOVIDO)
                continue;
            if (!Registro_satisfaz(&reg, criterios, nCriterios))
                continue;
            remocao_logica(arqBIN, Idxs, &qtdIdx, &cab, posReg, reg.tamanhoRegistro, reg.idPoPs);
        }
    }

    cab.status = STATUS_CONSISTENTE;
    arquivo_inicio(arqBIN);
    Escrever_Cabecalho(arqBIN, &cab);
    fclose(arqBIN);

    // regravando o arqIdx
    arqIdx = fopen(nomeIndice, "wb");
    if (arqIdx == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        free(Idxs);
        return;
    }

    arquivo_EscreveChar(arqIdx, STATUS_CONSISTENTE);
    for (int i = 0; i < qtdIdx; i++)
    {
        arquivo_EscreveInt(arqIdx, Idxs[i].idPoPs);
        arquivo_EscreveLongLong(arqIdx, Idxs[i].byteOffset);
    }

    fclose(arqIdx);
    free(Idxs);

    BinarioNaTela(nomeBin);
    BinarioNaTela(nomeIndice);
}

// Insere uma chave no vetor de indices mantendo a ordem crescente de idPoPs
// (abre espaco com realloc e empurra para a direita as chaves maiores)
void insere_chaveIdx(RegistroIndice **Idx, int *qtdIdx, int id, long long offset)
{
    *Idx = realloc(*Idx, (*qtdIdx + 1) * sizeof(RegistroIndice));

    int posicao = *qtdIdx;
    // procura a posicao do menor indice ate o momento
    while (posicao > 0 && (*Idx)[posicao - 1].idPoPs > id)
    {
        (*Idx)[posicao] = (*Idx)[posicao - 1];
        posicao--;
    }
    // insercao
    (*Idx)[posicao].idPoPs = id;
    (*Idx)[posicao].byteOffset = offset;
    (*qtdIdx)++;
}

// Grava um registro novo no arquivo de dados usando a estrategia Worst Fit.
// Foi separada de funcao_INSERT para ser reutilizada tambem pelo UPDATE.
// A lista de removidos e ordenada do maior para o menor espaco, entao o
// primeiro da lista (topo) e sempre o maior.
//  - lista vazia ou topo menor que o novo: escreve no fim do arquivo
//  - topo comporta o novo: reaproveita o espaco do topo e o tira da lista
// Em ambos os casos atualiza o cabecalho e o indice.
// Retorna 1 em sucesso e 0 em falha de leitura.
int insere_registro_worst_fit(FILE *arqBIN, Cabecalho *cab, Registro *regNovo, RegistroIndice **Idxs, int *qtdIdx)
{
    Registro regTopo;
    int tamNovo = calcula_tamanho_registro(regNovo);
    int tamTopo = 0;
    long long prox = LISTA_VAZIA;
    long long offsetTopo = cab->topoLista;
    long long offsetInsert;

    // se existe removido, le o topo para saber seu tamanho e o proximo da lista
    if (offsetTopo != LISTA_VAZIA)
    {
        if (!Ler_registro_offset(arqBIN, offsetTopo, &regTopo))
        {
            printf("Falha no processamento do arquivo.\n");
            return 0;
        }
        tamTopo = regTopo.tamanhoRegistro;
        prox = regTopo.encadeamento;
    }

    // o registro novo entra como ativo e fora da lista de removidos
    regNovo->removido = REGISTRO_NAO_REMOVIDO;
    regNovo->encadeamento = LISTA_VAZIA;

    // Worst Fit: insere no fim se a lista estiver vazia ou se o maior espaco nao comportar
    if (offsetTopo == LISTA_VAZIA || tamTopo < tamNovo)
    {
        // caso 1: escreve no fim, com o tamanho exato do registro
        offsetInsert = cab->proxByteOffset;
        regNovo->tamanhoRegistro = tamNovo;

        arquivo_Posiciona(arqBIN, offsetInsert);
        Escrever_registro(arqBIN, regNovo);

        cab->proxByteOffset += tamNovo;
        cab->nroPoPs++;
    }
    else
    {
        // caso 2: reaproveita o espaco do topo (mantem o tamanho dele, a sobra vira '$')
        offsetInsert = offsetTopo;
        regNovo->tamanhoRegistro = tamTopo;

        arquivo_Posiciona(arqBIN, offsetInsert);
        Escrever_registro(arqBIN, regNovo);

        // o topo da lista passa a ser o proximo removido
        cab->topoLista = prox;
        cab->nroRegRem--;
        cab->nroPoPs++;
    }

    // registros com id nulo nao entram no indice
    if (regNovo->idPoPs != VALOR_NULO_INT)
    {
        insere_chaveIdx(Idxs, qtdIdx, regNovo->idPoPs, offsetInsert);
    }

    return 1;
}

// Funcao 13: insere N registros lidos da entrada padrao
// Fluxo: carrega o indice na memoria, marca binario e indice como inconsistentes,
// insere cada registro com insere_registro_worst_fit e, no fim, regrava
// cabecalho e indice como consistentes.
void funcao_INSERT(char *nomeBin, char *nomeIdx, int n)
{
    FILE *arqBIN = fopen(nomeBin, "rb+");
    FILE *arqIdx = fopen(nomeIdx, "rb+");

    if (arqBIN == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    if (arqIdx == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        fclose(arqBIN);
        return;
    }

    Cabecalho cab;
    char statusIdx;
    if (!Ler_Cabecalho(arqBIN, &cab) || cab.status == STATUS_INCONSISTENTE || !arquivo_LeChar(arqIdx, &statusIdx) || statusIdx == STATUS_INCONSISTENTE)
    {
        printf("Falha no processamento do arquivo.\n");
        fclose(arqBIN);
        fclose(arqIdx);
        return;
    }

    // carregando o arquido de indice
    fseek(arqIdx, 0, SEEK_END);
    long long tamArqIdx = arquivo_PosicaoAtual(arqIdx);
    int qtdIdx = (int)((tamArqIdx - 1) / (sizeof(int) + sizeof(long long)));
    RegistroIndice *Idxs = NULL;

    if (qtdIdx > 0)
    {
        Idxs = (RegistroIndice *)malloc(qtdIdx * sizeof(RegistroIndice));
        arquivo_Posiciona(arqIdx, 1);

        // leitura do arquivo
        for (int i = 0; i < qtdIdx; i++)
        {
            arquivo_LeInt(arqIdx, &Idxs[i].idPoPs);
            arquivo_LeLongLong(arqIdx, &Idxs[i].byteOffset);
        }
    }

    // marca arquivo como inconsistente durante a modificacao
    cab.status = STATUS_INCONSISTENTE;
    arquivo_inicio(arqBIN);
    Escrever_Cabecalho(arqBIN, &cab);

    arquivo_inicio(arqIdx);
    arquivo_EscreveChar(arqIdx, STATUS_INCONSISTENTE);
    fclose(arqIdx);

    Registro reg_aux, reg;
    char nome[TAM_MAX_STRING], pais[TAM_MAX_STRING], sigla[TAM_SIGLA + 1];
    int id, tamNovo, tamTopo;
    long long offsetTopo, prox, offsetInsert;
    // realizando a leitura dos N dados
    for (int i = 0; i < n; i++)
    {

        scanf("%d", &id);
        ScanQuoteString(nome);
        ScanQuoteString(pais);
        ScanQuoteString(sigla);

        // nova estrturua de reg
        // monta o registro com os dados lidos (campo vazio vira nulo)
        Inicializa_registro(&reg_aux);
        reg_aux.idPoPs = id;
        strcpy(reg_aux.nomePoPs, nome);
        strcpy(reg_aux.nomePais, pais);

        if (sigla[0] != '\0')
        {
            reg_aux.siglaPais[0] = sigla[0];
            reg_aux.siglaPais[1] = (sigla[1] != '\0') ? sigla[1] : VALOR_NULO_CHAR;
        }

        tamNovo = calcula_tamanho_registro(&reg_aux);
        offsetTopo = cab.topoLista;

        // a escolha do espaco (fim do arquivo ou removido) e feita pela funcao auxiliar
        if (!insere_registro_worst_fit(arqBIN, &cab, &reg_aux, &Idxs, &qtdIdx))
            break;
    }

    // voltando a consistencia
    cab.status = STATUS_CONSISTENTE;
    arquivo_inicio(arqBIN);
    Escrever_Cabecalho(arqBIN, &cab);
    fclose(arqBIN);

    // regrava o indice inteiro, ja atualizado e ordenado
    arqIdx = fopen(nomeIdx, "wb");
    if (arqIdx == NULL)
    {
        printf("Falha no processamento do arquivo.");
        free(Idxs);
        return;
    }

    arquivo_EscreveChar(arqIdx, STATUS_CONSISTENTE);
    for (int i = 0; i < qtdIdx; i++)
    {
        arquivo_EscreveInt(arqIdx, Idxs[i].idPoPs);
        arquivo_EscreveLongLong(arqIdx, Idxs[i].byteOffset);
    }

    fclose(arqIdx);
    free(Idxs);

    BinarioNaTela(nomeBin);
    BinarioNaTela(nomeIdx);
}

// Aplica em *reg os campos de atualizacao recebidos (pares nomeCampo/valor).
// So os campos citados em "dados" sao alterados; os demais ficam como estavam.
// Valor vazio em siglaPais vira nulo ("$$").
void atualiza_registro(Criterio *dados, int nDados, Registro *reg)
{

    for (int i = 0; i < nDados; i++)
    {
        if (strcmp(dados[i].nomeCampo, "idPoPs") == 0)
        {
            reg->idPoPs = campoIntCMP(dados[i].valor);
        }
        if (strcmp(dados[i].nomeCampo, "nomePoPs") == 0)
        {
            strcpy(reg->nomePoPs, dados[i].valor);
        }
        if (strcmp(dados[i].nomeCampo, "nomePais") == 0)
        {
            strcpy(reg->nomePais, dados[i].valor);
        }
        else if (strcmp(dados[i].nomeCampo, "siglaPais") == 0)
        {
            if (dados[i].valor[0] == '\0')
            {
                reg->siglaPais[0] = VALOR_NULO_CHAR;
                reg->siglaPais[1] = VALOR_NULO_CHAR;
            }
            else
            {
                reg->siglaPais[0] = dados[i].valor[0];
                reg->siglaPais[1] = (dados[i].valor[1] != '\0') ? dados[i].valor[1] : VALOR_NULO_CHAR;
            }
        }
    }
}

// Funcao 14: atualiza registros. Para cada uma das N operacoes le os criterios
// de busca e os campos a atualizar, e para cada registro encontrado:
//  - se o registro atualizado cabe no espaco antigo: sobrescreve no mesmo lugar
//    (o indice so muda se o idPoPs mudou)
//  - se nao cabe: remove logicamente o antigo e insere o novo com Worst Fit
void funcao_UPDATE(char *nomeBin, char *nomeIdx, int n)
{
    FILE *arqBIN = fopen(nomeBin, "rb+");
    FILE *arqIdx = fopen(nomeIdx, "rb+");

    if (arqBIN == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    if (arqIdx == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        fclose(arqBIN);
        return;
    }

    Cabecalho cab;
    char statusIdx;
    if (!Ler_Cabecalho(arqBIN, &cab) || cab.status == STATUS_INCONSISTENTE || !arquivo_LeChar(arqIdx, &statusIdx) || statusIdx == STATUS_INCONSISTENTE)
    {
        printf("Falha no processamento do arquivo.\n");
        fclose(arqBIN);
        fclose(arqIdx);
        return;
    }

    // carregando o arquido de indice
    fseek(arqIdx, 0, SEEK_END);
    long long tamArqIdx = arquivo_PosicaoAtual(arqIdx);
    int qtdIdx = (int)((tamArqIdx - 1) / (sizeof(int) + sizeof(long long)));
    RegistroIndice *Idxs = NULL;

    if (qtdIdx > 0)
    {
        Idxs = (RegistroIndice *)malloc(qtdIdx * sizeof(RegistroIndice));
        arquivo_Posiciona(arqIdx, 1);

        // leitura do arquivo
        for (int i = 0; i < qtdIdx; i++)
        {
            arquivo_LeInt(arqIdx, &Idxs[i].idPoPs);
            arquivo_LeLongLong(arqIdx, &Idxs[i].byteOffset);
        }
    }

    // marca arquivo como inconsistente durante a modificacao
    cab.status = STATUS_INCONSISTENTE;
    arquivo_inicio(arqBIN);
    Escrever_Cabecalho(arqBIN, &cab);

    arquivo_inicio(arqIdx);
    arquivo_EscreveChar(arqIdx, STATUS_INCONSISTENTE);
    fclose(arqIdx);

    for (int i = 0; i < n; i++)
    {
        int nBuscas, nUpdates;

        // leituras dos N/p parametros de busca e atualização
        // Buscas = criterios para achar os registros; Updates = campos a alterar
        scanf("%d", &nBuscas);
        Criterio Buscas[MAX_CRITERIOS], Updates[MAX_CRITERIOS];
        for (int j = 0; j < nBuscas; j++)
        {
            scanf("%s", Buscas[j].nomeCampo);
            ScanQuoteString(Buscas[j].valor);
        }

        scanf("%d", &nUpdates);
        for (int j = 0; j < nUpdates; j++)
        {
            scanf("%s", Updates[j].nomeCampo);
            ScanQuoteString(Updates[j].valor);
        }

        long long posicao = TAM_CABECALHO;
        // fixa o fim da varredura antes de comecar: registros inseridos no fim
        // durante este laco (caso "nao cabe") nao sao reprocessados
        long long limBusca = cab.proxByteOffset;

        // verificando se ha regs que possuem os parametros de busca
        while (posicao < limBusca)
        {
            long long posReg = posicao;
            Registro regAntigo;
            arquivo_Posiciona(arqBIN, posReg);
            if (!Ler_registro(arqBIN, &regAntigo))
                break;

            // o proximo registro comeca logo apos o tamanho total deste
            posicao = posReg + regAntigo.tamanhoRegistro;

            if (regAntigo.removido == REGISTRO_REMOVIDO)
                continue;
            if (!Registro_satisfaz(&regAntigo, Buscas, nBuscas))
                continue;

            // copia o registro e aplica as alteracoes na copia
            Registro regNovo = regAntigo;
            atualiza_registro(Updates, nUpdates, &regNovo);

            int tamAntigo = regAntigo.tamanhoRegistro;
            int tamNovo = calcula_tamanho_registro(&regNovo);

            if (tamNovo <= tamAntigo)
            {
                // caso 1: cabe no mesmo espaco, sobrescreve no lugar
                // atualiza os paranmentros
                regNovo.removido = REGISTRO_NAO_REMOVIDO;
                regNovo.tamanhoRegistro = tamAntigo;
                regNovo.encadeamento = LISTA_VAZIA;

                arquivo_Posiciona(arqBIN, posReg);
                Escrever_registro(arqBIN, &regNovo);

                // verificação da chave nova
                // se o id mudou: tira a chave antiga do indice e poe a nova (mesmo offset)
                if (regAntigo.idPoPs != regNovo.idPoPs)
                {
                    if (regAntigo.idPoPs != VALOR_NULO_INT && Idxs != NULL)
                        remove_chaveIdx(Idxs, &qtdIdx, regAntigo.idPoPs);
                    if (regNovo.idPoPs != VALOR_NULO_INT)
                        insere_chaveIdx(&Idxs, &qtdIdx, regNovo.idPoPs, posReg);
                }
            }
            else
            {
                // caso 2: o novo registro nao cabe, entao remove o antigo e reinsere
                // o novo Reg é maior do que o tam disponivel
                // (remocao_logica ja tira a chave antiga do indice)
                remocao_logica(arqBIN, Idxs, &qtdIdx, &cab, posReg, tamAntigo, regAntigo.idPoPs);

                // insercao do novo reg
                // (insere_registro_worst_fit poe a chave nova no indice)
                if (!insere_registro_worst_fit(arqBIN, &cab, &regNovo, &Idxs, &qtdIdx))
                    break;
            }
        }
    }

    // voltando a consistencia
    cab.status = STATUS_CONSISTENTE;
    arquivo_inicio(arqBIN);
    Escrever_Cabecalho(arqBIN, &cab);
    fclose(arqBIN);

    // regrava o indice inteiro, ja atualizado e ordenado
    arqIdx = fopen(nomeIdx, "wb");
    if (arqIdx == NULL)
    {
        printf("Falha no processamento do arquivo.");
        free(Idxs);
        return;
    }

    arquivo_EscreveChar(arqIdx, STATUS_CONSISTENTE);
    for (int i = 0; i < qtdIdx; i++)
    {
        arquivo_EscreveInt(arqIdx, Idxs[i].idPoPs);
        arquivo_EscreveLongLong(arqIdx, Idxs[i].byteOffset);
    }

    fclose(arqIdx);
    free(Idxs);

    BinarioNaTela(nomeBin);
    BinarioNaTela(nomeIdx);
}
