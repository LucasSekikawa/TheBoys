#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "entidades.h"
#include "inicia.h"
#include "fila.h"
#include "destroi.h"
#include "lef.h"

struct Mundo *cria_mundo()
{
    struct Mundo *mundo = malloc(sizeof(struct Mundo));
    if (mundo == NULL)
    {
        return NULL;
    }

    srand(time(NULL));

    // Atribuicao dos valores pre-determinados
    mundo->Relogio = T_INICIO;
    mundo->TamanhoMundo.x = N_TAMANHO_MUNDO;
    mundo->TamanhoMundo.y = N_TAMANHO_MUNDO;
    mundo->NHABILIDADES = N_HABILIDADES;
    mundo->NHEROIS = N_HEROIS;
    mundo->NBASES = N_BASES;
    mundo->NMISSOES = N_MISSOES;
    mundo->NCompostosV = N_COMPOSTOS_V;

    mundo->Eventos_Tratados = 0;
    mundo->Herois_Mortos = 0;
    mundo->Missoes_Cumpridas = 0;
    mundo->Total_Tentativas = 0;

    // Alocação e verificacao do vetor de ponteiros Herois
    mundo->Herois = malloc(mundo->NHEROIS * sizeof(struct Heroi *));
    if (mundo->Herois == NULL) {
        free(mundo);
        return NULL;
    }

    // Alocação e verificacao do vetor de ponteiros Bases
    mundo->Bases = malloc(mundo->NBASES * sizeof(struct Base *));
    if (mundo->Bases == NULL) {
        free(mundo->Herois);
        free(mundo);
        return NULL;
    }
        
    // Alocacao e verificacao do vetor de ponteiros Missoes
    mundo->Missoes = malloc(mundo->NMISSOES * sizeof(struct Missao *)); 
    if (mundo->Missoes == NULL) {
        free(mundo->Bases); 
        free(mundo->Herois); 
        free(mundo);
        return NULL;
    }

    for (int i=0; i<mundo->NHEROIS; i++)
    {
        mundo->Herois[i] = cria_heroi(i);
        if (mundo->Herois[i] == NULL)
        {
            // Passa apenas o numero de herois para libera-los
            libera_mundo_falha(mundo, i, 0, 0); 
            return NULL;
        }
    }
    for (int j=0; j<mundo->NBASES; j++)
    {
        mundo->Bases[j] = cria_base(j);
        if (mundo->Bases[j] == NULL)
        {
            // Passa os herois criados, e o numero de bases para libera-los
            libera_mundo_falha(mundo, mundo->NHEROIS, j, 0); 
            return NULL;
        }     
    }
    
    for (int k=0; k<mundo->NMISSOES; k++)
    {
        mundo->Missoes[k] = cria_missao(k);
        if (mundo->Missoes[k] == NULL)
        {
            // Passa todos herois, todas as bases, e as missoes para serem liberadas
            libera_mundo_falha(mundo, mundo->NHEROIS, mundo->NBASES, k); 
            return NULL;
        }
    }

    // Criacao da fila de eventos futuros
    mundo->lef = fprio_cria();

    // Caso a lef receba NULL, tudo que foi alocado ate agora deve ser liberado
    if (mundo->lef == NULL) {
        libera_mundo_falha(mundo, mundo->NHEROIS, mundo->NBASES, mundo->NMISSOES); 
        return NULL;
    }

    return mundo; 
}

struct Heroi *cria_heroi (int id)
{
    struct Heroi *heroi_ptr;
    heroi_ptr = malloc(sizeof(struct Heroi));
    if (heroi_ptr == NULL)
        return NULL;

    // Atribuicoes iniciais
    heroi_ptr->ID = id;
    heroi_ptr->Experiencia = 0;
    heroi_ptr->Paciencia = rand() % 101 ;
    heroi_ptr->Velocidade = (rand() % 4951) + 50;
    heroi_ptr->Habilidades = cjto_aleat((rand() % 3) + 1, N_HABILIDADES);

    // Testa se cjto_aleat nao retornou null
    if (heroi_ptr->Habilidades == NULL) {
        free(heroi_ptr);
        return NULL;
    }

    // Inicia em -1 pois ainda nao tem base
    heroi_ptr->Base = -1;
    heroi_ptr->Status = 1;

    return heroi_ptr;
}

struct Base *cria_base (int id)
{
    struct Base *base_ptr;
    base_ptr = malloc(sizeof(struct Base));
    if (base_ptr == NULL)
        return NULL;

    // Atribuicoes inicias
    base_ptr->ID = id;
    base_ptr->local.x = rand() % N_TAMANHO_MUNDO;
    base_ptr->local.y = rand() % N_TAMANHO_MUNDO;
    base_ptr->Lotacao = (rand() % 8) + 3;
    base_ptr->Fila_max = 0;
    base_ptr->Missoes_Cumpridas = 0;
    
    // Checagem de alocacoes
    base_ptr->Presentes = cjto_cria(N_HEROIS); 
    if (base_ptr->Presentes == NULL) {
        free(base_ptr);
        return NULL;
    }

    base_ptr->Espera = fila_cria ();
    if (base_ptr->Espera == NULL) {
        free(base_ptr->Presentes);
        free(base_ptr);
        return NULL;
    }

    return base_ptr;
}

struct Missao *cria_missao(int id)
{
    struct Missao *missao_ptr;
    missao_ptr = malloc(sizeof(struct Missao));
    if (missao_ptr == NULL)
        return NULL;
        
    // Atribuicoes iniciais
    missao_ptr->ID = id;
    missao_ptr->local.x = rand() % N_TAMANHO_MUNDO;
    missao_ptr->local.y = rand() % N_TAMANHO_MUNDO;
    missao_ptr->tentativas = 0;
    missao_ptr->status = 0;

    // Checagem de alocacao
    missao_ptr->Habilidades = cjto_aleat((rand() % 5) + 6, N_HABILIDADES);
    if (missao_ptr->Habilidades == NULL) {
        free(missao_ptr);
        return NULL;
    }

    return missao_ptr;
}