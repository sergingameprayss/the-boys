// programa principal do projeto "The Boys - 2024/2"
// Autor: Sergio Sivonei de SantAna Filho, GRR 20242337

#include <stdlib.h>
#include <stdio.h>

#include "mundo.h"
#include "fprio.h"
#include "eventos.h"


// programa principal
int main ()
{
  struct mundo *m;
  struct fprio_t *lef;

  srand(0);
  
  //inicializa a lef
  lef = fprio_cria();

  // iniciar o mundo
  m = cria_mundo();
  inicializa_mundo(m);
  inicializa_herois(m);
  inicializa_bases(m);
  inicializa_missoes(m);

  //simulador dos eventos do mundo até ele acabar, no fim imprime as estatisticas
  simula_eventos(lef, m);

  //libera e destroi mundo e a lef
  libera_herois(m);
  libera_bases(m);
  libera_missoes(m);
  lef = fprio_destroi(lef);
  m = destroi_mundo(m);

  return 0;
}

