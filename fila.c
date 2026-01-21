#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

struct fila_t *fila_cria ()
{
    struct fila_t *ptr_fila_t= malloc(sizeof(struct fila_t));
    if (ptr_fila_t == NULL)
        return NULL;

    ptr_fila_t->prim = NULL;
    ptr_fila_t->ult = NULL;
    ptr_fila_t->num = 0;

    return ptr_fila_t;
}


struct fila_t *fila_destroi (struct fila_t *f)
{
    struct fila_nodo_t *atual;
    struct fila_nodo_t *proximo;

    if ( f == NULL)
        return NULL;

    if ( f->num != 0 )
    {    
        /* Inicia o nodo atual como o primeiro da lista */
        atual = f->prim;
    
        /* Percorre todos os nodos da lista */
        while (atual != NULL)
        {
            /* Guarda o endereco do proximo nodo
               antes de liberar o nodo atual */
            proximo = atual->prox;
            //free(atual->item); decidir se faco
            free(atual);
            atual = proximo;
        }   
    }

    free(f);

    f = NULL;
    return NULL;
}


int fila_insere (struct fila_t *f, int item) 
{
    struct fila_nodo_t *ptr_novo = malloc(sizeof(struct fila_nodo_t));
   
    if (ptr_novo == NULL)
        return -1;
    
    /* Inicializa o novo elemento da lista */
    ptr_novo->item = item;
    ptr_novo->prox = NULL;

    /* Checa se a lista esta vazia, caso ela esteja,
       o valor inserido sera o primeiro, caso contrario,
       ele é colocado para a ultima posicao da lista */ 
    if (f->num == 0)
    {
        f->prim = ptr_novo;  
        f->ult = ptr_novo;
    }     
    else
    {
        f->ult->prox = ptr_novo;
        f->ult = ptr_novo;
    }

    f->num += 1;

    return f->num;
}


int fila_retira (struct fila_t *f, int *item) 
{
    struct fila_nodo_t *removido;

    if (f->num == 0)
        return 0;    
    
    /* Usa a struct auxiliar 'removido' para guardar 
       o nodo que sera removido*/
    removido = f->prim;

    /* Armazena o item que sera retornado */
    *item = removido->item;

    /* Atualiza o novo primeiro nodo da fila */
    f->prim = removido->prox;

    free(removido);

    /* Atualiza o tamanho da lista com a remocao */
    f->num -= 1;    
    
    /* Trata o caso em que a fila fica vazia */
    if (f->num == 0) 
        f->ult = NULL;

    return 1;            

}


int fila_tamanho (struct fila_t *f) 
{
    if (f == NULL)
        return -1;
    return f->num;
}


void fila_imprime (struct fila_t *f)
{
    struct fila_nodo_t *atual = f->prim;

    if (f == NULL) 
        return;

    while (atual != NULL) {
        printf("%lld", (long long int)atual->item);
        if (atual->prox != NULL) {
            printf(" ");
        }
        atual = atual->prox;
    }
}

