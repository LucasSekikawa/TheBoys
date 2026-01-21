#include <stdio.h>
#include <stdlib.h>
#include "entidades.h"
#include "fila.h"
#include "inicia.h"
#include "destroi.h"
#include "conjunto.h"

void destroi_mundo(struct Mundo *mundo)
{
    if (mundo == NULL) {
        return;
    }

    // Libera o espaco dos herois
    for (int i = 0; i < mundo->NHEROIS; i++) {
        if (mundo->Herois[i] != NULL) {
            // Destroi os conjuntos das habilidades
            cjto_destroi(mundo->Herois[i]->Habilidades);
            free(mundo->Herois[i]); 
        }
    }
    free(mundo->Herois); 
    mundo->Herois = NULL;

    // Libera o espaco das bases
    for (int k = 0; k < mundo->NBASES; k++) {
        if (mundo->Bases[k] != NULL) {
            // Destroi o conjunto de presentes e a fila de espera
            cjto_destroi(mundo->Bases[k]->Presentes);
            fila_destroi(mundo->Bases[k]->Espera);
            free(mundo->Bases[k]); 
        }
    }
    free(mundo->Bases);
    mundo->Bases = NULL;

    // Libera o espaco das missoes
    for (int j = 0; j < mundo->NMISSOES; j++) {
        if (mundo->Missoes[j] != NULL) {
            // Destroi o conjunto das habilidades
            cjto_destroi(mundo->Missoes[j]->Habilidades);
            free(mundo->Missoes[j]); 
        }
    }
    free(mundo->Missoes); 

    // Libera a fila de prioridades
    fprio_destroi(mundo->lef);

    // Destroi o mundo
    free(mundo);
}

void libera_mundo_falha(struct Mundo *mundo, int herois_criados, int bases_criadas, int missoes_criadas)
{
    if (mundo == NULL)
        return;

    /* Libera a memoria de todas as missoes que foram criadas
       e por fim libera o vetor de ponteiros */
    if (mundo->Missoes != NULL) {
        for (int i = 0; i < missoes_criadas; i++) 
            libera_missao(mundo->Missoes[i]); 
        free(mundo->Missoes); 
    }

    /* Libera a memoria de todas as bases que foram criadas
       e por fim libera o vetor de ponteiros */
    if (mundo->Bases != NULL) {
        for (int j = 0; j < bases_criadas; j++)
            libera_base(mundo->Bases[j]); 
        free(mundo->Bases); 
    }

    /* Libera a memoria de todas os herois que foram criadas
       e por fim libera o vetor de ponteiros */
    if (mundo->Herois != NULL) {
        for (int k = 0; k < herois_criados; k++) 
            libera_heroi(mundo->Herois[k]); 
        free(mundo->Herois); 
    }
    
    // Libera o ponteiro para mundo
    free(mundo);
}

void libera_missao (struct Missao *missao)
{
    if (missao == NULL) 
        return;
    
    // Destroi primeiro o conjunto de habilidades
    if (missao->Habilidades != NULL)
        cjto_destroi(missao->Habilidades); 
    
    // Libera a struct Missao
    free(missao);
}

void libera_base (struct Base *base)
{
    if (base == NULL)
        return;
    
    // Destroi o conjunto de herois presentes
    if (base->Presentes != NULL)
        cjto_destroi(base->Presentes);

    // Destroi a fila da base
    if (base->Espera != NULL)
        fila_destroi(base->Espera);

    free(base);
}

void libera_heroi (struct Heroi *heroi)
{   
    if (heroi == NULL)
        return;

    // Destroi o conjunto de habilidades do heroi
    if (heroi->Habilidades != NULL)
        cjto_destroi(heroi->Habilidades);

    free(heroi);
}