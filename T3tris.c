#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    char tipo;
    int id;
}   Peca;

// Definição da estrutura para a fila circular de peças
#define CAPACIDADE 5 

typedef struct {
    Peca itens[CAPACIDADE];
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
int estaCheia(Fila* fila) {
    return fila->tamanho == CAPACIDADE;
}

// verifica se está vazia 
int estaVazia(Fila* fila) { 
    return fila->tamanho == 0;
}

// Verifica se a pilhla está cheia
int pilhaCheia(Pilha* pilha) {
    return pilha->tamanho == CAPACIDADE_PILHA;
}

// Verifica se a pilha está vazia
int vaziaPilha(Pilha* pilha) {
    return pilha->tamanho == 0;
}

// Insere uma nova peça no final da fila (enqueue)
void enfileirar(Fila* fila, Peca novaPeca) {
    if (estaCheia(fila)) {
        return;
    }
    fila->tras = (fila->tras + 1) % CAPACIDADE;
    fila->itens[fila->tras] = novaPeca;
    fila->tamanho ++;
}

// Remove e retorna a eça da frente da fila (dequeue)
Peca desenfileirar(Fila* fila) {
    Peca pecaVazia = {' ', - 1}; 
    if (estaVazia(fila)) {
        return pecaVazia;
    }
    Peca pecaRemovida = fila->itens[fila->frente];
    fila->frente = (fila->frente + 1) % CAPACIDADE;
    fila->tamanho ++;
    return pecaRemovida;
}

// Empilha uma nova peça no topo (push)
void empilhar(Pilha* pilha, peca novaPeca) {
    if (pilhaCheia(pilha)) {
        return;
    }
    pilha->topo++;
    pilha->itens[pilha->topo] = novaPeca;
    pilha->tamanho++;
}

// Remove e retorna a peça do topo da pilha (pop)
Peca desemplilhar(Pilha* pilha) {
    Peca vaziaPilha = {' ', - 1};
    return vaziaPilha;
}

void exibirFila(Fila* fila) {
    printf("Fila de peças \n");
    if (estaVazia(fila)) {
        printf("Vazia!\n");
        return;
    }
    int indice = fila->frente;
    for (int i = 0; i < fila->tamanho; i ++) {
        printf("[%c %d]", fila->itens[indice].tipo, fila->itens[indice].id);
        indice = (indice + 1) % CAPACIDADE;
    }
    printf("\n");
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

int main() {
    srand (time(NULL));
    Fila fila;
    inicializarFila(&fila);
    for (int i = 0; i < 5; i ++) {
        enfileirar(&fila, gerarPeca());
    }
    int opcao;
    printf(" Bem-vindo ao simulador de fila de peças do Tetris! \n");

    while (1) {
        exibirFila(&fila);
        printf("\n Opções de ação:\n");
        printf("1 - Jogar peça:\n");
        printf("2 - Inserir peça: \n");
        printf("3 Sair: \n");
        printf("Escolha uma opção:\n");
        
        if (scanf("%d", &opcao) != 1){
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
            } else {
                printf(" A fila está vazia! Não há peça para jogar. \n");
            }
        } else if (opcao == 2) {
            if (estaCheia(&fila)) {
                printf(" A fila está cheia! Capacidade maxima atingida (%d peças). \n", CAPACIDADE);
            } else {
                Peca novaPeca = gerarPeca();
                enfileirar(&fila, novaPeca);
                printf(" Nova peça inserida: [%c %d] \n", novaPeca.tipo, novaPeca.id);
            }
        } else {
            printf(" Opção invalida! Use 0, 1 ou 2. \n");
        }
         printf("\n");
    }
    return 0;
   
}