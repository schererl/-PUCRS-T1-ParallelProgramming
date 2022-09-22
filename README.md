# -PUCRS-T1-ParallelProgramming

# Problema-Proposto

Relaxação do algoritmo *Sequential Halving*, um algoritmo de Inteligência Artificial para jogos que tem como objetivo fazer uma estimativa do espaço de ações de um jogo, dado um estado, como por exemplo uma configuração atual de um tabuleiro. A estimativa é composta por dois momentos, as simulações de monte-carlo e a diminuição iterativa do espaço-de-ações cortando de metade em metade. 

Normalmente é necessário que exista um modelo do domínio do problema para que seja possível gerar as configurações do tabuleiro, buscar por quais ações são aplicáveis a dada estado e um função de transformação de um par estado-ação em um novo estado. Como o propósito deste trabalho é avaliar o paralelismo da aplicação, não nos preocuparemos em implementar tal modelo de abstração. Para explorarmos o funcionamento do algoritmo, a etapa de simulação de monte-carlo que é o local do código onde ocorre o processamento pesado, usaremos um método que define aleatóriamente um tempo de processamento entre dois intervalos. O propósito de realizar as simulações é estimar a recompensa esperada da ação a ser avaliada, dada a soma das recompensas obtidas a partir das simulações realizadas. Reproduziremos este processo de duas formas, primeiro é gerado um tempo de processamento aleatório entre um intervalo, neste período o programa entrará em *loop* executando instruções dispensáveis que servem apenas para gerar trabalho. No segundo momento é gerado um valor aleatório de recompensa no intervalo [0,1] para servir como estimativa da recompensa obtida da ação. A outra etapa do algoritmo, calcula o *budget* destinado para cada ação, ordena as mesmas de acordo com as recompensas obtidas na etapa de simulação e dispensa metade do espaço de ações a cada iteração do menor para o maior.

## Código
Explicar em pseudocódigo as 2 etapas principais:

1- Sequential Halving

```
Budget (budget de simulações)
N (número de ações)
A (espaço-de-ações)
Vn (número de ações virtual)
Bn (budget virtual)
Vn = N

enquanto(Vn > 1):
  Bn = calcula_budget(Budget) 
  Para cada ação 'a' em A[0:Vn]:
    simula(a, Bn)
  ordena(A[0:Vn])
  Vn = ceil(Vn/2)
```
2- Simulação Monte-Carlo (a, Bn)

```
budget_alocado = (Bn)
tempo_simulacao = rand(limite_inferior, limite_superior)
enquanto(tempo < tempo_simulacao):
  trabalho()
recompensa = random(0,1)
retorna recompensa
```


# Discussão Modelos de Paralelismo

Para o problema proposto, existem três etapas do processo que podem ser paralelizadas:

  * **Paralelismo de Raiz:** Cada thread executa o *sequential halving* de forma independente, divindo o budget definido igualmente e depois juntam-se os resultados.

  * **Paralelismo de Ação** Cada thread executar um certo número de ações de forma independente.

  * **Paralelismo de Simulação** Cada thread executa uma parte do budget destinado para cada ação de forma independente.

* Discussão de possibilidade de escalonadores a serem usados:
**ler sobre os tipos escalonadores da aula**


## Paralelismo de Ação

testes prévios **exemplo**:

*Tabela de resutados*

**paralelismo de folha**

BUDGET|THREADS|TIPO ESCALONAMENTO|TEMPO EXECUÇÃO|MC INTERVAL (sec)|NÚMERO AÇÕES|
--- | --- | --- | --- | --- |--- |
40000|1|164.50 s|dinâmico|[0.002m 0.009]|[10]
40000|2|82.45 s|dinâmico|[0.002m 0.009]|[10]
40000|3|54.89 s|dinâmico|[0.002m 0.009]|[10]
40000|4|41.27 s|dinâmico|[0.002m 0.009]|[10]
40000|6|27.53 s|dinâmico|[0.002m 0.009]|[10]
40000|8|20.66 s|dinâmico|[0.002m 0.009]|[10]
40000|16|10.38 s|dinâmico|[0.002m 0.009]|[10]

**paralelismo de ação**

BUDGET|THREADS|TIPO ESCALONAMENTO|TEMPO EXECUÇÃO|MC INTERVAL (sec)|NÚMERO AÇÕES|
--- | --- | --- | --- | --- |--- |
40000|1|164.50 s|dinâmico|[0.002m 0.009]|[10]
40000|2|87.85 s|dinâmico|[0.002m 0.009]|[10]
40000|3|71.28 s|dinâmico|[0.002m 0.009]|[10]
40000|4|65.83 s|dinâmico|[0.002m 0.009]|[10]
40000|6|49.52 s|dinâmico|[0.002m 0.009]|[10]
40000|8|49.64 s|dinâmico|[0.002m 0.009]|[10]
40000|16|44.27 s|dinâmico|[0.002m 0.009]|[10]

**alterações na carga:**

* grão 2 (folha):
  - 4 threads 41.28s
  - 8 threads 20.71s  
  - 16 threads 10.43s
  
* grão 2 (ação):
  - 4 threads 98.68 s
  - 8 threads 87.96 s
  - 16 threads 87.99 s

* grão 10 (folha):
  - 4 threads 41.59s
  - 8 threads 21.04s
  - 16 threads 10.80s

* grão 100 (folha):
  - 4 threads 44.68s
  - 8 threads 26.83s
  - 16 threads 15.61s

  
