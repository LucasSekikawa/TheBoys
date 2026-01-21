#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"

struct fprio_t *fprio_cria ()
{
  struct fprio_t *ponteiroFila;
  ponteiroFila = malloc(sizeof(struct fprio_t));
  
  if (ponteiroFila == NULL)
    return NULL;

  ponteiroFila -> prim = NULL;
  ponteiroFila -> fim = NULL;
  ponteiroFila -> num = 0;

  return ponteiroFila;
}


struct fprio_t *fprio_destroi (struct fprio_t *f)
{
  struct fpnodo_t *atual;
  struct fpnodo_t *proximo;

  if (f == NULL)
    return NULL;

  atual = f->prim;

  /* Percorre a lista usando a variável "atual",
   liberando o item e, em seguida, o nodo,
   para todos os elementos da lista. */
  while ( atual != NULL)
  {
    if (atual->item)
      free(atual->item);
    proximo = atual->prox;
    free(atual);
    atual = proximo;
  }

  free(f);

  return NULL;
}


int fprio_insere (struct fprio_t *f, void *item, int tipo, int prio)
{
    struct fpnodo_t *atual;
    struct fpnodo_t *anterior;     
    struct fpnodo_t *novoItem;

    // Checa primeiro se os ponteiros do parametro nao sao vazios
    if ((!f) || (!item))
      return -1;

    novoItem = malloc(sizeof(struct fpnodo_t));
    if ((novoItem == NULL))
        return -1; 

    // Atribuicao dos valores para o novo item
    novoItem->item = item;
    novoItem->tipo = tipo;
    novoItem->prio = prio;
    novoItem->prox = NULL; 

    // Variaveis auxiliares para percorrer a lista
    atual = f->prim;
    anterior = NULL; 


    while (atual != NULL) 
    {
      /* Checa se o elemento ja estava na lista,
         caso esteja, libera o espaço do novo item
         e retorna o codigo de erro */
      if (atual->item == item) 
        {
            free(novoItem); 
            return -1;      
        }
        
      // Sai do loop caso o novo item chegue na sua posicao
      if (novoItem->prio < atual->prio) 
        break; 
      
      // Avança para o proximo elemento da lista
        anterior = atual;
        atual = atual->prox;
    }
    
    /* Se o anterior valer NULL, significa que o loop nao foi executado,
       ou seja, a lista nao possui nenhum elemento, e o primeiro deles
       será o novo item */
    if (anterior == NULL) 
    {
        novoItem->prox = f->prim;
        f->prim = novoItem;
    } 

    // Caso contrario o item sera adicionado no meio ou no fim
    else 
    {
        novoItem->prox = anterior->prox;
        anterior->prox = novoItem;
    }

    
    /* Checa se o ponteiro atual é NULL, ou seja, se o elemento
       foi adicionado na ultima posicao, para poder atualizar
       o ultimo elemento da fila */
    if (atual == NULL) 
        f->fim = novoItem;

    f->num++;
    return f->num;   
}

void *fprio_retira (struct fprio_t *f, int *tipo, int *prio)
{
  struct fpnodo_t *aux;
  void *item_retirado;

  if ((!f) || (!tipo) || (!prio) || (f->num == 0) )
    return NULL;

  // Guarda o elemento a ser retirado em aux
  aux = f->prim;
  // Guarda o item que sera retirado
  item_retirado = aux->item;

  // Faz o primeiro elemento ser o que costumava ser o segundo
  f->prim = f->prim->prox;

  *tipo = aux->tipo;
  *prio = aux->prio;

  free(aux);
  f->num--;

  // Se a fila ficou vazia, atualiza o valor do fim para NULL
  if (f->num == 0) 
      f->fim = NULL;
  
  return item_retirado;
}


int fprio_tamanho (struct fprio_t *f)
{
    if (f == NULL)
        return -1;
    return f->num;
}


void fprio_imprime (struct fprio_t *f)
{
  struct fpnodo_t *atual = f->prim;

  while ( atual != NULL )
  {
    printf("(%d %d)", atual->tipo, atual->prio );
    if (atual->prox != NULL)
      printf(" ");
    atual = atual->prox;
  }
}