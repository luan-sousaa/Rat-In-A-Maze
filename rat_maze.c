#include <stdio.h>

#define N 4

/* Verifica se (row, col) é uma célula válida para o rato visitar */
int isValid(int maze[N][N], int row, int col)
{
    return (row >= 0 && row < N &&
            col >= 0 && col < N &&
            maze[row][col] == 1);
}

/*
 * Imprime o labirinto resolvido com símbolos visuais:
 *   ->  célula pertencente ao caminho encontrado
 *   |   barreira (maze == 0)
 *   .   célula livre não utilizada no caminho
 * Cada célula ocupa 3 caracteres para manter a grade alinhada.
 */
void printSolution(int maze[N][N], int sol[N][N])
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (sol[i][j])
                printf("-> ");
            else if (maze[i][j] == 0)
                printf("|  ");
            else
                printf(".  ");
        }
        printf("\n");
    }
}

/*
 * Backtracking recursivo.
 * Marca a célula atual como parte do caminho (sol=1), tenta mover
 * para direita e depois para baixo. Se nenhum movimento leva à
 * solução, desmarca (backtrack) e retorna 0.
 */
int solveMaze(int maze[N][N], int sol[N][N], int row, int col)
{
    /* Caso base: chegou ao destino */
    if (row == N - 1 && col == N - 1) {
        sol[row][col] = 1;
        return 1;
    }

    /* Célula atual deve ser válida antes de prosseguir */
    if (!isValid(maze, row, col))
        return 0;

    /* Inclui esta célula no caminho */
    sol[row][col] = 1;

    /* Tenta mover para direita */
    if (solveMaze(maze, sol, row, col + 1))
        return 1;

    /* Tenta mover para baixo */
    if (solveMaze(maze, sol, row + 1, col))
        return 1;

    /* Nenhum movimento funcionou: backtrack */
    sol[row][col] = 0;
    return 0;
}

int main(void)
{
    int maze[N][N] = {
        {1, 0, 0, 0},
        {1, 1, 0, 1},
        {0, 1, 0, 0},
        {1, 1, 1, 1}
    };

    /* Matriz de solução inicializada com zeros */
    int sol[N][N] = {0};

    if (solveMaze(maze, sol, 0, 0))
        printSolution(maze, sol);
    else
        printf("No solution found\n");

    return 0;
}
