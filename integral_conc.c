#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "timer.h"

int nthreads, limite_inferior, limite_superior, numero_intervalos = 2, n_threads_executadas = 0, lock = 0, calculou = 0;
float erro, resp_conc, retangulo_maior;
pthread_mutex_t mutex;
pthread_cond_t cond;

float funcaoA(float x);
float funcaoB(float x);
float funcaoC(float x);
float funcaoD(float x);
float funcaoE(float x);
float funcaoF(float x);
float funcaoG(float x);

void *calculaIntegralConcorrente(void *arg)
{
    int i, thread_id = *(int *)arg;
    float div, resp;

    while (!calculou)
    {
        div = (float)(limite_superior - limite_inferior) / numero_intervalos;

        resp = 0;
        for (i = thread_id; i <= numero_intervalos; i += nthreads)
        {
            resp += div * funcaoD((limite_inferior + (div * i)) - (div / 2));
        }

        pthread_mutex_lock(&mutex);
        resp_conc += resp;
        n_threads_executadas++;

        if (n_threads_executadas < nthreads)
        {
            //printf("Thread %d lockada\n", thread_id);
            lock++;
            pthread_cond_wait(&cond, &mutex);
            lock--;
        }
        else
        {
            //printf("Thread %d fazendo verificação\n", thread_id);
            //printf("valor da diferença entre as areas conc: %f\n", fabs(retangulo_maior - fabs(resp_conc)));
            if (fabs(retangulo_maior - fabs(resp_conc)) < erro)
            {
                calculou = 1;
                pthread_cond_broadcast(&cond);
            }
            else
            {
                while (lock != nthreads - 1)
                {
                }

                numero_intervalos++;
                retangulo_maior = resp_conc;
                resp_conc = 0;
                n_threads_executadas = 0;
                pthread_cond_broadcast(&cond);
            }
        }

        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t *threads;
    int i, *id;
    double inicio, fim, tempo_total;

    pthread_mutex_init(&mutex, NULL);

    if (argc < 5)
    {
        fprintf(stderr, "Digite: %s <numero_threads> <erro> <limite_inferior> <limite_superior>\n", argv[0]);
        exit(-1);
    }

    if (atoi(argv[3]) > atoi(argv[4]))
    {
        fprintf(stderr, "Informe um limite inferior menor que o limite superior.\n", argv[0]);
        exit(-1);
    }

    nthreads = atoi(argv[1]);
    erro = atof(argv[2]);
    limite_inferior = atoi(argv[3]);
    limite_superior = atoi(argv[4]);

    GET_TIME(inicio);

    threads = (pthread_t *)malloc(sizeof(pthread_t) * nthreads);
    if (threads == NULL)
    {

        printf("--ERRO: malloc do tid\n");
        exit(-1);
    }

    GET_TIME(fim);
    tempo_total = fim - inicio;
    printf("Tempo de inicialização das %d threads: %lf\n", nthreads, tempo_total);

    retangulo_maior = (float)(limite_superior - limite_inferior) * funcaoD((limite_inferior + limite_superior) / 2);
    retangulo_maior = fabs(retangulo_maior);

    GET_TIME(inicio);

    for (i = 0; i < nthreads; i++)
    {
        id = malloc(sizeof(int));
        if (id == NULL)
        {
            printf("--ERRO: malloc do id\n");
            exit(-1);
        }
        *id = i + 1;

        if (pthread_create(&threads[i], NULL, calculaIntegralConcorrente, (void *)id))
        {
            printf("--ERRO: pthread create da thread %d\n", i);
            exit(-1);
        }
    }

    for (i = 0; i < nthreads; i++)
    {

        if (pthread_join(threads[i], NULL))
        {
            printf("--ERRO: pthread join da thread %d\n", i);
            exit(-1);
        }
    }

    GET_TIME(fim);
    tempo_total = fim - inicio;
    printf("Tempo de execução das %d threads: %lf\n", nthreads, tempo_total);

    printf("Resposta da Integral: %f\n", resp_conc);

    pthread_mutex_destroy(&mutex);

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