#include "Caixa.h"
#include "Fila.h"
#include "Cliente.h"
#include "Produto.h"
#include "Util.h"
#include "Iterador.h"
#include "Info.h"
#include <stdlib.h>
#include <stdio.h>

struct caixa {
    Info info;
    Fila filaClientes;
    Cliente clienteSerAtendido;
    int tempoRestanteCliente;
    int numero;
};

// Funcao auxiliar para calcular o tempo de atendimento de um cliente
int calculaTempoAtendimento(Cliente cliente) {

    if (clienteNumeroCaixa(cliente) == 0) {
        return 0;
    }

    int tempo = 50;

    //iterador para percorrer todos os produtos do cesto
    Iterador itCesto = clienteIteradorCestoOrdenado(cliente);
    int numItens = 0;
    while(temSeguinteIterador(itCesto)) {
        seguinteIterador(itCesto);
        numItens++;
    }
    destroiIterador(itCesto);

    tempo += numItens;

    return tempo;
}

Caixa caixaCria(int numero) {
    Caixa c = malloc(sizeof(struct caixa));
    if (c == NULL) {
        semMemoria();
        return NULL;
    }
    c->info=criaInfo("Caixa",caixaDestroi,caixaDestroiTudo,NULL,NULL,NULL);
    if(c->info==NULL)
    {
        free(c);
        semMemoria();
        return NULL;
    }

    c->filaClientes = criaFila();
    if (c->filaClientes == NULL) {
        free(c);
        semMemoria();
        return NULL;
    }

    c->numero = numero;
    c->clienteSerAtendido = NULL;
    c->tempoRestanteCliente = 0;
    return c;
}

void caixaDestroi(Caixa c) {
    if (c == NULL) return;
    destroiFila(c->filaClientes);
    free(c);
}

void caixaDestroiTudo(Caixa c) {
    if (c == NULL) return;
    destroiTudoFila(c->filaClientes);
    free(c);
}

int caixaAdicionaCliente(Caixa c, Cliente cliente) {
    if (c == NULL || cliente == NULL) return 0;
    acrescentaFila(c->filaClientes, cliente);
    return 1;
}

Cliente caixaRemoveClienteAtendido(Caixa c) {
    if (c == NULL || c->clienteSerAtendido == NULL) return NULL;
    Cliente cliente = c->clienteSerAtendido;
    c->clienteSerAtendido = NULL;
    c->tempoRestanteCliente = 0;
    return cliente;
}

Cliente caixaClienteSerAtendido(Caixa c) {
    if (c == NULL) return NULL;
    return c->clienteSerAtendido;
}

int caixaTemClienteSerAtendido(Caixa c) {
    if (c == NULL) return 0;
    return c->clienteSerAtendido != NULL;
}

void caixaIniciaAtendimento(Caixa c) {
    if (c == NULL || c->clienteSerAtendido != NULL || tamanhoFila(c->filaClientes) == 0) return;
    c->clienteSerAtendido = removeFila(c->filaClientes);
    c->tempoRestanteCliente = calculaTempoAtendimento(c->clienteSerAtendido);
}

int caixaProcessaTempo(Caixa c) {
    if (c == NULL || c->clienteSerAtendido == NULL) return 0;

    c->tempoRestanteCliente--;

    if (c->tempoRestanteCliente <= 0) {
        return 1;
    }
    return 0;
}

int caixaNumero(Caixa c) {
    if (c == NULL) return -1;
    return c->numero;
}

int caixaTempoRestante(Caixa c) {
     if (c == NULL || c->clienteSerAtendido == NULL) return 0;
    return c->tempoRestanteCliente;
}

int caixaTamanhoFila(Caixa c) {
    if (c == NULL) return 0;
    return tamanhoFila(c->filaClientes);
}

Fila caixaFilaClientes(Caixa c){
    if(c == NULL) return NULL;
    return c->filaClientes;
}

Iterador caixaIteradorFila(Caixa c) {
    if (c == NULL) return NULL;
    return iteradorFila(c->filaClientes);
}
