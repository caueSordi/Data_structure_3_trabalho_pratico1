# Resumo Trabalho 2:

## Descrição do Arquivo de Dados PoPs

### Registro de Cabeçalho:
* status: ["0',"1"] -  1 byte (string)
* topoLista: byteoffset ou -1 - 8 bytes (int)
* proxByteOffset: inicio =  0  e incrementa - 8 bytes (int)
* nroRegRem: inicio = 0. - 8 bytes (int)
* nroPoPs: qtd de dados que estao armazenados - 4 bytes (int)
tamTotal = 25 bytes

### Registro de Dados:
* idPoPs: não pode ser nulo. 4 bytes (int)
* siglaPais: 2 bytes( string)
* nomePoPs: X bytes(string) {TAMANHO VARIAVEL}
* nomePais: X bytes(string) {TAMANHO VARIAVEL}
* removido: 1 byte (string)
* tamReg: 4 bytes (int)
* encadeamentoLista: 8 bytes (int)

## Descrição do Arquivo de Índice Primário indexaPoPs

O arquivo de dados PoPs é indexado por um índice primário, que é definido sobre o
campo idPoPs. O índice primário também é um arquivo, sendo chamado de arquivo de
índice indexaPoPs.

### Registro de Cabeçalho
* status: ["0',"1"] -  1 byte (string)

### Registro de Dados
* idPoPs: 4 bytes (int)
* byteoffset: byte offset do registro do arquivo de dados que se refere ao idPoPs – 8 bytes (int)
tamTotal = 12 bytes

## Funções
1. Create Table - parece ser igual ao T1
2. Create Index - usado para criar um índice sobre
um campo (ou um conjunto de campos) de busca. 
    * Crie um arquivo de índice primário para um arquivo de dados de entrada.
    * . O arquivo de índice primário deve ser gerado de acordo com as especificações da Descrição do Arquivo de Índice Primário 
    * Resumo: no momento da criação, o arquivo de índice primário deve possuir um registro de cabeçalho e vários registros de dados.
3. Select FROM - igual ao T1
4. Select FROM WHERE - Em situações nas quais a busca for feita considerando o campo idPoPs, deve ser utilizado o arquivo de índice indexaPoPs para se fazer a busca. Tem-se, nesse caso, uma busca indexada. Para os demais casos, deve ser feita uma busca sequencial
    * deve ser executada n vezes seguidas, msm quando nao o criterio nao seja satisfeito
5. Delete - remoção logica  - igual ao T1 
    * A implementação dessa funcionalidade deve ser realizada usando o conceito de lista de registros logicamente removidos de acordo com a abordagem Worst Fit
6. Insert Into - a inserção de novos registros em um arquivo de dados de entrada, baseado na abordagem dinâmica de reaproveitamento de espaços de registros logicamente removidos. A implementação dessa funcionalidade deve ser realizada usando o conceito de lista de registros logicamente removidos de acordo com a abordagem Worst Fit
    * Na entrada desta funcionalidade, os dados são referentes aos seguintes campos, na seguinte ordem: idPoPs, nomePoPs, nomePais, siglaPais.
7. UPDATE - Permita a atualização de registros de um arquivo de dados de entrada, baseado na abordagem dinâmica de reaproveitamento de espaços de registros logicamente removidos. 