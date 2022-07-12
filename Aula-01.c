# include <stdio.h>
#include <time.h> //biblioteca do ranger

typedef struct 
{
   int mina;
   int vizinhanca;
   int estado;
}Celula;

//variaveis globais
Celula campo[10][20];
int linha, coluna;

void inicializaCampo() 
{//essa função inicializa todas as posições com 0
    for(int l = 0; l < 10; l++)
    {
        for (int c = 0; c < 20; c++)
        {
            campo[l][c].estado = 0;
            campo[l][c].mina = 0;
            campo[l][c].vizinhanca = 0;
        }
    }
}

void sortearBombas()
{//sorteia as posições das bombas
    srand(time(NULL));
    for (int i = 0; i < 40; i++)
    {
        linha = rand() % 10;
        coluna = rand() % 20;
        if (campo[linha][coluna].mina = 0)
        {//se não houver bomba na posição escolhida, adicione uma
            campo[linha][coluna].mina = 1;
        }
        else
        {//se já houver bomba, ele vai aumentar mais um sorteio
            i--;
        }
    }
    
}

int coordenadaValida(int l, int c)
{//olha se as coordenadas são validas, se não resultam em num maiores ou menores que o campo
    if (l >= 0 && l < 10 && c >= 0 && c < 20)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int bombasVizinhas(int l, int c)
{//analiza se ha bombas vizinhas ao redor
    int q_bombas = 0;

    if (coordenadaValida(l-1, c) == 1 && campo[l-1][c].mina == 1)
    {
        q_bombas++;
    }

    if (coordenadaValida(l+1, c) == 1 && campo[l+1][c].mina == 1)
    {
        q_bombas++;
    }

    if (coordenadaValida(l, c-1) == 1 && campo[l][c-1].mina == 1)
    {
        q_bombas++;
    }

    if (coordenadaValida(l, c+1) == 1 && campo[l][c+1].mina == 1)
    {
        q_bombas++;
    }

    return q_bombas;    
}

void analizaVizinhos()
{//manda as posições para serem analizadas
    for (int l = 0; l < 10; l++)
    {
        for (int c = 0; c < 20; c++)
        {
            campo[l][c].vizinhanca = bombasVizinhas(l, c);
        }
    }
}

void imprimeCampo()
{
    for (int l = 0; l < 10; l++)
    {
        printf("%d ", l);
    }
    

    for (int l = 0; l < 10; l++)
    {
        for (int c = 0; c < 20; c++)
        {
            if (campo[l][c].estado == 1)
            {//tá aberta?
                if (campo[l][c].mina == 1)
                {//se for uma mina, ele imprime *
                    printf("*");
                }
                else
                {//se não é mina, imprime a vizinhança
                    printf("%d", campo[l][c].vizinhanca);
                }                
            }
            else
            {//se não está aberta, ele imprime um espaço
                printf(" ");
            }  
        }
        printf("\n");
    }
}

int main(void)
{
    inicializaCampo();
    sortearBombas();
    analizaVizinhos();
    imprimir();
}