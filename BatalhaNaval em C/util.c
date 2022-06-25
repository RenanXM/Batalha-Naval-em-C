# include <stdio.h>
# include <stdlib.h>
# include "estrutura.h"

// Necessário para Jogadas.c , Gameplay.c e Tabuleiro.c
// Retorna um inteiro aleatório de 0 a n-1
int randRange(int n) 
{
  int limit, r;
  
  limit = RAND_MAX - (RAND_MAX % n);

  while((r = rand()) >= limit);

  return r % n;
}


// Função para sair do nó T->prim (ou posição A1 do tabuleiro) e deslocarmos d passos para a direita e b passos para baixo
no *Deslocamento (no *prim, int d, int b)
{
    int i, j;
    no *x;
    x = prim;
    i = j = 0;
    while (i < d){
        x = x->dir;
        i += 1;
    }
    while (j < b){
        x = x->baixo;
        j += 1;
    }
    return x;
}

