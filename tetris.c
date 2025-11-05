#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// ====== STRUCTS ======
typedef struct {
    int id;
    char tipo;
} Peca;

typedef struct {
    Peca fila[TAM_FILA];
    int frente, tras, qtd;
} FilaCircular;

typedef struct {
    Peca pilha[TAM_PILHA];
    int topo;
} Pilha;

// ====== FUNÇÕES BASE ======
char tipos[] = {'I', 'O', 'T', 'L', 'S', 'Z', 'J'};

Peca gerarPeca() {
    Peca nova;
    nova.id = rand() % 1000;
    nova.tipo = tipos[rand() % 7];
    return nova;
}

// ====== FUNÇÕES FILA ======
void inicializarFila(FilaCircular *f) {
    f->frente = 0;
    f->tras = -1;
    f->qtd = 0;
    for (int i = 0; i < TAM_FILA; i++) {
        f->fila[i] = gerarPeca();
        f->tras = (f->tras + 1) % TAM_FILA;
        f->qtd++;
    }
}

int filaVazia(FilaCircular *f) { return f->qtd == 0; }
int filaCheia(FilaCircular *f) { return f->qtd == TAM_FILA; }

Peca removerFila(FilaCircular *f) {
    if (filaVazia(f)) {
        printf("Fila vazia!\n");
        Peca p = {-1, '-'};
        return p;
    }
    Peca removida = f->fila[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->qtd--;
    return removida;
}

void inserirFila(FilaCircular *f, Peca p) {
    if (filaCheia(f)) {
        printf("Fila cheia! (circular, substituindo...)\n");
        f->frente = (f->frente + 1) % TAM_FILA;
        f->qtd--;
    }
    f->tras = (f->tras + 1) % TAM_FILA;
    f->fila[f->tras] = p;
    f->qtd++;
}

void exibirFila(FilaCircular *f) {
    printf("\n[FILA DE PEÇAS FUTURAS]\n");
    if (filaVazia(f)) {
        printf("Fila vazia.\n");
        return;
    }
    int i = f->frente;
    for (int c = 0; c < f->qtd; c++) {
        printf("(%d - %c) ", f->fila[i].id, f->fila[i].tipo);
        i = (i + 1) % TAM_FILA;
    }
    printf("\n");
}

// ====== FUNÇÕES PILHA ======
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) { return p->topo == -1; }
int pilhaCheia(Pilha *p) { return p->topo == TAM_PILHA - 1; }

void push(Pilha *p, Peca x) {
    if (pilhaCheia(p)) {
        printf("Pilha cheia!\n");
        return;
    }
    p->pilha[++p->topo] = x;
}

Peca pop(Pilha *p) {
    if (pilhaVazia(p)) {
        printf("Pilha vazia!\n");
        Peca v = {-1, '-'};
        return v;
    }
    return p->pilha[p->topo--];
}

void exibirPilha(Pilha *p) {
    printf("\n[PILHA DE RESERVA]\n");
    if (pilhaVazia(p)) {
        printf("Pilha vazia.\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        printf("(%d - %c)\n", p->pilha[i].id, p->pilha[i].tipo);
    }
}

// ====== FUNÇÕES NÍVEIS ======
void nivelNovato() {
    FilaCircular fila;
    inicializarFila(&fila);
    int opcao;
    do {
        exibirFila(&fila);
        printf("\n[1] Jogar peça\n[2] Inserir nova peça\n[0] Sair\nEscolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            Peca jogada = removerFila(&fila);
            printf("Peça jogada: (%d - %c)\n", jogada.id, jogada.tipo);
            inserirFila(&fila, gerarPeca());
        } else if (opcao == 2) {
            inserirFila(&fila, gerarPeca());
        }

    } while (opcao != 0);
}

void nivelAventureiro() {
    FilaCircular fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    int opcao;
    do {
        exibirFila(&fila);
        exibirPilha(&pilha);
        printf("\n[1] Jogar peça\n[2] Reservar peça\n[3] Usar peça reservada\n[0] Sair\nEscolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            Peca jogada = removerFila(&fila);
            printf("Peça jogada: (%d - %c)\n", jogada.id, jogada.tipo);
            inserirFila(&fila, gerarPeca());
        } else if (opcao == 2) {
            if (!filaVazia(&fila)) {
                Peca reservada = removerFila(&fila);
                push(&pilha, reservada);
                printf("Peça reservada: (%d - %c)\n", reservada.id, reservada.tipo);
                inserirFila(&fila, gerarPeca());
            }
        } else if (opcao == 3) {
            Peca usada = pop(&pilha);
            if (usada.id != -1)
                printf("Peça usada: (%d - %c)\n", usada.id, usada.tipo);
        }

    } while (opcao != 0);
}

void nivelMestre() {
    FilaCircular fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    Peca ultimaJogada = {-1, '-'};
    int opcao;
    do {
        exibirFila(&fila);
        exibirPilha(&pilha);
        printf("\n[1] Jogar peça\n[2] Reservar peça\n[3] Usar peça reservada\n[4] Trocar topo com frente\n[5] Desfazer última jogada\n[6] Inverter fila com pilha\n[0] Sair\nEscolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                ultimaJogada = removerFila(&fila);
                printf("Peça jogada: (%d - %c)\n", ultimaJogada.id, ultimaJogada.tipo);
                inserirFila(&fila, gerarPeca());
                break;
            case 2:
                if (!filaVazia(&fila)) {
                    Peca reservada = removerFila(&fila);
                    push(&pilha, reservada);
                    inserirFila(&fila, gerarPeca());
                }
                break;
            case 3: {
                Peca usada = pop(&pilha);
                if (usada.id != -1)
                    printf("Usou peça reservada: (%d - %c)\n", usada.id, usada.tipo);
                break;
            }
            case 4: // troca topo com frente
                if (!pilhaVazia(&pilha) && !filaVazia(&fila)) {
                    Peca temp = pilha.pilha[pilha.topo];
                    pilha.pilha[pilha.topo] = fila.fila[fila.frente];
                    fila.fila[fila.frente] = temp;
                    printf("Peça do topo trocada com a da frente!\n");
                }
                break;
            case 5: // desfazer
                if (ultimaJogada.id != -1) {
                    inserirFila(&fila, ultimaJogada);
                    printf("Desfez jogada anterior: (%d - %c)\n", ultimaJogada.id, ultimaJogada.tipo);
                    ultimaJogada.id = -1;
                } else {
                    printf("Nenhuma jogada para desfazer.\n");
                }
                break;
            case 6: { // inverter fila com pilha
                Pilha tempPilha;
                inicializarPilha(&tempPilha);
                while (!filaVazia(&fila))
                    push(&tempPilha, removerFila(&fila));
                while (!pilhaVazia(&pilha))
                    inserirFila(&fila, pop(&pilha));
                while (!pilhaCheia(&pilha) && !pilhaVazia(&tempPilha))
                    push(&pilha, pop(&tempPilha));
                printf("Fila e pilha invertidas!\n");
                break;
            }
        }
    } while (opcao != 0);
}

// ====== MAIN ======
int main() {
    srand(time(NULL));
    int nivel;
    do {
        printf("\n===== TETRIS STACK =====\n");
        printf("[1] Nível Novato\n[2] Nível Aventureiro\n[3] Nível Mestre\n[0] Sair\nEscolha: ");
        scanf("%d", &nivel);
        switch (nivel) {
            case 1: nivelNovato(); break;
            case 2: nivelAventureiro(); break;
            case 3: nivelMestre(); break;
        }
    } while (nivel != 0);
    printf("Encerrando o jogo...\n");
    return 0;
}
