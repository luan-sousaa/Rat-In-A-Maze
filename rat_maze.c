#include <stdio.h>

#define TAMANHO 4

/* Verifica se (linha, coluna) é uma célula válida para o rato visitar */
int ehValida(int labirinto[TAMANHO][TAMANHO], int linha, int coluna)
{
    return (linha >= 0 && linha < TAMANHO &&
            coluna >= 0 && coluna < TAMANHO &&
            labirinto[linha][coluna] == 1);
}

/*
 * Imprime o labirinto resolvido com símbolos visuais:
 *   ->  célula pertencente ao caminho encontrado
 *   |   barreira (labirinto == 0)
 *   .   célula livre não utilizada no caminho
 * Cada célula ocupa 3 caracteres para manter a grade alinhada.
 */
void imprimirSolucao(int labirinto[TAMANHO][TAMANHO], int solucao[TAMANHO][TAMANHO])
{
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            if (solucao[i][j] == 1)
                printf("-> ");
            else if (labirinto[i][j] == 0)
                printf("|  ");
            else
                printf(".  ");
        }
        printf("\n");
    }
}

/*
 * Backtracking recursivo.
 * Marca a célula atual como parte do caminho (solucao=1), tenta mover
 * para direita e depois para baixo. Se nenhum movimento leva à
 * solução, desmarca (backtrack) e retorna 0.
 */
int resolverLabirinto(int labirinto[TAMANHO][TAMANHO], int solucao[TAMANHO][TAMANHO], int linha, int coluna)
{
    /* Caso base: chegou ao destino */
    if (linha == TAMANHO - 1 && coluna == TAMANHO - 1) {
        solucao[linha][coluna] = 1;
        return 1;
    }

    /* Célula atual deve ser válida antes de prosseguir */
    if (!ehValida(labirinto, linha, coluna))
        return 0;

    /* Inclui esta célula no caminho */
    solucao[linha][coluna] = 1;

    /* Tenta mover para direita */
    if (resolverLabirinto(labirinto, solucao, linha, coluna + 1))
        return 1;

    /* Tenta mover para baixo */
    if (resolverLabirinto(labirinto, solucao, linha + 1, coluna))
        return 1;

    /* Nenhum movimento funcionou: backtrack */
    solucao[linha][coluna] = 0;
    return 0;
}

int main(void)
{
    int labirinto[TAMANHO][TAMANHO] = {
        {1, 0, 0, 0},
        {1, 1, 0, 1},
        {0, 1, 0, 0},
        {1, 1, 1, 1}
    };

    /* Matriz de solução inicializada com zeros */
    int solucao[TAMANHO][TAMANHO] = {0};

    if (resolverLabirinto(labirinto, solucao, 0, 0))
        imprimirSolucao(labirinto, solucao);
    else
        printf("Nenhuma solucao encontrada\n");

    return 0;
}
