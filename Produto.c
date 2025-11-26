#include "Produto.h"
#include "Info.h"
#include "Util.h"
#include <stdlib.h>
#include <string.h>

struct produto {
    Info info;
    char designacao[MAX_DESIGNACAO + 1];
    int quantidade;
    int preco;
    int peso;
};

Produto produtoCria(string designacao, int quantidade, int preco, int peso) {
    Produto p = malloc(sizeof(struct produto));
    if (p == NULL) semMemoria();
    p->info = criaInfo("Produto", produtoDestroi, produtoDestroi, comparaProduto, NULL, produtoDesignacao);
    if (p->info == NULL)
        {
        free(p);
        semMemoria();
        }

    strncpy(p->designacao, designacao, MAX_DESIGNACAO);
    p->designacao[MAX_DESIGNACAO] = '\0';
    p->quantidade = quantidade;
    p->preco = preco;
    p->peso = peso;

    return p;
}

void produtoDestroi(Produto p) {
    free(p);
}

string produtoDesignacao(Produto p) {
    return p->designacao;
}

int produtoQuantidade(Produto p) {
    return p->quantidade;
}

int produtoPreco(Produto p) {
    return p->preco;
}

int produtoPeso(Produto p) {
    return p->peso;
}

void produtoIncrementaQuantidade(Produto p, int quantidade) {
    p->quantidade += quantidade;
}

void produtoDecrementaQuantidade(Produto p, int quantidade) {
    p->quantidade -= quantidade;
}

int produtoEAvulso(Produto p) {
    return p->peso == 1;
}

int comparaProduto(Produto p1, Produto p2) {
    return strcmp(p1->designacao, p2->designacao);
}
