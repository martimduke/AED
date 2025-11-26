#include "Cliente.h"
#include "Dicionario.h"
#include "Iterador.h"
#include "Util.h"
#include "Info.h"
#include "Str.h"
#include "Produto.h"
#include <stdlib.h>
#include <string.h>

struct cliente {
    Info info;
    Dicionario cesto;
    int numero;
    int numeroCaixa;
};

Cliente clienteCria(int numero) {
    Cliente c = malloc(sizeof(struct cliente));
    if (c == NULL) semMemoria();

    c->info = criaInfo("Cliente", clienteDestroi, clienteDestroiTudo, NULL, NULL, NULL);
    if (c->info == NULL)
        {
        destroiDicionario(c->cesto);
        free(c);
        semMemoria();
        }

    c->numero = numero;
    c->cesto = criaDicionario();
    c->numeroCaixa = -1;

    return c;
}

void clienteDestroi(Cliente c) {
    destroiDicionario(c->cesto);
    free(c);
        }

void clienteDestroiTudo(Cliente c) {
    destroiTudoDicionario(c->cesto);
    free(c);
}

int clienteNumero(Cliente c) {
    return c->numero;
}

int clienteAdicionaProduto(Cliente c, Produto p, int quantidade) {
    if (quantidade < 0) return 0;
    if (quantidade == 0) return 1;

    string designacao = produtoDesignacao(p);
    Str chave = criaStr(designacao);
    if (chave == NULL) semMemoria();

    //Se ja existir o produto no cesto incrementa o produto com uma nova quantidade
    if (existeDicionario(c->cesto, chave)) {
        Str chaveObter = criaStr(designacao);
        if (chaveObter == NULL) semMemoria();
        Produto pCesto = elementoDicionario(c->cesto, chaveObter);
        produtoIncrementaQuantidade(pCesto, quantidade);
    } else { //se nao cria e coloca no cesto com a quantidade
        Produto pNovo = produtoCria(designacao, quantidade, produtoPreco(p), produtoPeso(p));
        if (pNovo == NULL) semMemoria();

        Str novaChave = criaStr(designacao);
        if (novaChave == NULL) {
            produtoDestroi(pNovo);
            semMemoria();
            }

        if (!acrescentaDicionario(c->cesto, novaChave, pNovo)) {
            destroiStr(novaChave);
            produtoDestroi(pNovo);
            return 0;
            }
    }

    return 1;
}

Iterador clienteIteradorCestoOrdenado(Cliente c) {
    return iteradorOrdenadoDicionario(c->cesto);
}

int clienteCestoVazio(Cliente c) {
    return tamanhoDicionario(c->cesto) == 0;
}

int clienteNumeroCaixa(Cliente c) {
    if (c == NULL) return -1;
    return c->numeroCaixa;
}

void clienteDefineNumeroCaixa(Cliente c, int numeroCaixa) {
    if (c == NULL) return;
    c->numeroCaixa = numeroCaixa;
}

int clienteTotalPagar(Cliente c) {
    if (c == NULL) return 0;

    int total = 0;
    Iterador it = clienteIteradorCestoOrdenado(c);
    //percorre o iterador ordenado "cesto" e guarda em total o preco pela quantidade de cada produto do cesto
    while (temSeguinteIterador(it)) {
        Produto p = seguinteIterador(it);
        total += produtoPreco(p) * produtoQuantidade(p);
    }
    destroiIterador(it);
    return total;
}

int clientePesoTotalCesto(Cliente c) {
    if (c == NULL) return 0;

    int pesoTotal = 0;
    Iterador it = clienteIteradorCestoOrdenado(c);
    //percorre o iterador ordenado "cesto" e guarda em pesototal o peso pela quantidade de cada produto do cesto
    while (temSeguinteIterador(it)) {
        Produto p = seguinteIterador(it);
        pesoTotal += produtoPeso(p) * produtoQuantidade(p);
    }
    destroiIterador(it);
    return pesoTotal;
}
