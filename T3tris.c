#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    char tipo;
    int id;
} Peca;

// === CAPACIDADES CORRIGIDAS ===
#define CAPACIDADE_FILA 5
#define CAPACIDADE_PILHA 3

typedef struct {
    Peca itens[CAPACIDADE_FILA];
    int frente;
    int tras;
    int tamanho;
} Fila;

typedef struct {
    Peca itens[CAPACIDADE_PILHA];
    int topo;
    int tamanho;
} Pilha;

// === INICIALIZAÇÕES ===
void inicializarFila(Fila* fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;
}

void inicializarPilha(Pilha* pilha) {
    pilha->topo = -1;
    pilha->tamanho = 0;
}

// === VERIFICAÇÕES ===
int estaCheiaFila(Fila* fila) {
    return fila->tamanho == CAPACIDADE_FILA;
}

int estaVaziaFila(Fila* fila) {
    return fila->tamanho == 0;
}

int estaCheiaPilha(Pilha* pilha) {
    return pilha->tamanho == CAPACIDADE_PILHA;
}

int estaVaziaPilha(Pilha* pilha) {
    return pilha->tamanho == 0;
}

// === OPERAÇÕES NA FILA ===
void enfileirar(Fila* fila, Peca novaPeca) {
    if (estaCheiaFila(fila)) return;
    fila->tras = (fila->tras + 1) % CAPACIDADE_FILA;
    fila->itens[fila->tras] = novaPeca;
    fila->tamanho++;
}

Peca desenfileirar(Fila* fila) {
    Peca pecaVazia = {' ', -1};
    if (estaVaziaFila(fila)) return pecaVazia;
    Peca removida = fila->itens[fila->frente];
    fila->frente = (fila->frente + 1) % CAPACIDADE_FILA;
    fila->tamanho--;
    return removida;
}

// === OPERAÇÕES NA PILHA ===
void empilhar(Pilha* pilha, Peca novaPeca) {
    if (estaCheiaPilha(pilha)) return;
    pilha->topo++;
    pilha->itens[pilha->topo] = novaPeca;
    pilha->tamanho++;
}

Peca desempilhar(Pilha* pilha) {
    Peca pecaVazia = {' ', -1};
    if (estaVaziaPilha(pilha)) return pecaVazia;
    Peca removida = pilha->itens[pilha->topo];
    pilha->topo--;
    pilha->tamanho--;
    return removida;
}

// === EXIBIÇÃO ===
void exibirFila(Fila* fila) {
    printf("Fila de peças\t");
    if (estaVaziaFila(fila)) {
        printf("Vazia\n");
    } else {
        int idx = fila->frente;
        for (int i = 0; i < fila->tamanho; i++) {
            printf("[%c %d] ", fila->itens[idx].tipo, fila->itens[idx].id);
            idx = (idx + 1) % CAPACIDADE_FILA;
        }
        printf("\n");
    }
}

void exibirPilha(Pilha* pilha) {
    printf("Pilha de reserva\t(Topo -> Base): ");
    if (estaVaziaPilha(pilha)) {
        printf("Vazia\n");
    } else {
        for (int i = pilha->topo; i >= 0; i--) {
            printf("[%c %d] ", pilha->itens[i].tipo, pilha->itens[i].id);
        }
        printf("\n");
    }
}

void exibirEstado(Fila* fila, Pilha* pilha) {
    printf("\nEstado atual:\n");
    exibirFila(fila);
    exibirPilha(pilha);
}

// === GERAÇÃO DE PEÇA ===
Peca gerarPeca() {
    static int proximoId = 0;
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.tipo = tipos[rand() % 4];
    p.id = proximoId++;
    return p;
}

// === ADICIONAR NOVA PEÇA NA FILA ===
void adicionarNovaPeca(Fila* fila) {
    if (!estaCheiaFila(fila)) {
        enfileirar(fila, gerarPeca());
    }
}

// === MAIN ===
int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche fila inicial
    for (int i = 0; i < CAPACIDADE_FILA; i++) {
        enfileirar(&fila, gerarPeca());
    }

    int opcao;

    printf("Bem-vindo ao simulador de gerenciamento de peças no Tetris Stack!\n\n");

    while (1) {
        exibirEstado(&fila, &pilha);

        printf("\nOpções de Ação:\n");
        printf("Código\tAção\n");
        printf("1\tJogar peça\n");
        printf("2\tReservar peça\n");
        printf("3\tUsar peça reservada\n");
        printf("0\tSair\n");
        printf("Opção: ");

        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n');
            printf("Entrada inválida!\n");
            continue;
        }

        if (opcao == 0) {
            printf("Saindo... Até logo!\n");
            break;
        }

        else if (opcao == 1) {
            // JOGAR PEÇA
            Peca jogada = desenfileirar(&fila);
            if (jogada.id != -1) {
                printf("Peça jogada: [%c %d]\n", jogada.tipo, jogada.id);
                adicionarNovaPeca(&fila);
            } else {
                printf("Fila vazia!\n");
            }
        }

        else if (opcao == 2) {
            // RESERVAR PEÇA
            if (estaVaziaFila(&fila)) {
                printf("Fila vazia! Nada para reservar.\n");
            } else if (estaCheiaPilha(&pilha)) {
                printf("Reserva cheia! (máx %d)\n", CAPACIDADE_PILHA);
            } else {
                Peca reservada = desenfileirar(&fila);
                empilhar(&pilha, reservada);
                printf("Peça reservada: [%c %d]\n", reservada.tipo, reservada.id);
                adicionarNovaPeca(&fila);
            }
        }

        else if (opcao == 3) {
            // USAR PEÇA RESERVADA
            Peca usada = desempilhar(&pilha);
            if (usada.id != -1) {
                printf("Peça usada da reserva: [%c %d]\n", usada.tipo, usada.id);
            } else {
                printf("Reserva vazia!\n");
            }
        }

        else {
            printf("Opção inválida! Use 0 a 3.\n");
        }

        printf("\n---\n");
    }

    return 0; 
}