#include "Supermercado.h"
#include "Stock.h"
#include "Cliente.h"
#include "Str.h"
#include "Sequencia.h"
#include "Iterador.h"
#include "Util.h"
#include "Info.h"
#include "Produto.h"
#include "Caixa.h"
#include "Historico.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

struct supermercado {
    Info info;
    Stock stock;
    Sequencia clientes;
    Caixa caixas[NUM_CAIXAS];
    Historico historico;
    int tempoCorrente;
    int proximoNumeroCliente;
};

Supermercado supermercadoCria(void) {
    Supermercado s = malloc(sizeof(struct supermercado));
    if (s == NULL) semMemoria();

    s->stock = stockCria();
    if (s->stock == NULL) {
        free(s);
        semMemoria();
    }

    s->clientes = criaSequencia();
    if (s->clientes == NULL) {
        stockDestroi(s->stock);
        free(s);
        semMemoria();
    }

    s->info = criaInfo("Supermercado", supermercadoDestroi, supermercadoDestroiTudo, NULL, NULL, NULL);
    if (s->info == NULL) {
        destroiSequencia(s->clientes);
        stockDestroi(s->stock);
        free(s);
        semMemoria();
    }

    for (int i = 0; i < NUM_CAIXAS; i++) {
        s->caixas[i] = caixaCria(i);
        if (s->caixas[i] == NULL) {
            for (int j = 0; j < i; j++) {
                caixaDestroiTudo(s->caixas[j]);
            }
            destroiSequencia(s->clientes);
            stockDestroi(s->stock);
            free(s);
            semMemoria();
        }
    }

    s->historico = historicoCria();
    if (s->historico == NULL) {
        for (int i = 0; i < NUM_CAIXAS; i++) {
            caixaDestroiTudo(s->caixas[i]);
        }
        destroiSequencia(s->clientes);
        stockDestroi(s->stock);
        free(s);
        semMemoria();
    }

    s->tempoCorrente = 0;
    s->proximoNumeroCliente = 0;

    return s;
}

void supermercadoDestroi(Supermercado s) {
    if (s == NULL) return;
    destroiSequencia(s->clientes);
    stockDestroi(s->stock);
    free(s);
}

void supermercadoDestroiTudo(Supermercado s) {
    if (s == NULL) return;

    // Primeiro, destruir os clientes que estão nas caixas
    for (int i = 0; i < NUM_CAIXAS; i++) {
        if (s->caixas[i] != NULL) {
            // Destruir cliente que estao a ser atendidos
            if (caixaTemClienteSerAtendido(s->caixas[i])) {
                Cliente cliente = caixaRemoveClienteAtendido(s->caixas[i]);
                if (cliente != NULL) {
                    // Remover o cliente da sequencia antes de destruí-lo
                    int pos = 0;
                    while (pos < tamanhoSequencia(s->clientes)) {
                        Cliente c = elementoSequencia(s->clientes, pos);
                        if (clienteNumero(c) == clienteNumero(cliente)) {
                            removeSequencia(s->clientes, pos);
                            break;
                        }
                        pos++;
                    }
                    clienteDestroiTudo(cliente);
                }
            }

            // Destruir clientes na fila
            while (caixaTamanhoFila(s->caixas[i]) > 0) {
                Cliente cliente = removeFila(caixaFilaClientes(s->caixas[i]));
                if (cliente != NULL) {
                    // Remover o cliente da sequência antes de destruí-lo
                    int pos = 0;
                    while (pos < tamanhoSequencia(s->clientes)) {
                        Cliente c = elementoSequencia(s->clientes, pos);
                        if (clienteNumero(c) == clienteNumero(cliente)) {
                            removeSequencia(s->clientes, pos);
                            break;
                        }
                        pos++;
                    }
                    clienteDestroiTudo(cliente);
                }
            }
        }
    }

    // Depois destruir as caixas
    for (int i = 0; i < NUM_CAIXAS; i++) {
        if (s->caixas[i] != NULL) {
            caixaDestroi(s->caixas[i]);
        }
    }

    // Destruir o histórico
    if (s->historico != NULL) {
        historicoDestroiTudo(s->historico);
    }

    // Destruir os clientes restantes na sequência
    if (s->clientes != NULL) {
        while (tamanhoSequencia(s->clientes) > 0) {
            Cliente c = removeSequencia(s->clientes, 0);
            if (c != NULL) {
                clienteDestroiTudo(c);
            }
        }
        destroiSequencia(s->clientes);
    }

    // Destruir o stock
    if (s->stock != NULL) {
        stockDestroiTudo(s->stock);
    }

    //destruir o supermercado
    free(s);
}

int supermercadoAdicionaProduto(Supermercado s, string designacao, int quantidade, int preco, int peso) {
    if (s == NULL || designacao == NULL || quantidade < 0 || preco < 0 || peso <= 0) return 0;

    Produto p = produtoCria(designacao, quantidade, preco, peso);
    if (p == NULL) return 0;

    int resultado = stockAdicionaProduto(s->stock, p);
    if (!resultado) {
        produtoDestroi(p);
    }

    return resultado;
}

int supermercadoIncrementaProduto(Supermercado s, string designacao, int quantidade) {
    if (s == NULL || designacao == NULL || quantidade <= 0) return 0;
    return stockIncrementaProduto(s->stock, designacao, quantidade);
}

Stock supermercadoStock(Supermercado s) {
    return s->stock;
}

int supermercadoAdicionaCliente(Supermercado s) {
    if (s == NULL) return -1;

    Cliente c = clienteCria(s->proximoNumeroCliente);
    if (c == NULL) return -1;

    acrescentaSequencia(s->clientes, c);
    return s->proximoNumeroCliente++;
}

int supermercadoClienteAdicionaProduto(Supermercado s, int clienteNumero, string designacao, int quantidade) {
    if (s == NULL || designacao == NULL || clienteNumero < 0 || quantidade < 0) return 0;

    Cliente c = supermercadoObtemCliente(s, clienteNumero);
    if (c == NULL) return 0;

    if (!stockExisteProduto(s->stock, designacao)) return 0;

    Produto p = stockObtemProduto(s->stock, designacao);
    if (p == NULL) return 0;

    if (quantidade == 0) return 1;

    if (produtoQuantidade(p) < quantidade) return 0;

    if (!clienteAdicionaProduto(c, p, quantidade)) return 0;

    produtoDecrementaQuantidade(p, quantidade);
    return 1;
}

Iterador supermercadoIteradorClientesOrdenado(Supermercado s) {
    if (s == NULL) return NULL;
    return iteradorSequencia(s->clientes);
}

Cliente supermercadoObtemCliente(Supermercado s, int numero) {
    if (s == NULL || numero < 0 || numero >= tamanhoSequencia(s->clientes)) return NULL;
    return elementoSequencia(s->clientes, numero);
}

int supermercadoClienteVaiParaCaixa(Supermercado s, int clienteNumero, int numeroCaixa) {
    if (s == NULL || clienteNumero < 0 || numeroCaixa < 0 || numeroCaixa >= NUM_CAIXAS) return 0;

    Cliente cliente = supermercadoObtemCliente(s, clienteNumero);
    if (cliente == NULL) return 0;

    // Se o cliente já está em uma caixa, não pode mudar
    if (clienteNumeroCaixa(cliente) != -1) return 0;

    if (numeroCaixa == 0 && !clienteCestoVazio(cliente)) return 0;

    if (!caixaAdicionaCliente(s->caixas[numeroCaixa], cliente)) return 0;

    clienteDefineNumeroCaixa(cliente, numeroCaixa);

    // Iniciar atendimento imediatamente se a caixa estiver livre
    if (!caixaTemClienteSerAtendido(s->caixas[numeroCaixa])) {
        caixaIniciaAtendimento(s->caixas[numeroCaixa]);
    }

    return 1;
}

int supermercadoClienteVaiParaMelhorCaixa(Supermercado s, int clienteNumero) {
    if (s == NULL || clienteNumero < 0) return 0;

    Cliente cliente = supermercadoObtemCliente(s, clienteNumero);
    if (cliente == NULL) return 0;

    if (clienteNumeroCaixa(cliente) != -1) return 0;

    // Se o cliente tem cesto vazio, tenta primeiro a caixa 0
    if (clienteCestoVazio(cliente)) {
        if (supermercadoClienteVaiParaCaixa(s, clienteNumero, 0)) {
            return 1;
        }
    }

    int menorTamanho = INT_MAX;
    int melhorCaixaIndex = -1;

    // Comecar da caixa 1 se o cliente tem produtos, ou continuar da caixa 0 se não tem
    int caixaInicial;
    if (clienteCestoVazio(cliente)) {
        caixaInicial = 0;
    } else {
        caixaInicial = 1;
        }

    for (int i = caixaInicial; i < NUM_CAIXAS; i++) {

    int tamanhoAtual = caixaTamanhoFila(s->caixas[i]);
        if (caixaTemClienteSerAtendido(s->caixas[i])) {
            tamanhoAtual += 1;
        }
        if (tamanhoAtual < menorTamanho) {
            menorTamanho = tamanhoAtual;
            melhorCaixaIndex = i;
        }
    }

    if (melhorCaixaIndex == -1) return 0;

    return supermercadoClienteVaiParaCaixa(s, clienteNumero, melhorCaixaIndex);
}

void supermercadoAvancaTempo(Supermercado s, int segundos) {
    if (s == NULL || segundos < 0) return;

    for (int i = 0; i < segundos; i++) {
        s->tempoCorrente++;

        // Processar todas as caixas
        for (int j = 0; j < NUM_CAIXAS; j++) {
            Caixa caixaAtual = s->caixas[j];

            // Para caixa 0, processar todos os clientes imediatamente
            if (j == 0) {
                while (caixaTamanhoFila(caixaAtual) > 0 || caixaTemClienteSerAtendido(caixaAtual)) {
                    if (!caixaTemClienteSerAtendido(caixaAtual) && caixaTamanhoFila(caixaAtual) > 0) {
                        caixaIniciaAtendimento(caixaAtual);
                    }

                    if (caixaTemClienteSerAtendido(caixaAtual)) {
                        Cliente clienteConcluido = caixaRemoveClienteAtendido(caixaAtual);

                        historicoAdicionaEntrada(s->historico,clienteNumero(clienteConcluido),caixaNumero(caixaAtual),s->tempoCorrente,clienteTotalPagar(clienteConcluido),clientePesoTotalCesto(clienteConcluido));

                        int pos = 0;
                        while (pos < tamanhoSequencia(s->clientes)) {
                            Cliente c = elementoSequencia(s->clientes, pos);
                            if (clienteNumero(c) == clienteNumero(clienteConcluido)) {
                                removeSequencia(s->clientes, pos);
                                break;
                            }
                            pos++;
                        }

                        clienteDestroiTudo(clienteConcluido);
                    }
                }
            } else {
                // Para outras caixas, processar normalmente
                if (!caixaTemClienteSerAtendido(caixaAtual) && caixaTamanhoFila(caixaAtual) > 0) {
                    caixaIniciaAtendimento(caixaAtual);
                }

                if (caixaTemClienteSerAtendido(caixaAtual)) {
                    if (caixaProcessaTempo(caixaAtual)) {
                        Cliente clienteConcluido = caixaRemoveClienteAtendido(caixaAtual);

                        historicoAdicionaEntrada(s->historico,clienteNumero(clienteConcluido),caixaNumero(caixaAtual),s->tempoCorrente,clienteTotalPagar(clienteConcluido),
                        clientePesoTotalCesto(clienteConcluido));

                        int pos = 0;
                        while (pos < tamanhoSequencia(s->clientes)) {
                            Cliente c = elementoSequencia(s->clientes, pos);
                            if (clienteNumero(c) == clienteNumero(clienteConcluido)) {
                                removeSequencia(s->clientes, pos);
                                break;
                            }
                            pos++;
                        }

                        clienteDestroiTudo(clienteConcluido);

                        if (caixaTamanhoFila(caixaAtual) > 0) {
                            caixaIniciaAtendimento(caixaAtual);
                        }
                    }
                }
            }
        }
    }
}

Iterador supermercadoIteradorCaixasOrdenado(Supermercado s) {
    if (s == NULL) return NULL;

    // Criar uma copia do vetor de caixas
    Caixa* caixasCopia = malloc(sizeof(Caixa)*NUM_CAIXAS );
    if (caixasCopia == NULL) {
        semMemoria();
        return NULL;
    }

    // Copiar as caixas
    for (int i = 0; i < NUM_CAIXAS; i++) {
        caixasCopia[i] = s->caixas[i];
    }

    return criaIterador((void**)caixasCopia, NUM_CAIXAS);
}

Iterador supermercadoIteradorHistoricoOrdenado(Supermercado s) {
    if (s == NULL) return NULL;
    return historicoIteradorOrdenado(s->historico);
}

void supermercadoFecha(Supermercado s) {
    if (s == NULL) return;

    // Processar clientes com cesto vazio
    Iterador it = supermercadoIteradorClientesOrdenado(s);
    while (temSeguinteIterador(it)) {
        Cliente c = seguinteIterador(it);
        if (clienteCestoVazio(c) && clienteNumeroCaixa(c) == -1) {
            // Adicionar à fila da caixa 0
            caixaAdicionaCliente(s->caixas[0], c);
            clienteDefineNumeroCaixa(c, 0);
        }
    }
    destroiIterador(it);

    // Processar clientes com cesto não vazio
    it = supermercadoIteradorClientesOrdenado(s);
    while (temSeguinteIterador(it)) {
        Cliente c = seguinteIterador(it);
        if (!clienteCestoVazio(c) && clienteNumeroCaixa(c) == -1) {
            supermercadoClienteVaiParaCaixa(s, clienteNumero(c), 1);
        }
    }
    destroiIterador(it);

    // Processar todas as caixas
    for (int i = 0; i < NUM_CAIXAS; i++) {
        if (s->caixas[i] != NULL) {
            int momentoAtual = s->tempoCorrente;
            // Processar clientes na fila
            while (caixaTamanhoFila(s->caixas[i]) > 0 || caixaTemClienteSerAtendido(s->caixas[i])) {
                if (!caixaTemClienteSerAtendido(s->caixas[i]) && caixaTamanhoFila(s->caixas[i]) > 0) {
                    caixaIniciaAtendimento(s->caixas[i]);
                }

                if (caixaTemClienteSerAtendido(s->caixas[i])) {
                    Cliente clienteConcluido = caixaRemoveClienteAtendido(s->caixas[i]);

                    // Calcular o momento de saída com base no tempo de atendimento
                    int tempoAtendimento;
                    if(i==0)
                    {
                        tempoAtendimento=1;
                        momentoAtual = s->tempoCorrente +1;
                    }else{
                        tempoAtendimento=50;
                        momentoAtual += tempoAtendimento;
                    }


                    // Adicionar ao histórico
                    historicoAdicionaEntrada(s->historico,clienteNumero(clienteConcluido),caixaNumero(s->caixas[i]),momentoAtual,clienteTotalPagar(clienteConcluido),clientePesoTotalCesto(clienteConcluido));
                    clienteDefineNumeroCaixa(clienteConcluido, i);
                }
            }
        }
    }

    // Garantir que todos os clientes estão na fila de suas respectivas caixas
    it = supermercadoIteradorClientesOrdenado(s);
    while (temSeguinteIterador(it)) {
        Cliente c = seguinteIterador(it);
        int numeroCaixa = clienteNumeroCaixa(c);
        if (numeroCaixa != -1) {
            // Se o cliente não está na fila, adicionar
            if (!caixaTemClienteSerAtendido(s->caixas[numeroCaixa])) {
                caixaAdicionaCliente(s->caixas[numeroCaixa], c);
            }
        }
    }
    destroiIterador(it);
}

int supermercadoTempoCorrente(Supermercado s) {
    if (s == NULL) return 0;
    return s->tempoCorrente;
}


