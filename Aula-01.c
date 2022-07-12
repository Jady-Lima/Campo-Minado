#include <stdio.h>
#include <stdlib.h>
#include <time.h> //biblioteca do ranger

//struct de cedula
typedef struct
{
    int mina, vizinhaca, estado;
} Celula;

//struct para matriz
typedef struct
{
  int n_lin, n_col;
  Celula **cel;
} Campo;

//variaveis globais
int Linha = 10;
int Coluna = 20;

//cria a mtatriz de structs
Campo* criaMatriz()
{
  Campo* campo = malloc(sizeof(Campo));
  
  campo->cel = malloc(sizeof(Celula*) * Linha);
  campo->n_col = Coluna;
  campo->n_lin = Linha;

  for (int i = 0; i < Linha; i++)
    {
      campo->cel[i] = malloc(sizeof(Celula) * Coluna);
      if (campo->cel[i] == NULL)
      {
        printf("Memoria insuficiente para alocar a matriz\n");
      }
    }
  
  return campo;
}

//inicializa todos os campos da matriz com 0
void inicializaCampo(Campo* camp)
{
  for (int l = 0; l < Linha; l++)
    {
      for (int c = 0; c < Coluna; c++)
        {
          camp->cel[l][c].estado = 0;
          camp->cel[l][c].mina = 0;
          camp->cel[l][c].vizinhaca = 0;
        }
    }
}

//sorteia as posições das bombas
void sorteiaBombas(int m, Campo* camp)
{
  int lin, col;
  srand(time(NULL));
  for (int i = 0; i < m; i++)
  {
    lin = rand() % Linha;
    col = rand() % Coluna;
    
    if (camp->cel[lin][col].mina == 0)
    {
      //se não houver bomba na posição sorteada, ele adiciona uma
      camp->cel[lin][col].mina = 1;
    }

    else
    {
      //se já houver bomba, ele vai aumentar mai um sorteio
      i--;
    }
  }
}

//testa se a coordenada fornecida pelo user está dentro dos limites
int coordenadaValida(int lin, int col)
{
  if (lin >= 0 && lin < Linha && col >= 0 && col < Coluna)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

//analiza se na vizinhança existe bombas
int bombasVizinhas(int lin, int col, Campo* camp)
{
  int q_bombas = 0;

  if (coordenadaValida(lin - 1, col) == 1 && camp->cel[lin - 1][col].mina == 1)
  {
    q_bombas++;
  }

  if (coordenadaValida(lin - 1, col - 1) == 1 && camp->cel[lin - 1][col - 1].mina == 1)
  {
    q_bombas++;
  }

  if (coordenadaValida(lin - 1, col + 1) == 1 && camp->cel[lin - 1][col + 1].mina == 1)
  {
    q_bombas++;
  }

  if (coordenadaValida(lin + 1, col) == 1 && camp->cel[lin + 1][col].mina == 1)
  {
    q_bombas++;
  }

  if (coordenadaValida(lin + 1, col - 1) == 1 && camp->cel[lin + 1][col - 1].mina == 1)
  {
    q_bombas++;
  }

  if (coordenadaValida(lin + 1, col + 1) == 1 && camp->cel[lin + 1][col + 1].mina == 1)
  {
    q_bombas++;
  }

  if (coordenadaValida(lin, col - 1) == 1 && camp->cel[lin][col - 1].mina == 1)
  {
    q_bombas++;
  }

  if (coordenadaValida(lin, col + 1) == 1 && camp->cel[lin][col + 1].mina == 1)
  {
    q_bombas++;
  }

  return q_bombas;
}

//manda as posições para serem analizadas
void analizaVizinhos(Campo* camp)
{
  for (int l = 0; l < Linha; l++)
    {
      for (int c = 0; c < Coluna; c++)
        {
          camp->cel[l][c].vizinhaca = bombasVizinhas(l, c, camp);
        }
    }
}

//imprime o tabuleiro
void imprimeCampo(Campo* camp)
{
  printf("\n\t  ------------------------CAMPO MINADO-------------------------\n\n");
  printf("\t   ");
  for (int c = 0; c < Coluna; c++)
  {
      //imprime os indices das colunas
      if (c < 10)
      {
          printf(" %d ", c);
      }
      else
      {
          printf("%d ", c);
      }
  }
  printf("\n\t  -------------------------------------------------------------\n");

  for (int l = 0; l < Linha; l++)
    {
        printf("\t%d |", l);
        for (int c = 0; c < Coluna; c++)
        {
            if (camp->cel[l][c].estado == 1)
            {
                //tá aberta?
                if (camp->cel[l][c].mina == 1)
                {
                    //se for uma mina, ele imprime *
                    printf("*");
                }
                else
                {
                    //se não é mina, imprime a vizinhança
                    printf("%d", camp->cel[l][c].vizinhaca);
                }
            }
            else
            {
                //se não está aberta, ele imprime um espaço
                printf(" ");
            }
            printf(" |");
        }
        printf("\n\t  -------------------------------------------------------------\n");
    }

  
}

//abre as posições solicitadas pelo usuario
void abrirCampo(int lin, int col, Campo* camp)
{
  if (coordenadaValida(lin, col) == 1 && camp->cel[lin][col].estado == 0)
    {
        camp->cel[lin][col].estado = 1;

        //chama a função para os 8 vizinhos
        if (camp->cel[lin][col].vizinhaca == 0)
        {
            abrirCampo(lin - 1, col, camp);
            abrirCampo(lin - 1, col - 1, camp);
            abrirCampo(lin - 1, col + 1, camp);
            abrirCampo(lin + 1, col, camp);
            abrirCampo(lin + 1, col - 1, camp);
            abrirCampo(lin + 1, col + 1, camp);
            abrirCampo(lin, col - 1, camp);
            abrirCampo(lin, col + 1, camp);
        }
    }
}

//analiza se o jogador ganhou
int ganhou(Campo* camp)
{
  int m_fechadas = 0;
    for (int l = 0; l < Linha; l++)
    {
        for (int c = 0; c < Coluna; c++)
        {
            //se está fechada e não é uma bomba
            if (camp->cel[l][c].estado == 0 && camp->cel[l][c].mina == 0)
            {
                m_fechadas++;
            }
        }
    }
    return m_fechadas;
}

//recebe do user as coordenadas para jogo 
void jogar(Campo* camp)
{
  int l, c;

    do
    {
        imprimeCampo(camp);
        do
        {
            printf("Digite as coordenadas [linha, coluna]:\n");
            scanf("%d %d", &l, &c);

            if (coordenadaValida(l, c) == 0)
            {
                printf("Coordenada inválida!\n\n");
            }
            if (camp->cel[l][c].estado == 1)
            {
                printf("Posição já aberta!\n\n");
            }
          
        }
        while (coordenadaValida(l, c) == 0 || camp->cel[l][c].estado == 1);

        abrirCampo(l, c, camp);
    }
    while (ganhou(camp) != 0 && camp->cel[l][c].mina == 0);

    if (camp->cel[l][c].mina == 1)
    {
        printf("\nQue pena, você perdeu :(\n");
    }

    else
    {
        printf("\nParabéns! Você ganhou. :)\n");
    }

    imprimeCampo(camp);
}

//imprime as instruções do jogo
void instrucoes(){
  printf("---------------------OLÁ, SEGUE ABAIXO AS INSTRUÇÕES DO JOGO!----------------------\n");
  printf("|       O jogo consiste em abrir todas as posições que não possuem bombas.        |\n");
  printf("|     Você pode escolher uma posição informando a linha e coluna, nessa ordem.    |\n");
  printf("|  Você também pode selecionar a opção de ajuda digitando como coordenada: 0 -1.  |\n");
  printf("|               Ou solicitar ver o tempo de jogo digitando: -1 0.                 |\n");
  printf("-----------------------------------------------------------------------------------\n");
}

//função auxiliar que chama as demais funções
void chamaFuncoes(Campo* camp)
{      
    int q_m;
    printf("\nDigite a quantidade de minas desejada: \n");
    scanf("%d", &q_m);
  
    inicializaCampo(camp);
    sorteiaBombas(q_m, camp);
    analizaVizinhos(camp);
    jogar(camp);
}

int main(void) {
  Campo* camp;
  int opc;
  float tempo;
  time_t t_inicial, t_final;
  
  camp = criaMatriz();
  do
    {
        //imprimir aqui as instruções do jogo
        instrucoes();
      
        t_inicial = time(NULL);
        chamaFuncoes(camp);
        
        t_final = time(NULL);
        tempo = difftime(t_final, t_inicial);
        printf("O tempo de execução do jogo foi %.0fs\n", tempo);

        printf("Digite 1 para jogar novamente: \n");
        scanf("%d", &opc);
    }
    while(opc == 1);
}