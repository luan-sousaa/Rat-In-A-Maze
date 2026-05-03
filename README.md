# Rat in a Maze — Guia Completo

## Sumário

1. [O que é o problema?](#1-o-que-é-o-problema)
2. [O que é Backtracking?](#2-o-que-é-backtracking)
3. [Estrutura do Labirinto](#3-estrutura-do-labirinto)
4. [Visão Geral do Código](#4-visão-geral-do-código)
5. [Linha a Linha — Explicação Completa](#5-linha-a-linha--explicação-completa)
   - [Cabeçalho e Constante](#51-cabeçalho-e-constante)
   - [Função ehValida](#52-função-ehvalida)
   - [Função imprimirSolucao](#53-função-imprimirsolucao)
   - [Função resolverLabirinto (o coração do algoritmo)](#54-função-resolverlabirinto-o-coração-do-algoritmo)
   - [Função main](#55-função-main)
6. [O Algoritmo Passo a Passo (Simulação Manual)](#6-o-algoritmo-passo-a-passo-simulação-manual)
7. [Saída Visual e Como Ler](#7-saída-visual-e-como-ler)
8. [Conceitos de C Utilizados](#8-conceitos-de-c-utilizados)
9. [Complexidade](#9-complexidade)
10. [Como Compilar e Executar](#10-como-compilar-e-executar)
11. [Perguntas Frequentes em Apresentações](#11-perguntas-frequentes-em-apresentações)

---

## 1. O que é o problema?

**Rat in a Maze** (Rato no Labirinto) é um problema clássico de algoritmos.

Imagine um labirinto representado por uma grade TAMANHO×TAMANHO. Um rato está na célula superior esquerda **(0,0)** e precisa chegar à célula inferior direita **(TAMANHO-1, TAMANHO-1)**.

Regras:
- O rato só pode se mover para **direita (→)** ou para **baixo (↓)**
- Células com valor `1` são **caminhos livres**
- Células com valor `0` são **paredes/barreiras**
- O objetivo é encontrar **um caminho válido** de (0,0) até (3,3)

```
Início → (0,0)                    Destino → (3,3)
┌────┬────┬────┬────┐
│ S  │ |  │ |  │ |  │   S = Start (início)
├────┼────┼────┼────┤
│    │    │ |  │    │
├────┼────┼────┼────┤
│ |  │    │ |  │ |  │
├────┼────┼────┼────┤
│    │    │    │  E │   E = End (destino)
└────┴────┴────┴────┘
```

---

## 2. O que é Backtracking?

Backtracking é uma técnica algorítmica de **tentativa e erro com desfazimento**.

O algoritmo funciona assim:

```
1. Tente um caminho
2. Se funcionar → continue
3. Se não funcionar → DESFAÇA a escolha (backtrack) e tente outro caminho
4. Repita até achar a solução ou esgotar todas as possibilidades
```

**Analogia do dia a dia:** Imagine que você está num labirinto físico. Você segue um corredor. Chegou num beco sem saída? Você VOLTA até a última bifurcação e tenta o outro caminho. Isso é backtracking.

**Por que é eficiente aqui?**  
Em vez de testar TODAS as combinações possíveis (força bruta), o backtracking abandona um caminho assim que percebe que ele não pode levar à solução — isso se chama **poda** (pruning).

---

## 3. Estrutura do Labirinto

O labirinto usado no código é uma matriz 4×4:

```c
int labirinto[4][4] = {
    {1, 0, 0, 0},   // linha 0
    {1, 1, 0, 1},   // linha 1
    {0, 1, 0, 0},   // linha 2
    {1, 1, 1, 1}    // linha 3
};
```

Visualmente, com coordenadas (linha, coluna):

```
         coluna0  coluna1  coluna2  coluna3
linha0  [  1  ] [  0  ] [  0  ] [  0  ]
linha1  [  1  ] [  1  ] [  0  ] [  1  ]
linha2  [  0  ] [  1  ] [  0  ] [  0  ]
linha3  [  1  ] [  1  ] [  1  ] [  1  ]
```

O único caminho possível (movendo só para direita ou baixo):

```
(0,0) → (1,0) → (1,1) → (2,1) → (3,1) → (3,2) → (3,3)
```

---

## 4. Visão Geral do Código

O programa é dividido em **4 funções**:

| Função | Tipo de retorno | O que faz |
|---|---|---|
| `ehValida` | `int` (0 ou 1) | Checa se uma célula pode ser visitada |
| `imprimirSolucao` | `void` | Imprime o labirinto resolvido com símbolos visuais |
| `resolverLabirinto` | `int` (0 ou 1) | O algoritmo de backtracking em si |
| `main` | `int` | Ponto de entrada: define o labirinto, chama o resolvedor |

**Fluxo de execução:**

```
main()
  └─ resolverLabirinto(labirinto, solucao, 0, 0)      ← inicia o backtracking
        └─ ehValida(...)                               ← valida cada célula
        └─ resolverLabirinto(...) recursivo            ← tenta direita
        └─ resolverLabirinto(...) recursivo            ← tenta baixo
  └─ imprimirSolucao(labirinto, solucao)              ← exibe o resultado
```

---

## 5. Linha a Linha — Explicação Completa

### 5.1 Cabeçalho e Constante

```c
#include <stdio.h>
```
Inclui a biblioteca padrão de entrada/saída do C. É necessária para usar `printf`.  
Essa é a **única** biblioteca externa usada — o programa é completamente autocontido.

---

```c
#define TAMANHO 4
```
Define `TAMANHO` como uma **constante de pré-processador** com valor `4`.

- Não ocupa memória em tempo de execução — é uma substituição textual feita pelo compilador antes de compilar
- Todos os lugares no código onde aparece `TAMANHO` são substituídos por `4`
- **Vantagem:** para mudar o tamanho do labirinto, basta alterar este único número

---

### 5.2 Função `ehValida`

```c
int ehValida(int labirinto[TAMANHO][TAMANHO], int linha, int coluna)
{
    return (linha >= 0 && linha < TAMANHO &&
            coluna >= 0 && coluna < TAMANHO &&
            labirinto[linha][coluna] == 1);
}
```

**O que faz:** Verifica se a posição `(linha, coluna)` é válida para o rato visitar.

**Parâmetros:**
- `labirinto[TAMANHO][TAMANHO]` — o labirinto original (somente leitura aqui)
- `linha` — linha que queremos verificar
- `coluna` — coluna que queremos verificar

**As condições verificadas (todas precisam ser verdadeiras):**

| Condição | Significado |
|---|---|
| `linha >= 0` | A linha não saiu pela borda de cima |
| `linha < TAMANHO` | A linha não saiu pela borda de baixo |
| `coluna >= 0` | A coluna não saiu pela borda esquerda |
| `coluna < TAMANHO` | A coluna não saiu pela borda direita |
| `labirinto[linha][coluna] == 1` | A célula é um caminho livre (não é parede) |

**Retorno:**
- `1` (verdadeiro) — pode visitar
- `0` (falso) — não pode visitar

**Por que essa função é importante?**  
Sem ela, o programa tentaria acessar `labirinto[-1][0]` ou `labirinto[5][5]`, o que causa comportamento indefinido em C (crash ou dados corrompidos). Ela é a **guarda de segurança** do algoritmo.

---

### 5.3 Função `imprimirSolucao`

```c
void imprimirSolucao(int labirinto[TAMANHO][TAMANHO], int solucao[TAMANHO][TAMANHO])
{
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            if (solucao[i][j])
                printf("-> ");
            else if (labirinto[i][j] == 0)
                printf("|  ");
            else
                printf(".  ");
        }
        printf("\n");
    }
}
```

**O que faz:** Percorre as duas matrizes e imprime cada célula com um símbolo visual.

**Por que recebe dois parâmetros?**  
Recebe tanto `labirinto` quanto `solucao` porque precisa distinguir três situações diferentes:

```
solucao[i][j] == 1                    → célula faz parte do caminho  → imprime "->"
solucao[i][j] == 0 e labirinto == 0   → célula é uma barreira        → imprime "|"
solucao[i][j] == 0 e labirinto == 1   → célula livre, não usada      → imprime "."
```

**Os dois `for` aninhados:**
- O `for` externo (`i`) percorre as **linhas** de 0 até TAMANHO-1
- O `for` interno (`j`) percorre as **colunas** de 0 até TAMANHO-1
- Juntos, visitam todas as TAMANHO×TAMANHO células da grade

**O `printf("\n")` após o `for` interno:**  
Imprime uma quebra de linha ao final de cada linha da grade, fazendo a saída parecer uma tabela.

**O espaçamento `"-> "`, `"|  "`, `".  "`:**  
Cada símbolo ocupa exatamente **3 caracteres** (símbolo + espaços de preenchimento). Isso garante que as colunas fiquem alinhadas mesmo com símbolos de tamanhos diferentes.

---

### 5.4 Função `resolverLabirinto` (o coração do algoritmo)

```c
int resolverLabirinto(int labirinto[TAMANHO][TAMANHO], int solucao[TAMANHO][TAMANHO], int linha, int coluna)
```

**Parâmetros:**
- `labirinto[TAMANHO][TAMANHO]` — o labirinto original (nunca é modificado)
- `solucao[TAMANHO][TAMANHO]` — a matriz de solução (vai sendo preenchida e apagada conforme o backtracking)
- `linha` — linha da posição atual do rato
- `coluna` — coluna da posição atual do rato

---

```c
    if (linha == TAMANHO - 1 && coluna == TAMANHO - 1) {
        solucao[linha][coluna] = 1;
        return 1;
    }
```

**Caso base da recursão** — a condição de parada.

Quando `linha == 3` e `coluna == 3` (canto inferior direito), o rato chegou ao destino.  
- Marca a célula de destino como parte do caminho (`solucao[3][3] = 1`)
- Retorna `1` sinalizando **sucesso**

Este é o único momento em que a recursão para com sucesso. Sem o caso base, a função chamaria a si mesma infinitamente.

---

```c
    if (!ehValida(labirinto, linha, coluna))
        return 0;
```

**Verificação de validade.**

Antes de qualquer coisa, checa se a célula atual pode ser visitada.  
Se não puder (saiu dos limites ou é parede), retorna `0` imediatamente — **poda do backtracking**.

O `!` é o operador de negação lógica: `!ehValida(...)` significa "se NÃO for válida".

---

```c
    solucao[linha][coluna] = 1;
```

**Marcação otimista.**

O algoritmo assume que esta célula faz parte do caminho e a marca como `1` na matriz de solução.  
Esta marcação pode ser desfeita mais adiante se nenhum caminho funcionar a partir daqui.

---

```c
    if (resolverLabirinto(labirinto, solucao, linha, coluna + 1))
        return 1;
```

**Tentativa de movimento para a DIREITA.**

Chama `resolverLabirinto` recursivamente com a mesma linha (`linha`) e próxima coluna (`coluna + 1`).  
Se essa chamada retornar `1`, significa que o caminho foi encontrado — propaga o sucesso retornando `1`.

---

```c
    if (resolverLabirinto(labirinto, solucao, linha + 1, coluna))
        return 1;
```

**Tentativa de movimento para BAIXO.**

Só é executada se o movimento para direita **falhou**.  
Chama `resolverLabirinto` com próxima linha (`linha + 1`) e mesma coluna (`coluna`).  
Se essa chamada retornar `1`, propaga o sucesso.

**A ordem importa?**  
Sim. O algoritmo sempre tenta direita antes de baixo. Isso define qual dos possíveis caminhos é encontrado quando existem múltiplas soluções.

---

```c
    solucao[linha][coluna] = 0;
    return 0;
```

**O BACKTRACK — a parte mais importante.**

Se nem mover para direita nem mover para baixo funcionaram, esta célula **não faz parte de nenhum caminho válido**.

- `solucao[linha][coluna] = 0` — **DESFAZ** a marcação feita anteriormente
- `return 0` — informa à chamada anterior que este caminho falhou

Isso faz o algoritmo "voltar" na árvore de recursão e tentar outra opção.

---

### 5.5 Função `main`

```c
int main(void)
{
```
Ponto de entrada do programa em C. `void` indica que não recebe argumentos da linha de comando.

---

```c
    int labirinto[TAMANHO][TAMANHO] = {
        {1, 0, 0, 0},
        {1, 1, 0, 1},
        {0, 1, 0, 0},
        {1, 1, 1, 1}
    };
```

**Declaração e inicialização do labirinto.**

Array bidimensional estático de inteiros. Por ser declarado dentro de `main` (variável local), fica na **pilha (stack)** — sem alocação dinâmica (`malloc`/`free`).  
Os valores são definidos diretamente no código (hardcoded) para teste.

---

```c
    int solucao[TAMANHO][TAMANHO] = {0};
```

**Declaração da matriz de solução, zerada.**

`= {0}` é um atalho em C para inicializar **todos os elementos** do array com zero.  
Esta matriz começa completamente vazia e vai sendo preenchida pelo `resolverLabirinto` conforme o caminho é encontrado.

---

```c
    if (resolverLabirinto(labirinto, solucao, 0, 0))
        imprimirSolucao(labirinto, solucao);
    else
        printf("Nenhuma solucao encontrada\n");
```

**Lógica principal:**

1. Chama `resolverLabirinto` a partir da posição inicial `(0, 0)`
2. Se retornar `1` (caminho encontrado) → imprime a solução visual
3. Se retornar `0` (sem caminho) → imprime mensagem de erro

---

```c
    return 0;
```

Encerra o programa retornando `0` ao sistema operacional, indicando **execução bem-sucedida**.  
Por convenção em C, qualquer valor diferente de `0` indicaria um erro.

---

## 6. O Algoritmo Passo a Passo (Simulação Manual)

Acompanhe o que acontece internamente quando o programa roda:

```
Labirinto:               Solução sendo construída:
1  0  0  0               ?  .  .  .
1  1  0  1               .  .  .  .
0  1  0  0               .  .  .  .
1  1  1  1               .  .  .  .
```

**Passo 1:** `resolverLabirinto(0,0)` — célula válida, marca `solucao[0][0]=1`
```
-> .  .  .
.  .  .  .
.  .  .  .
.  .  .  .
```

**Passo 2:** Tenta direita → `resolverLabirinto(0,1)` — `labirinto[0][1]=0`, INVÁLIDA → retorna 0

**Passo 3:** Tenta baixo → `resolverLabirinto(1,0)` — válida, marca `solucao[1][0]=1`
```
-> .  .  .
-> .  .  .
.  .  .  .
.  .  .  .
```

**Passo 4:** Tenta direita → `resolverLabirinto(1,1)` — válida, marca `solucao[1][1]=1`
```
-> .  .  .
-> -> .  .
.  .  .  .
.  .  .  .
```

**Passo 5:** Tenta direita → `resolverLabirinto(1,2)` — `labirinto[1][2]=0`, INVÁLIDA → retorna 0

**Passo 6:** Tenta baixo → `resolverLabirinto(2,1)` — válida, marca `solucao[2][1]=1`
```
-> .  .  .
-> -> .  .
.  -> .  .
.  .  .  .
```

**Passo 7:** Tenta direita → `resolverLabirinto(2,2)` — `labirinto[2][2]=0`, INVÁLIDA → retorna 0

**Passo 8:** Tenta baixo → `resolverLabirinto(3,1)` — válida, marca `solucao[3][1]=1`

**Passo 9:** Tenta direita → `resolverLabirinto(3,2)` — válida, marca `solucao[3][2]=1`

**Passo 10:** Tenta direita → `resolverLabirinto(3,3)` — **DESTINO ALCANÇADO!** `solucao[3][3]=1`, retorna 1

Todos os retornos propagam `1` até `main`. Solução final:
```
-> |  |  |
-> -> |  .
|  -> |  |
.  -> -> ->
```

---

## 7. Saída Visual e Como Ler

```
-> |  |  |
-> -> |  .
|  -> |  |
.  -> -> ->
```

| Símbolo | Significado |
|---|---|
| `->` | Célula que faz parte do caminho encontrado |
| `\|` | Barreira (parede intransponível) |
| `.` | Célula livre, mas não usada no caminho |

**Lendo o caminho:**  
Siga todos os `->` da esquerda para a direita, de cima para baixo:  
`(0,0) → (1,0) → (1,1) → (2,1) → (3,1) → (3,2) → (3,3)`

---

## 8. Conceitos de C Utilizados

### Arrays bidimensionais
```c
int labirinto[4][4]
```
Armazenados em memória de forma **linear** (linha por linha). `labirinto[i][j]` acessa o elemento na linha `i`, coluna `j`.

### Passagem de arrays para funções
Em C, arrays são passados como **ponteiro para o primeiro elemento**. Não são copiados — as funções trabalham diretamente na memória original.

### Recursão
Uma função que chama a si mesma. Cada chamada cria um novo **frame na pilha (stack frame)** com suas próprias variáveis locais (`linha`, `coluna`).

### `#define` vs `const`
`#define TAMANHO 4` é uma diretiva de pré-processamento — não existe em tempo de execução. `const int TAMANHO = 4` seria uma variável real. Para tamanho de arrays estáticos em C, `#define` é a abordagem tradicional.

### Retorno como sinalizador booleano
C não tem tipo `bool` nativo (antes do C99). Usa-se `int`: `0` = falso, qualquer valor diferente de zero = verdadeiro.

---

## 9. Complexidade

| Tipo | Complexidade |
|---|---|
| **Tempo** (pior caso) | O(2^(TAMANHO²)) — exponencial, pois cada célula pode ser visitada ou não |
| **Tempo** (caso típico) | Muito melhor na prática — o backtracking poda caminhos inválidos cedo |
| **Espaço** | O(TAMANHO²) — para a matriz `solucao` + profundidade máxima da recursão O(TAMANHO²) |

Para TAMANHO=4, o pior caso teórico é 2^16 = 65.536 operações — completamente trivial para um computador moderno.

---

## 10. Como Compilar e Executar

```bash
# Compilar com avisos habilitados
gcc -Wall -o rat_maze rat_maze.c

# Executar
./rat_maze
```

**O que cada flag faz:**
- `gcc` — GNU C Compiler
- `-Wall` — habilita **todos os avisos** (warnings). Boa prática para pegar erros sutis
- `-o rat_maze` — define o nome do executável de saída
- `rat_maze.c` — arquivo fonte de entrada

**Para testar "Nenhuma solucao encontrada":**  
Altere `labirinto[1][0]` de `1` para `0` — o único caminho inicial fica bloqueado.

---

## 11. Perguntas Frequentes em Apresentações

**P: Por que só direita e baixo? O rato não poderia voltar?**  
R: Restringir os movimentos simplifica o problema e evita ciclos infinitos. Em versões mais avançadas, o rato pode ir nas 4 direções, mas aí é necessário um array `visitado[][]` para não revisitar células.

**P: O que acontece se existirem múltiplos caminhos?**  
R: O algoritmo retorna o **primeiro** que encontrar (priorizando direita antes de baixo). Ele não garante o caminho mais curto — para isso, usaríamos BFS (Busca em Largura).

**P: Por que arrays estáticos e não alocação dinâmica?**  
R: Para este problema de tamanho fixo, arrays estáticos são mais simples, mais rápidos (sem overhead de `malloc`) e sem risco de vazamento de memória.

**P: E se o `labirinto[0][0]` for `0`?**  
R: O `ehValida` retorna `0` imediatamente na primeira chamada de `resolverLabirinto(0,0)`, e o programa imprime "Nenhuma solucao encontrada".

**P: Qual a diferença entre `labirinto` e `solucao`?**  
R: `labirinto` é **somente leitura** — representa o labirinto e nunca é modificado. `solucao` é **escrita/leitura** — começa zerada e registra o caminho encontrado, sendo modificada durante o backtracking.

**P: O que é um "frame de pilha"?**  
R: Cada vez que `resolverLabirinto` chama a si mesma, o sistema operacional reserva um bloco de memória na pilha (stack) para guardar os valores de `linha` e `coluna` daquela chamada específica. Quando a função retorna, esse bloco é liberado.
