#include <stdlib.h>
#include <stdio.h>

#include "mundo.h"
#include "fprio.h"
#include "conjunto.h"
#include "lista.h"
#include "eventos.h"


#define CHEGA 1
#define ESPERA 2
#define DESISTE 3
#define AVISA 4
#define ENTRA 5
#define SAI 6
#define VIAJA 7
#define MORRE 8
#define MISSAO 9
#define FIM 10


struct evento *cria_ev (int heroi, int local){
  struct evento *evento;

  if(!(evento = malloc(sizeof(struct evento))))
    return NULL;
  
  evento->heroi = heroi;
  evento->local = local;
  
  return evento;
}

// retorna 1 em caso de sucesso e 0 em caso de erro
void eventos_iniciais (struct fprio_t *lef){
  struct evento *ev;
  int base, tempo;

  //para cada heroi diz um destino e marca o evento chega apos um tempo aleatorio de ate 3 dias
  for(int i = 0; i < N_HEROIS; i++){
    base = aleat(0, N_BASES - 1);
    ev = cria_ev (i, base); 
    tempo = aleat(0, 4320); 
    fprio_insere(lef, ev, 1, tempo); 
  }
  
  //para cada missao marca o evento missao em um tempo aleatorio ate o fim do mundo
  for(int i = 0; i < N_MISSOES; i++){
    tempo = aleat(0, T_FIM_DO_MUNDO);
    ev = cria_ev(-1, i);
    fprio_insere(lef, ev, 9, tempo);
  }
  

  ev = cria_ev(0, base);
  fprio_insere(lef, ev, 10, T_FIM_DO_MUNDO);
}



int simula_eventos(struct fprio_t *lef, struct mundo *m){
  struct evento *ev;
  int tipo, tempo;
  
  //cria os eventos iniciais da simulacao
  eventos_iniciais(lef);

  //enquanto o relogio nao for o tempo do fim do mundo, retira e trata os eventos
  //presentes na lef, podendo ser uma das 10 opcoes
  do {
    //atualiza numero total de eventos
    m->eventos += 1;

    ev = fprio_retira(lef, &tipo, &tempo);
    if(ev == NULL)
      return 0;
    m->relogio = tempo;
    switch (tipo)
    {
    case CHEGA: 
      chega(lef, m, ev);

      break;
    case ESPERA:
      espera(lef, m, ev); 

      break;
    case DESISTE:
      desiste(lef, m, ev);
      break;
    case AVISA:
      avisa(lef,m, ev);
      break;
    case ENTRA:
      entra(lef, m, ev);
      break;
    
    case SAI:
      sai(lef, m, ev);
      break;

    case VIAJA:
      viaja(lef, m, ev);
      break;
    
    case MORRE:
      morre(lef, m, ev);
      break;

    case MISSAO:
      missao(lef, m, ev);
      break;

    case FIM:
      fim(m);
      break;

    default:
      break;
    }

    free(ev);
  } while(m->relogio != T_FIM_DO_MUNDO);

  return 1;
}

//EVENTOS

void chega (struct fprio_t *lef, struct mundo *m, struct evento *ev){ 
  struct heroi *h = m->herois;
  struct base *b = m->bases;
  struct evento *novo_ev;
  int espera, tempo = m->relogio;

  if(!(h[ev->heroi].vivo))
    return;

  //atualiza base do heroi
  h[ev->heroi].base = b[ev->local].id;

  //se houver lugar e nao houver fila o heroi espera, caso contrario calcula se ele espera ou nao
  if((b[ev->local].lotacao > cjto_card(b[ev->local].presentes)) && (lista_tamanho(b[ev->local].espera) == 0))
    espera = 1;
  else 
    espera = ((h[ev->heroi].paciencia) > (10 * lista_tamanho(b[ev->local].espera)));

  //cria novo evento
  novo_ev = cria_ev(ev->heroi, ev->local);

  //se ele espera, insere na lef evento espera, caso contrario insere evento desiste
  if(espera){
    fprio_insere(lef, novo_ev, ESPERA, tempo);
    printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) ESPERA\n", tempo, ev->heroi, h[ev->heroi].base, cjto_card(b[ev->local].presentes), b[ev->local].lotacao);
  } else {
    fprio_insere(lef, novo_ev, DESISTE, tempo);
    printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) DESISTE\n", tempo, ev->heroi, h[ev->heroi].base, cjto_card(b[ev->local].presentes), b[ev->local].lotacao);
  }

}

void espera(struct fprio_t *lef,struct mundo *m, struct evento *ev){
  struct heroi *h = m->herois;
  struct base *b = m->bases;
  struct evento *novo_ev;
  int tempo = m->relogio;

  if(!(h[ev->heroi].vivo))
    return;

  //insere o heroi na fila de espera da base
  lista_insere(b[ev->local].espera, h[ev->heroi].id, -1); 

  //confere se eh a maior fila, se for atualiza maiorfila da base
  if(b[ev->local].maior_fila < lista_tamanho(b[ev->local].espera))
    b[ev->local].maior_fila = lista_tamanho(b[ev->local].espera);

  //cria e insere na lef evento avisa
  novo_ev = cria_ev(ev->heroi, ev->local);
  fprio_insere(lef, novo_ev, AVISA, tempo); 

  printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", tempo, ev->heroi, h[ev->heroi].base, (-1 + lista_tamanho(b[ev->local].espera)));
}

void desiste(struct fprio_t *lef, struct mundo *m, struct evento *ev){
  struct heroi *h = m->herois;
  struct evento *novo_ev;
  int tempo = m->relogio;

  if(!(h[ev->heroi].vivo))
    return;
  
  //cria e insere na lef o evento viaja ate uma base aleatoria
  novo_ev = cria_ev(ev->heroi, aleat(0, N_BASES - 1));
  fprio_insere(lef, novo_ev, VIAJA, tempo);

  printf("%6d: DESISTE HEROI %2d BASE %d\n", tempo, ev->heroi, ev->local); 
}

void avisa(struct fprio_t *lef, struct mundo *m, struct evento *ev){ 
  struct base *b = m->bases;
  struct evento *novo_ev;
  int tempo = m->relogio;

  printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA [ ", tempo, b[ev->local].id, cjto_card(b[ev->local].presentes), b[ev->local].lotacao);
  lista_imprime(b[ev->local].espera);
  printf(" ]\n");

  //enquanto a base nao estiver lotada e houver fila de espera, retira da fila e inclui na base o primeiro heroi da fila
  while ((b[ev->local].lotacao > cjto_card(b[ev->local].presentes)) && (lista_tamanho(b[ev->local].espera) != 0)){
    lista_retira(b[ev->local].espera, &ev->heroi, -1); 
    cjto_insere(b[ev->local].presentes, ev->heroi);

    //cria e insere na lef evento entra
    novo_ev = cria_ev(ev->heroi, ev->local);
    fprio_insere(lef, novo_ev, ENTRA, tempo);

    printf("%6d: AVISA  PORTEIRO BASE %d ADMITE %2d\n", tempo, b[ev->local].id, novo_ev->heroi);
  }

}

void entra(struct fprio_t *lef, struct mundo *m, struct evento *ev){
  struct heroi *h = m->herois;
  struct base *b = m->bases;
  struct evento *novo_ev;
  int tpb, tempo = m->relogio;

  if(!(h[ev->heroi].vivo))
    return;

  tpb = 15 + h[ev->heroi].paciencia * aleat(1, 20);

  //cria e insere na lef evento sai
  novo_ev = cria_ev(ev->heroi, ev->local);
  fprio_insere(lef, novo_ev, SAI, tempo + tpb);

  printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d\n", tempo, h[ev->heroi].id, b[ev->local].id, cjto_card(b[ev->local].presentes), b[ev->local].lotacao, tempo + tpb);
}

void sai(struct fprio_t *lef, struct mundo *m, struct evento *ev){
  struct heroi *h = m->herois;
  struct base *b = m->bases;
  struct evento *novo_ev;
  int tempo = m->relogio;

  if(!(h[ev->heroi].vivo))
    return;
  
  //retira heroi dos presentes da base
  cjto_retira(b[ev->local].presentes, h[ev->heroi].id);

  //cria e insere na lef evento viaja para uma base aleatoria
  novo_ev = cria_ev(ev->heroi, aleat(0, N_BASES - 1));
  fprio_insere(lef, novo_ev, VIAJA, tempo);

  //cria e insere na lef o evento avisa indicando que abriu vaga na base
  novo_ev = cria_ev(ev->heroi, ev->local);
  fprio_insere(lef, novo_ev, AVISA, tempo);


  printf("%6d: SAI    HEROI %2d BASE %d (%2d/%2d)\n", tempo, h[ev->heroi].id, b[ev->local].id, cjto_card(b[ev->local].presentes), b[ev->local].lotacao);
}

void viaja(struct fprio_t *lef, struct mundo *m, struct evento *ev){
  struct heroi *h = m->herois;
  struct base *b = m->bases;
  struct evento *novo_ev;
  int distancia, duracao, tempo = m->relogio;

  if(!(h[ev->heroi].vivo))
    return;

  distancia = distancia_cart(b[h[ev->heroi].base].local, b[ev->local].local); 
  duracao = distancia / h[ev->heroi].velocidade; 
  
  //cria e insere na lef evento chega
  novo_ev = cria_ev(ev->heroi, ev->local);
  fprio_insere(lef, novo_ev, CHEGA, tempo + duracao);

  printf("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", tempo, h[ev->heroi].id, b[h[ev->heroi].base].id, b[ev->local].id, distancia, h[ev->heroi].velocidade, tempo + duracao);
}

void morre(struct fprio_t *lef, struct mundo *m, struct evento *ev){
  struct heroi *h = m->herois;
  struct base *b = m->bases;
  struct evento *novo_ev;
  int tempo = m->relogio;

  //retira o heroi dos presentes na base
  cjto_retira(b[h[ev->heroi].base].presentes, h[ev->heroi].id);
  
  //muda o status dele para morto
  h[ev->heroi].vivo = 0;
  
  //cria evento avisa e insere na lef
  novo_ev = cria_ev(-1, h[ev->heroi].base);
  fprio_insere(lef, novo_ev, AVISA, tempo);
  
  printf("%6d: MORRE  HEROI %2d MISSAO %d\n", tempo, h[ev->heroi].id, ev->local);
}

//select sort que ordena o vetor de struct distancia em ordem crescente as distancias
void ordena_id(struct distancia_base v[], int tam){
  struct distancia_base aux;
  int i, j, min;

  for(i = 0; i < tam - 1; i++){
    min = i;
    for(j = i + 1; j < tam; j++){
      if(v[j].dist < v[min].dist)
        min = j;
    }
    aux = v[min];
    v[min] = v[i];
    v[i] = aux;
  }

}


void missao(struct fprio_t *lef, struct mundo *m, struct evento *ev){ 
  struct heroi *h = m->herois;
  struct base *b = m->bases;
  struct missao *s = m->missoes;
  struct evento *novo_ev;
  struct cjto_t *c_hab, *c_hab_aux;
  struct distancia_base d[N_BASES];
  int h_id[N_BASES];
  int i, j, risco;
  int tempo = m->relogio, bmp = -1, k = 0;

  s[ev->local].tentativas += 1;

  printf("%6d: MISSAO %d TENT %d HAB REQ: [ ", tempo, ev->local, s[ev->local].tentativas);
  cjto_imprime(s[ev->local].habilidades);
  printf(" ]\n");

  //preenche o vetor com as distancias das bases desordenado
  for(i = 0; i < N_BASES; i++){
    d[i].id = i;
    d[i].dist = distancia_cart(b[i].local, s[ev->local].local);
  }

  //ordena o vetor de structs em ordem crescente de distancia
  ordena_id(d, N_BASES);


  //loop das bases, indo da mais proxima ate a mais longe
  for(i = 0; i < N_BASES; i++){

    c_hab = cjto_cria(10); 

    //confere quais herois estao nessa base
    for(j = 0; j < N_HEROIS; j++){
      //d[i] eh o id da base mais proxima da vez
      if(cjto_pertence(b[d[i].id].presentes, h[j].id) == 1){ 


        c_hab_aux = c_hab;
        //conjunto de habilidades formado pelos herois dessa base
        c_hab = cjto_uniao(c_hab_aux, h[j].habilidades);
        cjto_destroi(c_hab_aux);

        //vetor que guarda quais herois estao nessa base
        h_id[k] = h[j].id; 
        k++; 
      } 
    }

    //confere se o conjunto de hab da base contem as habilidades necessarias, 
    //se conter sai do loop
    if(cjto_contem(c_hab, s[ev->local].habilidades) == 1){
      //bmp recebe id da base capacitada
      bmp = d[i].id;
      break;
    }

    //reseta o k pois nao encontrou o bmp ainda
    k = 0;
    cjto_destroi(c_hab);
  }

  //se bmp = -1 nao achou base que cumpra a missao, 
  //então adia ela em 1 dia
  if(bmp == -1){
    c_hab = NULL;
    novo_ev = cria_ev(ev->heroi, ev->local);
    fprio_insere(lef, novo_ev, MISSAO, tempo + 24*60);
    printf("%6d: MISSAO %d IMPOSSIVEL\n", tempo, s[ev->local].id);

    return;
  }

  m->m_cumpridas += 1;
  b[bmp].missoes += 1;

  //para cada heroi da base que cumpriu a missao calcula o risco 
  //cada heroi ou morre ou ganha experiencia....
  for(i = 0; i < k; i++){
    risco = (s[ev->local].perigo / (h[h_id[i]].paciencia + h[h_id[i]].experiencia + 1));

    //testa se heroi morre
    if(risco > aleat(0, 30)){
      novo_ev = cria_ev(h_id[i], s[ev->local].id); 
      fprio_insere(lef, novo_ev, MORRE, tempo);
    } else  
        h[h_id[i]].experiencia += 1;
  }   

  printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: [ ", tempo, s[ev->local].id, bmp);
  cjto_imprime(c_hab);
  printf(" ]\n");  
  c_hab = cjto_destroi(c_hab);
  
}


void fim(struct mundo *m){ 
  struct heroi *h = m->herois;
  struct base *b = m->bases;
  struct missao *s = m->missoes;
  
  int tempo = m->relogio;
  int mortos, tentativas, min_ten, max_ten;
  tentativas = mortos = max_ten = 0;

  //imprime para cada heroi o id, se esta vivo ou morto, paciencia, valocidade e experiencia,
  //tambem calcula quantos herois estão mortos
  for(int i = 0; i < N_HEROIS; i++){
    if(h[i].vivo){
      printf("\nHEROI %2d VIVO  PAC %3d VEL %4d EXP %4d HABS [ ", i, h[i].paciencia, h[i].velocidade, h[i].experiencia);
    } else {
        printf("\nHEROI %2d MORTO PAC %3d VEL %4d EXP %4d HABS [ ", i, h[i].paciencia, h[i].velocidade, h[i].experiencia);
        mortos++;
      }
    
    cjto_imprime(h[i].habilidades);
    printf(" ]");
  }
  
  //para cada base indica o id, lotacao, maior fila registrada, e quantidade de missoes que participou
  for(int i = 0; i < N_BASES; i++){
    printf("\nBASE %2d LOT %2d FILA MAX %2d MISSOES %d", i, b[i].lotacao, b[i].maior_fila, b[i].missoes); 
  }

  printf("\nEVENTOS TRATADOS: %d\n", m->eventos);

  printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n", m->m_cumpridas, N_MISSOES, ((float)m->m_cumpridas / (float)N_MISSOES) * 100);
  
  //inicializa o minimo de tentativas
  min_ten = s[0].tentativas;
  //calcula a quantidade total de tentativas, e o valor minimo e max de tentativas de uma unica missao
  for(int i = 0; i < N_MISSOES; i++){
    tentativas += s[i].tentativas;
    if(min_ten > s[i].tentativas)
      min_ten = s[i].tentativas;
    else  
      if(max_ten < s[i].tentativas)
        max_ten = s[i].tentativas;
  }

  printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n", min_ten, max_ten, ((float)tentativas /  (float)N_MISSOES));
  printf("TAXA MORTALIDADE: %.1f%%\n", ((float)mortos / (float)N_HEROIS) * 100);
  printf("FIM DO MUNDO NO MOMENTO %d\n", tempo);
}

