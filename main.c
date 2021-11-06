#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#define BOMBA '@'
#define ASTERISCO '*'
#define CERQUILHA '#'
#define VAZIO '\0'
#define TRUE 1
#define FALSE 0
#define CONTINUAR 0
#define PERDEU -1
#define GANHOU 1
#define ACERTOU  1
#define ERROU  0
#define QNTD_PONTOS_PRA_GANHAR 5

typedef struct
{
    int tamanhoMapa;
    int quantidadeBombas;
    int quantidadePalavras;
    char** mapa;
    char** mapaMascarado; // # ou *
    char** palavrasAcertadas;
} JOGO;

typedef struct
{
    int x;
    int y;
} POSICAO;

char PLAYER[] = " _ ";
int pontuacao = 0;
POSICAO posJogador;
char letrasEncontradas[10];
int iLetrasEncontradas = 0;
char pernambuco[11] = "PERNAMBUCO";

int main()
{
    srand(time(NULL));

    menu();
}

void menu()
{
    char opcao = '0';

    do
    {
        system("cls");
        printf("\n\n**** Evite as bombas em Pernambuco ****\n");
        printf("\n");
        printf("[1] - Novo jogo\n");
        printf("[2] - Instrucoes\n");
        printf("[3] - Sair\n");
        printf("\n");

        opcao = getch();

        switch(opcao)
        {
        case '1':
            novoJogo();
            break;
        case '2':
            instrucoes();
            break;
        case '3':
            break;
        default:
            yellow();
            printf("Opcao invalida");
            reset();
            getch();
        }
    }
    while(opcao != '3' );
}

void instrucoes()
{
    system("cls");
    printf("\n\n**** Instrucoes ****\n");
    printf("\n");
    printf("     W - Cima\n");
    printf("     S - Baixo\n");
    printf("     A - Esquerda\n");
    printf("     D - Direita\n");
    printf("     P - Digitar uma palavra\n");
    printf(" ENTER - Abrir uma casa / Confirmacao\n");
    printf("ESPACO - Abrir uma casa\n");
    printf("\n");

    printf("Digite ENTER para voltar ao menu\n");
    getch();
}

void novoJogo()
{

    char opcao = '0';
    JOGO jogo;

    do
    {
        system("cls");
        printf("\n\n**** Novo Jogo ****\n");
        printf("\n");
        printf("[1] - Facil (8x8) - 5 bombas\n");
        printf("[2] - Medio (12x12) - 10 bombas\n");
        printf("[3] - Dificil (16x16) - 15 bombas\n");
        printf("[4] - Voltar ao menu\n");
        printf("\n");

        opcao = getch();

        switch(opcao)
        {
        case '1':
            jogo.quantidadeBombas = 5;
            jogo.tamanhoMapa = 8;
            jogar(jogo);
            return;
        case '2':
            jogo.quantidadeBombas = 10;
            jogo.tamanhoMapa = 12;
            jogar(jogo);
            return;
        case '3':
            jogo.quantidadeBombas = 15;
            jogo.tamanhoMapa = 16;
            jogar(jogo);
            return;
        case '4':
            return;
        default:
            yellow();
            printf("Opcao invalida");
            reset();
            getch();
        }
    }
    while(opcao != '4' || opcao != '3' || opcao != '2' || opcao != '1');
}

void jogar(JOGO *jogo)
{

    // reseta os valores toda vez que iniciar um novo jogo
    int resultadoFinal = 0;
    int resultado = 0;
    pontuacao = 0;
    posJogador.x = 0;
    posJogador.y = 0;
    iLetrasEncontradas = 0;
    jogo->quantidadePalavras = 0;

    char **palavras = (char **)malloc(QNTD_PONTOS_PRA_GANHAR * sizeof(char*));
    for(int i = 0; i < QNTD_PONTOS_PRA_GANHAR; i++) palavras[i] = (char*) malloc(11 * sizeof(char));

    jogo->palavrasAcertadas = palavras;

    for(int i = 0; i < arrayLength(letrasEncontradas); i++)
    {
        letrasEncontradas[i] = VAZIO;
    }

    // carrega o mapa com as bombas
    criarMapa(jogo);

    // cria uma mascara para o mapa original
    criarMapaMascarado(jogo);

    do
    {
        resultadoFinal = resultado;
        system("cls");

        printf("\n Palavras ja usadas: \n-");
        for(int i = 0; i < jogo->quantidadePalavras; i++)
        {
            printf(" %s -", jogo->palavrasAcertadas[i]);
        }

        printf("\n\n\n");

        for(int y = 0; y < jogo->tamanhoMapa; y++)
        {
            for(int x = 0; x < jogo->tamanhoMapa; x++)
            {
                // Mostra o cursor do player na posicao atual do mesmo
                if(x == posJogador.x && y == posJogador.y && resultado != PERDEU)
                {
                    printf(PLAYER);
                }
                else
                {
                    // mostra os valores do mapa original,
                    // caso no mapa mascarado estiver com * na mesma posicao
                    if(jogo->mapaMascarado[y][x] == ASTERISCO)
                    {
                        if(jogo->mapa[y][x] == BOMBA)
                        {
                            red();
                        }
                        else if(jogo->mapa[y][x] >= 'A' && jogo->mapa[y][x] <= 'Z')
                        {
                            green();
                        }
                        else
                        {
                            reset();
                        }

                        printf(" %c ", jogo->mapa[y][x]);
                        reset();
                    }
                    else
                    {
                        printf(" %c ", jogo->mapaMascarado[y][x]);
                    }
                }
            }

            printf("\n");
        }

        if(resultado == 0)
            resultado = executarAcao(jogo);
    }
    while (resultadoFinal != PERDEU && (pontuacao < QNTD_PONTOS_PRA_GANHAR || resultadoFinal == CONTINUAR));

    if(resultadoFinal == GANHOU)
    {
        green();
        printf("\n\n****** GANHOU! ******\n\n");
        reset();
        system("pause");
    }
    else if(resultadoFinal == PERDEU)
    {
        red();
        printf("\n\n!!!!!! PERDEU! !!!!!!!\n\n");
        reset();
        system("pause");
    }

    free(jogo->mapa);
    free(jogo->mapaMascarado);
    free(jogo->palavrasAcertadas);
}

void criarMapa(JOGO* jogo)
{

    int qntPosLivre = jogo->tamanhoMapa * jogo->tamanhoMapa;
    POSICAO posicoesLivres[qntPosLivre];

    char **mapa = (char **)malloc(jogo->tamanhoMapa * sizeof(char*));
    for(int i = 0; i < jogo->tamanhoMapa; i++) mapa[i] = (char*) malloc(jogo->tamanhoMapa * sizeof(char));

    jogo->mapa = mapa;

    // Cria o mapa vazio (com espacos em branco)
    for(int y = 0; y < jogo->tamanhoMapa; y++)
    {
        for(int x = 0; x < jogo->tamanhoMapa; x++)
        {
            jogo->mapa[y][x] = ' ';

            // Salva as posicoes livres do Mapa em um array unidimencional
            posicoesLivres[y * jogo->tamanhoMapa + x].x = x;
            posicoesLivres[y * jogo->tamanhoMapa + x].y = y;
        }
    }

    // Coloca as bombas aleatoriamente no Mapa
    for(int i = 0; i < jogo->quantidadeBombas; i++)
    {
        int x = rand() % jogo->tamanhoMapa;
        int y = rand() % jogo->tamanhoMapa;

        jogo->mapa[y][x] = BOMBA;
        posicoesLivres[y * jogo->tamanhoMapa + x].x = -1;
        posicoesLivres[y * jogo->tamanhoMapa + x].y = -1;
        qntPosLivre--;
    }

    // Conta e coloca os numeros no Mapa
    for(int y = 0; y < jogo->tamanhoMapa; y++)
    {
        for(int x = 0; x < jogo->tamanhoMapa; x++)
        {
            int contadorBombas = 0;

            if(jogo->mapa[y][x] != BOMBA)
            {

                // conta +1 para cada bomba ao redor da posicao (x,y)
                // sem ultrapassar os limites do Mapa
                if((y+1 < jogo->tamanhoMapa) && (jogo->mapa[y+1][x+0] == BOMBA))
                {
                    contadorBombas++;
                }

                if((y+1 < jogo->tamanhoMapa) && (x+1 < jogo->tamanhoMapa) && (jogo->mapa[y+1][x+1] == BOMBA))
                {
                    contadorBombas++;
                }

                if((y-1 >= 0) && (x+1 < jogo->tamanhoMapa) && (jogo->mapa[y-1][x+1] == BOMBA))
                {
                    contadorBombas++;
                }

                if((y-1 >= 0) && (jogo->mapa[y-1][x+0] == BOMBA))
                {
                    contadorBombas++;
                }

                if((y-1 >= 0) && (x-1 >= 0) && (jogo->mapa[y-1][x-1] == BOMBA))
                {
                    contadorBombas++;
                }

                if((x-1 >= 0) && (jogo->mapa[y+0][x-1] == BOMBA))
                {
                    contadorBombas++;
                }

                if((x+1 < jogo->tamanhoMapa) && (jogo->mapa[y+0][x+1] == BOMBA))
                {
                    contadorBombas++;
                }

                if((y+1 < jogo->tamanhoMapa) && (x-1 >= 0) && (jogo->mapa[y+1][x-1] == BOMBA))
                {
                    contadorBombas++;
                }

                if(contadorBombas > 0)
                {
                    posicoesLivres[y * jogo->tamanhoMapa + x].x = -1;
                    posicoesLivres[y * jogo->tamanhoMapa + x].y = -1;
                    qntPosLivre--;
                    jogo->mapa[y][x] = contadorBombas + '0';
                }
            }
        }
    }

    // Cria um novo array para salvar as posicoes livres restantes no Mapa,
    // Apos o preenchimento das bombas e dos numeros
    POSICAO posLivres[qntPosLivre];
    for(int i = 0, j = 0; i < jogo->tamanhoMapa * jogo->tamanhoMapa; i++)
    {
        if(posicoesLivres[i].x != -1 && posicoesLivres[i].y != -1)
        {
            posLivres[j].x = posicoesLivres[i].x;
            posLivres[j].y = posicoesLivres[i].y;
            j++;
        }
    }

    // Gera 1 numero aleatorio diferente para cada letra contida em 'pernambuco'
    // para usar no array com as posicoes livres restantes
    int qntNumerosAleatorios = strlen(pernambuco);
    int iSorteado = 0, numerosAleatorios[qntNumerosAleatorios];
    do
    {
        numerosAleatorios[iSorteado] = rand() % qntPosLivre;
        int igual = 0;
        for(int j = 0; j < iSorteado; j++)
        {
            if(numerosAleatorios[j] == numerosAleatorios[iSorteado])
                igual = TRUE;
        }

        if(igual == FALSE)
            iSorteado++;
    }
    while(iSorteado < qntNumerosAleatorios);


    // para cada numero aleatorio gerado
    // pega uma posicao livre das restantes
    // para inserir uma letra contida em 'pernambuco'
    for(int i = 0; i < qntNumerosAleatorios; i++)
    {
        int y = posLivres[numerosAleatorios[i]].y;
        int x = posLivres[numerosAleatorios[i]].x;

        jogo->mapa[y][x] = pernambuco[i];
    }
}

void criarMapaMascarado(JOGO* jogo)
{

    char **mapaMascarado = (char **)malloc(jogo->tamanhoMapa * sizeof(char*));
    for(int i = 0; i < jogo->tamanhoMapa; i++) mapaMascarado[i] = (char*) malloc(jogo->tamanhoMapa * sizeof(char));

    jogo->mapaMascarado = mapaMascarado;

    // Iniciando o Mapa Mascarado com Cerquilhas (#)
    for(int x = 0; x < jogo->tamanhoMapa; x++)
    {
        for(int y = 0; y < jogo->tamanhoMapa; y++)
        {
            jogo->mapaMascarado[x][y] = CERQUILHA;
        }
    }
}

// 0 = continua, -1 = perdeu
int mostrarPosicao(
    int x, int y,
    JOGO* jogo)
{

    jogo->mapaMascarado[y][x] = ASTERISCO;

    if(jogo->mapa[y][x] == BOMBA)
    {
        return PERDEU;
    }
    else
    {
        // Se na posicao (x,y) tem uma letra,
        // add a letra no array de letras encontradas
        for(int i = 0; i < strlen(pernambuco); i++)
        {
            if(pernambuco[i] == jogo->mapa[y][x])
            {
                letrasEncontradas[iLetrasEncontradas] = jogo->mapa[y][x];
                iLetrasEncontradas++;
            }
        }

        return CONTINUAR;
    }
}

// -1 = perdeu, 0 = continua, 1 = ganhou
int executarAcao(JOGO* jogo)
{
    char opcao = getch();

    switch(opcao)
    {
    case 'W':
    case 'w':
    {
        if((posJogador.y-1) >= 0)
        {
            posJogador.y--;
        }
        return CONTINUAR;
    }
    case 'S':
    case 's':
    {
        if((posJogador.y+1) <= (jogo->tamanhoMapa - 1))
        {
            posJogador.y++;
        }
        return CONTINUAR;
    }
    case 'A':
    case 'a':
    {
        if((posJogador.x-1) >= 0)
        {
            posJogador.x--;
        }
        return CONTINUAR;
    }
    case 'D':
    case 'd':
    {
        if((posJogador.x+1) <= (jogo->tamanhoMapa - 1))
        {
            posJogador.x++;
        }
        return CONTINUAR;
    }
    case 'P':
    case 'p':
    {

        char palavra[] = {VAZIO, VAZIO, VAZIO,VAZIO, VAZIO, VAZIO,VAZIO, VAZIO, VAZIO,VAZIO};
        printf("\nDigite a palavra: ");
        fflush(stdin);
        scanf("%s", &palavra);


        int palavraEstaValida = validarPalavra(palavra);

        if(palavraEstaValida == TRUE)
        {
            int resultado = consultarPalavra(jogo, palavra);
            return marcarPontuacao(resultado);
        }

        return CONTINUAR;
    }
    case '\r':
    case ' ':
    {
        return mostrarPosicao(posJogador.x, posJogador.y, jogo);
    }
    default:
        return CONTINUAR;
    }
}

int validarPalavra(char* palavra)
{

    int i = 0;
    while( palavra[i] != VAZIO )
    {
        palavra[i] = toupper(palavra[i]);
        int encontrou = FALSE;
        for(int j = 0; j < iLetrasEncontradas; j++)
        {
            if(palavra[i] == letrasEncontradas[j])
            {
                encontrou = TRUE;
            }
        }

        if(encontrou == FALSE)
        {
            yellow();
            printf("\n\nEncontre todas as letras da palavra!\n");
            reset();
            getch();
            return FALSE;
        }

        i++;
    }

    return TRUE;
}

int arrayLength(char * arr)
{
    return (int)( sizeof(arr) / sizeof(arr[0]));
}

FILE* abrirAquivo(char* nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "r");

    return arquivo;
}

// 0 = continua, 1 = ganhou
int consultarPalavra(JOGO* jogo, char* palavra)
{
    for(int i = 0; i < jogo->quantidadePalavras; i++)
    {
        if (strcmp(jogo->palavrasAcertadas[i],palavra) == 0)
        {
            yellow();
            printf("\n\nEssa palavra ja foi usada, tente outra\n");
            reset();
            getch();
            return CONTINUAR;
        }
    }

    FILE* arquivoPalavras = abrirAquivo("./palavras.txt");

    if(arquivoPalavras == NULL)
    {
        yellow();
        printf("\n\nNão foi possível carregar as palavras");
        reset();
        getch();
    }
    else
    {
        while(!feof(arquivoPalavras))
        {
            char palavraArquivo[11];
            fgets(palavraArquivo, 11, arquivoPalavras);

            // remove o \n do final da palavra vinda do arquivo
            size_t ln = strlen(palavraArquivo) - 1;
            if (*palavraArquivo && palavraArquivo[ln] == '\n')
                palavraArquivo[ln] = VAZIO;

            // Se encontrou a palavra digita no arquivo, então
            // verifica se a pontuação foi alcançada e se foi ACERTOU
            if(strcmp(palavraArquivo, palavra) == 0)
            {
                strcpy(jogo->palavrasAcertadas[jogo->quantidadePalavras],palavra);
                jogo->quantidadePalavras++;

                close(arquivoPalavras);

                return ACERTOU;
            }
        }
        close(arquivoPalavras);
    }

    return ERROU;

}

int marcarPontuacao (int resultado)
{
    if (resultado == ACERTOU)
    {
        pontuacao = pontuacao + 1;
        if (pontuacao < QNTD_PONTOS_PRA_GANHAR)
        {
            blue();
            printf("\n\nVoce acertou %i palavras, acerte mais %i palavras para ganhar!!\n\n", pontuacao, (QNTD_PONTOS_PRA_GANHAR-pontuacao));
            reset();
            getch();

            return CONTINUAR;
        }
        else
        {
            return GANHOU;
        }
    }
    else
    {
        return CONTINUAR;
    }

}

void red()
{
    printf("\033[0;31m");
}

void green()
{
    printf("\033[0;32m");
}

void yellow()
{
    printf("\033[0;33m");
}

void blue()
{
    printf("\033[0;36m");
}

void reset ()
{
    printf("\033[0m");
}

