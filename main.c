#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

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
    char** palavras;
} dicionario;

typedef struct
{
    int x;
    int y;
} posicao;

posicao posJogador;



int main()
{
    srand(time(NULL));

    menu();
}

FILE* abrirAquivo(char* nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "r");

    return arquivo;
}

int consultarPalavra(char * palavra) {

    FILE* arquivoPalavras = abrirAquivo("./palavras.txt");

    //char palavras[111][9];

    if(arquivoPalavras == NULL) {
        printf("Não foi possível carregar as palavras");
    }else {

        int i = 0;
        while(!feof(arquivoPalavras)) {
            char palavraArquivo[9];
            fgets(palavraArquivo, 9, arquivoPalavras);

            size_t ln = strlen(palavraArquivo) - 1;
            if (*palavraArquivo && palavraArquivo[ln] == '\n')
                palavraArquivo[ln] = '\0';

            if(strcmp(palavraArquivo, palavra) == 0) {
                return 1;
            }
        }
        close(arquivoPalavras);
    }

    return 0;
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
        printf("[2] - Sair\n");
        printf("\n");

        opcao = getch();

        switch(opcao)
        {
        case '1':
            novoJogo();
            break;
        case '2':
            break;
        default:
            printf("Opcao invalida");
        }
    }
    while(opcao != '2' );
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
        printf("[1] - Facil (8x8) - 5 bombas\n");
        printf("[2] - Medio (12x12) - 15 bombas\n");
        printf("[3] - Dificil (16x16) - 25 bombas\n");
        printf("[4] - Voltar ao menu\n");
        printf("\n");

        opcao = getch();

        switch(opcao)
        {
        case '1':
            gameObject.quantidadeBombas = 5;
            gameObject.tamanhoMapa = 8;
            jogar(gameObject);
            break;
        case '2':
            gameObject.quantidadeBombas = 15;
            gameObject.tamanhoMapa = 12;
            jogar(gameObject);
            break;
        case '3':
            gameObject.quantidadeBombas = 25;
            gameObject.tamanhoMapa = 16;
            jogar(gameObject);
            break;
        case '4':
            return;
        default:
            printf("Opcao invalida");
        }
    }
    while(opcao != '4' || opcao != '3' || opcao != '2' || opcao != '1');
}

void jogar(game *gameObject)
{
    int ganhou = 0;
    posJogador.x = 0;
    posJogador.y = 0;

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

int mostrarPosicao(
    int x, int y,
    game* gameObject) {


    if(gameObject->mapa[x][y] != BOMBA)
    {
        gameObject->mapaMascarado[x][y] = ASTERISCO;
        return 1;
    }
    else
    {
        gameObject->mapaMascarado[x][y] = ASTERISCO;
        return 0;
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

        int ganhou = consultarPalavra(palavra);

        if(ganhou == 1) {
            printf("GANHOU!\n\n");

            system("pause");
            getch();

            menu();
        }

        break;
    }
    case '\r':
    {
        int perdeu = mostrarPosicao(posJogador.x, posJogador.y, gameObject);

        if(perdeu == 0) {
            printf("PERDEU!\n\n");
            system("pause");
            getch();
            menu();
        }
    }
    }
}


