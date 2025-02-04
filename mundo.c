#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "mundo.h"
#include "lista.h"
#include "conjunto.h"


struct mundo *cria_mundo (){
  struct mundo *m;
  if(!(m = malloc(sizeof(struct mundo))))
    return NULL;
  return m;
}

void inicializa_mundo (struct mundo *m){

  m->Nherois = N_HEROIS;            
  m->Nbases = N_BASES;              
  m->Nmissoes = N_MISSOES;           
  m->Nhabilidades = N_HABILIDADES;   
  m->tamanhoMundo = N_TAMANHO_MUNDO;
  m->relogio = T_INICIO;
  m->eventos = 0;
  m->m_cumpridas = 0;
}

struct mundo *destroi_mundo (struct mundo *m){
  free(m);
  return NULL;
}

int aleat (int min, int max){
  return min + rand() % (max - min + 1);
}

void inicializa_bases (struct mundo *m){
  struct base *b = m->bases;

  //para cada elemento do vetor tais atribuicoes sao feitas
  for(int i = 0; i < N_BASES; i++){
    b[i].id = i; 
    b[i].lotacao = aleat(3, 10); 
    b[i].presentes = cjto_cria(N_HEROIS); 
    b[i].espera = lista_cria(); 
    b[i].local.x = aleat(0, N_TAMANHO_MUNDO - 1);
    b[i].local.y = aleat(0, N_TAMANHO_MUNDO - 1);
    b[i].maior_fila = 0;
    b[i].missoes = 0;
  }
}


void inicializa_herois (struct mundo *m){
  struct heroi *h = m->herois;

  //para cada elemento do vetor tais atribuicoes sao feitas
  for(int i = 0; i < N_HEROIS; i++){
    h[i].id = i;
    h[i].experiencia = 0;
    h[i].paciencia = aleat(0, 100);
    h[i].velocidade = aleat(50, 5000);
    h[i].vivo = 1;
    h[i].habilidades = cjto_aleat(aleat(1, 3), N_HABILIDADES);
  }

}

void inicializa_missoes (struct mundo *m){
  struct missao *s = m->missoes;

  //para cada elemento do vetor tais atribuicoes sao feitas
  for(int i = 0; i < N_MISSOES; i++){
    s[i].id = i;
    s[i].perigo = aleat(0, 100);
    s[i].habilidades = cjto_aleat(aleat(6, 10), N_HABILIDADES);
    s[i].local.x = aleat(0, N_TAMANHO_MUNDO - 1);
    s[i].local.y = aleat(0, N_TAMANHO_MUNDO - 1);
    s[i].tentativas = 0;
    s[i].cumprida = 0;
  }
}

void libera_herois(struct mundo *m){
  struct heroi *h = m->herois;

  //libera a estrutura dinamica de cada heroi do vetor
  for(int i = 0; i < N_HEROIS; i++)
      h[i].habilidades = cjto_destroi(h[i].habilidades);

}

void libera_bases(struct mundo *m){
  struct base *b = m->bases;

  //libera as estruturas dinamicas de cada base do vetor
  for(int i = 0; i < N_BASES; i++){
    b[i].espera = lista_destroi(b[i].espera);
    b[i].presentes = cjto_destroi(b[i].presentes);
  }

}

void libera_missoes(struct mundo *m){
  struct missao *s = m->missoes;

  //libera a estrutura dinamica de cada missao do vetor
  for(int i = 0; i < N_MISSOES; i++)
      s[i].habilidades = cjto_destroi(s[i].habilidades);

}

int distancia_cart (struct cart o, struct cart d){
  return sqrt(pow(d.x - o.x, 2) + pow(d.y - o.y, 2)); 
}

