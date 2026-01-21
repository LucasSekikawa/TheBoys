#ifndef DESTROI
#define DESTROI

#include "entidades.h"
#include "conjunto.h"


void libera_mundo_falha(struct Mundo *mundo, int herois_criados, int bases_criadas, int missoes_criadas);
void libera_missao (struct Missao *missao);
void libera_base (struct Base *base);
void libera_heroi (struct Heroi *heroi);
void destroi_mundo(struct Mundo *mundo);


#endif
