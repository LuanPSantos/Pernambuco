#include<stdio.h>
#include <stdlib.h>
#include<conio.h>

char BOMBA = '@';
char ASTERISCO = '*';
char CERQUILHA = '#';
char VAZIO = '0';

typedef struct
{
    int tamanhoMapa;
    int quantidadeBombas;
    char** mapa;
    char** mapaMascarado; // # e *
} game;

typedef struct
{
    int x;
    int y;
} posicao;

posicao posJogador;

int main()
{
    posJogador.x = 0;
    posJogador.y = 0;
    srand(time(NULL));

    menu();
}

FILE* abrirAquivo(char* nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "a");

    return arquivo;
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
        printf("[2] - Recordes\n");
        printf("[3] - Sair\n");
        printf("\n");

        opcao = getch();

        switch(opcao)
        {
        case '1':
            novoJogo();
            break;
        case '2':
            recordes();
            break;
        case '3':
            break;
        default:
            printf("Opcao invalida");
        }
    }
    while(opcao != '3' );
}

void novoJogo()
{

    char opcao = '0';
    game gameObject;


    do
    {
        system("cls");
        printf("\n\n**** Novo Jogo ****\n");
        printf("\n");
        printf("[1] - Facil (8x8) - 13 bombas\n");
        printf("[2] - Médio (12x12) - 28 bombas\n");
        printf("[3] - Difícil (16x16) - 51 bombas\n");
        printf("[4] - Voltar ao menu\n");
        printf("\n");

        opcao = getch();

        switch(opcao)
        {
        case '1':
            gameObject.quantidadeBombas = 3;
            gameObject.tamanhoMapa = 8;
            jogar(gameObject);
            break;
        case '2':
            gameObject.quantidadeBombas = 28;
            gameObject.tamanhoMapa = 12;
            jogar(gameObject);
            break;
        case '3':
            gameObject.quantidadeBombas = 51;
            gameObject.tamanhoMapa = 16;
            jogar(gameObject);
            break;
        case '4':
            break;
        default:
            printf("Opcao invalida");
        }
    }
    while(opcao != '4' || opcao != '3' || opcao != '2' || opcao != '1');
}

void jogar(game *gameObject)
{
    int ganhou = 0;

    // carrega o mapa com as bombas
    criarMapa(gameObject);

    // cria uma mascara para o mapa original

    criarMapaMascarado(gameObject);

    do
    {
        system("cls");
        // mostra os valores do mapa original,
        // caso na mascara estiver com * na mesma
        // posicao
        for(int x = 0; x < gameObject->tamanhoMapa; x++)
        {
            for(int y = 0; y < gameObject->tamanhoMapa; y++)
            {
                if(x == posJogador.x && y == posJogador.y)
                {
                    printf(" _ ");
                }
                else
                {
                    if(gameObject->mapaMascarado[x][y] == ASTERISCO)
                    {
                        printf(" %c ", gameObject->mapa[x][y]);
                    }
                    else
                    {
                        printf(" %c ", gameObject->mapaMascarado[x][y]);
                    }
                }

            }

            printf("\n");
        }

        clicaNoMapa(gameObject);
    }
    while (ganhou != 1);

    free(gameObject->mapa);
    free(gameObject->mapaMascarado);
}

void criarMapa(game* gameObject)
{

    int qntPosLivre = gameObject->tamanhoMapa * gameObject->tamanhoMapa;
    posicao posicoesLivres[qntPosLivre];

    char **mapa = (char **)malloc(gameObject->tamanhoMapa * sizeof(char*));
    for(int i = 0; i < gameObject->tamanhoMapa; i++) mapa[i] = (char*) malloc(gameObject->tamanhoMapa * sizeof(char));

    gameObject->mapa = mapa;

    // Cria o mapa vazio
    for(int y = 0; y < gameObject->tamanhoMapa; y++)
    {
        for(int x = 0; x < gameObject->tamanhoMapa; x++)
        {
            gameObject->mapa[y][x] = ' ';

            posicoesLivres[y * gameObject->tamanhoMapa + x].x = x;
            posicoesLivres[y * gameObject->tamanhoMapa + x].y = y;
        }
    }

    // Coloca as bombas
    for(int i = 0; i < gameObject->quantidadeBombas; i++)
    {
        int x = rand() % gameObject->tamanhoMapa;
        int y = rand() % gameObject->tamanhoMapa;

        gameObject->mapa[y][x] = BOMBA;
        posicoesLivres[y * gameObject->tamanhoMapa + x].x = -1;
        posicoesLivres[y * gameObject->tamanhoMapa + x].y = -1;
        qntPosLivre--;
    }

    // Coloca os numeros
    for(int x = 0; x < gameObject->tamanhoMapa; x++)
    {
        for(int y = 0; y < gameObject->tamanhoMapa; y++)
        {
            int contadorBombas = 0;

            if(gameObject->mapa[x][y] != BOMBA)
            {

                if((x+1 < gameObject->tamanhoMapa) && (gameObject->mapa[x+1][y+0] == BOMBA))
                {
                    contadorBombas++;
                }

                if((x+1 < gameObject->tamanhoMapa) && (y+1 < gameObject->tamanhoMapa) && (gameObject->mapa[x+1][y+1] == BOMBA))
                {
                    contadorBombas++;
                }

                if((x-1 >= 0) && (y+1 < gameObject->tamanhoMapa) && (gameObject->mapa[x-1][y+1] == BOMBA))
                {
                    contadorBombas++;
                }

                if((x-1 >= 0) && (gameObject->mapa[x-1][y+0] == BOMBA))
                {
                    contadorBombas++;
                }

                if((x-1 >= 0) && (y-1 >= 0) && (gameObject->mapa[x-1][y-1] == BOMBA))
                {
                    contadorBombas++;
                }

                if((y-1 >= 0) && (gameObject->mapa[x+0][y-1] == BOMBA))
                {
                    contadorBombas++;
                }

                if((y+1 < gameObject->tamanhoMapa) && (gameObject->mapa[x+0][y+1] == BOMBA))
                {
                    contadorBombas++;
                }

                if((x+1 < gameObject->tamanhoMapa) && (y-1 >= 0) && (gameObject->mapa[x+1][y-1] == BOMBA))
                {
                    contadorBombas++;
                }

                if(contadorBombas != 0) {
                    posicoesLivres[x * gameObject->tamanhoMapa + y].x = -1;
                    posicoesLivres[x * gameObject->tamanhoMapa + y].y = -1;
                    qntPosLivre--;
                    gameObject->mapa[x][y] = contadorBombas + VAZIO;
                }


            }
        }
    }

    posicao posLivres[qntPosLivre];
    for(int i = 0, j = 0; i < gameObject->tamanhoMapa * gameObject->tamanhoMapa; i++) {
        if(posicoesLivres[i].x != -1 && posicoesLivres[i].y != -1) {
            posLivres[j].x = posicoesLivres[i].x;
            posLivres[j].y = posicoesLivres[i].y;
            j++;
        }
    }

    char palavra[10] = "PERNAMBUCO";

    int iSorteado = 0, numerosAleatorios[10];
    do{
        numerosAleatorios[iSorteado] = rand() % qntPosLivre;
        int igual = 0;
        for(int j = 0; j < iSorteado; j++){
            if(numerosAleatorios[j] == numerosAleatorios[iSorteado])
                igual = 1;
        }

        if(igual == 0)
            iSorteado++;
    }while(iSorteado < 10);


    for(int i = 0; i < 10; i++)
    {
        gameObject->mapa[posLivres[numerosAleatorios[i]].y][posLivres[numerosAleatorios[i]].x] = palavra[i];
    }

}

void criarMapaMascarado(game* gameObject)
{

    char **mapaMascarado = (char **)malloc(gameObject->tamanhoMapa * sizeof(char*));
    for(int i = 0; i < gameObject->tamanhoMapa; i++) mapaMascarado[i] = (char*) malloc(gameObject->tamanhoMapa * sizeof(char));

    gameObject->mapaMascarado = mapaMascarado;

    for(int x = 0; x < gameObject->tamanhoMapa; x++)
    {
        for(int y = 0; y < gameObject->tamanhoMapa; y++)
        {
            gameObject->mapaMascarado[x][y] = CERQUILHA;
        }
    }
}

void clicaNoMapa(game* gameObject)
{
    char opcao = getch();

    switch(opcao)
    {
    case 'w':
    {
        if((posJogador.x-1) >= 0)
        {
            posJogador.x--;
        }
        break;
    }
    case 's':
    {
        if((posJogador.x+1) <= (gameObject->tamanhoMapa - 1))
        {
            posJogador.x++;
        }
        break;
    }
    case 'a':
    {
        if((posJogador.y-1) >= 0)
        {
            posJogador.y--;
        }
        break;
    }
    case 'd':
    {
        if((posJogador.y+1) <= (gameObject->tamanhoMapa - 1))
        {
            posJogador.y++;
        }
        break;
    }
    case 'p':
    {

        char palavra[10];
        printf("Digite a palavra: ");
        scanf("%s", &palavra);

        printf("\nHUUUM %s", palavra);
        break;
    }
    case '\r':
    {
        mostrarPosicao(posJogador.x, posJogador.y, gameObject);
    }
    }


}

void mostrarPosicao(
    int x, int y,
    game* gameObject) {


    if(gameObject->mapa[x][y] != BOMBA)
    {
        gameObject->mapaMascarado[x][y] = ASTERISCO;
    }
    else
    {
        gameObject->mapaMascarado[x][y] = ASTERISCO;
    }
}
/*
void mostrarPosicao(
    int x, int y,
    game* gameObject)
{
    if(gameObject->mapaMascarado[x][y] == ASTERISCO)
    {
        return;
    }

    if(gameObject->mapa[x][y] != BOMBA)
    {
        gameObject->mapaMascarado[x][y] = ASTERISCO;
    }
    else
    {
        gameObject->mapaMascarado[x][y] = ASTERISCO;
    }

    if(gameObject->mapa[x][y] == VAZIO)
    {
        if(x+1 < gameObject->tamanhoMapa)
        {
            mostrarPosicao(x + 1, y + 0, gameObject);
        }

        if((x+1 < gameObject->tamanhoMapa) && (y+1 < gameObject->tamanhoMapa))
        {
            mostrarPosicao(x + 1, y + 1, gameObject);
        }

        if((x-1 >= 0) && (y+1 < gameObject->tamanhoMapa))
        {
            mostrarPosicao(x - 1, y + 1, gameObject);
        }

        if((x-1 >= 0))
        {
            mostrarPosicao(x - 1, y + 0, gameObject);
        }

        if((x-1 >= 0) && (y-1 >= 0))
        {
            mostrarPosicao(x - 1, y - 1, gameObject);
        }

        if((y-1 >= 0))
        {
            mostrarPosicao(x + 0, y - 1, gameObject);
        }

        if((y+1 < gameObject->tamanhoMapa))
        {
            mostrarPosicao(x + 0, y + 1, gameObject);
        }

        if((x+1 < gameObject->tamanhoMapa) && (y-1 >= 0))
        {
            mostrarPosicao(x + 1, y - 1, gameObject);
        }
    }
}*/

void recordes()
{
}


