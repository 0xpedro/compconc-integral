#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "timer.h"

int limite_inferior, limite_superior, numero_intervalos = 2;
float erro, resp_seq, retangulo_maior;

float funcaoA(float x);
float funcaoB(float x);
float funcaoC(float x);
float funcaoD(float x);
float funcaoE(float x);
float funcaoF(float x);
float funcaoG(float x);

float calculaIntegralSequencial()
{
    float div, resp = 0;
    int i;

    div = (float)(limite_superior - limite_inferior) / numero_intervalos;

    retangulo_maior = (float)(limite_superior - limite_inferior) * funcaoD((limite_inferior + limite_superior) / 2);
    retangulo_maior = fabs(retangulo_maior);

    for (i = 1; i <= numero_intervalos; i++)
    {
        resp += div * funcaoD((limite_inferior + (div * i)) - (div / 2));
    }

    if (fabs(retangulo_maior - resp) < erro)
    {
        return resp;
    }
    else
    {
        numero_intervalos++;
        retangulo_maior = resp;
        calculaIntegralSequencial();
    }
}

int main(int argc, char *argv[])
{
    int i;
    double inicio, fim, tempo_total;

    if (argc < 4)
    {
        fprintf(stderr, "Digite: %s <erro> <limite_inferior> <limite_superior>\n", argv[0]);
        exit(-1);
    }

    erro = atof(argv[1]);
    limite_inferior = atoi(argv[2]);
    limite_superior = atoi(argv[3]);

    GET_TIME(inicio);

    resp_seq = calculaIntegralSequencial();

    GET_TIME(fim);

    tempo_total = fim - inicio;

    printf("Resposta da Sequencial: %f\n", resp_seq);
    printf("Tempo total de execução: %.8lf\n", tempo_total);

    return 0;
}

float funcaoA(float x)
{
    return 1 + x;
}

float funcaoB(float x)
{
    if (x < -1 || x > 1)
    {
        printf("Não é possível calcular o valor da função nesse intervalo\n");
        exit(-1);
    }
    return sqrt(1 - pow(x, 2));
}

float funcaoC(float x)
{
    return sqrt(1 + pow(x, 4));
}

float funcaoD(float x)
{
    return sin(pow(x, 2));
}

float funcaoE(float x)
{
    return cos(pow(M_E, -x));
}

float funcaoF(float x)
{
    return cos(pow(M_E, -x)) * x;
}

float funcaoG(float x)
{
    return cos(pow(M_E, -x)) * (0.005 * pow(x, 3) + 1);
}