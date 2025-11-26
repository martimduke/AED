#ifndef STOCK_H
#define STOCK_H

#include "Produto.h"
#include "Dicionario.h"
#include "Iterador.h"
#include "Info.h"
#include "Util.h"
#include "Str.h"

typedef struct stock *Stock;

//Cria um novo stock
Stock stockCria(void);
//destroi o stock
void stockDestroi(Stock s);
//destroi o stock e os produtos
void stockDestroiTudo(Stock s);
//adiciona o produto ao stock
int stockAdicionaProduto(Stock s, Produto p);
//Aumenta a quantidade do produto
int stockIncrementaProduto(Stock s, string designacao, int quantidade);
//verifica se o produto existe no stock
int stockExisteProduto(Stock s, string designacao);
//Retorna o produto do stock da sua designacao
Produto stockObtemProduto(Stock s, string designacao);
//iterador para percorrer por ordem os produtos
Iterador stockIteradorOrdenado(Stock s);

#endif
