#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10


// Estrutura de uma sala

typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;


// Estrutura da árvore BST de pistas

typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;


// Estrutura da Tabela Hash (pista -> suspeito)

typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode *prox;
} HashNode;

HashNode *tabelaHash[TAM_HASH];


// Função Hash simples (baseada na soma dos caracteres)

int funcaoHash(const char *chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++)
        soma += chave[i];
    return soma % TAM_HASH;
}


// Insere associação pista/suspeito na tabela hash

void inserirNaHash(const char *pista, const char *suspeito) {
    int indice = funcaoHash(pista);
    HashNode *novo = (HashNode*)malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabelaHash[indice];
    tabelaHash[indice] = novo;
}


// Consulta o suspeito correspondente a uma pista

char* encontrarSuspeito(const char *pista) {
    int indice = funcaoHash(pista);
    HashNode *atual = tabelaHash[indice];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return "Desconhecido";
}


// Cria uma sala dinamicamente

Sala* criarSala(const char *nome, const char *pista) {
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));
    if (!novaSala) {
        printf("Erro ao alocar memória para sala!\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    novaSala->esquerda = novaSala->direita = NULL;
    return novaSala;
}

// -------------------------
// Cria um nó de pista
// -------------------------
PistaNode* criarPistaNode(const char *pista) {
    PistaNode *novo = (PistaNode*)malloc(sizeof(PistaNode));
    strcpy(novo->pista, pista);
    novo->esquerda = novo->direita = NULL;
    return novo;
}


// Insere uma pista na árvore BST

PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL)
        return criarPistaNode(pista);

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);
    return raiz;
}


// Exibe as pistas coletadas
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}


// Explora a mansão e coleta pistas

void explorarSalas(Sala *salaAtual, PistaNode **pistas) {
    if (salaAtual == NULL)
        return;

    char escolha;
    printf("\nVocê está na sala: %s\n", salaAtual->nome);

    if (strlen(salaAtual->pista) > 0) {
        printf("💡 Pista encontrada: \"%s\"\n", salaAtual->pista);
        *pistas = inserirPista(*pistas, salaAtual->pista);
    } else {
        printf("Nenhuma pista aqui.\n");
    }

    printf("\nMover-se para (e = esquerda, d = direita, s = sair): ");
    scanf(" %c", &escolha);

    if (escolha == 'e' || escolha == 'E')
        explorarSalas(salaAtual->esquerda, pistas);
    else if (escolha == 'd' || escolha == 'D')
        explorarSalas(salaAtual->direita, pistas);
    else if (escolha == 's' || escolha == 'S')
        printf("Encerrando exploração...\n");
    else {
        printf("Opção inválida.\n");
        explorarSalas(salaAtual, pistas);
    }
}


// Verifica o suspeito final (julga o jogador)

int contarPistasSuspeito(PistaNode *raiz, const char *suspeito) {
    if (raiz == NULL) return 0;
    int cont = 0;
    if (strcmp(encontrarSuspeito(raiz->pista), suspeito) == 0)
        cont++;
    cont += contarPistasSuspeito(raiz->esquerda, suspeito);
    cont += contarPistasSuspeito(raiz->direita, suspeito);
    return cont;
}

void verificarSuspeitoFinal(PistaNode *pistas) {
    char suspeito[50];
    printf("\n==============================\n");
    printf("      JULGAMENTO FINAL\n");
    printf("==============================\n");
    printf("Digite o nome do suspeito que você acha ser o culpado: ");
    scanf(" %[^\n]", suspeito);

    int qtd = contarPistasSuspeito(pistas, suspeito);
    if (qtd >= 2) {
        printf("\n✅ Acusação correta! %s é o culpado.\n", suspeito);
        printf("As pistas confirmam sua suspeita (%d evidências encontradas).\n", qtd);
    } else {
        printf("\n❌ Acusação incorreta! %s não tem pistas suficientes contra si.\n", suspeito);
        printf("Apenas %d pista(s) o relacionam ao caso.\n", qtd);
    }
}

// Libera memória

void liberarSalas(Sala *raiz) {
    if (raiz != NULL) {
        liberarSalas(raiz->esquerda);
        liberarSalas(raiz->direita);
        free(raiz);
    }
}

void liberarPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}

// Função principal
int main() {
    // Configuração do mapa da mansão
    Sala *hall = criarSala("Hall de Entrada", "Pegadas de sapato masculino.");
    Sala *salaEstar = criarSala("Sala de Estar", "Copo de vinho com batom.");
    Sala *cozinha = criarSala("Cozinha", "Faca limpa mas molhada.");
    Sala *biblioteca = criarSala("Biblioteca", "Livro sobre venenos.");
    Sala *jardim = criarSala("Jardim", "Luvas jogadas na terra.");
    Sala *quarto = criarSala("Quarto Secreto", "Foto rasgada de um casal.");

    // Conectando salas (árvore binária)
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->direita = quarto;

    // Associação das pistas aos suspeitos
    inserirNaHash("Pegadas de sapato masculino.", "Sr. Ricardo");
    inserirNaHash("Copo de vinho com batom.", "Sra. Helena");
    inserirNaHash("Faca limpa mas molhada.", "Chef Marcos");
    inserirNaHash("Livro sobre venenos.", "Sra. Helena");
    inserirNaHash("Luvas jogadas na terra.", "Jardineiro Carlos");
    inserirNaHash("Foto rasgada de um casal.", "Sr. Ricardo");

    printf("Bem-vindo ao Detective Quest - Capítulo Final!\n");
    printf("Explore a mansão e colete pistas para encontrar o verdadeiro culpado.\n");

    PistaNode *pistas = NULL;
    explorarSalas(hall, &pistas);

    printf("\n==============================\n");
    printf(" PISTAS COLETADAS (ordem A-Z) \n");
    printf("==============================\n");
    exibirPistas(pistas);

    verificarSuspeitoFinal(pistas);

    liberarSalas(hall);
    liberarPistas(pistas);
    printf("\nFim da jornada. Até a próxima, detetive!\n");
    return 0;
}
