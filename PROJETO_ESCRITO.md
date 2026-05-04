# Projeto: Rat in a Maze com Backtracking em C

---

## 1. Introdução

Este projeto implementa o problema clássico de algoritmos conhecido como **Rat in a Maze** (Rato no Labirinto), utilizando a linguagem de programação C e a técnica de **backtracking recursivo**.

O problema consiste em encontrar um caminho válido dentro de um labirinto representado por uma matriz, onde um rato parte do canto superior esquerdo e precisa chegar ao canto inferior direito, podendo se mover apenas para a direita ou para baixo.

O objetivo do projeto é demonstrar na prática o funcionamento do backtracking como estratégia de resolução de problemas, além de explorar conceitos fundamentais da linguagem C, como arrays bidimensionais, recursão e funções.

---

## 2. Descrição do Problema

O labirinto é representado por uma matriz 4×4 de inteiros, onde cada célula pode assumir dois valores:

- `1` — célula livre, o rato pode passar
- `0` — parede, o rato não pode passar

O rato começa na posição `(0,0)`, canto superior esquerdo, e deve chegar à posição `(3,3)`, canto inferior direito. Os únicos movimentos permitidos são para a **direita** e para **baixo**, sem possibilidade de retroceder ou se mover na diagonal.

O labirinto utilizado no projeto é o seguinte:

```c
int labirinto[4][4] = {
    {1, 0, 0, 0},
    {1, 1, 0, 1},
    {0, 1, 0, 0},
    {1, 1, 1, 1}
};
```

Visualmente:

```
         coluna0  coluna1  coluna2  coluna3
linha0  [  1  ] [  0  ] [  0  ] [  0  ]
linha1  [  1  ] [  1  ] [  0  ] [  1  ]
linha2  [  0  ] [  1  ] [  0  ] [  0  ]
linha3  [  1  ] [  1  ] [  1  ] [  1  ]
```

---

## 3. Técnica Utilizada — Backtracking

Backtracking é uma técnica algorítmica baseada em **tentativa e erro com desfazimento**. O algoritmo explora um caminho possível e, ao encontrar um beco sem saída, desfaz as escolhas feitas e tenta uma alternativa diferente.

A principal vantagem do backtracking em relação à força bruta é a **poda**: assim que o algoritmo detecta que um caminho não pode levar à solução, ele abandona esse caminho imediatamente, sem continuar explorando suas ramificações.

No contexto deste projeto, o backtracking funciona da seguinte forma:

1. O rato entra em uma célula e a marca como parte do caminho
2. Tenta se mover para a direita
3. Se não conseguir, tenta se mover para baixo
4. Se nenhuma direção funcionar, **desfaz** a marcação da célula atual e retorna à célula anterior
5. Repete o processo até encontrar o destino ou esgotar todas as possibilidades

---

## 4. Estrutura do Código

O programa é composto por quatro funções, cada uma com responsabilidade bem definida:

| Função | Tipo de retorno | Responsabilidade |
|---|---|---|
| `ehValida` | `int` | Verifica se uma célula pode ser visitada |
| `resolverLabirinto` | `int` | Executa o backtracking recursivo |
| `imprimirSolucao` | `void` | Exibe o resultado visualmente |
| `main` | `int` | Ponto de entrada do programa |

---

## 5. Explicação das Funções

### 5.1 `ehValida`

```c
int ehValida(int labirinto[TAMANHO][TAMANHO], int linha, int coluna)
{
    return (linha >= 0 && linha < TAMANHO &&
            coluna >= 0 && coluna < TAMANHO &&
            labirinto[linha][coluna] == 1);
}
```

Essa função é a guarda de segurança do algoritmo. Antes de o rato se mover para qualquer célula, ela verifica duas coisas:

**Se a célula está dentro dos limites do labirinto:**
Em C, acessar uma posição fora de um array causa comportamento indefinido — o programa pode travar ou ler dados corrompidos. Por isso, a função verifica se `linha` e `coluna` estão dentro do intervalo válido (0 a 3).

**Se a célula é um caminho livre:**
Verifica se o valor em `labirinto[linha][coluna]` é `1`. Valor `0` significa parede e o rato não pode passar.

A função retorna `1` se a célula for válida e `0` caso contrário. Todas as condições precisam ser verdadeiras ao mesmo tempo — o operador `&&` garante isso.

---

### 5.2 `resolverLabirinto`

```c
int resolverLabirinto(int labirinto[TAMANHO][TAMANHO], int solucao[TAMANHO][TAMANHO], int linha, int coluna)
```

Esta é a função principal do projeto — onde o backtracking acontece. Ela é chamada recursivamente, ou seja, chama a si mesma para explorar cada nova posição do labirinto.

**Caso base — chegou ao destino:**
```c
if (linha == TAMANHO - 1 && coluna == TAMANHO - 1) {
    solucao[linha][coluna] = 1;
    return 1;
}
```
Quando o rato alcança a célula `(3,3)`, o problema está resolvido. A célula é marcada e a função retorna `1` sinalizando sucesso para todas as chamadas anteriores na pilha de recursão.

**Validação da célula atual:**
```c
if (!ehValida(labirinto, linha, coluna))
    return 0;
```
Antes de prosseguir, verifica se a célula atual pode ser visitada. Se não puder, retorna `0` imediatamente — isso é a poda do backtracking.

**Marcação otimista:**
```c
solucao[linha][coluna] = 1;
```
O algoritmo assume que esta célula faz parte do caminho e a marca. Essa marcação pode ser desfeita mais adiante se nenhuma direção funcionar a partir daqui.

**Tentativa de movimento para a direita:**
```c
if (resolverLabirinto(labirinto, solucao, linha, coluna + 1))
    return 1;
```
Chama a si mesmo com a próxima coluna. Se o caminho for encontrado a partir daqui, o sucesso se propaga.

**Tentativa de movimento para baixo:**
```c
if (resolverLabirinto(labirinto, solucao, linha + 1, coluna))
    return 1;
```
Executada apenas se o movimento para a direita falhou. Chama a si mesmo com a próxima linha.

**Backtrack — desfazimento:**
```c
solucao[linha][coluna] = 0;
return 0;
```
Se nem direita nem baixo funcionaram, a célula atual não faz parte de nenhum caminho válido. A marcação é desfeita e a função retorna `0`, informando à chamada anterior que este caminho falhou.

---

### 5.3 `imprimirSolucao`

```c
void imprimirSolucao(int labirinto[TAMANHO][TAMANHO], int solucao[TAMANHO][TAMANHO])
```

Responsável por exibir o resultado de forma visual. Percorre todas as células da grade e imprime um símbolo para cada uma, consultando as duas matrizes ao mesmo tempo:

| `solucao[i][j]` | `labirinto[i][j]` | Símbolo | Significado |
|---|---|---|---|
| 1 | 1 | `->` | Célula do caminho encontrado |
| 0 | 0 | `\|` | Parede — posição bloqueada |
| 0 | 1 | `.` | Célula livre não utilizada |

As duas matrizes são necessárias juntas porque, sem o `labirinto`, seria impossível distinguir uma parede de uma célula livre não utilizada — ambas teriam `solucao == 0`.

Cada símbolo ocupa exatamente três caracteres na saída, incluindo espaços de preenchimento, garantindo o alinhamento visual da grade.

---

### 5.4 `main`

```c
int main(void)
```

Ponto de entrada do programa. Define o labirinto com os valores fixos para teste, inicializa a matriz de solução com zeros e chama `resolverLabirinto` a partir da posição inicial `(0,0)`.

A matriz `solucao` é inicializada com `{0}` para garantir que todos os elementos comecem zerados. Sem isso, a memória poderia conter valores aleatórios de outros programas, corrompendo o resultado.

Ao final, verifica o retorno de `resolverLabirinto`:
- Se retornou `1` → chama `imprimirSolucao` para exibir o caminho
- Se retornou `0` → imprime a mensagem `"Nenhuma solucao encontrada"`

---

## 6. Fluxo de Execução

```
main()
  │
  ├─ define labirinto[4][4]
  ├─ define solucao[4][4] = {0}
  │
  └─ resolverLabirinto(labirinto, solucao, 0, 0)
        │
        ├─ ehValida(labirinto, 0, 0) → válida
        ├─ marca solucao[0][0] = 1
        │
        ├─ tenta direita → resolverLabirinto(..., 0, 1)
        │     └─ ehValida → inválida (parede) → return 0
        │
        └─ tenta baixo → resolverLabirinto(..., 1, 0)
              ├─ ehValida → válida
              ├─ marca solucao[1][0] = 1
              ├─ tenta direita → resolverLabirinto(..., 1, 1)
              │     ├─ marca solucao[1][1] = 1
              │     ├─ tenta direita → inválida (parede) → return 0
              │     └─ tenta baixo → resolverLabirinto(..., 2, 1)
              │           ├─ marca solucao[2][1] = 1
              │           ├─ tenta direita → inválida (parede) → return 0
              │           └─ tenta baixo → resolverLabirinto(..., 3, 1)
              │                 ├─ marca solucao[3][1] = 1
              │                 └─ tenta direita → resolverLabirinto(..., 3, 2)
              │                       ├─ marca solucao[3][2] = 1
              │                       └─ tenta direita → resolverLabirinto(..., 3, 3)
              │                             └─ DESTINO! solucao[3][3]=1 → return 1
              │                       return 1 ← propaga sucesso
              │                 return 1 ← propaga sucesso
              │           return 1 ← propaga sucesso
              │     return 1 ← propaga sucesso
              return 1 ← propaga sucesso
  │
  └─ imprimirSolucao(labirinto, solucao)
```

---

## 7. Saída do Programa

Após a execução, o programa exibe o labirinto resolvido:

```
-> |  |  |
-> -> |  .
|  -> |  |
.  -> -> ->
```

O caminho encontrado, lendo todos os símbolos `->`, é:

```
(0,0) → (1,0) → (1,1) → (2,1) → (3,1) → (3,2) → (3,3)
```

---

## 8. Conceitos de C Aplicados

**Arrays bidimensionais**
O labirinto e a solução são representados como arrays estáticos de inteiros. Por serem declarados dentro de `main`, ficam alocados na pilha (stack), sem necessidade de gerenciamento de memória dinâmica.

**`#define` como constante**
`#define TAMANHO 4` é uma diretiva de pré-processamento. Antes da compilação, o compilador substitui todas as ocorrências de `TAMANHO` pelo valor `4`. Isso centraliza o controle do tamanho do labirinto em um único ponto do código.

**Recursão**
Cada chamada de `resolverLabirinto` cria um novo frame na pilha de execução com seus próprios valores de `linha` e `coluna`. Quando a função retorna, o frame é removido e a execução volta ao ponto anterior.

**Retorno booleano com `int`**
C não possui tipo booleano nativo antes do C99. Por convenção, `0` representa falso e qualquer valor diferente de zero representa verdadeiro. As funções `ehValida` e `resolverLabirinto` utilizam esse padrão para sinalizar falha ou sucesso.

**Passagem de arrays por referência**
Em C, arrays são passados para funções como ponteiro para o primeiro elemento. Isso significa que as funções trabalham diretamente na memória original — não há cópia dos dados.

---

## 9. Complexidade

| Tipo | Complexidade |
|---|---|
| Tempo (pior caso) | O(2^(TAMANHO²)) — cada célula pode ser visitada ou não |
| Tempo (caso típico) | Muito menor na prática — o backtracking poda caminhos inválidos cedo |
| Espaço | O(TAMANHO²) — para a matriz `solucao` e a pilha de recursão |

Para TAMANHO igual a 4, o pior caso teórico é 2^16 = 65.536 operações, o que é trivial para qualquer computador moderno.

---

## 10. Como Compilar e Executar

```bash
gcc -Wall -o rat_maze rat_maze.c
./rat_maze
```

- `gcc` — compilador C
- `-Wall` — habilita todos os avisos de compilação
- `-o rat_maze` — nome do executável gerado
- `rat_maze.c` — arquivo fonte

Para testar o caso sem solução, basta alterar `labirinto[1][0]` de `1` para `0`, bloqueando o único caminho possível. O programa então exibirá `"Nenhuma solucao encontrada"`.
