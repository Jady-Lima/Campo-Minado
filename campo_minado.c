#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//cores em ANSI utilizadas
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GRAY    "\e[0;37m"
#define ANSI_COLOR_GREEN   "\e[0;32m"
#define ANSI_COLOR_YELLOW  "\e[0;33m"
#define ANSI_COLOR_BLUE    "\e[0;34m"
#define ANSI_COLOR_PURPLE  "\e[0;35m"
#define ANSI_COLOR_FUNDO   "\e[0;41m"
#define ANSI_COLOR_RESET   "\x1b[0m"

//struct para rank
typedef struct pessoa
{
    char nome[20];
    float time;
} Pessoa;

//struct de cedula
typedef struct
{
    int vizinhaca, estado, mina;
} Celula;

//struct para matriz
typedef struct
{
    int n_lin, n_col;
    Celula **cel;
} Campo;

//struct para armazenar as posições
typedef struct
{
    int l, c;
} Posicao;

//variaveis globais
int Linha = 10;
int Coluna = 20;

//cria a matriz de structs
Campo *criaMatriz()
{
    Campo *campo = malloc(sizeof(Campo));

    campo->cel = malloc(sizeof(Celula *) * Linha);
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

//função que destroi a matriz
void destroiCampo(Campo **camp)
{
    Campo *C = *camp;
    if (C == NULL)
    {
        return;
    }
    free(C);
    *camp = NULL;
}

//inicializa todos os campos da matriz com 0
void inicializaCampo(Campo *camp)
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

//sorteia as bombas de maneira aleatoria no campo
void sorteiaBombas(int m, Campo *camp)
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
            //se já houver bomba, ele vai aumentar um sorteio
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
int bombasVizinhas(int lin, int col, Campo *camp)
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
void analizaVizinhos(Campo *camp)
{
    for (int l = 0; l < Linha; l++)
    {
        for (int c = 0; c < Coluna; c++)
        {
            camp->cel[l][c].vizinhaca = bombasVizinhas(l, c, camp);
        }
    }
}

//iniciar uma posição no jogo
void inicia(Campo *camp)
{
    int lin, col;
    srand(time(NULL));
    lin = rand() % Linha;
    col = rand() % Coluna;

    if (camp->cel[lin][col].mina == 0 && camp->cel[lin][col].vizinhaca != 0)
    {
        camp->cel[lin][col].estado = 1;
    }
    else
    {
        inicia(camp);
    }
}

//imprime o tabuleiro
void imprimeCampo(Campo *camp)
{
    printf(ANSI_COLOR_PURPLE "\n\t  ------------------------CAMPO MINADO-------------------------\n\n");
    printf("\t   ");
    for (int c = 0; c < Coluna; c++)
    {
        //imprime os indices das colunas
        if (c < 10)
        {
            printf(ANSI_COLOR_RESET " %d ", c);
        }
        else
        {
            printf("%d ", c);
        }
    }
    printf(ANSI_COLOR_PURPLE "\n\t  -------------------------------------------------------------\n");

    for (int l = 0; l < Linha; l++)
    {
        printf(ANSI_COLOR_RESET "\t%d ", l);
        printf(ANSI_COLOR_BLUE"|");
        for (int c = 0; c < Coluna; c++)
        {
            if (camp->cel[l][c].estado == 1)
            {
                //tá aberta?
                if (camp->cel[l][c].mina == 1)
                {
                    //se for uma mina, ele imprime *
                    printf(ANSI_COLOR_RED"*"ANSI_COLOR_RESET);
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
            printf(ANSI_COLOR_BLUE" |");
        }
        printf(ANSI_COLOR_PURPLE "\n\t  -------------------------------------------------------------\n");
    }
}

//IMPRIMIR BOMBA AO PERDER
void imprimirBomba()
{
    printf("\t\t");
    printf(ANSI_COLOR_RED "\t\t　　＼　　　　　　☆\n");
    printf("\t\t\t　　　　　　　　　　　　　|　　　　　☆\n");
    printf("\t\t\t　　　　　　　　　　(⌒ ⌒ヽ　　　/\n");
    printf("\t\t\t　　　　＼　　（´⌒　　⌒　　⌒ヾ　　　／\n");
    printf("\t\t\t　　　　　 （’⌒　;　⌒　　　::⌒　　\n");
    printf("\t\t\t　　　　　（´　　　　　）　　:::　）　／\n");
    printf("\n");
    printf("\t\t\t　　☆─　（´⌒;:　　　　::⌒`）　:;　　）\n");
    printf("\n");
    printf("\t\t\t　　　　（⌒::　　　::　　　　　::⌒　）\n");
    printf("\n");
    printf("\t\t\t　　 　 （　　　　ゝ　　ヾ　丶　　　)   ─\n"ANSI_COLOR_RESET);
}

//abre as posições solicitadas pelo usuario
void abrirCampo(int lin, int col, Campo *camp)
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
int ganhou(Campo *camp)
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

//abre uma posição para ajudar o jogador
void ajuda(Campo *camp)
{
    Posicao *posicoes_livres = malloc(0);
    int tam_vetor = 0;

    for (int l = 0; l < Linha; l++)
    {
        for (int c = 0; c < Coluna; c++)
        {
            if (camp->cel[l][c].estado == 0 && camp->cel[l][c].mina == 0)
            {
                tam_vetor += 1;
                posicoes_livres = realloc(posicoes_livres, sizeof(Posicao) * tam_vetor);
                int ult_indice = tam_vetor - 1;

                //armazena a linha e coluna na posição do vetor
                posicoes_livres[ult_indice].l = l;
                posicoes_livres[ult_indice].c = c;
            }
        }
    }

    int ind_posicao = rand() % tam_vetor;//sorteia um elemento do vetor
    int lin = posicoes_livres[ind_posicao].l;
    int col = posicoes_livres[ind_posicao].c;
    printf("\n Coordenada selecionada: l %d, c %d \n", lin, col);
    abrirCampo(lin, col, camp);
}

//função responsavél pelo tempo da partida
time_t tempoCorrido(time_t t_inicial)
{
    float tempo;
    time_t t_final;
    t_final = time(NULL);
    tempo = difftime(t_final, t_inicial);
    return tempo;
}

//recebe do user as coordenadas para jogo
void jogar(Campo *camp, time_t t_inicial)
{
    int l, c, ajud = 0;
    float tempo;

    inicia(camp);
    do
    {
        imprimeCampo(camp);
        do
        {
            printf("\nDigite as coordenadas [linha, coluna]:\n");
            scanf("%d %d", &l, &c);

            if (l == 0 && c == -1)
            {
                //chama a função de tempo
                tempo = tempoCorrido(t_inicial);
                printf("O tempo do jogo é: %.0fs\n", tempo);
            }

            if (l == 0 && c == -2 && ajud < 3)
            {
                ajuda(camp);
                ajud++;

                if (ganhou(camp) == 0)
                {
                    break;
                }

                else
                {
                    imprimeCampo(camp);
                }
            }

            if (l == 0 && c == -2 && ajud >= 3)
            {
                printf("\nVocê esgotou a quantidade de ajuda por jogo!\n");
            }

            if (camp->cel[l][c].estado == 1)
            {
                printf("\nCoordenada já aberta.\n");
            }

        }
        while (coordenadaValida(l, c) == 0 || camp->cel[l][c].estado == 1 || (l == 0 && c == -2));

        abrirCampo(l, c, camp);
    }
    while (ganhou(camp) != 0 && camp->cel[l][c].mina == 0);

    imprimeCampo(camp);

    if (camp->cel[l][c].mina == 1)
    {
        printf(ANSI_COLOR_RED"\n\t\t\t\t\tQUE PENA, VOCÊ PERDEU! :(\n"ANSI_COLOR_RESET);
        imprimirBomba();
    }

    else
    {
        printf(ANSI_COLOR_BLUE);
        printf("\t\t\t\t　　  ☆        ☆      ☆      ☆   \n");
        printf("\t\t\t\t        ＼     |       |     /    \n");
        printf("\n\t\t\t\t ☆─  PARABÉNS, VOCÊ GANHOU!!  -☆\n");
        printf("\t\t\t\t        /     |      |     ＼    \n");
        printf("\t\t\t\t　　  ☆       ☆      ☆       ☆   \n");
        printf(ANSI_COLOR_RESET);
    }
}

//executa o jogo de maneira automatica
void jogoAutomatico(Campo *camp)
{
    int lin, col;
    do
    {
        Posicao *pos_fechada = malloc(0);
        int tam_vetor = 0;

        for (int l = 0; l < Linha; l++)
        {
            for (int c = 0; c < Coluna; c++)
            {
                if (camp->cel[l][c]. estado == 0)
                {
                    tam_vetor += 1;
                    pos_fechada = realloc(pos_fechada, sizeof(Posicao) * tam_vetor);
                    int ult_elem = tam_vetor - 1;
                    pos_fechada[ult_elem].l = l;
                    pos_fechada[ult_elem].c = c;
                }
            }
        }

        int ind_posicao = rand() % tam_vetor;
        lin = pos_fechada[ind_posicao].l;
        col = pos_fechada[ind_posicao].c;
        printf("\n Coordenada selecionada: l %d, c %d \n", lin, col);
        abrirCampo(lin, col, camp);
        imprimeCampo(camp);
    }
    while (ganhou(camp) != 0 && camp->cel[lin][col].mina == 0);

    if (camp->cel[lin][col].mina == 1)
    {
        printf(ANSI_COLOR_RED"\n\t\t\t\t\tQUE PENA, VOCÊ PERDEU! :(\n"ANSI_COLOR_RESET);
        imprimirBomba();
    }

    else
    {
        printf(ANSI_COLOR_BLUE);
        printf("\t\t\t\t　　  ☆        ☆      ☆      ☆   \n");
        printf("\t\t\t\t        ＼     |       |     /    \n");
        printf("\n\t\t\t\t ☆─  PARABÉNS, VOCÊ GANHOU!!   -☆\n");
        printf("\t\t\t\t        /     |      |     ＼    \n");
        printf("\t\t\t\t　　  ☆       ☆      ☆       ☆   \n");
        printf(ANSI_COLOR_RESET);
    }
}

//imprime as instruções do jogo
void instrucoes()
{
    printf(ANSI_COLOR_BLUE);
    printf("---------------------OLÁ, SEGUE ABAIXO AS INSTRUÇÕES DO JOGO!----------------------\n");
    printf("|       O jogo consiste em abrir todas as posições que não possuem bombas.        |\n");
    printf("|     Você pode escolher uma posição informando a linha e coluna, nessa ordem.    |\n");
    printf("|        Você possui 3 tentativas de ajuda e pode usar-las digitando: 0 -2        |\n");
    printf("|               Ou solicitar ver o tempo de jogo digitando: 0 -1.                 |\n");
    printf("-----------------------------------------------------------------------------------\n");
    printf(ANSI_COLOR_RESET);
}

//ordena o vetor do menor para o maior
void organiza_vetor(Pessoa *pes, int tam_v)
{
    Pessoa pes_aux;
    for (int i = 0; i < tam_v; i++)
    {
        for (int j = 0; j < tam_v; j++)
        {
            if (pes[j].time >= pes[j + 1].time)
            {
                pes_aux = pes[j];
                pes[j] = pes[j + 1];
                pes[j + 1] = pes_aux;
            }
        }
    }
}

//recebe o vetor no arquivo
void faz_rank(Pessoa *pes, int cont)
{
    FILE *rank = fopen("Pontuacao.txt", "w");
    if (rank == NULL)
    {
        printf("Erro na abertura do arquivo\n");
    }

    else
    {
        for (int i = 1; i < (cont + 1); i++)
        {
            fprintf(rank, "Nome: %s \t\t\t\t\t\t Tempo: %ds\n", pes[i].nome, (int) pes[i].time);
        }
        fclose(rank);
    }
}

//imprime os dados do arquivo
void imprime_rank(int cont)
{
    //Exibe todos os jogos e tempos de games
    char *nom, nome[50];
    int i = 0;
    FILE *rank;
    rank = fopen("Pontuacao.txt", "rt");

    printf("\t----------SEGUE ABAIXO O NOME DOS JOGADORES E SEUS TEMPOS DE JOGO----------\n\n");
    if (rank == NULL)
    {
        printf("Problema na leitura do arquivo.\n");
    }

    else
    {
        while (i < cont)
        {
            nom = fgets(nome, 29, rank);
            printf("\t\t\t\t%s\n", nom);
            i++;
        }
    }
    fclose(rank);
}

//função auxiliar que chama as demais funções
void chamaFuncoes(Campo *camp, time_t t_inicial)
{
    //recebe a quant de minas e sorteia
    int q_m, aut;
    printf("\nDigite a quantidade de minas desejada: \n");
    scanf("%d", &q_m);
    inicializaCampo(camp);
    sorteiaBombas(q_m, camp);
    analizaVizinhos(camp);

    //chamar o automatico
    printf("\nModo automatico: \n");
    printf("[0] - NÃO \n");
    printf("[1] - SIM \n");
    scanf("%d", &aut);

    if (aut == 0)
    {
        jogar(camp, t_inicial);
    }

    if (aut == 1)
    {
        jogoAutomatico(camp);
    }
}

int main(int argc, char *argv[])
{
    Campo *camp;
    Pessoa *pes;
    pes = (Pessoa *) malloc(100 * sizeof(Pessoa));

    int opc, cont = 0;
    time_t t_inicial, t_final;

    camp = criaMatriz();
    do
    {
        instrucoes();

        printf("Nome: ");
        scanf("\n%[^\n]", pes[cont].nome);

        t_inicial = time(NULL);
        chamaFuncoes(camp, t_inicial);

        pes[cont].time = tempoCorrido(t_inicial);
        printf("\nO tempo de execução do jogo foi %.0fs\n\n", pes[cont].time);
        cont++;

        printf("Digite 1 para jogar novamente: \n");
        scanf("%d", &opc);
    }
    while (opc == 1);

    organiza_vetor(pes, cont);
    faz_rank(pes, cont);
    imprime_rank(cont);
    destroiCampo(&camp);
}