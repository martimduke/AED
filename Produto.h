#ifndef PRODUTO_H
#define PRODUTO_H
#include "Util.h"
#include "Info.h"
#define MAX_DESIGNACAO 100

typedef struct produto *Produto;

//Cria um novo produto
Produto produtoCria(string designacao, int quantidade, int preco, int peso);
//Destroi o produto
void produtoDestroi(Produto p);
//funcao que devolve o nome do produto
string produtoDesignacao(Produto p);
//Retorna a quantidade atual do produto
int produtoQuantidade(Produto p);
//Retorna o preco do produto
int produtoPreco(Produto p);
//Retorna o peso do produto
int produtoPeso(Produto p);
//Aumenta a quantidade do produto
void produtoIncrementaQuantidade(Produto p, int quantidade);
//Diminui a quantidade do produto
void produtoDecrementaQuantidade(Produto p, int quantidade);
//Retorna a 1 se o produto é avulso ou 0 se nao for
int produtoEAvulso(Produto p);
//Compara dois produtos com base na sua designacao
int comparaProduto(Produto p1, Produto p2);

#endif
