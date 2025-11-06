#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Estrutura que representa uma peça do Tetris
typedef struct {
    char tipo;  // 'I', 'O', 'T', 'L'
    int id;     // Identificador único incremental
} Peca;

// === CONFIGURAÇÕES FIXAS ===
#define CAPACIDADE_FILA 5
#define CAPACIDADE_PILHA 3

// Fila circular com capacidade fixa
typedef struct {
    Peca itens[CAPACIDADE_FILA];
    int frente;   // Índice da peça mais antiga
    int tras;     // Índice da próxima posição livre
    int tamanho;  // Quantidade atual de peças
} Fila;

// Pilha LIFO com capacidade limitada
typedef struct {
    Peca itens[CAPACIDADE_PILHA];
    int topo;     // Índice da peça no topo
    int tamanho;  // Quantidade atual de peças
} Pilha;

// === INICIALIZAÇÃO ===
void inicializarFila(Fila* f) {
    f->frente = 0;
    f->tras = -1;
    f->tamanho = 0;
}

void inicializarPilha(Pilha* p) {
    p->topo = -1;
    p->tamanho = 0;
}

// === VERIFICAÇÕES DE ESTADO ===
int filaCheia(Fila* f)   { return f->tamanho == CAPACIDADE_FILA; }
int filaVazia(Fila* f)   { return f->tamanho == 0; }
int pilhaCheia(Pilha* p) { return p->tamanho == CAPACIDADE_PILHA; }
int pilhaVazia(Pilha* p) { return p->tamanho == 0; }

// === OPERAÇÕES NA FILA ===
void enfileirar(Fila* f, Peca p) {
    if (filaCheia(f)) return;
    f->tras = (f->tras + 1) % CAPACIDADE_FILA;
    f->itens[f->tras] = p;
    f->tamanho++;
}

Peca desenfileirar(Fila* f) {
    Peca invalida = {' ', -1};
    if (filaVazia(f)) return invalida;
    Peca removida = f->itens[f->frente];
    f->frente = (f->frente + 1) % CAPACIDADE_FILA;
    f->tamanho--;
    return removida;
}

// === OPERAÇÕES NA PILHA ===
void empilhar(Pilha* p, Peca peca) {
    if (pilhaCheia(p)) return;
    p->topo++;
    p->itens[p->topo] = peca;
    p->tamanho++;
}

Peca desempilhar(Pilha* p) {
    Peca invalida = {' ', -1};
    if (pilhaVazia(p)) return invalida;
    Peca removida = p->itens[p->topo];
    p->topo--;
    p->tamanho--;
    return removida;
}

// === GERAÇÃO DE PEÇA ALEATÓRIA ===
Peca gerarPeca() {
    static int proximoId = 0;
    const char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.tipo = tipos[rand() % 4];
    p.id = proximoId++;
    return p;
}

// === ADICIONAR NOVA PEÇA (mantém fila cheia quando possível) ===
void adicionarNovaPeca(Fila* f) {
    if (!filaCheia(f)) {
        enfileirar(f, gerarPeca());
    }
}

// === EXIBIÇÃO DO ESTADO ===
void exibirFila(Fila* f) {
    printf("Fila de peças");
    if (filaVazia(f)) {
        printf("\tVazia\n");
    } else {
        printf("\t");
        int idx = f->frente;
        for (int i = 0; i < f->tamanho; i++) {
            printf("[%c %d] ", f->itens[idx].tipo, f->itens[idx].id);
            idx = (idx + 1) % CAPACIDADE_FILA;
        }
        printf("\n");
    }
}

void exibirPilha(Pilha* p) {
    printf("Pilha de reserva\t(Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("Vazia\n");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].tipo, p->itens[i].id);
        }
        printf("\n");
    }
}

void exibirEstado(Fila* f, Pilha* p) {
    printf("\nEstado atual:\n");
    exibirFila(f);
    exibirPilha(p);
}

// === TROCA SIMPLES: frente da fila ↔ topo da pilha ===
void trocarPecaAtual(Fila* f, Pilha* p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("Troca não realizada: fila ou pilha vazia.\n");
        return;
    }
    Peca temp = f->itens[f->frente];
    f->itens[f->frente] = p->itens[p->topo];
    p->itens[p->topo] = temp;
    printf("Troca realizada entre frente da fila e topo da pilha.\n");
}

// === TROCA MÚLTIPLA: 3 primeiras da fila ↔ 3 da pilha ===
void trocaMultipla(Fila* f, Pilha* p) {
    if (f->tamanho < 3 || p->tamanho < 3) {
        printf("Troca múltipla não realizada: fila ou pilha com menos de 3 peças.\n");
        return;
    }

    // Índices das 3 primeiras da fila (circular)
    int idx1 = f->frente;
    int idx2 = (f->frente + 1) % CAPACIDADE_FILA;
    int idx3 = (f->frente + 2) % CAPACIDADE_FILA;

    // Troca com as 3 da pilha (topo, topo-1, topo-2)
    Peca temp;
    temp = f->itens[idx1];  f->itens[idx1] = p->itens[p->topo];   p->itens[p->topo] = temp;
    temp = f->itens[idx2];  f->itens[idx2] = p->itens[p->topo-1]; p->itens[p->topo-1] = temp;
    temp = f->itens[idx3];  f->itens[idx3] = p->itens[p->topo-2]; p->itens[p->topo-2] = temp;

    printf("Ação: troca realizada entre os 3 primeiros da fila e os 3 da pilha.\n");
}

// === MAIN ===
int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche fila inicial com 5 peças
    for (int i = 0; i < CAPACIDADE_FILA; i++) {
        enfileirar(&fila, gerarPeca());
    }

    int opcao;

    printf("Bem-vindo ao simulador de gerenciamento de peças no Tetris Stack!\n\n");

    while (1) {
        exibirEstado(&fila, &pilha);

        // Menu formatado conforme exemplo
        printf("\nOpções disponíveis:\n");
        printf("Código\tAção\n");
        printf("1\tJogar peça da frente da fila\n");
        printf("2\tEnviar peça da fila para a pilha de reserva\n");
        printf("3\tUsar peça da pilha de reserva\n");
        printf("4\tTrocar peça da frente da fila com o topo da pilha\n");
        printf("5\tTrocar os 3 primeiros da fila com as 3 peças da pilha\n");
        printf("0\tSair\n");
        printf("Opção escolhida: ");

        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n');
            printf("Entrada inválida! Tente novamente.\n");
            continue;
        }

        if (opcao == 0) {
            printf("Saindo do programa. Até logo!\n");
            break;
        }

        else if (opcao == 1) {
            // JOGAR PEÇA
            Peca jogada = desenfileirar(&fila);
            if (jogada.id != -1) {
                printf("Peça jogada: [%c %d]\n", jogada.tipo, jogada.id);
                adicionarNovaPeca(&fila);
            } else {
                printf("Fila vazia! Não há peça para jogar.\n");
            }
        }

        else if (opcao == 2) {
            // RESERVAR PEÇA
            if (filaVazia(&fila)) {
                printf("Fila vazia! Não há peça para reservar.\n");
            } else if (pilhaCheia(&pilha)) {
                printf("Pilha de reserva cheia! Capacidade máxima atingida (%d peças).\n", CAPACIDADE_PILHA);
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
                printf("Pilha de reserva vazia! Não há peça para usar.\n");
            }
        }

        else if (opcao == 4) {
            // TROCAR PEÇA ATUAL
            trocarPecaAtual(&fila, &pilha);
        }

        else if (opcao == 5) {
            // TROCA MÚLTIPLA
            trocaMultipla(&fila, &pilha);
            // Após troca múltipla, adiciona peças novas se necessário
            while (!filaCheia(&fila)) {
                adicionarNovaPeca(&fila);
            }
        }

        else {
            printf("Opção inválida! Use 0 a 5.\n");
        }

        printf("\n---\n");
    }

    return 0;
}