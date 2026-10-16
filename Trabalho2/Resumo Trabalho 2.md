# Resumo Trabalho 2:

## Registro de Cabeçalho:
* status: ["0',"1"] -  1 byte (string)
* topoLista: byteoffset ou -1 - 8 bytes (int)
* proxByteOffset: inicio =  0  e incrementa - 8 bytes (int)
* nroRegRem: inicio = 0. - 8 bytes (int)
* nroPoPs: qtd de dados que estao armazenados - 4 bytes (int)
tamTotal = 25 bytes

## Registro de Dados:
* idPoPs: não pode ser nulo. 4 bytes (int)
* siglaPais: 2 bytes( string)
* nomePoPs: X bytes(string) {TAMANHO VARIAVEL}
* nomePais: X bytes(string) {TAMANHO VARIAVEL}
* removido: 1 byte (string)
* tamReg: 4 bytes (int)
* encadeamentoLista: 8 bytes (int)

## Descrição do Arquivo de Índice Primário indexaPoPs
