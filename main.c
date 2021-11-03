#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

typedef struct
{
    int tamanhoMapa;
    int quantidadeBombas;
    char** mapa;
    char** mapaMascarado; // # ou *
} JOGO;

typedef struct
{
    int x;
    int y;
} POSICAO;

char BOMBA = '@';
char ASTERISCO = '*';
char CERQUILHA = '#';
char VAZIO = '\0';
int TRUE = 1;
int FALSE = 0;
int CONTINUAR = 0;
int PERDEU = -1;
int GANHOU = 1;
char PLAYER[] = " _ ";

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
        printf("\n\n**** Campo Minado ****\n");
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
            printf("Opcao invalida");
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
        printf("[2] - Medio (12x12) - 15 bombas\n");
        printf("[3] - Dificil (16x16) - 25 bombas\n");
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
            printf("Opcao invalida");
        }
    }
    while(opcao != '4' || opcao != '3' || opcao != '2' || opcao != '1');
}

void jogar(JOGO *jogo)
{

    // reseta os valores toda vez que iniciar um novo jogo
    int ganhou = 0;
    int resultado = 0;
    posJogador.x = 0;
    posJogador.y = 0;
    iLetrasEncontradas = 0;
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
        ganhou = resultado;
        system("cls");

        for(int y = 0; y < jogo->tamanhoMapa; y++)
        {
            for(int x = 0; x < jogo->tamanhoMapa; x++)
            {
                // Mostra o cursor do player na posicao atual do mesmo
                if(x == posJogador.x && y == posJogador.y && resultado != -1)
                {
                    printf(PLAYER);
                }
                else
                {
                    // mostra os valores do mapa original,
                    // caso no mapa mascarado estiver com * na mesma posicao
                    if(jogo->mapaMascarado[y][x] == ASTERISCO)
                    {
                        printf(" %c ", jogo->mapa[y][x]);
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
        {
            resultado = executarAcao(jogo);
        }
    }
    while (ganhou == 0);

    if(ganhou == 1)
    {
        printf("\n\n****** GANHOU! ******\n\n");

        system("pause");
        getch();

    }
    else if(ganhou == -1)
    {
        printf("\n\n!!!!!! PERDEU! !!!!!!!\n\n");
        system("pause");
        getch();

    }

    free(jogo->mapa);
    free(jogo->mapaMascarado);
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

                if(contadorBombas != 0)
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
                igual = 1;
        }

        if(igual == 0)
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
    case 'w':
    {
        if((posJogador.y-1) >= 0)
        {
            posJogador.y--;
        }
        return CONTINUAR;
    }
    case 's':
    {
        if((posJogador.y+1) <= (jogo->tamanhoMapa - 1))
        {
            posJogador.y++;
        }
        return CONTINUAR;
    }
    case 'a':
    {
        if((posJogador.x-1) >= 0)
        {
            posJogador.x--;
        }
        return CONTINUAR;
    }
    case 'd':
    {
        if((posJogador.x+1) <= (jogo->tamanhoMapa - 1))
        {
            posJogador.x++;
        }
        return CONTINUAR;
    }
    case 'p':
    {

        char palavra[] = {VAZIO, VAZIO, VAZIO,VAZIO, VAZIO, VAZIO,VAZIO, VAZIO, VAZIO,VAZIO};
        printf("Digite a palavra: ");
        scanf("%s", &palavra);

        int palavraEstaValida = validarPalavra(palavra);

        if(palavraEstaValida == TRUE)
        {
            return consultarPalavra(palavra);
        }

        return CONTINUAR;
    }
    case '\r':
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
            printf("\nEncontre todas as letras da palavra!\n");
            system("pause");
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
int consultarPalavra(char * palavra)
{

    FILE* arquivoPalavras = abrirAquivo("./palavras.txt");

    if(arquivoPalavras == NULL)
    {
        printf("Não foi possível carregar as palavras");
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
            // GANHOU
            if(strcmp(palavraArquivo, palavra) == 0)
            {
                close(arquivoPalavras);
                return GANHOU;
            }
        }
        close(arquivoPalavras);
    }

    return CONTINUAR;
}


