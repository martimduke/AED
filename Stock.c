#include "Stock.h"
#include "Dicionario.h"
#include "Produto.h"
#include "Iterador.h"
#include "Util.h"
#include "Info.h"
#include "Str.h"
#include <stdlib.h>
#include <string.h>

struct stock {
    Info info;
    Dicionario produtos;
};

Stock stockCria(void) {
    Stock s = malloc(sizeof(struct stock));
    if (s == NULL) semMemoria();

    s->info = criaInfo("Stock", stockDestroi, stockDestroiTudo, NULL, NULL, NULL);
    if (s->info == NULL)
        {
        destroiDicionario(s->produtos);
        free(s);
        semMemoria();
        }

    s->produtos = criaDicionario();


    return s;
}

void stockDestroi(Stock s) {
    destroiDicionario(s->produtos);
    free(s);
}

void stockDestroiTudo(Stock s) {
    destroiTudoDicionario(s->produtos);
    free(s);
}

int stockAdicionaProduto(Stock s, Produto p) {
    string designacao = produtoDesignacao(p);
    Str chave = criaStr(designacao);
    if (chave == NULL) semMemoria();

    if (existeDicionario(s->produtos, chave)) {
        return 0;
    }

    Str novaChave = criaStr(designacao);
    if (novaChave == NULL) semMemoria();

    if (!acrescentaDicionario(s->produtos, novaChave, p)) {
        destroiStr(novaChave);
        return 0;
    }

    return 1;
}

Produto stockObtemProduto(Stock s, string designacao) {
    Str chave = criaStr(designacao);
    if (chave == NULL) semMemoria();

    Produto p = elementoDicionario(s->produtos, chave);
    return p;
}

int stockExisteProduto(Stock s, string designacao) {
    Str chave = criaStr(designacao);
    if (chave == NULL) semMemoria();

    int resultado = existeDicionario(s->produtos, chave);
    return resultado;
}

int stockIncrementaProduto(Stock s, string designacao, int quantidade) {
    if (quantidade <= 0) return 0;

    Str chave = criaStr(designacao);
    if (chave == NULL) semMemoria();

    if (!existeDicionario(s->produtos, chave)) {
            return 0;
        }

    Str novachave = criaStr(designacao);
    if (novachave == NULL) semMemoria();

    Produto p = elementoDicionario(s->produtos, novachave);
    produtoIncrementaQuantidade(p, quantidade);

    return 1;
}

Iterador stockIteradorOrdenado(Stock s) {
    return iteradorOrdenadoDicionario(s->produtos);
}
