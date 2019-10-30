#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "timer.h"

int limite_inferior, limite_superior, numero_intervalos = 2;
double erro, resp_seq, retangulo_maior;

double funcaoA(double x);
double funcaoB(double x);
double funcaoC(double x);
double funcaoD(double x);
double funcaoE(double x);
double funcaoF(double x);
double funcaoG(double x);

double calculaIntegralSequencial()
{
    double div, resp = 0;
    int i;

    div = (double)(limite_superior - limite_inferior) / numero_intervalos;

    for (i = 1; i <= numero_intervalos; i++)
    {
        resp += div * funcaoD((limite_inferior + (div * i)) - (div / 2));
    }

    if (fabs(retangulo_maior - fabs(resp)) < erro)
    {
        return fabs(resp);
    }
    else
    {
        numero_intervalos++;
        retangulo_maior = fabs(resp);
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

    retangulo_maior = (double)(limite_superior - limite_inferior) * funcaoD((limite_inferior + limite_superior) / 2);
    retangulo_maior = fabs(retangulo_maior);
    resp_seq = calculaIntegralSequencial();

    GET_TIME(fim);

    tempo_total = fim - inicio;

    printf("Resposta da Integral da função D: %lf\n", resp_seq);
    printf("Tempo total de execução do calculo da função D: %.8lf\n", tempo_total);

    return 0;
}

double funcaoA(double x)
{
    return 1 + x;
}

double funcaoB(double x)
{
    if (x < -1 || x > 1)
    {
        printf("Não é possível calcular o valor da função nesse intervalo\n");
        exit(-1);
    }
    return sqrt(1 - pow(x, 2));
}

double funcaoC(double x)
{
    return sqrt(1 + pow(x, 4));
}

double funcaoD(double x)
{
    return sin(pow(x, 2));
}

double funcaoE(double x)
{
    return cos(pow(M_E, -x));
}

double funcaoF(double x)
{
    return cos(pow(M_E, -x)) * x;
}

double funcaoG(double x)
{
    return cos(pow(M_E, -x)) * (0.005 * pow(x, 3) + 1);
}
