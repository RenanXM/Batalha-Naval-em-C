# include <stdio.h>
# include <stdlib.h>
# include "estrutura.h"

// Depende de util.c

// 1) Função auxiliar
// retorna 0 -> x não é um nó adequado
// retorna 1 -> x é adequado e só tem 1 possibilidade de direção (liberdade ou para cima ou para direita)
// retorna 2 -> x é adequado e só tem 1 possibilidade de direção (liberadade ou para baixo ou para esquerda)
// retorna 3 -> x é adequado e tem 2 possibilidades de direção (liberdade ou para cima/baixo ou para direita/esquerda)
int verificador_no (no *x, int tipo_embarcacao, int verticalidade)
{
    int verificador = 0;

    // tipo_embarcacao == 4 -> jangada
    // tipo_embarcacao == 3 -> submarino
    // tipo_embarcacao == 2 -> fragata
    // tipo_embarcacao == 1 -> destroyers
    // tipo_embarcacao == 0 -> porta_avioes
    if (tipo_embarcacao == 4 || tipo_embarcacao == 3)
        verificador = 1;

    else if (tipo_embarcacao == 2){
        // Caso horizontal
        if (verticalidade == 0){
            if ( x->dir != NULL && x->dir->tipo_embarcacao == agua)
                verificador += 1;
            if ( x->esq != NULL && x->esq->tipo_embarcacao == agua) 
                verificador += 2;
        }
        // Caso vertical
        else{
            if (x->cima != NULL && x->cima->tipo_embarcacao == agua)
                verificador += 1;
            if (x->baixo != NULL && x->baixo->tipo_embarcacao == agua)
                verificador += 2;
        }
    }
    else if (tipo_embarcacao == 1){
        if (verticalidade == 0){
            if ( x->dir != NULL  && x->dir->tipo_embarcacao == agua && x->dir->dir != NULL && x->dir->dir->tipo_embarcacao == agua)
                verificador += 1;

            if ( x->esq != NULL && x->esq->tipo_embarcacao == agua && x->esq->esq != NULL && x->esq->esq->tipo_embarcacao == agua)
                verificador += 2;
        }

        else{
            if ( x->cima != NULL && x->cima->tipo_embarcacao == agua && x->cima->cima != NULL && x->cima->cima->tipo_embarcacao == agua)
                verificador += 1;

            if ( x->baixo != NULL && x->baixo->tipo_embarcacao == agua && x->baixo->baixo != NULL && x->baixo->baixo->tipo_embarcacao == agua)
                verificador += 2;
        }
    }

    else if (tipo_embarcacao == 0){
        if (verticalidade == 0){
            if ( x->dir != NULL && x->dir->tipo_embarcacao == agua && x->dir->dir != NULL && x->dir->dir->tipo_embarcacao == agua)
                if (x->dir->dir->dir != NULL && x->dir->dir->dir->tipo_embarcacao == agua)
                    if (x->dir->dir->dir->dir != NULL && x->dir->dir->dir->dir->tipo_embarcacao == agua)
                        verificador += 1;

            if ( x->esq != NULL && x->esq->tipo_embarcacao == agua && x->esq->esq != NULL && x->esq->esq->tipo_embarcacao == agua)
                if (x->esq->esq->esq != NULL && x->esq->esq->esq->tipo_embarcacao == agua)
                    if (x->esq->esq->esq->esq != NULL && x->esq->esq->esq->esq->tipo_embarcacao == agua)
                        verificador += 2;
        }

        else{
            if ( x->cima != NULL && x->cima->tipo_embarcacao == agua && x->cima->cima != NULL && x->cima->cima->tipo_embarcacao == agua)
                if (x->cima->cima->cima != NULL && x->cima->cima->cima->tipo_embarcacao == agua)
                    if (x->cima->cima->cima->cima != NULL && x->cima->cima->cima->cima->tipo_embarcacao == agua)
                        verificador += 1;

            if ( x->baixo != NULL && x->baixo->tipo_embarcacao == agua && x->baixo->baixo != NULL && x->baixo->baixo->tipo_embarcacao == agua)
                if (x->baixo->baixo->baixo != NULL && x->baixo->baixo->baixo->tipo_embarcacao == agua)
                    if (x->baixo->baixo->baixo->baixo != NULL && x->baixo->baixo->baixo->baixo->tipo_embarcacao == agua)
                        verificador += 2;
        }
    }

    // verificador == 0 -> x não é um nó adequado
    // verificador == 1 -> x é adequado e só tem 1 possibilidade de direção
    // verificador == 2 -> x é adequado e só tem 1 possibilidade de direção
    // verificador == 3 -> x é adequado e tem 2 possibilidades de direção
    return verificador;
}


// 2) Funções principais
// 2.1) Criar_Tabuleiro
// Cria um tabuleiro do zero, mas ainda não inicializa nenhuma embarcação
tabuleiro *Criar_Tabuleiro()
{
    tabuleiro *T;
    no *vetor[12][12];
    int i = 0, j;

    // Usamos uma matriz para inicializar os nós
    while (i < 12)
    {
        j = 0;
        while (j < 12)
        {
            vetor[i][j] = malloc(sizeof(no));
            vetor[i][j]->tipo_embarcacao = agua;
            vetor[i][j]->direcao = '0';
            vetor[i][j]->grafico = grafico_vazio;
            vetor[i][j]->grafico_original = grafico_vazio;
            vetor[i][j]->tiro = NAO;
            vetor[i][j]->destruicao_total = '0';
            vetor[i][j]->vertical = NAO;
            vetor[i][j]->num_aux = '0';
            j += 1;
        }
        i += 1;
    }

    i = 0;

    // Agora iremos interligar os nós entre si
    while (i < 12)
    {
        j = 0;
        while (j < 12)
        {
            /*
            A matriz pode ser vista dessa forma
            [0][0]  [0][1]  [0][2] ... [0][11]
            [1][0]  [1][1]  [1][2] ... [1][11]
                        .
                        .
                        .
            [10][0] [10][1] [10][2] ... [10][11]
            [11][0] [11][1] [11][2] ... [11][11]
            */
            if (j < 11)
                vetor[i][j]->dir = vetor[i][j+1];
            else
                vetor[i][11]->dir = NULL;


            if (j > 0)
                vetor[i][j]->esq = vetor[i][j-1];
            else
                vetor[i][0]->esq = NULL;


            if (i > 0)
                vetor[i][j]->cima = vetor[i-1][j];
            else
                vetor[0][j]->cima = NULL;
            

            if (i < 11)
                vetor[i][j]->baixo = vetor[i+1][j];
            else
                vetor[11][j]->baixo = NULL;
            
            j += 1;
        }
        i += 1;
    }

    // Inicializamos o tabuleiro em si
    T = malloc(sizeof(tabuleiro));
    T->prim = vetor[0][0];
    T->num_porta_avioes = '1';
    T->num_destroyers = '2';
    T->num_fragatas = '3';
    T->num_submarinos = '2';
    T->num_jangada = '1';
    T->perdeu = NAO;
    
    return T;
}


// 2.2) Colocar_as_pecas_Tabuleiro
// Dispõe as pecas do jogador no tabuleiro de forma aleatória
void Colocar_as_pecas_Tabuleiro (tabuleiro *T)
{
    char saida;
    int tipo_embarcacao, verticalidade, aux, passos_dir, passos_baixo, direcao;
    int qtd_porta_avioes, qtd_destroyers, qtd_fragatas, qtd_submarinos, qtd_jangada; 
    int i, j;
    no *x;
    qtd_porta_avioes = qtd_destroyers = qtd_fragatas = qtd_submarinos = qtd_jangada = 0;
    i = j = 0;

    while (j < 9){
        j += 1;
        i = 0;
        saida = NAO;
        // 1- Escolhemos o tipo de embarcação
        do{
            tipo_embarcacao = randRange(5);

            if (tipo_embarcacao == 0 && qtd_porta_avioes == 0){
                qtd_porta_avioes = 1;
                saida = SIM;
            }
            else if (tipo_embarcacao == 1 && qtd_destroyers < 2){
                qtd_destroyers += 1;
                saida = SIM;
            }
            else if (tipo_embarcacao == 2 && qtd_fragatas < 3){
                qtd_fragatas += 1;
                saida = SIM;
            }
            else if (tipo_embarcacao == 3 && qtd_submarinos < 2){
                qtd_submarinos += 1;
                saida = SIM;
            }
            else if (tipo_embarcacao == 4 && qtd_jangada == 0){
                qtd_jangada = 1;
                saida = SIM;
            }
        } while (saida == NAO);

        saida = NAO;
        // 2- Escolhemos a verticalidade: 0 para horizontal e 1 para vertical
        verticalidade = randRange(2);

        // 3- Escolhemos a posição de uma extremidade
        do{
            passos_dir = randRange(12);
            passos_baixo = randRange(12);
            x = Deslocamento(T->prim, passos_dir, passos_baixo);
            aux = verificador_no(x, tipo_embarcacao, verticalidade);
            if ( x->tipo_embarcacao == agua && aux != 0 ){
                x->vertical = 48 + verticalidade;
                saida = SIM;
            }
        } while (saida == NAO);

        // 4- Escolhemos a direção (para o caso do nó x ter liberdade para 2 direções)
        direcao = randRange(2);

        // 5- Dado tipo de embarcação, verticalidade, aux e direcao, iremos colocar toda a embarcação no tabuleiro
        if (tipo_embarcacao == 4){
            // Nesse caso, a embarcação só tem 1 casa.
            x->tipo_embarcacao = jangada;
            x->grafico = x->grafico_original = graf_jangada;
        }
        else if (tipo_embarcacao == 3){
            // Nesse caso, a embarcação só tem 1 casa.
            x->tipo_embarcacao = submarino;
            x->grafico = x->grafico_original = graf_submarino;
        }
        else if (tipo_embarcacao == 2){
            // Nesse caso, a embarcação tem 2 casas.
            x->tipo_embarcacao = fragata;
            // Liberdade ou para cima ou para direita
            if ((aux == 1) || (aux == 3 && direcao == 0)){
                // Caso horizontal
                if (verticalidade == 0){
                    x->grafico = x->grafico_original = graf_extremidade_esq;

                    x->dir->tipo_embarcacao = fragata;
                    x->dir->vertical = '0';
                    x->dir->grafico = x->dir->grafico_original = graf_extremidade_dir;
                }
                // Caso vertical
                else{
                    x->grafico = x->grafico_original = graf_extremidade_inf;

                    x->cima->tipo_embarcacao = fragata;
                    x->cima->vertical = '1';
                    x->cima->grafico = x->cima->grafico_original = graf_extremidade_sup;
                }
            }

            // Liberdade ou para baixo ou para esquerda
            else if ( (aux == 2) || (aux == 3 && direcao == 1) ){
                // Caso horizontal
                if (verticalidade == 0){
                    x->grafico = x->grafico_original = graf_extremidade_dir;

                    x->esq->tipo_embarcacao = fragata;
                    x->esq->vertical = '0';
                    x->esq->grafico = x->esq->grafico_original = graf_extremidade_esq;
                }
                // Caso vertical
                else{
                    x->grafico = x->grafico_original = graf_extremidade_sup;

                    x->baixo->tipo_embarcacao = fragata;
                    x->baixo->vertical = '1';
                    x->baixo->grafico = x->baixo->grafico_original = graf_extremidade_inf;
                }
            }
        }
        else if (tipo_embarcacao == 1){
            x->tipo_embarcacao = destroyer;
            // Liberdade ou para cima ou para direita
            if ( (aux == 1) || (aux == 3 && direcao == 0) ){
                // Caso horizontal
                if (verticalidade == 0){
                    x->grafico = x->grafico_original = graf_extremidade_esq;
                    while (i < 2){
                        x = x->dir;
                        x->tipo_embarcacao = destroyer;
                        x->vertical = '0';
                        if (i==0)
                            x->grafico = x->grafico_original = corpo_principal;
                        else
                            x->grafico = x->grafico_original = graf_extremidade_dir;
                        i += 1;
                    }
                }
                // Caso vertical
                else{
                    x->grafico = x->grafico_original = graf_extremidade_inf;
                    while (i < 2){
                        x = x->cima;
                        x->tipo_embarcacao = destroyer;
                        x->vertical = '1';
                        if (i==0)
                            x->grafico = x->grafico_original = corpo_principal;
                        else
                            x->grafico = x->grafico_original = graf_extremidade_sup;
                        i += 1;
                    }
                }
            }
            // Liberdade ou para baixo ou para esquerda
            else if ( (aux == 2) || (aux == 3 && direcao == 1 ) ){
                // Caso horizontal
                if (verticalidade == 0){
                    x->grafico = x->grafico_original = graf_extremidade_dir;

                    while (i < 2){
                        x = x->esq;
                        x->tipo_embarcacao = destroyer;
                        x->vertical = '0';
                        if (i==0)
                            x->grafico = x->grafico_original = corpo_principal;
                        else
                            x->grafico = x->grafico_original = graf_extremidade_esq;
                        i += 1;
                    }
                }
                // Caso vertical
                else{
                    x->grafico = x->grafico_original = graf_extremidade_sup;
                    while (i < 2){
                        x = x->baixo;
                        x->tipo_embarcacao = destroyer;
                        x->vertical = '1';
                        if (i==0)
                            x->grafico = x->grafico_original = corpo_principal;
                        else
                            x->grafico = x->grafico_original = graf_extremidade_inf;
                        i += 1;
                    }
                }
            }
        }
        else if (tipo_embarcacao == 0){
            x->tipo_embarcacao = porta_avioes;
            x->num_aux = '0';
            // Liberdade ou para cima ou para direita
            if ( (aux == 1) || (aux == 3 && direcao == 0)){
                x->direcao = '0';
                // Caso horizontal
                if (verticalidade == 0){
                    x->grafico = x->grafico_original = graf_extremidade_esq;
                    while (i < 4){
                        x = x->dir;
                        x->tipo_embarcacao = porta_avioes;
                        x->vertical = '0';
                        x->num_aux = 49 + i;
                        x->direcao = '0';
                        if (i < 3)
                            x->grafico = x->grafico_original = corpo_principal;
                        else
                            x->grafico = x->grafico_original = graf_extremidade_dir;
                        i += 1;
                    }      
                }
                // Caso vertical
                else{
                    x->grafico = x->grafico_original = graf_extremidade_inf;
                    while (i < 4){
                        x = x->cima;
                        x->tipo_embarcacao = porta_avioes;
                        x->vertical = '1';
                        x->num_aux = 49 + i;
                        x->direcao = '0';
                        if (i < 3)
                            x->grafico = x->grafico_original = corpo_principal;
                        else
                            x->grafico = x->grafico_original = graf_extremidade_sup;
                        i += 1;
                    }
                }
            }
            // Liberdade ou para baixo ou para esquerda
            else if ( (aux == 2) || (aux == 3 && direcao == 1) ){
                x->direcao = '1';
                // Caso horizontal
                if (verticalidade == 0){
                    x->grafico = x->grafico_original = graf_extremidade_dir;
                    while (i < 4){
                        x = x->esq;
                        x->tipo_embarcacao = porta_avioes;
                        x->vertical = '0';
                        x->num_aux = 49 + i;
                        x->direcao = '1';
                        if (i < 3)
                            x->grafico = x->grafico_original = corpo_principal;
                        else
                            x->grafico = x->grafico_original = graf_extremidade_esq;
                        i += 1;
                    }
                }
                // Caso vertical
                else{
                    x->grafico = x->grafico_original = graf_extremidade_sup;
                    while (i < 4){
                        x = x->baixo;
                        x->tipo_embarcacao = porta_avioes;
                        x->vertical = '1';
                        x->num_aux = 49 + i;
                        x->direcao = '1';
                        if (i < 3)
                            x->grafico = x->grafico_original = corpo_principal;
                        else
                            x->grafico = x->grafico_original = graf_extremidade_inf;
                        i += 1;
                    }
                }
            }
        }
    }
}


// 2.3) Libera o espaço do tabuleiro
void Liberando_espaco (tabuleiro *T)
{
    no *x, *aux;
    int i, j;
    x = T->prim;
    i = j = 0;

    while (i < 12){
        j = 0;
        // Se i é par, fazemos um percurso na linha pela direita
        if (i % 2 == 0)
            while (j < 12){
                if (j < 11)
                    aux = x->dir;
                else
                    aux = x->baixo;

                free(x);

                x = aux;
                j += 1;
            }
        // Caso contrário, fazemos um percurso na linha pela esquerda
        else
            while (j < 12){
                if (j < 11)
                    aux = x->esq;
                else
                    aux = x->baixo;

                free(x);

                x = aux;
                j += 1;
            }
        i += 1;
    }
    free(T);
}


// 2.4) Coloca o Tabuleiro na mesma situação que o Tabuleiro fica após o Criar_Tabuleiro().
void Reiniciar_Tabuleiro (tabuleiro *T)
{
    no *x, *aux;
    int i, j;

    i = j = 0;
    x = T->prim;
    T->num_porta_avioes = '1';
    T->num_destroyers = '2';
    T->num_fragatas = '3';
    T->num_submarinos = '2';
    T->num_jangada = '1';
    T->perdeu = NAO;

    // Reiniciamos todos os nós
    while (i < 12){
        j = 0;
        aux = x;
        while (j < 12){
            aux->tipo_embarcacao = agua;
            aux->num_aux = '0';
            aux->grafico = grafico_vazio;
            aux->grafico_original = grafico_vazio;
            aux->tiro = NAO;
            aux->destruicao_total = '0';
            aux->vertical = NAO;
            aux->direcao = '0';

            aux = aux->dir;
            j += 1;
        }
        x = x->baixo;
        i += 1;
    }
}

