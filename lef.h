#ifndef LEF
#define LEF

#include "entidades.h"
#include "fprio.h"
#include "inicia.h"
#include "eventos.h"


#define EV_CHEGA    1
#define EV_ESPERA   2
#define EV_DESISTE  3
#define EV_AVISA    4
#define EV_ENTRA    5
#define EV_SAI      6
#define EV_VIAJA    7
#define EV_MORRE    8
#define EV_MISSAO   9
#define EV_FIM      10

#define ID_INEXISTENTE -1

void inicia_lef(struct Mundo *mundo);
void processa_evento(struct Mundo *mundo, int tipo, struct Evento *evento_atual, int *status);
void inicia_herois_lef (struct Mundo *mundo);
void inicia_herois_lef (struct Mundo *mundo);
void inicia_missoes_lef (struct Mundo *mundo);
void agenda_fim_mundo (struct Mundo *mundo);


#endif