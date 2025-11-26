#include "Historico.h"
#include "Sequencia.h"
#include "Util.h"
#include "Iterador.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct historicoEntrada {

    int numeroCliente;
    int numeroCaixa;
    int momentoSaida;
    int totalPago;
    int pesoTotal;
    Sequencia registos;
};

Historico historicoCria(void) {
    Historico h = malloc(sizeof(struct historicoEntrada));
    if (h == NULL) {
        semMemoria();
        return NULL;
    }
    h->registos = criaSequencia();
    if (h->registos == NULL) {
        free(h);
        semMemoria();
        return NULL;
    }

    return h;
}

void historicoAdicionaEntrada(Historico h, int numeroCliente, int numeroCaixa, int momentoSaida, int totalPago, int pesoTotal) {
    if (h == NULL) return;

    // Criar e preencher a nova entrada do historico
    Historico novaEntrada = malloc(sizeof(struct historicoEntrada));
    if (novaEntrada == NULL) {
        semMemoria();
        return;
    }

    novaEntrada->numeroCliente = numeroCliente;
    novaEntrada->numeroCaixa = numeroCaixa;
    novaEntrada->momentoSaida = momentoSaida;
    novaEntrada->totalPago = totalPago;
    novaEntrada->pesoTotal = pesoTotal;
    novaEntrada->registos = criaSequencia();

    acrescentaSequencia(h->registos, novaEntrada);
}

int historicoNumeroCliente(Historico h) {
    if (h == NULL) return 0;
    return h->numeroCliente;
}

int historicoNumeroCaixa(Historico h) {
    if (h == NULL) return 0;
    return h->numeroCaixa;
}

int historicoMomentoSaida(Historico h) {
    if (h == NULL) return 0;
    return h->momentoSaida;
}

int historicoTotalPago(Historico h) {
    if (h == NULL) return 0;
    return h->totalPago;
}

int historicoPesoTotal(Historico h) {
    if (h == NULL) return 0;
    return h->pesoTotal;
}

// Funcao de comparacao para ordenar HistoricoEntrada por momento de saida
int comparaHistoricoEntrada(void* op1, void* op2) {
    Historico entrada1 = (Historico) op1;
    Historico entrada2 = (Historico) op2;
    // Ordenar por momento de saida (crescente)
    if (entrada1->momentoSaida < entrada2->momentoSaida) return -1;
    if (entrada1->momentoSaida > entrada2->momentoSaida) return 1;
    // caso o momento de saida for igual no dois, ordena por numero de cliente (crescente)
    if (entrada1->numeroCliente < entrada2->numeroCliente) return -1;
    if (entrada1->numeroCliente > entrada2->numeroCliente) return 1;
    return 0;
}

void destroiHistoricoEntrada(void* op) {
    if (op != NULL) {
        free(op);
    }
}

void historicoDestroi(Historico h) {
    if (h == NULL) return;

    if (h->registos != NULL) {
        while (tamanhoSequencia(h->registos) > 0) {
            Historico entrada = (Historico)removeSequencia(h->registos, 0);
            if (entrada != NULL) {
                free(entrada);
            }
        }
        destroiSequencia(h->registos);
    }

    free(h);
}

void historicoDestroiTudo(Historico h) {
    historicoDestroi(h);
}

Iterador historicoIteradorOrdenado(Historico h) {
    if (h == NULL) return NULL;
    return iteradorSequencia(h->registos);
}
