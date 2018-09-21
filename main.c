#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void inicializaVetorPesos(double *w){
    int i;
    srand((double) time(NULL)); // mudar a sequencia usando o relógio

    printf("vetor pesos sinapticos original:\n");
    for(i=0;i<36;i++){ // inicializar os pesos c valores aleatórios entre 0 e 1
        w[i] = ((double) rand() / (double)(RAND_MAX));
        printf("%lf\n",w[i]);
    }
    printf("**************\n");
}

void inicializaEntradas(double entradas[36][4]){

    double entradasAux[36][4] =
    {
        {-1,-1,-1,-1}, // colunas do bias
        {0.4329, -1.3719, 0.7020, -0.8535},{0.3024,0.2286,0.8630,2.7909},{0.1349, -0.6445, 1.0530,0.5687},{0.3374, -1.7163, 0.3670, -0.6283},
        {1.1434, -0.0485,0.6637,1.2606},{1.3749, -0.5071,0.4464,1.3009},{0.7221,-0.7587,0.7681,-0.5592},{0.4403,-0.8072,0.5154,-0.3129},
        {-0.5231, 0.3548,0.2538,1.5776},{0.3255,-2.0000,0.7112,-1.1209},{0.5824,1.3915,-0.2291,4.1735},
        {0.1340, 0.6081,0.4450,3.2230},{0.1480,-0.2988,0.4778,0.8649},{0.7359,0.1869,-0.0872,2.3584},{0.7115,-1.1469,0.3394,0.9573},
        {0.8251,-1.2840,0.8452,1.2382},{0.1569,0.3712,0.8825,1.7633},{0.0033,0.6835,0.5389,2.8249},{0.4243,0.8313,0.2634,3.5855},
        {1.0490,0.1326,0.9138,1.9792},{1.4276,0.5331,-0.0145,3.7286},{0.5971,1.4865,0.2904,4.6069},{0.8475,2.1479,0.3179,5.8235},
        {1.3967,-0.4171,0.6443,1.3927},{0.0044,1.5378,0.6099,4.7755},{0.2201,-0.5668,0.0515,0.7829},{0.6300,-1.2480,0.8591,0.8093},
        {-0.2479,0.8960,0.0547,1.7381},{-0.3088,-0.0929,0.8659,1.5483},{-0.5180,1.4974,0.5453,2.3993},{0.6833,0.8266,0.0829,2.8864},
        {0.4353,-1.4066,0.4207,-0.4879},{-0.1069,-3.2329,0.1856,-2.4572},{0.4662,0.6261,0.7304,3.4370},{0.8298,-1.4089,0.3119,1.3235}
    };

    int i,j;
    for(i=0;i<36;i++){
        for(j=0;j<4;j++){
            entradas[i][j] = entradasAux[i][j];
            //printf("%lf ",entradas[i][j]);
        }
        //printf("\n");
    }
}

void inicializaSaidasDesejadas(double *saidasDesejadas){

    double saidasDesejadasAux[35] = {1.0000, -1.0000, -1.0000, -1.0000, 1.0000,
    1.0000, 1.0000, 1.0000, -1.0000, 1.0000, -1.0000, -1.0000, 1.0000,
    1.0000,-1.0000,-1.0000, 1.0000, -1.0000, -1.0000, 1.0000, 1.0000, -1.0000,
    -1.0000,1.0000,-1.0000,1.0000,-1.0000,1.0000,-1.0000,1.0000,1.0000,1.0000,-1.0000,-1.0000,-1.0000};

    int i;
    for(i=0;i<35;i++){
        saidasDesejadas[i] = saidasDesejadasAux[i];
        //printf("%lf ",saidasDesejadas[i]);
    }
}

int sinal(double u){

    // neste caso estou utilizando uma função degrau bipolar
    if(u >= 0.0){
        return 1;
    }
    return -1;
}

double eqm(double *w, double entradas[36][4] ,double *saidasDesejadas){

    double p = 35.0; // qtd amostras
    int i,j;

    double eqmV = 0.0,u;

    for(i=0;i<36;i++){

        u = 0.0;

        for(j=0;j<4;j++){

            u += w[i]*entradas[i][j]; /// produto escalar dos pesos e das entradas
        }

        eqmV += (saidasDesejadas[i]- u)  * (saidasDesejadas[i]- u);
    }

    eqmV /= p;

    printf("%lf,\n",eqmV);

    return eqmV;
}

int main()
{
    int barreiraDeSegurancaEpocas = 100000; // se em 1000 épocas nao convergir a rede para pois o conjunto nao é linearmente separável

    double entradas[36][4];
    inicializaEntradas(entradas);

    double saidasDesejadas[35];
    inicializaSaidasDesejadas(saidasDesejadas);

    double w[36]; // pesos sinápticos
    inicializaVetorPesos(w);

    double taxaAprendizado = 0.0025,precisao = 0.000001,u;

    int contEpocas = 0;

    int i,j,y;

    double eqmAtual=0.0, eqmAnterior=0.0,verifica=0.0;

    /// algoritmo de treinamento regra delta
    do{
        eqmAnterior = eqm(w,entradas,saidasDesejadas);

        for(i=0;i<36;i++){

            u = 0.0;

            for(j=0;j<4;j++){

                u += w[i]*entradas[i][j]; /// produto escalar dos pesos e das entradas
            }

            for(j=0;j<4;j++){

                w[i] = w[i] + taxaAprendizado * ( saidasDesejadas[i]- u ) * entradas[i][j];
            }
        }
        contEpocas++;

        eqmAtual = eqm(w,entradas,saidasDesejadas);

        /*printf("eqmAtual = %lf\n",eqmAtual);
        printf("eqmAnterior = %lf\n",eqmAnterior);*/

    }while(eqmAnterior >= precisao && eqmAtual >= precisao && contEpocas <= barreiraDeSegurancaEpocas);

    printf("vetor pesos sinapticos final:\n");
    for(i=0;i<36;i++){

        printf("%lf\n",w[i]);
    }
    printf("@@@@@@@@@@@@@@@@\n\n");

    printf("qtd epocas = %d\n\n",contEpocas);

    // Fase de operação
    double testes[15][4] =
    {
        {0.9694, 0.6909, 0.4334, 3.4965}, {0.5427, 1.3832, 0.6390, 4.0352} , {0.6081, -0.9196, 0.5925, 0.1016},
        {-0.1618,0.4694,0.2030,3.0117},{0.1870,-0.2578,0.6124,1.7749},{0.4891,-0.5276,0.4378,0.6439},
        {0.3777,2.0149,0.7423,3.3932},{1.1498,-0.4067,0.2469,1.5866},{0.9325,1.0950,1.0359,3.3591},
        {0.5060,1.3317,0.9222,3.7174},{0.0497,-2.0656,0.6124,-0.6585},{0.4004,3.5369,0.9766,5.3532},
        {-0.1874,1.3343,0.5374,3.2189},{0.5060,1.3317,0.9222,3.7174},{1.6375,-0.7911,0.7537,0.5515}
    };

    for(i=0;i<15;i++){

        for(j=0;j<4;j++){

            u = w[i] * testes[i][j];
            y = sinal(u);

            if(y==-1){
                printf("%lf pertence a classe A\n",testes[i][j]);
            }else
                printf("%lf pertence a classe B\n",testes[i][j]);
            }
    }
    /*printf("\n");
    system("pause");*/
    return 0;
}
