#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

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
char letrasEncontradas[10];
int iLetrasEncontradas = 0;
char pernambuco[11] = "PERNAMBUCO";

int main()
{
    srand(time(NULL));

    menu();
}

int arrayLength(char * arr) {
    return (int)( sizeof(arr) / sizeof(arr[0]));
}

FILE* abrirAquivo(char* nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "r");

    return arquivo;
}

// 0 = continua, 1 = ganhou
int consultarPalavra(char * palavra) {

    FILE* arquivoPalavras = abrirAquivo("./palavras.txt");

    //char palavras[111][9];

    if(arquivoPalavras == NULL) {
        printf("Não foi possível carregar as palavras");
    }else {

        while(!feof(arquivoPalavras)) {
            char palavraArquivo[11];
            fgets(palavraArquivo, 11, arquivoPalavras);

            // remove o \n do final da palavra vinda do arquivo
            size_t ln = strlen(palavraArquivo) - 1;
            if (*palavraArquivo && palavraArquivo[ln] == '\n')
                palavraArquivo[ln] = VAZIO;

            if(strcmp(palavraArquivo, palavra) == 0) {
                close(arquivoPalavras);
                return GANHOU;
            }
        }
        close(arquivoPalavras);
    }

    return CONTINUAR;
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

void instrucoes() {
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
            return;
        case '2':
            gameObject.quantidadeBombas = 10;
            gameObject.tamanhoMapa = 12;
            jogar(gameObject);
            return;
        case '3':
            gameObject.quantidadeBombas = 15;
            gameObject.tamanhoMapa = 16;
            jogar(gameObject);
            return;
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

    // reseta os valores toda vez que iniciar um novo jogo
    int ganhou = 0;
    int resultado = 0;
    posJogador.x = 0;
    posJogador.y = 0;
    iLetrasEncontradas = 0;
    for(int i = 0; i < arrayLength(letrasEncontradas); i++) {
        letrasEncontradas[i] = VAZIO;
    }

    // carrega o mapa com as bombas
    criarMapa(gameObject);

    // cria uma mascara para o mapa original
    criarMapaMascarado(gameObject);

    do
    {
        ganhou = resultado;
        system("cls");

        for(int y = 0; y < gameObject->tamanhoMapa; y++)
        {
            for(int x = 0; x < gameObject->tamanhoMapa; x++)
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
                    if(gameObject->mapaMascarado[y][x] == ASTERISCO)
                    {
                        printf(" %c ", gameObject->mapa[y][x]);
                    }
                    else
                    {
                        printf(" %c ", gameObject->mapaMascarado[y][x]);
                    }
                }

            }

            printf("\n");
        }

        if(resultado == 0){
            resultado = executarAcao(gameObject);
        }
    }
    while (ganhou == 0);

    if(ganhou == 1) {
        printf("\n\n****** GANHOU! ******\n\n");

        system("pause");
        getch();

    }else if(ganhou == -1) {
        printf("\n\n!!!!!! PERDEU! !!!!!!!\n\n");
        system("pause");
        getch();

    }

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

    // Cria o mapa vazio (com espacos em branco)
    for(int y = 0; y < gameObject->tamanhoMapa; y++)
    {
        for(int x = 0; x < gameObject->tamanhoMapa; x++)
        {
            gameObject->mapa[y][x] = ' ';

            // Salva as posicoes livres do Mapa em um array unidimencional
            posicoesLivres[y * gameObject->tamanhoMapa + x].x = x;
            posicoesLivres[y * gameObject->tamanhoMapa + x].y = y;
        }
    }

    // Coloca as bombas aleatoriamente no Mapa
    for(int i = 0; i < gameObject->quantidadeBombas; i++)
    {
        int x = rand() % gameObject->tamanhoMapa;
        int y = rand() % gameObject->tamanhoMapa;

        gameObject->mapa[y][x] = BOMBA;
        posicoesLivres[y * gameObject->tamanhoMapa + x].x = -1;
        posicoesLivres[y * gameObject->tamanhoMapa + x].y = -1;
        qntPosLivre--;
    }

    // Conta e coloca os numeros no Mapa
    for(int y = 0; y < gameObject->tamanhoMapa; y++)
    {
        for(int x = 0; x < gameObject->tamanhoMapa; x++)
        {
            int contadorBombas = 0;

            if(gameObject->mapa[y][x] != BOMBA)
            {

                // conta +1 para cada bomba ao redor da posicao (x,y)
                // sem ultrapassar os limites do Mapa
                if((y+1 < gameObject->tamanhoMapa) && (gameObject->mapa[y+1][x+0] == BOMBA))
                {
                    contadorBombas++;
                }

                if((y+1 < gameObject->tamanhoMapa) && (x+1 < gameObject->tamanhoMapa) && (gameObject->mapa[y+1][x+1] == BOMBA))
                {
                    contadorBombas++;
                }

                if((y-1 >= 0) && (x+1 < gameObject->tamanhoMapa) && (gameObject->mapa[y-1][x+1] == BOMBA))
                {
                    contadorBombas++;
                }

                if((y-1 >= 0) && (gameObject->mapa[y-1][x+0] == BOMBA))
                {
                    contadorBombas++;
                }

                if((y-1 >= 0) && (x-1 >= 0) && (gameObject->mapa[y-1][x-1] == BOMBA))
                {
                    contadorBombas++;
                }

                if((x-1 >= 0) && (gameObject->mapa[y+0][x-1] == BOMBA))
                {
                    contadorBombas++;
                }

                if((x+1 < gameObject->tamanhoMapa) && (gameObject->mapa[y+0][x+1] == BOMBA))
                {
                    contadorBombas++;
                }

                if((y+1 < gameObject->tamanhoMapa) && (x-1 >= 0) && (gameObject->mapa[y+1][x-1] == BOMBA))
                {
                    contadorBombas++;
                }

                if(contadorBombas != 0) {
                    posicoesLivres[y * gameObject->tamanhoMapa + x].x = -1;
                    posicoesLivres[y * gameObject->tamanhoMapa + x].y = -1;
                    qntPosLivre--;
                    gameObject->mapa[y][x] = contadorBombas + '0';
                }
            }
        }
    }

    // Cria um novo array para salvar as posicoes livres restantes no Mapa,
    // Apos o preenchimento das bombas e dos numeros
    posicao posLivres[qntPosLivre];
    for(int i = 0, j = 0; i < gameObject->tamanhoMapa * gameObject->tamanhoMapa; i++) {
        if(posicoesLivres[i].x != -1 && posicoesLivres[i].y != -1) {
            posLivres[j].x = posicoesLivres[i].x;
            posLivres[j].y = posicoesLivres[i].y;
            j++;
        }
    }

    // Gera 1 numero aleatorio diferente para cada letra contida em 'pernambuco'
    // para usar no array com as posicoes livres restantes
    int qntNumerosAleatorios = strlen(pernambuco);
    int iSorteado = 0, numerosAleatorios[qntNumerosAleatorios];
    do{
        numerosAleatorios[iSorteado] = rand() % qntPosLivre;
        int igual = 0;
        for(int j = 0; j < iSorteado; j++){
            if(numerosAleatorios[j] == numerosAleatorios[iSorteado])
                igual = 1;
        }

        if(igual == 0)
            iSorteado++;
    }while(iSorteado < qntNumerosAleatorios);


    // para cada numero aleatorio gerado
    // pega uma posicao livre das restantes
    // para inserir uma letra contida em 'pernambuco'
    for(int i = 0; i < qntNumerosAleatorios; i++)
    {
        int y = posLivres[numerosAleatorios[i]].y;
        int x = posLivres[numerosAleatorios[i]].x;

        gameObject->mapa[y][x] = pernambuco[i];
    }
}

void criarMapaMascarado(game* gameObject)
{

    char **mapaMascarado = (char **)malloc(gameObject->tamanhoMapa * sizeof(char*));
    for(int i = 0; i < gameObject->tamanhoMapa; i++) mapaMascarado[i] = (char*) malloc(gameObject->tamanhoMapa * sizeof(char));

    gameObject->mapaMascarado = mapaMascarado;

    // Iniciando o Mapa Mascarado com Cerquilhas (#)
    for(int x = 0; x < gameObject->tamanhoMapa; x++)
    {
        for(int y = 0; y < gameObject->tamanhoMapa; y++)
        {
            gameObject->mapaMascarado[x][y] = CERQUILHA;
        }
    }
}

// 0 = continua, -1 = perdeu
int mostrarPosicao(
    int x, int y,
    game* gameObject) {

    gameObject->mapaMascarado[y][x] = ASTERISCO;

    if(gameObject->mapa[y][x] == BOMBA)
    {
        return PERDEU;
    }
    else
    {
        // Se na posicao (x,y) tem uma letra,
        // add a letra no array de letras encontradas
        for(int i = 0; i < strlen(pernambuco); i++) {
            if(pernambuco[i] == gameObject->mapa[y][x]) {
                letrasEncontradas[iLetrasEncontradas] = gameObject->mapa[y][x];
                iLetrasEncontradas++;
            }
        }

        return CONTINUAR;
    }
}

// -1 = perdeu, 0 = continua, 1 = ganhou
int executarAcao(game* gameObject)
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
        if((posJogador.y+1) <= (gameObject->tamanhoMapa - 1))
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
        if((posJogador.x+1) <= (gameObject->tamanhoMapa - 1))
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

        if(palavraEstaValida == TRUE) {
            return consultarPalavra(palavra);
        }

        return CONTINUAR;
    }
    case '\r':
    {
        return mostrarPosicao(posJogador.x, posJogador.y, gameObject);
    }
    default:
        return CONTINUAR;
    }
}

int validarPalavra(char* palavra) {

    int i = 0;
    while( palavra[i] != VAZIO ) {
        palavra[i] = toupper(palavra[i]);
        int encontrou = FALSE;
        for(int j = 0; j < iLetrasEncontradas; j++) {

            if(palavra[i] == letrasEncontradas[j]) {
                encontrou = TRUE;
            }
        }

        if(encontrou == FALSE) {
            printf("\nEncontre todas as letras da palavra!\n");
            system("pause");
            return FALSE;
        }

        i++;
    }

    return TRUE;
}


