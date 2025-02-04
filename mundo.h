// Este arquivo contem as structs das entidades bem como suas inicializações e funcoes necessarias para inicializacao e funcionamento do mundo
#ifndef MUNDO
#define MUNDO


#include "conjunto.h"

#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600 
#define N_TAMANHO_MUNDO 20000 
#define N_HABILIDADES 10
#define N_HEROIS (N_HABILIDADES * 5)
#define N_BASES (N_HEROIS / 5)
#define N_MISSOES (T_FIM_DO_MUNDO / 100)

struct cart {
    int x;
    int y;
};

struct heroi {

    int id; // número inteiro ≥ 0 que identifica unicamente o herói;
    struct cjto_t *habilidades;  // conjunto de habilidades que o herói possui. Cada habilidade é representada por um número inteiro ≥ 0;
    int paciencia; // número inteiro ≥ 0 que indica quão paciente uma pessoa é. Em nosso modelo, isso afeta as decisões de permanência em bases e ﬁlas;
    int velocidade; // número inteiro ≥ 0 indicando a velocidade de deslocamento de um herói, que irá afetar o tempo de deslocamento entre as bases;
    int experiencia; // número inteiro ≥ 0 que indica o número de missões em que o herói já participou;
    int base; // ID da base onde o herói se encontra no momento.

    int vivo; // q se esta vivo e 0 se morto
};

struct base {

    int id; // número inteiro ≥ 0 que identifica cada base;
    int lotacao; // número máximo de heróis naquela base;
    struct cjto_t *presentes; // conjunto dos IDs dos heróis que estão atualmente na base, constituem as equipes disponíveis para realizar missões;
    struct lista_t *espera; // fila onde os heróis esperam para poder entrar na base;
    struct cart local; // localização da base (par de coordenadas inteiras X, Y ≥ 0).
    int maior_fila; // numero da fila maxima que essa base ja teve 
    int missoes; //quantas missoes essa base participou
};

struct missao {
    int id;   // número inteiro ≥ 0 que identifica cada missao
    struct cjto_t *habilidades; // conjunto de habilidades necessarias para cumprir a missao
    int perigo; //perigo da missao
    struct cart local;  //local dela no plano caetesiano
    int tentativas; //numero de tentativas de cumprir a missao 
    int cumprida;  //marca se esta ou nao cumprida
};

struct mundo {

    int Nherois;             // número total de heróis no mundo;
    struct heroi herois[N_HEROIS];      // vetor representando todos os heróis;
    int Nbases;               // número total de bases no mundo;
    struct base bases[N_BASES];         // vetor representando todas as bases;
    int Nmissoes;           // número total de missões a cumprir;
    struct missao missoes[N_MISSOES];    // vetor representando todas as missões;
    int Nhabilidades;   // número de habilidades distintas possíveis;
    int tamanhoMundo; // coordenadas máximas do plano cartesiano (as coordenadas mínimas são 0, 0); 
                                        // vamos considerar que o mapa de nossa supercidade 
                                        // é representado por um plano cartesiano de tamanho tal que cada unidade representa 1 metro;
    int relogio;                        // número inteiro ≥ 0 indicando o tempo atual no mundo. 
                                        // Cada unidade de tempo no mundo simulado representa 1 minuto de tempo real.
    int eventos; // numero total de eventos tratados
    int m_cumpridas; // numero total de missoes cumpridas
};

//aleat usado para gerar numeros aleatorios entre min e max incluindo esses
int aleat (int min, int max); 

//funcao que cria o mundo dinamicamente
//retorna ponteiro para struct mundo 
//ou NULL em erro
struct mundo *cria_mundo ();

//libera o ponteiro mundo e retorna NULL
struct mundo *destroi_mundo (struct mundo *m);

//inicializa os valores do mundo
void inicializa_mundo (struct mundo *m);

//inicializa cada elemento do vetor de bases do mundo
void inicializa_bases (struct mundo *m);

//inicializa cada elemento do vetor de heroi do mundo
void inicializa_herois (struct mundo *m);

//iniciazlia cada elemento do vetor de missoes do mundo
void inicializa_missoes (struct mundo *m);

//libera a(s) estrutura(s) dinamica(s) presente(s) em cada elemento
//do vetor
void libera_herois(struct mundo *m);

//libera a(s) estrutura(s) dinamica(s) presente(s) em cada elemento
//do vetor
void libera_bases(struct mundo *m);

//libera a(s) estrutura(s) dinamica(s) presente(s) em cada elemento
//do vetor
void libera_missoes(struct mundo *m);

//calcula e retorna a distancia no plano cartesiano entre duas coordenadas
int distancia_cart (struct cart l1, struct cart l2);

#endif