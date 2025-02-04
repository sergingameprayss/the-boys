// TAD Fila de prioridades (FPRIO) genérica
// Carlos Maziero, DINF/UFPR, Out 2024
// Implementação com lista encadeada simples

// A COMPLETAR
#include <stdio.h>
#include <stdlib.h>

#include "fprio.h"


// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct fprio_t *fprio_cria (){
  struct fprio_t *f;

  if(!(f = malloc(sizeof(struct fprio_t))))
    return NULL;

  f->prim = NULL;
  f->num = 0;

  return f;
}

// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct fprio_t *fprio_destroi (struct fprio_t *f){
  struct fpnodo_t *aux;

  if(f == NULL)
    return NULL;
  
  while(f->prim != NULL){
    aux = f->prim;
    f->prim = aux->prox;
    free(aux->item); //
    free (aux);
  }

  free(f);
  return NULL;
}

// Insere o item na fila, mantendo-a ordenada por prioridades crescentes.
// Itens com a mesma prioridade devem respeitar a politica FIFO (retirar
// na ordem em que inseriu).
// Inserir duas vezes o mesmo item (o mesmo ponteiro) é um erro.
// Retorno: número de itens na fila após a operação ou -1 se erro.
int fprio_insere (struct fprio_t *f, void *item, int tipo, int prio){
  struct fpnodo_t *novo, *aux;

  if((f == NULL) || (item == NULL) || (prio < 0) || (tipo < 0))
    return -1;

  if(!(novo = malloc(sizeof(struct fpnodo_t))))
    return -1;

  novo->prio = prio;
  novo->item = item;
  novo->tipo = tipo;
  
  if(f->prim == NULL){
    f->prim = novo;
    novo->prox = NULL;

    return f->num++;
  }

  aux = f->prim;
  if(novo->item == aux->item){
    free(novo);
    novo = NULL;
    return -1;
  }

  if(novo->prio < f->prim->prio){
    novo->prox = f->prim;
    f->prim = novo;

    return f->num++;
  }

  while((aux->prox != NULL) && (novo->prio >= aux->prox->prio)){
    if(novo->item == aux->prox->item){ //nao faz sentido prioridade pode ser diferente
      free(novo);
      novo = NULL;
      return -1;
    }
    aux = aux->prox;
  }

  /*prioridade maior de todas, portanto insere como ultimo elemento*/
  if(aux->prox == NULL){
    aux->prox = novo;
    novo->prox = NULL;
    
    return f->num++;
  }

  novo->prox = aux->prox;
  aux->prox = novo;

  return f->num++;
}

// Retira o primeiro item da fila e o devolve; o tipo e a prioridade
// do item são devolvidos nos parâmetros "tipo" e "prio".
// Retorno: ponteiro para o item retirado ou NULL se fila vazia ou erro.
void *fprio_retira (struct fprio_t *f, int *tipo, int *prio){
  struct fpnodo_t *aux;
  void *item;

  if((f == NULL) || (f->prim == NULL) || (tipo == NULL) || (prio == NULL))
    return  NULL;

  aux = f->prim;
  f->prim = aux->prox;
  *tipo = aux->tipo;
  *prio = aux->prio;
  item = aux->item;

  free(aux);
  f->num--;

  return item;
}

// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fprio_tamanho (struct fprio_t *f){
  if(f == NULL)
    return -1;
  
  return f->num;
}

// Imprime o conteúdo da fila no formato "(tipo prio) (tipo prio) ..."
// Para cada item deve ser impresso seu tipo e sua prioridade, com um
// espaço entre valores, sem espaços antes ou depois e sem nova linha.
void fprio_imprime (struct fprio_t *f){
  struct fpnodo_t *aux;

  if((f == NULL) || (f->num == 0))
    return;
  
  aux = f->prim;
  for(int i = 0; i < (f->num - 1); i++){
    printf("(%d %d) ", aux->tipo, aux->prio);
    aux = aux->prox;
  }

  printf("(%d %d)", aux->tipo, aux->prio);
}