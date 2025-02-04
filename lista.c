// TAD lista de números inteiros
// Carlos Maziero - DINF/UFPR, Out 2024
//
// Implementação do TAD - a completar
//
// Implementação com lista encadeada dupla não-circular

#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

// Cria uma lista vazia.
// Retorno: ponteiro p/ a lista ou NULL em erro.
struct lista_t *lista_cria (){
  struct lista_t *l;

  if(!(l = malloc(sizeof(struct lista_t))))
    return NULL;
  
  l->prim = l->ult = NULL;
  l->tamanho = 0;

  return l;

}

// Remove todos os itens da lista e libera a memória.
// Retorno: NULL.
struct lista_t *lista_destroi (struct lista_t *lst){
  struct item_t *aux;

  while(lst->prim != NULL){
    aux = lst->prim;
    lst->prim = aux->prox;
    free (aux);
  }

  free(lst);

  return NULL;
}

// Nas operações insere/retira/consulta/procura, a lista inicia na
// posição 0 (primeiro item) e termina na posição TAM-1 (último item).

// Insere o item na lista na posição indicada;
// se a posição for além do fim da lista ou for -1, insere no fim.
// Retorno: número de itens na lista após a operação ou -1 em erro.
int lista_insere (struct lista_t *lst, int item, int pos){
  struct item_t *novo,*aux;

  if(lst == NULL)
    return -1;

  if(!(novo = malloc(sizeof(struct item_t))))
    return -1;

  novo->valor = item;

    /*lista vazia*/
  if(lst->prim == NULL){
    novo->prox = NULL;
    novo->ant = NULL;
    lst->prim = novo;
    lst->ult = novo;

    return lst->tamanho++;
  }
  /*ultimo elemento*/
  if((pos >= lst->tamanho) || (pos == -1)){
    novo->prox = NULL;
    novo->ant = lst->ult;
    lst->ult->prox = novo;
    lst->ult = novo;

    return lst->tamanho++;
  }

  aux = lst->prim;
  /*primeiro elemento sem ser lista vazia*/
  if(pos == 0){ 
    novo->prox = lst->prim;
    novo->ant = NULL;
    lst->prim->ant = novo;
    lst->prim = novo;

    return lst->tamanho++;
  }

  /*acha a posicao*/
  for(int i = 0; i < pos; i++)
    aux = aux->prox;
   
  novo->prox = aux; 
  novo->ant = aux->ant; 
  aux->ant->prox = novo; 
  aux->ant = novo; 
  
  return lst->tamanho++;
}

// Retira o item da lista da posição indicada.
// se a posição for -1, retira do fim.
// Retorno: número de itens na lista após a operação ou -1 em erro.
int lista_retira (struct lista_t *lst, int *item, int pos){
  struct item_t *aux;

  /*lista vazia, posição maior q o tamanho ou posição menor q -1*/
  if((lst == NULL) || (lst->prim == NULL) || (pos >= lst->tamanho) || (pos < -1))
    return -1;

  /*caso lista com unico elemento*/
  if(lst->tamanho == 1){
    aux = lst->ult;
    lst->ult = lst->prim = NULL;
    *item = aux->valor;
    free(aux);
    return lst->tamanho--;
  }
  /*retira o ultimo da lista nao unitaria*/
  if((pos == lst->tamanho - 1) || (pos == -1)){
    aux = lst->ult;
    lst->ult = aux->ant;
    *item = aux->valor;
    free(aux); 
    lst->ult->prox = NULL;

    return lst->tamanho--;
  }

  aux = lst->prim;
  if(pos == 0){
    lst->prim = aux->prox;
    *item = aux->valor;
    free(aux);
    lst->prim->ant = NULL;

    return lst->tamanho--;
  }

  /*acha o elemento da posicao indicada*/
  for(int i = 0; i < pos; i++)
    aux = aux->prox;

  aux->ant->prox = aux->prox;
  aux->prox->ant = aux->ant;
  *item = aux->valor;
  free(aux);

  return lst->tamanho--;

}

// Informa o valor do item na posição indicada, sem retirá-lo.
// se a posição for -1, consulta do fim.
// Retorno: número de itens na lista ou -1 em erro.
int lista_consulta (struct lista_t *lst, int *item, int pos){
  struct item_t *aux;

  /*lista vazia, posição maior q o tamanho ou posição menor q -1*/
  if((lst == NULL) || (lst->prim == NULL) || (pos >= lst->tamanho) || (pos < -1))
    return -1;

  /*caso lista com unico elemento*/
  if(lst->tamanho == 1){
    *item = lst->prim->valor;

    return lst->tamanho;
  }

  /*consulta o ultimo da lista nao unitaria*/
  if((pos == lst->tamanho - 1) || (pos == -1)){
    *item = lst->ult->valor;

    return lst->tamanho;
  }

  aux = lst->prim;
  if(pos == 0){
    *item = aux->valor;

    return lst->tamanho;
  }

  /*acha o elemento da posicao indicada*/
  for(int i = 0; i < pos; i++)
    aux = aux->prox;

  *item = aux->valor;

  return lst->tamanho;
}

// Informa a posição da 1ª ocorrência do valor indicado na lista.
// Retorno: posição do valor ou -1 se não encontrar ou erro.
int lista_procura (struct lista_t *lst, int valor){
  struct item_t *aux;

  /*lista vazia*/
  if((lst == NULL) || (lst->prim == NULL))
    return -1;

  aux = lst->prim;
  for(int i = 0; i < lst->tamanho; i++){
    if(aux->valor == valor)
      return i;
    aux = aux->prox;
  }

  return -1; 
}

// Informa o tamanho da lista (o número de itens presentes nela).
// Retorno: número de itens na lista ou -1 em erro.
int lista_tamanho (struct lista_t *lst){
  if((lst == NULL))
    return -1;
  
  return lst->tamanho;
}

// Imprime o conteúdo da lista do inicio ao fim no formato "item item ...",
// com um espaço entre itens, sem espaços antes/depois, sem newline.
void lista_imprime (struct lista_t *lst){
  struct item_t *aux;
  if((lst == NULL) || (lst->prim == NULL))
    return;

  aux = lst->prim;
  for(int i = 0; i < lst->tamanho - 1; i++){
    printf("%2d ", aux->valor);
    aux = aux->prox;
  }
  printf("%d", lst->ult->valor);

}


