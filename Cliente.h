#ifndef CLIENTE_H
#define CLIENTE_H
#include "Produto.h"
#include "Dicionario.h"
#include "Iterador.h"
#include "Info.h"
#include "Util.h"
#include "Str.h"

typedef struct cliente *Cliente;

//Cria um cliente com o seu numero de identificacao
Cliente clienteCria(int numero);
//destroi o cliente
void clienteDestroi(Cliente c);
//destroi o cliente e os seus produtos
void clienteDestroiTudo(Cliente c);
//retorna o numero de identificacao do cliente
int clienteNumero(Cliente c);
//caso o produto exista no cesto o cliente incrementa com a nova quantidade, caso nao exista no cesto cria e coloca com a quantidade nova
int clienteAdicionaProduto(Cliente c, Produto p, int quantidade);
//verifica se o cesto esta fazio
int clienteCestoVazio(Cliente c);
//retorna o total a pagar no cesto do cliente
int clienteTotalPagar(Cliente c);
//retorna o peso total do cesto do cliente
int clientePesoTotalCesto(Cliente c);
//retorna o numero da caixa que esta o cliente
int clienteNumeroCaixa(Cliente c);
//define ou altera o numero da caixa do cliente
void clienteDefineNumeroCaixa(Cliente c, int numeroCaixa);
//iterador ordenado para percorrer os produtos do cesto
Iterador clienteIteradorCestoOrdenado(Cliente c);

#endif
