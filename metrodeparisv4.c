#include <stdio.h>
#include <stdlib.h>

#define TAM 14
#define MIN_POR_KM 2
#define MIN_BALDEACAO 4

static const char *nome_linha[5] = {"-", "azul", "amarela", "vermelha", "verde"};

static const int linhas[TAM][TAM] = {
  {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 1, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0},
  {0, 1, 0, 1, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0},
  {0, 0, 1, 0, 1, 0, 0, 4, 0, 0, 0, 0, 4, 0},
  {0, 0, 0, 1, 0, 1, 2, 2, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 4, 2, 0, 0, 0, 2, 0, 0, 4, 0, 0},
  {0, 2, 3, 0, 0, 0, 0, 2, 0, 0, 3, 0, 0, 0},
  {0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0},
  {0, 0, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0}
};

static const float distancias_reais[TAM][TAM] = {
  {0,  10,  0,   0,   0, 0,   0,   0,    0,   0,   0,    0, 0,    0},
  {10, 0,   8.5, 0,   0, 0,   0,   0,    10,  3.5, 0,    0, 0,    0},
  {0,  8.5, 0,   6.3, 0, 0,   0,   0,    9.4, 0,   0,    0, 18.7, 0},
  {0,  0,   6.3, 0,   13, 0,  0,   15.3, 0,   0,   0,    0, 12.8, 0},
  {0,  0,   0,   13,  0, 3,   2.4, 30,   0,   0,   0,    0, 0,    0},
  {0,  0,   0,   0,   3, 0,   0,   0,    0,   0,   0,    0, 0,    0},
  {0,  0,   0,   0,   2.4, 0, 0,   0,    0,   0,   0,    0, 0,    0},
  {0,  0,   0,   15.3, 30, 0, 0,   0,    9.6, 0,   0,    6.4, 0,  0},
  {0,  10,  9.4, 0,   0, 0,   0,   9.6,  0,   0,   12.2, 0, 0,    0},
  {0,  3.5, 0,   0,   0, 0,   0,   0,    0,   0,   0,    0, 0,    0},
  {0,  0,   0,   0,   0, 0,   0,   0,    12.2, 0,  0,    0, 0,    0},
  {0,  0,   0,   0,   0, 0,   0,   6.4,  0,   0,   0,    0, 0,    0},
  {0,  0,   18.7, 12.8, 0, 0, 0,   0,    0,   0,   0,    0, 0,  5.1},
  {0,  0,   0,   0,   0, 0,   0,   0,    0,   0,   0,    0, 5.1,  0}
};

static const float distancias_diretas[TAM][TAM] = {
  {0,    10,   18.5, 24.8, 36.4, 38.8, 35.8, 25.4, 17.6, 9.1,  16.7, 27.3, 27.6, 29.8},
  {10,   0,    8.5,  14.8, 26.6, 29.1, 26.1, 17.3, 10,   3.5,  15.5, 20.9, 19.1, 21.8},
  {18.5, 8.5,  0,    6.3,  18.2, 20.6, 17.6, 13.6, 9.4,  10.3, 19.5, 19.1, 12.1, 16.6},
  {24.8, 14.8, 6.3,  0,    12,   14.4, 11.5, 12.4, 12.6, 16.7, 23.6, 18.6, 10.6, 15.4},
  {36.4, 26.6, 18.2, 12,   0,    3,    2.4,  19.4, 23.3, 28.2, 34.2, 24.8, 14.5, 17.9},
  {38.8, 29.1, 20.6, 14.4, 3,    0,    3.3,  22.3, 25.7, 30.3, 36.7, 27.6, 15.2, 18.2},
  {35.8, 26.1, 17.6, 11.5, 2.4,  3.3,  0,    20,   23,   27.3, 34.2, 25.7, 12.4, 15.6},
  {25.4, 17.3, 13.6, 12.4, 19.4, 22.3, 20,   0,    8.2,  20.3, 16.1, 6.4,  22.7, 27.6},
  {17.6, 10,   9.4,  12.6, 23.3, 25.7, 23,   8.2,  0,    13.5, 11.2, 10.9, 21.2, 26.6},
  {9.1,  3.5,  10.3, 16.7, 28.2, 30.3, 27.3, 20.3, 13.5, 0,    17.6, 24.2, 18.7, 21.2},
  {16.7, 15.5, 19.5, 23.6, 34.2, 36.7, 34.2, 16.1, 11.2, 17.6, 0,    14.2, 31.5, 35.5},
  {27.3, 20.9, 19.1, 18.6, 24.8, 27.6, 25.7, 6.4,  10.9, 24.2, 14.2, 0,    28.8, 33.6},
  {27.6, 19.1, 12.1, 10.6, 14.5, 15.2, 12.4, 22.7, 21.2, 18.7, 31.5, 28.8, 0,     5.1},
  {29.8, 21.8, 16.6, 15.4, 17.9, 18.2, 15.6, 27.6, 26.6, 21.2, 35.5, 33.6, 5.1,     0}
};

typedef struct No *cel;
typedef struct No {
  int estacao;
  int linha;
  float f;
  float g;
  float h;
  cel raiz;
  cel prox;
} No;

typedef struct {
  cel inicio;
  cel fim;
} Fila;

int esta_na_lista(int n, int *lista, int tam) {
  for (int i = 0; i < tam; ++i) {
    if (lista[i] == n) return 1;
  }
  return 0;
}

Fila* criar_fila() {
  Fila *fila = (Fila*)malloc(sizeof(Fila));
  fila->inicio = (cel)malloc(sizeof(No));
  fila->inicio->prox = NULL;
  fila->fim = fila->inicio;
  return fila;
}

int fila_vazia(Fila *fila) {
  return fila->inicio == fila->fim;
}

void inserir_fila(Fila *fila, No *no) {
  cel ant = fila->inicio;
  cel ind = fila->inicio->prox;

  while (ind != NULL && no->f >= ind->f) {
    ant = ind;
    ind = ind->prox;
  }

  ant->prox = no;
  no->prox = ind;
  if (ind == NULL) fila->fim = no;
}

No* remover_fila(Fila *fila) {
  if (fila_vazia(fila)) {
    printf("fila vazia\n");
    exit(1);
  }

  No *primeiro = fila->inicio->prox;
  if (primeiro == fila->fim) fila->fim = fila->inicio;

  fila->inicio->prox = primeiro->prox;
  primeiro->prox = NULL;
  return primeiro;
}

void remover_no_fila(Fila *fila, No *no) {
  cel ant = fila->inicio;
  cel ind = fila->inicio->prox; //ponteiro auxiliar que percorre a fila

  while (ind != NULL && ind != no){
    ant = ind;
    ind = ind->prox;
  }
  if (ind == NULL) return;

  ant->prox = ind->prox;
  if (fila->fim == ind) fila->fim = ant;
  ind->prox = NULL;
}

No* encontrar_no(Fila *fila, int estacao) {
  cel ind = fila->inicio->prox;
  while (ind){
    if (ind->estacao == estacao) return ind;
    ind = ind->prox;
  }
  return NULL;
}

void definir_no(No* no, int estacao, int linha, float g, float h, No* raiz) {
  no->estacao = estacao;
  no->linha = linha;
  no->f = g + h;
  no->g = g;
  no->h = h;
  no->raiz = raiz;
}

int esta_na_fila(Fila *fila, int estacao) {
  return encontrar_no(fila, estacao) != NULL;
}

void imprimir_no(const char *n_estacao, No *no) {
  printf("%s E%d  g=%.1f  h=%.1f  f=%.1f\n", n_estacao, no->estacao + 1, no->g, no->h, no->f);
}

No* a_estrela(int origem, int destino) {
  float h = distancias_diretas[origem][destino] * MIN_POR_KM;

  No *estacao_origem = (No*)malloc(sizeof(No));
  *estacao_origem = (No){origem, 0, h, 0, h, NULL, NULL}; //preenche os campos do no de uma vez

  Fila *lista_aberta = criar_fila(); //estacoes a serem analisadas
  inserir_fila(lista_aberta, estacao_origem);

  int lista_fechada[TAM]; //estacoes ja analisadas
  int tam_lista_fechada = 0;

  while (!fila_vazia(lista_aberta)){
    No *estacao_atual = remover_fila(lista_aberta);

    if (estacao_atual->estacao == destino) {
      return estacao_atual;
    }

    lista_fechada[tam_lista_fechada++] = estacao_atual->estacao;

    for (int vizinha = 0; vizinha < TAM; ++vizinha) {
      int nova_linha = linhas[estacao_atual->estacao][vizinha];
      if (!nova_linha) continue;
      if (esta_na_lista(vizinha, lista_fechada, tam_lista_fechada)) continue;

      // float g_teste = estacao_atual->g + valor_matriz(distancias_reais, estacao_atual->estacao, vizinha) * MIN_POR_KM;
      float g_teste = estacao_atual->g + distancias_reais[estacao_atual->estacao][vizinha] * MIN_POR_KM;
      if (estacao_atual->linha != 0 && estacao_atual->linha != nova_linha) {
        g_teste += MIN_BALDEACAO;
      }

      No *no_vizinho = encontrar_no(lista_aberta, vizinha);

      if (no_vizinho == NULL){
        no_vizinho = (No*)malloc(sizeof(No));
        definir_no(no_vizinho, vizinha, nova_linha, g_teste, distancias_diretas[vizinha][destino] * MIN_POR_KM, estacao_atual);
        inserir_fila(lista_aberta, no_vizinho);

      } else if (g_teste < no_vizinho->g){
        remover_no_fila(lista_aberta, no_vizinho);
        definir_no(no_vizinho, no_vizinho->estacao, nova_linha, g_teste, no_vizinho->h, estacao_atual);

        inserir_fila(lista_aberta, no_vizinho);
      }
    }
  }

  printf("Erro: nao existe rota entre as estacoes\n");
  exit(1);
}

void busca(int origem, int destino) {
  No *no_destino = a_estrela(origem, destino);
  No *ind = no_destino;

  int estacoes[TAM];
  int qtd_estacoes = 0;

  while (ind != NULL) {
    estacoes[qtd_estacoes++] = ind->estacao;
    ind = ind->raiz;
  }

  printf("\nMelhor percurso:\n");
  int linha_anterior = 0;
  for (int i = qtd_estacoes - 1; i > 0; --i) {
    int a = estacoes[i];
    int b = estacoes[i - 1];
    int linha_trecho = linhas[a][b];

    printf("Estacao %d -> Estacao %d  Tempo: %.1f min  (linha %s)\n",
           a + 1, b + 1, distancias_reais[a][b] * MIN_POR_KM, nome_linha[linha_trecho]);

    if (linha_anterior != 0 && linha_trecho != linha_anterior) {
      printf("   (troca de linha: +%d min)\n", MIN_BALDEACAO);
    }
    linha_anterior = linha_trecho;
  }

  int minutos = (int)(no_destino->g);
  printf("\nTempo Total: %d %s e %d %s\n",
         minutos / 60, (minutos / 60 == 1) ? "hora" : "horas",
         minutos % 60, (minutos % 60 == 1) ? "minuto" : "minutos");
}

int main() {
  int origem, destino;

  printf("Estacao de origem (1-%d): ", TAM);
  scanf("%d", &origem);

  printf("Estacao de destino (1-%d): ", TAM);
  scanf("%d", &destino);

  if(origem < 1 || origem > TAM || destino < 1 || destino > TAM) {
    printf("Estacao invalida.\n");
    return 1;
  }

  busca(origem - 1, destino - 1); //usa ind 0 por isso subtrai 1
  return 0;
}
