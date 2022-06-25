# include <stdio.h>
# include <stdlib.h>
# include "./estrutura.h"
/* 
Tiago Brandão Forte - 509298
1) Participou da estrutura.h, Tabuleiro.c, Gameplay.c, Jogadas.c, util.c e main.c
2) Gravou sobre util.c, Atualiza_Tabuleiro, jogada_computador e a parte da gameplay em si


Renan Xerez Marques - 508682
1) Participou da estrutura.h, Tabuleiro.c, Gameplay.c, Jogadas.c, util.c e main.c
2) Gravou sobre estrutura.h e Tabuleiro.c


Gustavo Wendell Moreira Valença - 508016
1) Participou da estrutura.h, Tabuleiro.c, Gameplay.c, Jogadas.c e main.c
2) Gravou sobre jogada_humano, verificar_coordenada, Jogo, main.c e a parte da gameplay em si
*/
int main(){
    tabuleiro *T_humano, *T_computador;
    char exit, resposta;
    int vitorias_humano, vitorias_computador;
    T_humano = Criar_Tabuleiro();
    T_computador = Criar_Tabuleiro();
    vitorias_humano = vitorias_computador = 0;
    exit = NAO;
    do{
        Colocar_as_pecas_Tabuleiro(T_humano);
        Colocar_as_pecas_Tabuleiro(T_computador);

        Jogo(T_humano, T_computador);
        // Ao chamarmos a função Jogo, só sairemos quando ou o humano ou o computador perder
        if (T_computador->perdeu == SIM){
            vitorias_humano += 1;
            printf ("O jogo acabou!\nVoce ganhou! Parabens!\n\n");
        }
        else{
            vitorias_computador += 1;
            printf ("O jogo acabou!\nVoce perdeu! Jogue melhor na proxima!\n\n");
        }
        printf("\nPLACAR:\n");
        printf("Voce: %d\n", vitorias_humano);
        printf("Computador: %d\n\n", vitorias_computador);

        printf("\nDeseja continuar jogando? \n1 para sim\n0 para nao\nSua opcao: ");
        scanf(" %c", &resposta);

        
        // Caso o usuário não queira mais jogar, liberamos o espaço reservado para os tabuleiros e para os nós
        if (resposta == NAO){
            exit = SIM;
            // Agora liberamos a memória alocada para os 2 tabuleiros e para todos os nós
            Liberando_espaco(T_computador);
            Liberando_espaco(T_humano);
            printf("\nAte a proxima!\n");
        }
        // Caso contrário, reiniciamos os 2 tabuleiros
        else{
            printf("\nReiniciando os tabuleiros... \n\n");
            Reiniciar_Tabuleiro(T_humano);
            Reiniciar_Tabuleiro(T_computador);
        }
    } while (exit != SIM);

    return EXIT_SUCCESS;
}
