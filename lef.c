#include <stdio.h>
#include <stdlib.h>
#include "lef.h"
#include "fprio.h"
#include "entidades.h"
#include "eventos.h"


void inicia_lef(struct Mundo *mundo)
{
    
    struct Evento *dados_evento; 
    int tipo_evento;           
    int tempo_evento;          
    
    // Eventos iniciais sao inseridos na lef
    inicia_herois_lef(mundo);
    inicia_missoes_lef(mundo);
    agenda_fim_mundo(mundo);

    // Variavel para controlar se o evento FIM chegou
    int status = 1;

    // Continua enquanto houver eventos e ainda nao for FIM
    while (mundo->lef->num > 0 && status) 
    {
        // Retira o primeiro evento da lef e recebe seus dados
        dados_evento = fprio_retira(mundo->lef, &tipo_evento, &tempo_evento);

        // Trata em caso de erro na retirada
        if (dados_evento == NULL) 
        {
            printf("Falha ao retirar evento da LEF. Simulação encerrada.\n");
            break; 
        }

        // Atualiza o relogio
        mundo->Relogio = tempo_evento;

        processa_evento(mundo, tipo_evento, dados_evento, &status);

        // Incrementa o numero de eventos tratados
        mundo->Eventos_Tratados++;

        // Limpeza dos dados
        free(dados_evento);
    }
}


void processa_evento(struct Mundo *mundo, int tipo, struct Evento *evento_atual, int *status) 
{
    switch (tipo)
    {
        case EV_CHEGA:  
            evento_chega(mundo, mundo->Relogio, evento_atual); 
            break;

        case EV_ESPERA: 
            evento_espera(mundo, mundo->Relogio, evento_atual); 
            break;

        case EV_DESISTE: 
            evento_desiste(mundo, mundo->Relogio, evento_atual);
            break;

        case EV_AVISA: 
            evento_avisa(mundo, mundo->Relogio, evento_atual); 
            break;

        case EV_ENTRA: 
            evento_entra(mundo, mundo->Relogio, evento_atual); 
            break;

        case EV_SAI: 
            evento_sai(mundo, mundo->Relogio, evento_atual); 
            break;

        case EV_VIAJA: 
            evento_viaja(mundo, mundo->Relogio, evento_atual);
            break;

        case EV_MORRE: 
            evento_morre(mundo, mundo->Relogio, evento_atual); 
            break;
        
        case EV_MISSAO: 
            evento_missao(mundo, mundo->Relogio, evento_atual); 
            break;
        
        case EV_FIM:
            evento_fim(mundo, mundo->Relogio);
            *status = 0;
            break;
    }
}


void inicia_herois_lef (struct Mundo *mundo)  
{
    // Checa o ponteiro de mundo
    if (mundo == NULL)
    {
        printf("Erro na alocacao do mundo.\n");
        return;
    }

    // Agenda os eventos para todos os herois criados
    for (int i=0; i<mundo->NHEROIS; i++)
    {
        struct Evento *novo_evento = malloc(sizeof(struct Evento));
        if (novo_evento == NULL)
        {
            printf("Erro na alocacao do evento de chegada do heroi.\n");
            return;
        }

        // Escolhe base aleatoria
        int id_base_destino = rand() % mundo->NBASES; 

        // Escolhe um tempo aleatório 
        int tempo_chegada = rand() % 4321; 
        
        /* Atualiza os valores de acordo com os numeros sorteados,
           apenas a missao que ainda nao foi atribuida ao heroi,
           sera usado o -1 para indicar isso */
        novo_evento->idHeroi = i;
        novo_evento->idBase = id_base_destino;
        novo_evento->idMissao = ID_INEXISTENTE;

        fprio_insere(mundo->lef, novo_evento, EV_CHEGA, tempo_chegada);
    }
}


void inicia_missoes_lef (struct Mundo *mundo) 
{
    if (mundo == NULL)
    {
        printf("Erro na alocacao do mundo.\n");
        return;
    }

    for(int i=0; i < mundo->NMISSOES; i++)
    {
        struct Evento *nova_missao = malloc(sizeof(struct Evento));
        if (nova_missao == NULL)
        {
            printf("Erro na alocacao do evento de criacao da missao.\n");
            return;
        }

        // Sorteia o tempo da missao
        int tempo = rand() % (T_FIM_DO_MUNDO + 1);

        // Preenche os campos da missao
        nova_missao->idMissao = i; 
        nova_missao->idHeroi = ID_INEXISTENTE; 
        nova_missao->idBase = ID_INEXISTENTE; 

        fprio_insere(mundo->lef, nova_missao, EV_MISSAO, tempo);
    }
}

void agenda_fim_mundo (struct Mundo *mundo)
{
    struct Evento *fim_do_mundo = malloc(sizeof(struct Evento));
    if (fim_do_mundo == NULL)
    {
        printf("Erro na alocacao do evento do fim do mundo.\n");
        return;
    }

    int tempo = T_FIM_DO_MUNDO;

    // Atribuicao de valores para nao deixar o evento vazio 
    fim_do_mundo->idBase = ID_INEXISTENTE;
    fim_do_mundo->idHeroi = ID_INEXISTENTE;
    fim_do_mundo->idMissao = ID_INEXISTENTE;
    

    fprio_insere(mundo->lef, fim_do_mundo, EV_FIM, tempo);
}


