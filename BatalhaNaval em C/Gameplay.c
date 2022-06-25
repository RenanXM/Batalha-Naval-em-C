# include <stdio.h>
# include <stdlib.h>
# include "estrutura.h"
// Depende de Jogadas.c, util.c e estrutura.h

// 1) Imprimir_Tabuleiros
// Imprime o tabuleiro do ponto de vista do Humano
void Imprimir_Tabuleiros (tabuleiro *T_hum, tabuleiro *T_comp, int k, char *ult_jog_hum, char *ult_jog_comp)
{
    no *x_hum, *aux_hum, *x_comp, *aux_comp;
    int i, j;

    i = j = 1;
    x_hum = T_hum->prim;
    x_comp = T_comp->prim;
    printf ("       Humano          Computador");
    printf ("\n\n");
    printf ("    ABCDEFGHIJKL      ABCDEFGHIJKL");
    printf ("\n");
    printf ("   +------------+    +------------+");
    printf ("\n");
    while (i <= 12)
    {
        j = 1;
        aux_hum = x_hum;
        aux_comp = x_comp;
        if (i < 10)
            printf ("  %d|", i);
        else
            printf (" %d|", i);
        // Tabuleiro humano
        while (j <= 12)
        {
            // No tabuleiro do humano, mostramos tudo para ele
            printf("%c", aux_hum->grafico);
            aux_hum = aux_hum->dir;
            j += 1;
        }
        printf ("+ ");
        if (i < 10)
            printf ("  %d|", i);
        else
            printf (" %d|", i);

        j = 1;
        // Tabuleiro Computador
        while (j <= 12)
        {
            // Mostramos apenas os espaços atacados
            // Caso o espaço não tenha sido atacado, não mostramos nada
            if (aux_comp->tiro == SIM)
                printf("%c", aux_comp->grafico);
            else
                printf (" ");
            aux_comp = aux_comp->dir;
            j += 1;
        }
        printf ("|\n");
        x_hum = x_hum->baixo;
        x_comp = x_comp->baixo;
        i += 1;
    }
    printf ("   +------------+    +------------+");
    printf ("\n\n");
    if (k == 0){
        printf ("Ultima jogada do computador: /\n");
        printf ("Ultima jogada do humano: /\n\n");
    }
    else if (k == 1){
        printf ("Ultima jogada do computador: /\n");
        printf ("Ultima jogada do humano: %s\n\n", ult_jog_hum);
    }
    else{
        printf ("Ultima jogada do computador: %s\n", ult_jog_comp);
        printf ("Ultima jogada do humano: %s\n\n", ult_jog_hum);
    }
}


// 2) Jogo
// Função que irá executar a gameplay, isto é, receber a jogada do humano, calcular a jogada do computador, verificar as condições 
// de fim de jogo, informar os eventos e mostrar o tabuleiro.
void Jogo(tabuleiro *T_hum, tabuleiro *T_comp)
{
    int i, j, qtd_jog_hum, qtd_jog_comp;
    char aux_comp, exit_humano, exit_comp;
    char jogada_humano[4];
    char jogada_computer[4];
    int computador_dir, computador_baixo;
    int computador_coords;
    no *ultimo_no_atacado;

    jogada_humano[0] = '0'; 
    jogada_computer[3] = '\0';
    qtd_jog_hum = qtd_jog_comp = 0;
    // "i" e "j" são variáveis que servem para verificarmos se o humano e o computador, respectivamente, já jogaram
    // Se i == 0, então o humano ainda não jogou
    // Se j == 0, então o computador ainda não jogou
    i = j = 0;
            
    // O jogo acontece até alguém perder
    while (T_hum->perdeu == NAO && T_comp->perdeu == NAO){
        if (i == 0 && j == 0){
            printf("\n\n            INICIO DO JOGO!\n\n");
            Imprimir_Tabuleiros(T_hum, T_comp, 0, NULL, NULL);
        }

        // Fazemos uma estrutura de repetição para que o humano jogue enquanto ele não acertar a água ou a jangada do computador e
        // enquanto ele e o computador não perdem
        exit_humano = NAO;
        do{
            printf ("\n     SUA JOGADA\n" );
            ultimo_no_atacado = func_jogada_humano(jogada_humano, T_comp, T_hum);

            Atualiza_Tabuleiro(ultimo_no_atacado, T_hum, T_comp, SIM);

            qtd_jog_hum += 1;
            printf ("\nTabuleiro apos a %da jogada do humano: \n\n", qtd_jog_hum);
            if (j == 0)
                Imprimir_Tabuleiros(T_hum, T_comp, 1, jogada_humano, jogada_computer);
            else
                Imprimir_Tabuleiros(T_hum, T_comp, 2, jogada_humano, jogada_computer);
            // Verificamos se o humano irá jogar novamente ou não
            // Possiveis casos:
            if (ultimo_no_atacado->tipo_embarcacao == agua) {
                exit_humano = SIM;
                printf("\nComo voce nao acertou uma embarcacao inimiga, voce perdeu a vez! \n");
            }
            else if (T_hum->perdeu == SIM && T_hum->num_jangada == '0'){
                exit_humano = SIM;
                printf("\nComo voce nao tem mais nenhuma embarcacao, voce perdeu o jogo!\n");
            }
            else if (T_hum->perdeu == SIM && T_hum->num_jangada == '1'){
                exit_humano = SIM;
                printf("\nComo voce nao tem mais nenhuma embarcacao alem da jangada, voce perdeu o jogo!\n");
            }
            else if (T_comp->perdeu == SIM && T_comp->num_jangada == '0'){
                exit_humano = SIM;
                printf("\nComo o computador nao tem mais nenhuma embarcacao, voce ganhou o jogo!\n");
            }
            else if (T_comp->perdeu == SIM && T_comp->num_jangada == '1'){
                exit_humano = SIM;
                printf("\nComo o computador nao tem mais nenhuma embarcacao alem da jangada, voce ganhou o jogo!\n");
            }
            else if(ultimo_no_atacado->tipo_embarcacao == jangada){
                exit_humano = SIM;
                printf("\nComo voce acertou a jangada inimiga, voce perdeu a vez! \n");
            }
            else
                printf("\nComo voce acertou uma embarcacao inimiga que nao era uma jangada, voce continuara jogando! \n");
        }while (exit_humano != SIM);

        i = 1;
        // Verificamos se a partida ainda não terminou
        if (T_hum->perdeu == NAO && T_comp->perdeu == NAO){
            // aux_comp serve como argumento para a função jogada_computador.
            // Se aux_comp == SIM, o computador ou não acertou nenhuma embarcação ou terminou de afundar uma embarcação na rodada passada. 
            // Também enquadramos o caso de essa ser a primeira jogada do computador.

            // Se aux_comp == NAO, o computador acertou uma embarcação e ainda não a afundou na rodada passada.
            aux_comp = SIM;
            exit_comp = NAO;
            do{
                printf ("\n     JOGADA DO COMPUTADOR\n" );
                // computador_dir e computador_baixo servem para guardar as coordenadas do último nó atacado pelo computador
                computador_coords = jogada_computador(aux_comp, computador_dir, computador_baixo, T_hum, T_comp);
                computador_dir = computador_coords % 12;
                computador_baixo = (computador_coords - computador_dir) / 12;

                // Achamos o nó atacado
                ultimo_no_atacado = Deslocamento(T_hum->prim, computador_dir, computador_baixo);

                // Montamos a string que representará a jogada do computador
                jogada_computer[0] = 65 + computador_dir;
                if (computador_baixo >= 10){
                    jogada_computer[1] = '1';
                    jogada_computer[2] = 39 + computador_baixo;
                }
                else if (computador_baixo == 9){
                    jogada_computer[1] = '1';
                    jogada_computer[2] = '0';
                }
                else{
                    jogada_computer[1] = 49 + computador_baixo;
                    jogada_computer[2] = '\0';
                }

                printf ("O computador atacou a posicao %s\n", jogada_computer);
                // Atualizamos o tabuleiro
                Atualiza_Tabuleiro(ultimo_no_atacado, T_hum, T_comp, NAO);

                qtd_jog_comp += 1;
                printf ("\nTabuleiro apos a %da jogada do computador: \n\n", qtd_jog_comp);
                Imprimir_Tabuleiros(T_hum, T_comp, 2, jogada_humano, jogada_computer);
                
                // Verificamos se o computador irá jogar novamente ou não
                if (ultimo_no_atacado->tipo_embarcacao == agua) {
                    exit_comp = SIM;
                    printf("\nComo o computador nao acertou uma embarcacao inimiga, ele perdeu a vez! \n");
                }
                else if (T_comp->perdeu == SIM && T_comp->num_jangada == '0'){
                    exit_comp = SIM;
                    printf("\nComo o computador nao tem mais nenhuma embarcacao, voce ganhou o jogo!\n");
                }
                else if (T_comp->perdeu == SIM && T_comp->num_jangada == '1'){
                    exit_comp = SIM;
                    printf("\nComo o computador nao tem mais nenhuma embarcacao alem da jangada, voce ganhou o jogo!\n");
                }
                else if (T_hum->perdeu == SIM && T_hum->num_jangada == '0'){
                    exit_comp = SIM;
                    printf("\nComo voce nao tem mais nenhuma embarcacao, voce perdeu o jogo!\n");
                }
                else if (T_hum->perdeu == SIM && T_hum->num_jangada == '1'){
                    exit_comp = SIM;
                    printf("\nComo voce nao tem mais nenhuma embarcacao alem da jangada, voce perdeu o jogo!\n");
                }
                else if(ultimo_no_atacado->tipo_embarcacao == jangada){
                    exit_comp = SIM;
                    printf("\nComo o computador acertou a jangada inimiga, ele perdeu a vez! \n");
                }
                // Caso o programa chegue até aqui, então o computador continuará jogando
                else{
                    printf("\nComo o computador acertou uma embarcacao inimiga que nao era uma jangada, ele continuara jogando! \n");
                    if (ultimo_no_atacado->tipo_embarcacao == submarino || (ultimo_no_atacado->tipo_embarcacao == fragata && ultimo_no_atacado->destruicao_total == '2') || (ultimo_no_atacado->tipo_embarcacao == destroyer && ultimo_no_atacado->destruicao_total == '3') || (ultimo_no_atacado->tipo_embarcacao == porta_avioes && ultimo_no_atacado->destruicao_total == '5'))
                        aux_comp = SIM;
                    else
                        aux_comp = NAO;
                }
            }while (exit_comp != SIM);
            j = 1;
        }
    }
}

