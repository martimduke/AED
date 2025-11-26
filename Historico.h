#ifndef HISTORICO_H
#define HISTORICO_H
#include "Sequencia.h"
#include "Util.h"

typedef struct historicoEntrada * Historico;

//cria historico
Historico historicoCria(void);
//destroi tudo do historico
void historicoDestroiTudo(Historico h);
//adiciona ao historico os dados fornecidos sobre o cliente, caixa, tempo de saida, total a pagar e peso total
void historicoAdicionaEntrada(Historico h, int numeroCliente, int numeroCaixa, int momentoSaida, int totalPago, int pesoTotal);
//retorna o numero do cliente da ultima entrada inserida
int historicoNumeroCliente(Historico h);
//retorna o numero da caixa da ultima entrada inserida
int historicoNumeroCaixa(Historico h);
//retorna o momento de saida da ultima entrada inserida
int historicoMomentoSaida(Historico h);
//retorna o valor total pago na última entrada inserida
int historicoTotalPago(Historico h);
//retorna o peso total pago na ultima entrada inserida
int historicoPesoTotal(Historico h);
//iterador para percorrer as entradas do historico
Iterador historicoIteradorOrdenado(Historico h);
#endif
