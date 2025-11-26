#ifndef SUPERMERCADO_H
#define SUPERMERCADO_H
#include "Stock.h"
#include "Cliente.h"
#include "Sequencia.h"
#include "Iterador.h"
#include "Info.h"
#include "Util.h"
#include "Str.h"
#include "Caixa.h"
#include "Historico.h"
#define NUM_CAIXAS 10

typedef struct supermercado *Supermercado;

//Cria supermercado com stock, sequencia de clientes, caixas e histórico
Supermercado supermercadoCria(void);
//Destroi supermercado
void supermercadoDestroi(Supermercado s);
//Destroi supermercado ,clientes, caixas, histórico e stock
void supermercadoDestroiTudo(Supermercado s);
//Avança o tempo em segundos, processando o atendimento em todas as caixas
void supermercadoAvancaTempo(Supermercado s, int segundos);
//Fecha o supermercado forcando os clientes sem cesto para caixa 0, clientes com cesto para caixa 1, e processa tudo at+e esvaziar as caixas
void supermercadoFecha(Supermercado s);
//Adiciona um novo cliente e retorna o numero atribuído
int supermercadoAdicionaCliente(Supermercado s);
//Cria e adiciona um produto ao stock com designação, quantidade, preço e peso fornecidos
int supermercadoAdicionaProduto(Supermercado s, string designacao, int quantidade, int preco, int peso);
//Incrementa uma quantidade ao produto ja existente no stock
int supermercadoIncrementaProduto(Supermercado s, string designacao, int quantidade);
//Faz o cliente adicionar uma quantidade do produto para ao seu cesto, retirando do stock
int supermercadoClienteAdicionaProduto(Supermercado s, int clienteNumero, string designacao, int quantidade);
//Coloca o cliente na fila da caixa
int supermercadoClienteVaiParaCaixa(Supermercado s, int clienteNumero, int numeroCaixa);
//Seleciona para o cliente  a caixa de fila mais curta e o mete o la
int supermercadoClienteVaiParaMelhorCaixa(Supermercado s, int clienteNumero);
//Retorna o tempo corrente do supermercado
int supermercadoTempoCorrente(Supermercado s);
//Reotorna o stock do supermercado
Stock supermercadoStock(Supermercado s);
//Retorna o Cliente que possui número
Cliente supermercadoObtemCliente(Supermercado s, int numero);
//Cria um iterador que percorre todos os clientes, na ordem em que entraram no supermercado
Iterador supermercadoIteradorClientesOrdenado(Supermercado s);
//Cria um iterador que percorre todas as caixas por ordem dos índices
Iterador supermercadoIteradorCaixasOrdenado(Supermercado s);
//Cria um iterador que percorre todas as entradas do histórico, na ordem cronológica em que foram registadas
Iterador supermercadoIteradorHistoricoOrdenado(Supermercado s);

#endif
