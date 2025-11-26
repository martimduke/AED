#include "Supermercado.h"
#include "Str.h"
#include "Iterador.h"
#include "Util.h"
#include "Info.h"
#include "Produto.h"
#include "Cliente.h"
#include "Sequencia.h"
#include "Stock.h"
#include "Caixa.h"
#include "Historico.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINHA 1000
#define MAX_DESIGNACAO 100

// Funções auxiliares para output padronizado
void invalidos(void);
void naoPode(void);

// Declaracoes das funcoes de comando
void cmdAjuda(string args);
void cmdClienteNovo(Supermercado s, string args);
void cmdClienteColocaProduto(Supermercado s, string args);
void cmdListaClientes(Supermercado s);
void cmdProdutoNovo(Supermercado s, string args);
void cmdReabasteceProduto(Supermercado s, string args);
void cmdListaStock(Supermercado s);
void cmdListaCaixas(Supermercado s);
void cmdClienteVaiParaCaixa(Supermercado s, string args);
void cmdClienteVaiParaMelhorCaixa(Supermercado s, string args);
void cmdAvancaTempo(Supermercado s, string args);
void cmdTerminaPrograma(Supermercado s, string args);
void cmdListaHistorico(Supermercado s);
void listaStock(Supermercado s);
void listaClientes(Supermercado s);
void listaCestoCliente(Cliente c);
void listaCaixas(Supermercado s);
void interpretador(Supermercado s);

// Função principal
int main(void) {
    Supermercado s = supermercadoCria();
    interpretador(s);
    supermercadoDestroiTudo(s);
    return 0;
}



void cmdAjuda(string args) {
    char lixo[MAX_LINHA];
    // Verifica se ha argumentos extras apos o '?'
    if (sscanf(args, "%s", lixo) == 1)
        invalidos();
    else {
        printf("  ? - Ajuda\n");
    printf("  C - Cliente novo\n");
    printf("  D x p n - Cliente coloca produto no cesto\n");
    printf("  E - lista Clientes\n");
    printf("  P p n c g - Produto novo\n");
    printf("  Q p n - reabastecimento de Produto.\n");
    printf("  R - lista Produtos em stock\n");
    printf("  Z - lista Caixas\n");
    printf("  H - lista Historico\n");
    printf("  T s - avanca Tempo\n");
    printf("  F - Fecha o supermercado e termina o programa\n");
    printf("  . - Termina o programa imediatamente\n");
}
}

void cmdClienteNovo(Supermercado s, string args) {
    char lixo[MAX_LINHA];
    // Verifica se ha argumentos extras apos o 'C'
    if (sscanf(args, "%s", lixo) == 1)
        invalidos();
    else {
        int numero = supermercadoAdicionaCliente(s);
        printf("Chega o cliente %d\n", numero);
    }
}

void cmdClienteColocaProduto(Supermercado s, string args) {
    int clienteNumero;
    char designacao[MAX_DESIGNACAO + 1];
    int quantidade;
    char lixo[MAX_LINHA];

    // Tenta ler os 3 argumentos obrigatorios e verifica se ha lixo extra
    if (sscanf(args, " %d %s %d %s", &clienteNumero, designacao, &quantidade, lixo) != 3) {
        invalidos();
        return;
    }

    // Cliente nao pode ser negativo
    if (clienteNumero < 0) {
        invalidos();
        return;
    }

    // Quantidade pode ser 0, mas nao negativa
    if (quantidade < 0) {
        invalidos();
         return;
     }

    // A validacao de quantidade 0 é feita dentro de supermercadoClienteAdicionaProduto
    if (supermercadoClienteAdicionaProduto(s, clienteNumero, designacao, quantidade)) {
        // Se a funcao retornou 1, imprime a mensagem
        printf("Acrescenta %s\n", designacao);
         } else {
        // Se a funcao retornou 0
        naoPode();
    }
}

void cmdListaClientes(Supermercado s) {
    printf("-- Clientes --\n");
    Iterador it = supermercadoIteradorClientesOrdenado(s);

    if (!temSeguinteIterador(it)) {
        printf("  vazio\n");
    } else {
        while (temSeguinteIterador(it)) {
            Cliente c = seguinteIterador(it);
            int numeroCaixa = clienteNumeroCaixa(c);
            if (numeroCaixa == -1) {
                printf("  Cliente %d:\n", clienteNumero(c));
            } else {
                printf("  Cliente %d [%d]:\n", clienteNumero(c), numeroCaixa);
            }
            listaCestoCliente(c); // Listar o cesto do cliente
        }
    }
    destroiIterador(it);
}

void cmdProdutoNovo(Supermercado s, string args) {
    char designacao[MAX_DESIGNACAO + 1], lixo[MAX_LINHA];
    int quantidade, preco, peso;

    // Tenta ler os 4 argumentos obrigatorios e verifica se ha lixo extra
    if (sscanf(args," %s %d %d %d %s", designacao, &quantidade, &preco, &peso, lixo) != 4) {
        invalidos();
        return;
    }

    // Quantidade, preco e peso nao podem ser negativos
    if (quantidade < 0 || preco < 0 || peso < 0) {
        invalidos();
        return;
    }
    // Peso unitario 1 indica produto avulso, mas peso 0 ou negativo nao
    if (peso <= 0) {
        invalidos();
        return;
            }

    // Ela retorna 1 em sucesso ou 0 se o produto ja existe
    if (supermercadoAdicionaProduto(s, designacao, quantidade, preco, peso)) {
        printf("Cria o novo produto %s\n", designacao);
    } else {
        naoPode();
    }
}

void cmdReabasteceProduto(Supermercado s, string args) {
    char designacao[MAX_DESIGNACAO + 1], lixo[MAX_LINHA];
    int quantidade;

    // Tenta ler os 2 argumentos obrigatorios e verifica se ha lixo extra
    if (sscanf(args, " %s %d %s", designacao, &quantidade, lixo) != 2) {
        invalidos();
        return;
            }

    // Quantidade nao pode ser negativa
    if (quantidade <= 0) {
        invalidos();
        return;
    }

    // Chamar a funcao do supermercado para incrementar o stock
    if (supermercadoIncrementaProduto(s, designacao, quantidade)) {
        printf("Reabastece o produto %s\n", designacao);
                } else {
        naoPode();
                }
            }

void cmdListaStock(Supermercado s) {
    listaStock(s);
}

void cmdListaCaixas(Supermercado s) {
    listaCaixas(s);
}

void cmdTerminaPrograma(Supermercado s, string args) {
    char lixo[MAX_LINHA];
    // Verifica se ha argumentos extras apos o '.'
    if (sscanf(args, "%s", lixo) == 1) {
        invalidos();
    } else {
        printf("Obrigado. Volte sempre!\n");
    }
}

void cmdClienteVaiParaCaixa(Supermercado s, string args) {
    int clienteNumero, numeroCaixa;
    char lixo[MAX_LINHA];

    // Tenta ler os 2 argumentos obrigatorios e verifica se ha lixo extra
    if (sscanf(args, " %d %d %s", &clienteNumero, &numeroCaixa, lixo) != 2) {
        invalidos();
        return;
    }

    // Validacoes adicionais
    if (clienteNumero < 0) {
        invalidos();
        return;
    }

    // Verifica se a caixa existe
    if (numeroCaixa < 0 || numeroCaixa >= NUM_CAIXAS) {
        naoPode();
        return;
    }

    // Chamar a funcao do supermercado para enviar o cliente para a caixa
    if (supermercadoClienteVaiParaCaixa(s, clienteNumero, numeroCaixa)) {
        printf("Cliente %d para caixa %d\n", clienteNumero, numeroCaixa);
    } else {
        naoPode();
    }
}

void cmdClienteVaiParaMelhorCaixa(Supermercado s, string args) {
    int clienteNumero;
    char lixo[MAX_LINHA];

    // Tenta ler o argumento obrigatorio e verifica se ha lixo extra
    if (sscanf(args, " %d %s", &clienteNumero, lixo) != 1) {
        invalidos();
        return;
    }

    // Validacoes adicionais
    if (clienteNumero < 0) {
        invalidos();
        return;
    }

    // Chamar a funcao do supermercado para enviar o cliente para a melhor caixa
    if (supermercadoClienteVaiParaMelhorCaixa(s, clienteNumero)) {
        printf("Cliente %d para caixa %d\n", clienteNumero, clienteNumeroCaixa(supermercadoObtemCliente(s, clienteNumero)));
    } else {
        naoPode();
    }
}

void cmdAvancaTempo(Supermercado s, string args) {
    int segundos;
    char lixo[MAX_LINHA];

    // Tenta ler o argumento obrigatório e verifica se há lixo extra
    if (sscanf(args, " %d %s", &segundos, lixo) != 1) {
        invalidos();
        return;
    }

    // Validações adicionais
    if (segundos < 0) {
        invalidos();
        return;
    }

    // Avançar o tempo no supermercado
    supermercadoAvancaTempo(s, segundos);
    printf("Tempo corrente: %d segundos\n", supermercadoTempoCorrente(s));
}

void cmdListaHistorico(Supermercado s) {
    printf("-- Historia --\n");
    Iterador it = supermercadoIteradorHistoricoOrdenado(s);

    if (!temSeguinteIterador(it)) {
        printf("  vazio\n");
    } else {
        while (temSeguinteIterador(it)) {
            Historico entrada = (Historico)seguinteIterador(it);
            printf("  Cliente %d: caixa %d, momento %d, pagou %d, peso %d\n",historicoNumeroCliente(entrada),historicoNumeroCaixa(entrada),historicoMomentoSaida(entrada),historicoTotalPago(entrada),historicoPesoTotal(entrada));
        }
    }
    destroiIterador(it);
}

// Funções auxiliares para a listagem

void listaStock(Supermercado s) {
    printf("-- Stock --\n");
    Stock stock = supermercadoStock(s);
    Iterador it = stockIteradorOrdenado(stock);

    if (!temSeguinteIterador(it)) {
        printf("  vazio\n");
    } else {
        while (temSeguinteIterador(it)) {
            Produto p = seguinteIterador(it);
            printf("  Produto %s: %d unidades, %d centimos, %d gramas\n",produtoDesignacao(p), produtoQuantidade(p), produtoPreco(p), produtoPeso(p));
        }
    }
    destroiIterador(it);
}

void listaClientes(Supermercado s) {
    printf("-- Clientes --\n");
    Iterador it = supermercadoIteradorClientesOrdenado(s);

    if (!temSeguinteIterador(it)) {
        printf("  vazio\n");
    } else {
        // A listagem de clientes é por ordem de chegada
        while (temSeguinteIterador(it)) {
            Cliente c = seguinteIterador(it);
            printf("  Cliente %d:\n", clienteNumero(c));
            listaCestoCliente(c);
        }
    }
    destroiIterador(it);
}

void listaCestoCliente(Cliente c) {
    Iterador itCesto = clienteIteradorCestoOrdenado(c);

    if (!temSeguinteIterador(itCesto)) {
        printf("    vazio\n");
                } else {
        while (temSeguinteIterador(itCesto)) {
            Produto p = seguinteIterador(itCesto);
            printf("    Produto %s: %d unidades, %d centimos, %d gramas\n",
                   produtoDesignacao(p), produtoQuantidade(p), produtoPreco(p), produtoPeso(p));
        }
    }
    destroiIterador(itCesto);
                }

void listaCaixas(Supermercado s) {
    printf("-- Caixas --\n");
    Iterador it = supermercadoIteradorCaixasOrdenado(s);

    if (!temSeguinteIterador(it)) {
        printf("  vazio\n");
    } else {
        while (temSeguinteIterador(it)) {
            Caixa c = seguinteIterador(it);
            printf("  Caixa %d:\n", caixaNumero(c));

            // Mostrar cliente sendo atendido
            if (caixaTemClienteSerAtendido(c)) {
                Cliente cliente = caixaClienteSerAtendido(c);
                printf("    Cliente %d: %d segundos, %d centimos\n",
                       clienteNumero(cliente),
                       caixaTempoRestante(c),
                       clienteTotalPagar(cliente));
            }

            // Mostrar clientes na fila
            Iterador itFila = caixaIteradorFila(c);
            while (temSeguinteIterador(itFila)) {
                Cliente clienteFila = seguinteIterador(itFila);
                int segundosEspera;
                if (caixaNumero(c) == 0) {
                    segundosEspera = 0;
                } else {
                    segundosEspera = 50;
                }

                int totalCentimos = clienteTotalPagar(clienteFila);
                printf("    Cliente %d: %d segundos, %d centimos\n",clienteNumero(clienteFila),segundosEspera,totalCentimos);
            }
            destroiIterador(itFila);

            // Se nao ha clientes sendo atendidos na fila
            if (!caixaTemClienteSerAtendido(c) && caixaTamanhoFila(c) == 0) {
                printf("    vazio\n");
            }
        }
    }
    destroiIterador(it);
}

char lerComando(string linha) {
    printf("> ");

    if (fgets(linha, MAX_LINHA, stdin) == NULL) {
        return '.';
                }

    linha[strcspn(linha, "\n")] = 0;

    if (strlen(linha) == 0) {
        return '\n';
    }

    return toupper(linha[0]);
}

void interpretador(Supermercado s) {
    char linha[MAX_LINHA];
    char cmd;
    string args;

    do {
        cmd = lerComando(linha);
        args = linha + 1;
        while (*args == ' ' || *args == '\t') args++;

        switch (cmd) {
            case '?': cmdAjuda(args); break;
            case 'C': cmdClienteNovo(s, args); break;
            case 'D': cmdClienteColocaProduto(s, args); break;
            case 'E': cmdListaClientes(s); break;
            case 'P': cmdProdutoNovo(s, args); break;
            case 'Q': cmdReabasteceProduto(s, args); break;
            case 'R': cmdListaStock(s); break;
            case 'T': cmdAvancaTempo(s, args); break;
            case 'X': cmdClienteVaiParaCaixa(s, args); break;
            case 'Y': cmdClienteVaiParaMelhorCaixa(s, args); break;
            case 'Z': cmdListaCaixas(s); break;
            case 'H': cmdListaHistorico(s); break;
            case 'F':
                supermercadoFecha(s);
                cmdListaClientes(s);
                cmdListaCaixas(s);
                cmdListaHistorico(s);
                printf("Obrigado. Volte sempre!\n");
                return;
            case '.': cmdTerminaPrograma(s, args); break;
            case '\n': break;
            default: printf("> Comando desconhecido.\n"); break;
        }
    } while (cmd != 'F' && cmd != '.');
}

//Implementação das funções auxiliares de output

void invalidos(void){
	printf("Dados invalidos.\n");
        }

void naoPode(void){
	printf("Nao pode fazer isso.\n");
}
