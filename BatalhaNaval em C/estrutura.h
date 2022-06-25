#ifndef ESTRUTURA
#define ESTRUTURA

// Tipos de embarcação
# define jangada 'j'
# define destroyer 'd'
# define fragata 'f'
# define submarino 's'
# define porta_avioes 'p'
# define agua 'a'

// Tipos de gráfico
# define agua_atacada 'O'
# define grafico_vazio ' '
# define embarcacao_atingida '*'
# define graf_submarino '@'
# define graf_jangada '&'
# define graf_extremidade_sup '^'
# define graf_extremidade_inf 'v'
# define graf_extremidade_dir '>'
# define graf_extremidade_esq '<'
# define corpo_principal '#'

# define SIM '1'
# define NAO '0'

// num_aux serve para identificar as diferentes partes de uma mesma embarcação. Só mexi nesse campo em porta_avioes e em destroyers
// Os campos vertical e direcao servem para identificar a orientação de uma embarcação: (em relação à parte da embarcação com num_aux == '0')
// vertical == '0' e direcao == '0' -> horizontal na direita
// vertical == '0' e direcao == '1' -> horizontal na esquerda
// vertical == '1' e direcao == '0' -> vertical para cima
// vertical == '1' e direcao == '1' -> vertical para baixo
typedef struct no
{
    char tipo_embarcacao; 
    char vertical;
    char direcao;
    char num_aux; //num_aux serve para guardar a numeração daquela parte da embarcação
    char grafico;
    char grafico_original;
    char tiro;
    char destruicao_total; // contabiliza quantas partes de uma embarcação foram atingidas
    struct no *dir;
    struct no *esq;
    struct no *cima;
    struct no *baixo;
} no;

typedef struct tabuleiro
{
    no *prim;
    char num_porta_avioes;
    char num_destroyers;
    char num_fragatas;
    char num_submarinos;
    char num_jangada;
    char perdeu;
} tabuleiro;

// Funções usadas em Gameplay.c, Jogadas.c e Tabuleiro.c
int randRange(int);
no *Deslocamento (no *, int, int);

// Funções usadas na main.c
tabuleiro *Criar_Tabuleiro();
void Colocar_as_pecas_Tabuleiro (tabuleiro *);
void Liberando_espaco (tabuleiro *);
void Reiniciar_Tabuleiro (tabuleiro *);
void Jogo(tabuleiro *, tabuleiro *);

// Funções usadas em Gameplay.c
no *func_jogada_humano(char *, tabuleiro *, tabuleiro *);
int jogada_computador (char, int, int, tabuleiro *, tabuleiro *);
void Atualiza_Tabuleiro (no *, tabuleiro *, tabuleiro *, char);

#endif