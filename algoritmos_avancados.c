#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura que representa uma sala da mansão
typedef struct Sala {
    char nome[50];
    char pista[100]; // Pista encontrada neste cômodo
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura para armazenar as pistas coletadas (BST)
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// CRIAÇÃO DAS ESTRUTURAS
// Cria uma sala dinamicamente com seu nome e pista associada
Sala* criarSala(const char *nome, const char *pista) {
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Cria um novo nó de pista
PistaNode* criarPistaNode(const char *pista) {
    PistaNode *novo = (PistaNode*)malloc(sizeof(PistaNode));
    if (novo == NULL) {
        printf("Erro ao alocar memória para pista!\n");
        exit(1);
    }
    strcpy(novo->pista, pista);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// FUNÇÕES DE BST (PISTAS) 
// Insere uma nova pista na árvore BST (ordem alfabética)
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL) {
        return criarPistaNode(pista);
    }

    if (strcmp(pista, raiz->pista) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (strcmp(pista, raiz->pista) > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    return raiz;
}

// Exibe as pistas em ordem alfabética (in-order)
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

// Libera memória da árvore de pistas
void liberarPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}

//  FUNÇÃO DE EXPLORAÇÃO

// Explora a mansão e coleta pistas
void explorarSalasComPistas(Sala *salaAtual, PistaNode **pistas) {
    char escolha;

    if (salaAtual == NULL) {
        return;
    }

    printf("\nVocê está na sala: %s\n", salaAtual->nome);

    // Se houver uma pista neste cômodo, adiciona à BST
    if (strlen(salaAtual->pista) > 0) {
        printf("💡 Você encontrou uma pista: \"%s\"\n", salaAtual->pista);
        *pistas = inserirPista(*pistas, salaAtual->pista);
    } else {
        printf("Nenhuma pista encontrada aqui.\n");
    }

    // Opções de navegação
    printf("\nPara onde deseja ir? (e = esquerda, d = direita, s = sair): ");
    scanf(" %c", &escolha);

    if (escolha == 'e' || escolha == 'E') {
        if (salaAtual->esquerda != NULL) {
            explorarSalasComPistas(salaAtual->esquerda, pistas);
        } else {
            printf("Não há caminho à esquerda!\n");
            explorarSalasComPistas(salaAtual, pistas);
        }
    } else if (escolha == 'd' || escolha == 'D') {
        if (salaAtual->direita != NULL) {
            explorarSalasComPistas(salaAtual->direita, pistas);
        } else {
            printf("Não há caminho à direita!\n");
            explorarSalasComPistas(salaAtual, pistas);
        }
    } else if (escolha == 's' || escolha == 'S') {
        printf("\nExploração encerrada pelo jogador.\n");
        return;
    } else {
        printf("Opção inválida! Tente novamente.\n");
        explorarSalasComPistas(salaAtual, pistas);
    }
}

// UNÇÃO PRINCIPAL

int main() {
    // Criação das salas com suas pistas
    Sala *hallEntrada = criarSala("Hall de Entrada", "Pegadas misteriosas perto da porta.");
    Sala *salaEstar = criarSala("Sala de Estar", "Um copo quebrado sobre a mesa.");
    Sala *cozinha = criarSala("Cozinha", "Uma luva manchada de vinho.");
    Sala *biblioteca = criarSala("Biblioteca", "Um livro aberto com anotações estranhas.");
    Sala *jardim = criarSala("Jardim", "Terra revirada recentemente.");
    Sala *quartoSecreto = criarSala("Quarto Secreto", "Um colar com as iniciais da vítima.");

    // Montagem do mapa da mansão (árvore binária)
    hallEntrada->esquerda = salaEstar;
    hallEntrada->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->direita = quartoSecreto;

    printf("🕵️‍♂️ Bem-vindo ao Detective Quest!\n");
    printf("Explore a mansão e colete pistas para resolver o mistério.\n");

    // Raiz da árvore de pistas
    PistaNode *pistasColetadas = NULL;

    // Início da exploração
    explorarSalasComPistas(hallEntrada, &pistasColetadas);

    // Exibe todas as pistas coletadas
    printf("\n==============================\n");
    printf(" PISTAS COLETADAS (ordem A-Z) \n");
    printf("==============================\n");
    if (pistasColetadas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        exibirPistas(pistasColetadas);
    }

    // Libera memória
    free(hallEntrada);
    free(salaEstar);
    free(cozinha);
    free(biblioteca);
    free(jardim);
    free(quartoSecreto);
    liberarPistas(pistasColetadas);

    printf("\nFim da jornada. Até a próxima, detetive!\n");

    return 0;
}
