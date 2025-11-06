#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    char tipo;
    int id;
}   Peca;

// Definição da estrutura para a fila circular de peças
#define CAPACIDADE_FILA 

typedef struct {
    Peca itens[CAPACIDADE_FILA];
    int frente;
    int tras;
    int tamanho;
} Fila;

// Definição da estrutura para a pilha de reserva de peças 
#define CAPACIDADE_PILHA 3
typedef struct {
    Peca itens[CAPACIDADE_PILHA];
    int topo;
    int tamanho;
} Pilha;

// Função para inicializar a fila 
void inicializarFila(Fila* fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila-> tamanho = 0;
}

// Função para inicializar pilha
void inicializarPilha(Pilha* pilha) {
    pilha->topo = -1;
    pilha-> tamanho = 0;
}

// Verifica se a fila está cheia 
int Filacheia(Fila* fila) {
    return fila->tamanho == CAPACIDADE_FILA;
}

// verifica se está vazia 
int Filavazia(Fila* fila) { 
    return fila->tamanho == 0;
}

// Verifica se a pilhla está cheia
int pilhaCheia(Pilha* pilha) {
    return pilha->tamanho == CAPACIDADE_PILHA;
}

// Verifica se a pilha está vazia
int Pilhavazia(Pilha* pilha) {
    return pilha->tamanho == 0;
}

// Insere uma nova peça no final da fila (enqueue)
void enfileirar(Fila* fila, Peca novaPeca) {
    if (Filacheia(fila)) {
        return;
    }
    fila->tras = (fila->tras + 1) % CAPACIDADE_FILA;
    fila->itens[fila->tras] = novaPeca;
    fila->tamanho ++;
}

// Remove e retorna a eça da frente da fila (dequeue)
Peca desenfileirar(Fila* fila) {
    Peca pecaVazia = {' ', - 1}; 
    if (Filavazia(fila)) {
        return pecaVazia;
    }
    Peca pecaRemovida = fila->itens[fila->frente];
    fila->frente = (fila->frente + 1) % CAPACIDADE_FILA;
    fila->tamanho --;
    return pecaRemovida;
}

// Empilha uma nova peça no topo (push)
void empilhar(Pilha* pilha, Peca novaPeca) {
    if (pilhaCheia(pilha)) {
        return;
    }
    pilha->topo++;
    pilha->itens[pilha->topo] = novaPeca;
    pilha->tamanho++;
}

// Remove e retorna a peça do topo da pilha (pop)
Peca desempilhar(Pilha* pilha) {
    Peca pecaVazia = {' ', -1};
    if (Pilhavazia(pilha)) {
        return pecaVazia;
    }
    Peca pecaRemovida = pilha->itens[pilha->topo];
    pilha->topo--;
    pilha->tamanho--;
    return pecaRemovida;
}

// Exibe o estado atual da fila
void exibirFila(Fila* fila) {
    printf("Fila de peças \n");
    if (Filavazia(fila)) {
        printf("Vazia!\n");
    } else {
        int indice = fila->frente;
        for (int i = 0; i < fila->tamanho; i ++); {
            printf("[%C %d]", fila->itens[indice].tipo, fila->itens[indice].id);
            indice = (indice + 1 % CAPACIDADE_FILA);
        }
    }
    printf("\n");
    
}

// Exibe o estado atual da pilha
void exibirPilha(Pilha* pilha) {
    printf("Pilha de reserva\t(Topo -> Base): ");
    if (Pilhavazia(pilha)) {
        printf("Vazia");
    } else {
        for (int i = pilha->topo; i >= 0; i --) {
            printf("[%c %d]", pilha->itens[i].topo, pilha->itens[i].id);
        }
    }
    printf("\n");
}

// Exibe o estdo atual completo (fila ou pilha)
void exibiEstado(Fila* fila, Pilha* pilha) {
    printf("\n Estado atual:\n");
    exibirFila(fila);
    exibirPilha(pilha);
}

// Gera uma nova peça automaticamente 
Peca gerarPeca() {
    static int proximoId = 0;
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca novaPeca;
    novaPeca.tipo = tipos[rand() % 4];
    novaPeca.id = proximoId ++;
    return novaPeca;
}

// Add uma nova peça gerada ao final da fila, mantém a fila sempre cheia, conforme requisito
void addnovaaPeca(Fila* fila) {
    if (!Filacheia(fila)) {
        Peca novaPeca = gerarPeca();
        enfileirar(fila, novaPeca);
    }
}

// Inicialização da semente para números aleatórios
int main() {
    srand (time(NULL));

    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    for (int i = 0; i < CAPACIDADE_FILA; i ++) {
        enfileirar(&fila, gerarPeca());
    }
    int opcao;
    printf(" Bem-vindo ao simulador de fila de peças do Tetris! \n");

    while (1) {
        exibirFila(&fila, &pilha);
        printf("\n Opções de ação:\n");
        printf("Código\tAção\n");
        printf("1 - \tJogar peça\n");
        printf("2 - \tReserva peça\n");
        printf("3 - \tUsar peça reservada\n");
        printf("0 - \tSair\n");
        
        if (scanf("%d", &opcao) != 1) {
            while (getchar () != '\n');
            printf("Opção invalida! Tente novamente. \n");
            continue;
        }
        if (opcao == 0) {
            printf("Saindo do programa. Até logo! \n");
            break;

        } else if (opcao == 1) {
            Peca pecaJogada = desenfileirar(&fila);
            if (pecaJogada.id != -1) {
                printf(" Peça jogada: [%c %d]\n", pecaJogada.tipo, pecaJogada.id);
                addnovaaPeca(&fila);

            } else {
                printf(" A fila está vazia! Não há peça para jogar. \n");
            }

        } else if (opcao == 2) {
            if (Filavazia(&fila)) {
                printf("A fila está vazia! Não há espaço para reservar.\n");
            }

        } else if (pilhaCheia(&pilha)) {
            printf("A pilha de reserva está cheia! CApacidade máxima atingida (%d peças).\n", CAPACIDADE_PILHA);
        
        } else {
            Peca pecaReservada = desenfileirar(&fila);
            empilhar(&pilha, pecaReservada);
            printf("Peça reservada: [%c %d]\n", pecaReservada.tipo, pecaReservada.id);
            addnovaaPeca(&fila);
        }
    } else if (opcao == 3) {
        Peca pecaUsada = desempilhar(&pilha);
        if (pecaUSada.id != -1) {
            printf("Peça usada da reserva: [%c %d]\n", pecaUsada.tipo, pecaUSada.id);
        } else {
            printf("A pilha da reserva está vazia! Não há peças para usar.\n");
        }
    } else {
        printf("Opção invalida! USe 0, 1, 2 ou 3.\n");
    }
        printf("\n --- \n");
        return 0;
}