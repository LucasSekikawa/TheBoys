#ifndef EVENTOS
#define EVENTOS

#include "entidades.h"
#include "conjunto.h"
#include <math.h>

struct Evento{
    int idBase;
    int idMissao;
    int idHeroi;
};


void evento_chega (struct Mundo *mundo, int tempo_atual, struct Evento *evento_atual);
void evento_espera (struct Mundo *mundo, int tempo_atual, struct Evento *evento_atual);
void evento_desiste (struct Mundo *mundo, int tempo_atual, struct Evento *evento_atual);
void evento_viaja(struct Mundo *mundo, int tempo_atual, struct Evento *evento_atual);
void evento_avisa (struct Mundo *mundo, int tempo_atual, struct Evento *evento_atual);
void evento_entra(struct Mundo *mundo, int tempo_atual, struct Evento *evento_atual);
void evento_sai(struct Mundo *mundo, int tempo_atual, struct Evento *evento_atual);
void evento_morre(struct Mundo *mundo, int tempo_atual, struct Evento *evento_atual);
void evento_missao(struct Mundo *mundo, int tempo_atual, struct Evento *evento_atual);
void evento_fim(struct Mundo *mundo, int tempo_atual);

int calcula_distancia(struct Base *base, struct Missao *missao);
int equipe_apta(struct Mundo *mundo, int tempo_atual, struct Base *base, struct Missao *missao);
void incrementa_experiencia(struct Mundo *mundo, int id_base, int id_excluir);
int mais_experiente(struct Mundo *mundo, int id_base);
void imprime_estatisticas_heroi(struct Heroi *heroi);
void imprime_estatisticas_base(struct Base *base);
void imprime_estatisticas_globais(struct Mundo *mundo);



#endif