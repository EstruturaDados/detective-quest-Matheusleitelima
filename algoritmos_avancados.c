#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura que representa uma sala da mansão
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Função para criar uma sala de forma dinâmica
Sala* criarSala(const char *nome) {
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Função para explorar as salas da mansão
void explorarSalas(Sala *salaAtual) {
    char escolha;

    if (salaAtual == NULL) {
        return;
    }

    printf("\nVocê está na sala: %s\n", salaAtual->nome);

    // Se não há caminhos à esquerda nem à direita, termina exploração
    if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
        printf("Não há mais caminhos a seguir. Fim da exploração!\n");
        return;
    }

    // Menu de opções
    printf("Para onde deseja ir? (e = esquerda, d = direita, s = sair): ");
    scanf(" %c", &escolha);

    if (escolha == 'e' || escolha == 'E') {
        if (salaAtual->esquerda != NULL) {
            explorarSalas(salaAtual->esquerda);
        } else {
            printf("Não há caminho à esquerda!\n");
            explorarSalas(salaAtual);
        }
    } else if (escolha == 'd' || escolha == 'D') {
        if (salaAtual->direita != NULL) {
            explorarSalas(salaAtual->direita);
        } else {
            printf("Não há caminho à direita!\n");
            explorarSalas(salaAtual);
        }
    } else if (escolha == 's' || escolha == 'S') {
        printf("Exploração encerrada pelo jogador.\n");
        return;
    } else {
        printf("Opção inválida! Tente novamente.\n");
        explorarSalas(salaAtual);
    }
}

// Função principal
int main() {
    // Criação das salas da mansão
    Sala *hallEntrada = criarSala("Hall de Entrada");
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *cozinha = criarSala("Cozinha");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *jardim = criarSala("Jardim");
    Sala *quartoSecreto = criarSala("Quarto Secreto");

    // Montagem do mapa da mansão (árvore binária)
    hallEntrada->esquerda = salaEstar;
    hallEntrada->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->direita = quartoSecreto;

    printf("Bem-vindo ao Detective Quest! Explore a mansão.\n");

    // Início da exploração
    explorarSalas(hallEntrada);

    // Liberação da memória alocada
    free(hallEntrada);
    free(salaEstar);
    free(cozinha);
    free(biblioteca);
    free(jardim);
    free(quartoSecreto);

    return 0;
}
