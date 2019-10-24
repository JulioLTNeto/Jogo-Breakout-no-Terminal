#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>

int kbhit()
{
    struct termios term;
    tcgetattr(0, &term);

    struct termios term2 = term;
    term2.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &term2);

    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);

    tcsetattr(0, TCSANOW, &term);

    return byteswaiting > 0;
}

int recebe() {
    char ch;
    struct termios oldt, newt;

    tcgetattr ( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr ( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr ( STDIN_FILENO, TCSANOW, &oldt );

    return ch;
}

int main()
{
    int buffer = 1;
    char letra;
    int iniciou = 0;

    int vidas = 5;
    int pontuacao = 0;

    int contadorTempo = 0;
    int mostrar = 0;

    int xBarra = 12;
    int yBarra = 10;

    int xP = 9;
    int yP = 15;

    int oX = 0;
    int oY = 1;

    int sairEstado = 0;
    int vOY = 0;

    char tela[12][32];

    int comecar = 0;

    while(letra != 27){
        if(contadorTempo >= 10000){
            if(comecar){
                if(yP <= 1){
                    oY = 1;
                }
                if(yP >= 29){
                    oY = 0;
                }

                if(xP <= 1){
                    oX = 1;
                    sairEstado = 1;
                }
                if(xP >= 10){
                    vidas--;
                    xP = 9;
                    yP = 15;

                    xBarra = 12;
                    yBarra = 10;

                    oX = 0;
                    oY = 1;
                    comecar = 0;
                    continue;
                }

                if(oX == 0){
                    xP = xP-1;
                }
                if(oX == 1){
                    xP = xP+1;
                }

                if(oY == 0){
                    yP = yP-1;
                }
                if(oY == 1){
                    yP = yP+1;
                }
                if(tela[xP][yP] == '&' || tela[xP][yP] == '_'){
                    if(tela[xP][yP] == '&'){
                        pontuacao++;
                        sairEstado = 1;
                    }

                    if(tela[xP][yP] == '_' && letra == 32){
                        oX = 0;
                        vOY = oY;
                        oY = 4;
                    }else{
                        oX = !oX;
                        oY = oY;
                    }

                    tela[xP][yP] = ' ';

                    if(oX == 0){
                        xP = xP-1;
                    }
                    if(oX == 1){
                        xP = xP+1;
                    }

                    if(oY == 0){
                        yP = yP-1;
                    }
                    if(oY == 1){
                        yP = yP+1;
                    }
                }
                //if(sairEstado){
                 //       oY = vOY;
                 //       sairEstado = 0;
                //}
            }
            contadorTempo = 0;
        }

        if(mostrar == 0){
            for(int c1 = 0; c1 < 12; c1++){
                for(int c2 = 0; c2 < 32; c2++){

                    if(tela[c1][c2] != '&'){
                        tela[c1][c2] = ' ';
                    }

                    if(!iniciou){
                        int possibilidade = rand()%100;
                        if(possibilidade > 50){
                            if(c1 < 5){
                                tela[c1][c2] = '&';
                            }
                        }
                    }

                    if((c2 == xBarra || c2 == xBarra+1 || c2 == xBarra+2 || c2 == xBarra+3 || c2 == xBarra+4) && c1 == yBarra){
                        tela[c1][c2] = '_';
                    }
                    if(c2 == 0 || c2 == 31){
                        tela[c1][c2] = '|';
                    }
                    if(c1 == 0 || c1 == 11){
                        tela[c1][c2] = '-';
                    }
                    if(comecar){
                        tela[xP][yP] = '#';
                    }

                    printf("%c", tela[c1][c2]);
                }
                if(c1 == 6){
                    if(comecar){
                        printf("Vidas: %i", vidas);
                    }else printf("Aperte SPACE para começar");
                }
                if(c1 == 5){
                    if(comecar){
                        printf("Pontuacao: %i", pontuacao);
                    }
                }
                printf("\n");
            }
            iniciou = 1;
        }
        mostrar++;

        buffer = kbhit();

        if(buffer == 1){
            letra = recebe();

            if(letra == 'a' && comecar){
                if(xBarra > 1){
                    xBarra = xBarra-1;
                }
            }
            if(letra == 'd' && comecar){
                if(xBarra <= 25){
                    xBarra = xBarra+1;
                }
            }
            if(letra == 32){
                comecar = 1;
            }
        }
        if(vidas <= 0){
            printf("Perdeu!\n");
            break;
        }
        contadorTempo++;
        if(mostrar >= 1000){
            system("clear");
            mostrar = 0;
        }
    }
    printf("Adeus!\n");
    return 0;
}
