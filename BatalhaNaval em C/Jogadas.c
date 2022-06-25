# include <stdio.h>
# include <stdlib.h>
# include <ctype.h>
# include <string.h>
# include "estrutura.h"

// Depende de util.c e estrutura.h

// 1) Função auxiliar
// Retorna 1 se a jogada do humano for válida e 0 caso contrário
char verificar_coordenada (char *coordenada, tabuleiro *computer)
{
    int numeros_qtd = 0, letras_qtd = 0;
    char jogada[7], numero_str[3];
    char letra;
    int numero, i;
    no *tmp;

    // Recebendo coordenadas
    printf("Entre a sua jogada: ");
    scanf(" %[^\n]", jogada);
    jogada[6] = '\0';
    
    for (i = 0; jogada[i] != '\0'; i++){
        if (toupper(jogada[i]) >= 'A' && toupper(jogada[i]) <= 'Z'){
            letra = toupper(jogada[i]);
            letras_qtd++;
        }
        else if (jogada[i] >= '0' && jogada[i] <= '9'){
            numero_str[numeros_qtd] = jogada[i];
            numeros_qtd++;
        }
    }

    // Informação insuficiente ou excessiva
    if (numeros_qtd > 2 || numeros_qtd < 1 || letras_qtd != 1){
        printf("Jogada invalida. Tente novamente!\n");
        return 0;
    }

    // Número inválido 
    if (numero_str[0] == '0'){
        printf("Jogada invalida. Tente novamente!\n");
        return 0;
    }
    if (numero_str[0] > '1' && numeros_qtd > 1){
        printf("Jogada invalida. Tente novamente!\n");
        return 0;
    }
    if (numero_str[0] == '1' && numeros_qtd == 2 && numero_str[1] > '2'){
        printf("Jogada invalida. Tente novamente!\n");
        return 0;
    }

    // Letra inválida
    if (letra > 'L'){
        printf("Jogada invalida. Tente novamente!\n");
        return 0;
    }

    // Consertando a entrada em forma X00 (letra seguida de numero)
    coordenada[0] = letra;
    coordenada[1] = numero_str[0];
    if (numeros_qtd == 2){
        numero_str[2] = '\0';

        coordenada[2] = numero_str[1];
        coordenada[3] = '\0';
    }
    else {
        coordenada[2] = '\0';
        numero_str[1] = '\0';
    }
    
    // Convertendo o número de string para inteiro
    numero = atoi(numero_str) - 1;
    
    // Nó para a posição escolhida
    tmp = Deslocamento(computer->prim, (int) letra - 65, numero);

    // Embarcação ou agua atingida 
    if (tmp->tiro == SIM){
        printf("Jogada invalida. Tente novamente!\n");
        return 0;
    }

    // Valid
    return 1;
}


// 2) Funções principais
// 2.1) Jogada player
// Retorna o ponteiro para o nó da posição escolhida pelo humano
no *func_jogada_humano (char *coordenada, tabuleiro *computer, tabuleiro *jogador)
{
    char valid_play;
    int letra;
    int numero;
    no *node;

    // Verifica se pode jogar em determinada coordenada escolhida
    valid_play = 0;
    while (!valid_play){
        valid_play = verificar_coordenada(coordenada, computer);
    }
    
    // Letra da coordenada 
    letra = coordenada[0] - 65;
    
    // Numero da coordenada
    if (coordenada[2] == '\0'){
        numero = coordenada[1] - 48;
    }
    else if (coordenada[3] == '\0'){
        numero = 10 + coordenada[2] - 48;
    }
    numero--;

    // Nó presente na coordenada validada
    node = Deslocamento(computer->prim, letra, numero);

    return node;
}


// 2.2) Jogada do computador
// Retornamos um inteiro que irá representar a posição do nó atacado por ultimo pelo computador
int jogada_computador (char aux, int dir_ult_no_atac, int baixo_ult_no_atac, tabuleiro *T_humano, tabuleiro *T_computador)
{
    no *escolha, *ultimo_no_atacado;
    int dir, baixo, sorteio;
    char exit;
    exit = NAO;
    // Se aux == SIM, o computador ou não acertou nenhuma embarcação ou terminou de afundar uma embarcação na rodada passada. 
    // Também enquadramos o caso de essa ser a primeira jogada do computador. O computador deve atacar uma posição aleatoriamente, 
    // que ainda não tiver sido atacada
    if (aux == SIM)
        do{
            dir = randRange(12);
            baixo = randRange(12);
            escolha = Deslocamento(T_humano->prim, dir, baixo);

            if (escolha->tiro == NAO)
                exit = SIM;
        } while (exit == NAO);
    // Se aux == NAO, o computador acertou uma embarcação e ainda não a afundou na rodada passada.
    // O computador deve tentar prosseguir nas casas adjacentes até terminar de afundá-la.
    else{
        // Observe que a embarcação atacada só pode ser ou fragata ou destroyer ou porta_avioes
        ultimo_no_atacado = Deslocamento(T_humano->prim, dir_ult_no_atac, baixo_ult_no_atac);
        do{
            // Fazemos um sorteio para decidir que direção iremos atacar
            sorteio = randRange(4);
            if (sorteio == 0 && ultimo_no_atacado->cima != NULL && ultimo_no_atacado->cima->tiro == NAO){
                dir = dir_ult_no_atac;
                baixo = baixo_ult_no_atac - 1;
                exit = SIM;
            }
            else if(sorteio == 1 && ultimo_no_atacado->dir != NULL && ultimo_no_atacado->dir->tiro == NAO){
                dir = dir_ult_no_atac + 1;
                baixo = baixo_ult_no_atac;
                exit = SIM;
            }
            else if(sorteio == 2 && ultimo_no_atacado->baixo != NULL && ultimo_no_atacado->baixo->tiro == NAO){
                dir = dir_ult_no_atac;
                baixo = baixo_ult_no_atac + 1;
                exit = SIM;
            }
            else if(sorteio == 3 && ultimo_no_atacado->esq != NULL && ultimo_no_atacado->esq->tiro == NAO){
                dir = dir_ult_no_atac - 1;
                baixo = baixo_ult_no_atac;
                exit = SIM;
            }
            else {
                // Verificamos se o nó está "ilhado", isto é, se todos os nós adjacentes ao último nó atacado já foram atingidos
                // Nesse caso, podemos ir para direito e para baixo
                if (dir_ult_no_atac == 0 && baixo_ult_no_atac == 0){
                    if (ultimo_no_atacado->baixo->tiro == SIM && ultimo_no_atacado->dir->tiro == SIM){
                        sorteio = randRange(2);
                        if (sorteio == 0){
                            ultimo_no_atacado = ultimo_no_atacado->baixo;
                            baixo_ult_no_atac += 1;
                        }
                        else{
                            ultimo_no_atacado = ultimo_no_atacado->dir;
                            dir_ult_no_atac += 1;
                        }
                    }
                }
                // Nesse caso, podemos ir para a direita e para cima
                else if(dir_ult_no_atac == 0 && baixo_ult_no_atac == 11){
                    if (ultimo_no_atacado->dir->tiro == SIM && ultimo_no_atacado->cima->tiro == SIM){
                        sorteio = randRange(2);
                        if (sorteio == 0){
                            ultimo_no_atacado = ultimo_no_atacado->dir;
                            dir_ult_no_atac += 1;
                        }
                        else{
                            ultimo_no_atacado = ultimo_no_atacado->cima;
                            baixo_ult_no_atac -= 1;
                        }
                    }
                }
                // Nesse caso, podemos ir para a esquerda e para baixo
                else if(dir_ult_no_atac == 11 && baixo_ult_no_atac == 0){
                    if (ultimo_no_atacado->baixo->tiro == SIM && ultimo_no_atacado->esq->tiro == SIM){
                        sorteio = randRange(2);
                        if (sorteio == 0){
                            ultimo_no_atacado = ultimo_no_atacado->baixo;
                            baixo_ult_no_atac += 1;
                        }
                        else{
                            ultimo_no_atacado = ultimo_no_atacado->esq;
                            dir_ult_no_atac -= 1;
                        }
                    }
                }
                // Nesse caso, podemos ir para a esquerda e para cima
                else if (dir_ult_no_atac == 11 && baixo_ult_no_atac == 11){
                    if (ultimo_no_atacado->esq->tiro == SIM && ultimo_no_atacado->cima->tiro == SIM){
                        sorteio = randRange(2);
                        if (sorteio == 0){
                            ultimo_no_atacado = ultimo_no_atacado->esq;
                            dir_ult_no_atac -= 1;
                        }
                        else{
                            ultimo_no_atacado = ultimo_no_atacado->cima;
                            baixo_ult_no_atac -= 1;
                        }
                    }                   
                }
                // Nesse caso, podemos ir para baixo, cima e direita
                else if (dir_ult_no_atac == 0 && baixo_ult_no_atac < 11){
                    if (ultimo_no_atacado->cima->tiro == SIM && ultimo_no_atacado->baixo->tiro == SIM && ultimo_no_atacado->dir->tiro == SIM){
                        sorteio = randRange(3);
                        if (sorteio == 0){
                            ultimo_no_atacado = ultimo_no_atacado->cima;
                            baixo_ult_no_atac -= 1;
                        }
                        else if (sorteio == 1){
                            ultimo_no_atacado = ultimo_no_atacado->dir;
                            dir_ult_no_atac += 1;
                        }
                        else{
                            ultimo_no_atacado = ultimo_no_atacado->baixo;
                            baixo_ult_no_atac += 1;
                        }
                    }
                }
                // Nesse caso, podemos ir para baixo, direita e esquerda
                else if (dir_ult_no_atac < 11 && baixo_ult_no_atac == 0){
                    if (ultimo_no_atacado->baixo->tiro == SIM && ultimo_no_atacado->esq->tiro == SIM && ultimo_no_atacado->dir->tiro == SIM){
                        sorteio = randRange(3);
                        if (sorteio == 0){
                            ultimo_no_atacado = ultimo_no_atacado->baixo;
                            baixo_ult_no_atac += 1;
                        }
                        else if (sorteio == 1){
                            ultimo_no_atacado = ultimo_no_atacado->dir;
                            dir_ult_no_atac += 1;
                        }
                        else{
                            ultimo_no_atacado = ultimo_no_atacado->esq;
                            dir_ult_no_atac -= 1;
                        }
                    }
                }
                // Nesse caso, podemos ir para a direita, cima e esquerda
                else if (dir_ult_no_atac < 11 && baixo_ult_no_atac == 11){
                    if (ultimo_no_atacado->cima->tiro == SIM && ultimo_no_atacado->esq->tiro == SIM && ultimo_no_atacado->dir->tiro == SIM){
                        sorteio = randRange(3);
                        if (sorteio == 0){
                            ultimo_no_atacado = ultimo_no_atacado->cima;
                            baixo_ult_no_atac -= 1;
                        }
                        else if (sorteio == 1){
                            ultimo_no_atacado = ultimo_no_atacado->dir;
                            dir_ult_no_atac += 1;
                        }
                        else{
                            ultimo_no_atacado = ultimo_no_atacado->esq;
                            dir_ult_no_atac -= 1;
                        }
                    }
                }
                // Nesse caso, podemos ir para baixo, esquerda e cima
                else if (dir_ult_no_atac == 11 && baixo_ult_no_atac < 11){
                    if (ultimo_no_atacado->cima->tiro == SIM && ultimo_no_atacado->esq->tiro == SIM && ultimo_no_atacado->baixo->tiro == SIM){
                        sorteio = randRange(3);
                        if (sorteio == 0){
                            ultimo_no_atacado = ultimo_no_atacado->cima;
                            baixo_ult_no_atac -= 1;
                        }
                        else if (sorteio == 1){
                            ultimo_no_atacado = ultimo_no_atacado->baixo;
                            baixo_ult_no_atac += 1;
                        }
                        else{
                            ultimo_no_atacado = ultimo_no_atacado->esq;
                            dir_ult_no_atac -= 1;
                        }
                    }
                }
                // Nesse caso, podemos ir para cima, baixo, esquerda e direita
                else if (dir_ult_no_atac < 11 && baixo_ult_no_atac < 11){
                    if (ultimo_no_atacado->cima->tiro == SIM && ultimo_no_atacado->esq->tiro == SIM && ultimo_no_atacado->baixo->tiro == SIM && ultimo_no_atacado->dir->tiro == SIM){
                        sorteio = randRange(4);
                        if (sorteio == 0){
                            ultimo_no_atacado = ultimo_no_atacado->cima;
                            baixo_ult_no_atac -= 1;
                        }
                        else if (sorteio == 1){
                            ultimo_no_atacado = ultimo_no_atacado->dir;
                            dir_ult_no_atac += 1;
                        }
                        else if (sorteio == 2){
                            ultimo_no_atacado = ultimo_no_atacado->baixo;
                            baixo_ult_no_atac += 1;
                        }
                        else{
                            ultimo_no_atacado = ultimo_no_atacado->esq;
                            dir_ult_no_atac -= 1;
                        }
                    }
                }
            } 
        } while(exit == NAO);
    }
    return (dir + 12 * baixo);
}


// 2.3) Atualiza_Tabuleiro 
// Recebe o último nó atacado e realiza as mudanças necessárias tanto nos nós quanto no tabuleiro em si
void Atualiza_Tabuleiro (no *no_atacado, tabuleiro *T_humano, tabuleiro *T_computador, char humano)
{
    // Se humano == SIM, foi o humano que atacou
    // Se humano == NAO, foi o computador que atacou
    no *aux_1, *aux_2, *copia;
    int sorteio, num_1, num_2, i, j;

    no_atacado->tiro = SIM;
    no_atacado->destruicao_total += 1;
    if (no_atacado->tipo_embarcacao == agua){
        no_atacado->grafico = agua_atacada;
        if (humano == SIM)
            printf("\nVoce acertou a agua!\n");
        else
            printf("\nO computador acertou a agua!\n");
    }
    else if (no_atacado->tipo_embarcacao == jangada){
        if (humano == SIM){
            printf ("\nVoce acabou de afundar a jangada do computador!\n");
            T_computador->num_jangada = '0';
            aux_1 = aux_2 = T_humano->prim;
        }
        else{
            printf ("\nO computador acabou de afundar a sua jangada!\n");
            T_humano->num_jangada = '0';
            no_atacado->grafico = embarcacao_atingida;
            aux_1 = aux_2 = T_computador->prim;
        }

        if ( (humano == SIM && T_humano->num_submarinos != '0') || (humano == NAO && T_computador->num_submarinos != '0') ){
            // Agora iremos retirar um submarino do atacante caso o atacante tenha ao menos 1 submarino
            sorteio = randRange(2);
            if (sorteio == 0){ 
                while (aux_1->tipo_embarcacao != submarino || (aux_1->tipo_embarcacao == submarino && aux_1->tiro == SIM) ){
                    aux_1 = aux_1->dir;
                    if (aux_1 == NULL){
                        aux_2 = aux_2->baixo;
                        aux_1 = aux_2;
                    }
                }
            }
            else{
                while (aux_1->tipo_embarcacao != submarino || (aux_1->tipo_embarcacao == submarino && aux_1->tiro == SIM) ){
                    aux_1 = aux_1->baixo;
                    if (aux_1 == NULL){
                        aux_2 = aux_2->dir;
                        aux_1 = aux_2;
                    }
                }
            }
            aux_1->tiro = SIM;
            aux_1->destruicao_total += 1;
            if (humano == SIM){
                printf("\nComo voce afundou a jangada do computador e voce ainda tem pelo menos 1 submarino, voce perdeu um submarino!\n");
                aux_1->grafico = embarcacao_atingida;
                T_humano->num_submarinos -= 1;
            }
            else{
                printf("\nComo o computador afundou a sua jangada e ele ainda tem pelo menos 1 submarino, o computador perdeu um submarino!\n");
                T_computador->num_submarinos -= 1;
            }
        }
    }
    else if (no_atacado->tipo_embarcacao == submarino){
        if (humano == SIM){
            T_computador->num_submarinos -= 1;
            printf ("\nVoce acabou de afundar um submarino do computador!\n");
        }
        else{
            printf ("\nO computador acabou de afundar um submarino seu!\n");
            T_humano->num_submarinos -= 1;
            no_atacado->grafico = embarcacao_atingida;
        }
    }
    else if (no_atacado->tipo_embarcacao == fragata){
        if (no_atacado->destruicao_total == '1')
            no_atacado->grafico = embarcacao_atingida;
        else{
            if (humano == SIM){
                printf ("\nVoce acabou de afundar uma fragata do computador!\n");
                T_computador->num_fragatas -= 1;
                no_atacado->grafico = no_atacado->grafico_original;
            }
            else{
                printf ("\nO computador acabou de afundar uma fragata sua!\n");
                T_humano->num_fragatas -= 1;
                no_atacado->grafico = embarcacao_atingida;
            }
        }
        // Verificamos qual parte da embarcação foi acertada e, com base nisso, fazemos as alterações necessárias
        if (no_atacado->grafico_original == graf_extremidade_sup){
            no_atacado->baixo->destruicao_total += 1;
            if (humano == SIM && no_atacado->baixo->destruicao_total == '2')
                no_atacado->baixo->grafico = graf_extremidade_inf;
            else
                no_atacado->baixo->grafico = embarcacao_atingida;
        }
        else if (no_atacado->grafico_original == graf_extremidade_inf){
            no_atacado->cima->destruicao_total += 1;
            if (humano == SIM && no_atacado->cima->destruicao_total == '2')
                no_atacado->cima->grafico = graf_extremidade_sup;
            else
                no_atacado->cima->grafico = embarcacao_atingida;
        }

        else if (no_atacado->grafico_original == graf_extremidade_dir){
            no_atacado->esq->destruicao_total += 1;
            if (humano == SIM && no_atacado->esq->destruicao_total == '2')
                no_atacado->esq->grafico = graf_extremidade_esq;
            else
                no_atacado->esq->grafico = embarcacao_atingida;
        }
        else{
            no_atacado->dir->destruicao_total += 1;
            if (humano == SIM && no_atacado->dir->destruicao_total == '2')
                no_atacado->dir->grafico = graf_extremidade_dir;
            else
                no_atacado->dir->grafico = embarcacao_atingida;
        }
    }
    else if (no_atacado->tipo_embarcacao == destroyer){
        if (humano == SIM){
            if (no_atacado->destruicao_total == '3'){
                printf ("\nVoce acabou de afundar um destroyer do computador!\n");
                T_computador->num_destroyers -= 1;
                no_atacado->grafico = no_atacado->grafico_original;
            }
            else
                no_atacado->grafico = embarcacao_atingida;
        }
        else{
            no_atacado->grafico = embarcacao_atingida;
            if (no_atacado->destruicao_total == '3'){
                printf ("\nO computador acabou de afundar um destroyer seu!\n");
                T_humano->num_destroyers -= 1;
            }
        }

        // Verificamos qual parte da embarcação foi acertada e, com base nisso, fazemos as alterações necessárias
        if (no_atacado->grafico_original == graf_extremidade_dir){
            no_atacado->esq->destruicao_total += 1;
            no_atacado->esq->esq->destruicao_total += 1;
            if (humano == SIM && no_atacado->destruicao_total == '3'){
                no_atacado->esq->grafico = no_atacado->esq->grafico_original;
                no_atacado->esq->esq->grafico = no_atacado->esq->esq->grafico_original;
            }
            else{
                no_atacado->esq->grafico = embarcacao_atingida;
                no_atacado->esq->esq->grafico = embarcacao_atingida;
            }
        }
        else if (no_atacado->grafico_original == graf_extremidade_esq){
            no_atacado->dir->destruicao_total += 1;
            no_atacado->dir->dir->destruicao_total += 1;
            if (humano == SIM && no_atacado->destruicao_total == '3'){
                no_atacado->dir->grafico = no_atacado->dir->grafico_original;
                no_atacado->dir->dir->grafico = no_atacado->dir->dir->grafico_original;
            }
            else{
                no_atacado->dir->grafico = embarcacao_atingida;
                no_atacado->dir->dir->grafico = embarcacao_atingida;
            }
        }
        else if (no_atacado->grafico_original == graf_extremidade_sup){
            no_atacado->baixo->destruicao_total += 1;
            no_atacado->baixo->baixo->destruicao_total += 1;
            if (humano == SIM && no_atacado->destruicao_total == '3'){
                no_atacado->baixo->grafico = no_atacado->baixo->grafico_original;
                no_atacado->baixo->baixo->grafico = no_atacado->baixo->baixo->grafico_original;
            }
            else{
                no_atacado->baixo->grafico = embarcacao_atingida;
                no_atacado->baixo->baixo->grafico = embarcacao_atingida;
            }
        }
        else if (no_atacado->grafico_original == graf_extremidade_inf){
            no_atacado->cima->destruicao_total += 1;
            no_atacado->cima->cima->destruicao_total += 1;
            if (humano == SIM && no_atacado->destruicao_total == '3'){
                no_atacado->cima->grafico = no_atacado->cima->grafico_original;
                no_atacado->cima->cima->grafico = no_atacado->cima->cima->grafico_original;
            }
            else{
                no_atacado->cima->grafico = embarcacao_atingida;
                no_atacado->cima->cima->grafico = embarcacao_atingida;
            }
        }
        else if (no_atacado->grafico_original == corpo_principal){
            if (no_atacado->vertical == SIM){
                no_atacado->cima->destruicao_total += 1;
                no_atacado->baixo->destruicao_total += 1;
                if (humano == SIM && no_atacado->destruicao_total == '3'){
                    no_atacado->cima->grafico = no_atacado->cima->grafico_original;
                    no_atacado->baixo->grafico = no_atacado->baixo->grafico_original;
                }
                else{
                    no_atacado->cima->grafico = embarcacao_atingida;
                    no_atacado->baixo->grafico = embarcacao_atingida;
                }
            }
            else{
                no_atacado->dir->destruicao_total += 1;
                no_atacado->esq->destruicao_total += 1;
                if (humano == SIM && no_atacado->destruicao_total == '3'){
                    no_atacado->dir->grafico = no_atacado->dir->grafico_original;
                    no_atacado->esq->grafico = no_atacado->esq->grafico_original;
                }
                else{
                    no_atacado->dir->grafico = embarcacao_atingida;
                    no_atacado->esq->grafico = embarcacao_atingida;
                }
            }
        }
    }
    else if (no_atacado->tipo_embarcacao == porta_avioes){
        copia = no_atacado;
        if (humano == SIM){
            if (no_atacado->destruicao_total == '5'){
                printf ("\nVoce acabou de afundar o porta avioes do computador!\n");
                T_computador->num_porta_avioes = '0';
                no_atacado->grafico = no_atacado->grafico_original;
            }
            else
                no_atacado->grafico = embarcacao_atingida;
        }
        else{
            no_atacado->grafico = embarcacao_atingida;
            if (no_atacado->destruicao_total == '5'){
                printf ("\nO computador acabou de afundar o seu porta avioes!\n");
                T_humano->num_porta_avioes = '0';
            }
        }

        // Verificamos qual parte da embarcação foi acertada e, com base nisso, fazemos as alterações necessárias
        num_1 = (int) no_atacado->num_aux - 48;
        num_2 = 4 - num_1;
        i = j = 0;
        // Horizontal na direita
        if (no_atacado->vertical == '0' && no_atacado->direcao == '0'){
            // num_1 indica quantas casas há na esquerda de no_atacado
            // num_2 indica quantas casas há na direita de no_atacado
            while (i < num_1){
                no_atacado = no_atacado->esq;
                no_atacado->destruicao_total += 1;
                if (humano == SIM && no_atacado->destruicao_total == '5')
                    no_atacado->grafico = no_atacado->grafico_original;
                else
                    no_atacado->grafico = embarcacao_atingida;
                i += 1;
            }
            no_atacado = copia;
            while (j < num_2){
                no_atacado = no_atacado->dir;
                no_atacado->destruicao_total += 1;
                if (humano == SIM && no_atacado->destruicao_total == '5')
                    no_atacado->grafico = no_atacado->grafico_original;
                else
                    no_atacado->grafico = embarcacao_atingida;
                j += 1;
            }
        } // Horizontal na esquerda
        else if(no_atacado->vertical == '0' && no_atacado->direcao == '1'){
            // num_1 indica quantas casas há na direita de no_atacado
            // num_2 indica quantas casas há na esquerda de no_atacado
            while (i < num_1){
                no_atacado = no_atacado->dir;
                no_atacado->destruicao_total += 1;
                if (humano == SIM && no_atacado->destruicao_total == '5')
                    no_atacado->grafico = no_atacado->grafico_original;
                else
                    no_atacado->grafico = embarcacao_atingida;
                i += 1;
            }
            no_atacado = copia;
            while (j < num_2){
                no_atacado = no_atacado->esq;
                no_atacado->destruicao_total += 1;
                if (humano == SIM && no_atacado->destruicao_total == '5')
                    no_atacado->grafico = no_atacado->grafico_original;
                else
                    no_atacado->grafico = embarcacao_atingida;                    
                j += 1;
            }
        } // Vertical para cima
        else if(no_atacado->vertical == '1' && no_atacado->direcao == '0'){
            // num_1 indica quantas casas há abaixo de no_atacado
            // num_2 indica quantas casas há acima de no_atacado
            while (i < num_1){
                no_atacado = no_atacado->baixo;
                no_atacado->destruicao_total += 1;
                if (humano == SIM && no_atacado->destruicao_total == '5')
                    no_atacado->grafico = no_atacado->grafico_original;
                else
                    no_atacado->grafico = embarcacao_atingida;
                i += 1;
            }
            no_atacado = copia;
            while (j < num_2){
                no_atacado = no_atacado->cima;
                no_atacado->destruicao_total += 1;
                if (humano == SIM && no_atacado->destruicao_total == '5')
                    no_atacado->grafico = no_atacado->grafico_original;
                else
                    no_atacado->grafico = embarcacao_atingida;
                j += 1;
            }
        } // Vertical para baixo
        else{
            // num_1 indica quantas casas há acima de no_atacado
            // num_2 indica quantas casas há abaixo de no_atacado
            while (i < num_1){
                no_atacado = no_atacado->cima;
                no_atacado->destruicao_total += 1;
                if (humano == SIM && no_atacado->destruicao_total == '5')
                    no_atacado->grafico = no_atacado->grafico_original;
                else
                    no_atacado->grafico = embarcacao_atingida;
                i += 1;
            }
            no_atacado = copia;
            while (j < num_2){
                no_atacado = no_atacado->baixo;
                no_atacado->destruicao_total += 1;
                if (humano == SIM && no_atacado->destruicao_total == '5')
                    no_atacado->grafico = no_atacado->grafico_original;
                else
                    no_atacado->grafico = embarcacao_atingida;
                j += 1;
            }
        }
    }

    // Agora verificamos se, após o ataque, alguém perdeu
    if (T_computador->num_submarinos == '0' && T_computador->num_fragatas == '0' && T_computador->num_destroyers == '0' && T_computador->num_porta_avioes == '0')
        T_computador->perdeu = SIM;

    if (T_humano->num_submarinos == '0' && T_humano->num_fragatas == '0' && T_humano->num_destroyers == '0' && T_humano->num_porta_avioes == '0')
        T_humano->perdeu = SIM;
}

