#ifndef CAIXA_H
#define CAIXA_H

#include "Fila.h"
#include "Cliente.h"
#include "Info.h"

typedef struct caixa * Caixa;

//cria uma caixa com numero associado
Caixa caixaCria(int numero);
//destroi a caixa
void caixaDestroi(Caixa c);
//destroi a caxa e os clientes
void caixaDestroiTudo(Caixa c);
//adiciona um cliente a caixa
int caixaAdicionaCliente(Caixa c, Cliente cliente);
//inicia o atendimento do proximo cliente da fila e se houver define o tempo necessario para o atendimento
void caixaIniciaAtendimento(Caixa c);
//retorna o calculo do do tempo de atendimento com base nos produtos
int calculaTempoAtendimento(Cliente cliente);
//retorna 1 se a caixa tem um cliente a ser atendito, 0 se nao
int caixaTemClienteSerAtendido(Caixa c);
//
int caixaProcessaTempo(Caixa c);
//retorna o numero da caixa
int caixaNumero(Caixa c);
//retorna o tempo restante para terminar o atendimento atual da caixa
int caixaTempoRestante(Caixa c);
//retorna o numero de clientes que estao a espera na fila da caixa
int caixaTamanhoFila(Caixa c);
//remove e retorna o cliente que acabou de ser atendido
Cliente caixaRemoveClienteAtendido(Caixa c);
//retorna o cliente que esta neste momento a ser atendido
Cliente caixaClienteSerAtendido(Caixa c);
//retorna a fila de clientes associada a caixa
Fila caixaFilaClientes(Caixa c);
//iterador para percorrer a fila de clientes da caixa pela ordem de chegada
Iterador caixaIteradorFila(Caixa c);

#endif
