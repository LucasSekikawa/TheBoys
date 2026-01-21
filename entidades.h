#ifndef ENTIDADES
#define ENTIDADES

#include "fprio.h"
#include "conjunto.h"


struct Heroi {
    int ID;
    struct cjto_t* Habilidades; 
    int Paciencia;
    int Velocidade;
    int Experiencia;
    int Base;
    int Status;
};

struct Localizacao {
    int x;
    int y;
};

struct Base {
    int ID;
    int Lotacao;
    int Fila_max;
    int Missoes_Cumpridas;
    struct cjto_t* Presentes;
    struct fila_t* Espera;
    struct Localizacao local;

};

struct Missao {
    int ID;
    struct cjto_t* Habilidades; 
    struct Localizacao local;
    int status;
    int tentativas;
};

struct Mundo {
    int NHEROIS;
    int NBASES;
    int NMISSOES;
    int NHABILIDADES;
    int NCompostosV;
    struct Heroi **Herois;
    struct Base **Bases;
    struct Missao **Missoes;
    struct Localizacao TamanhoMundo;
    struct fprio_t *lef;
    int Relogio;
    int Eventos_Tratados;
    int Missoes_Cumpridas;
    int Herois_Mortos;
    int Total_Tentativas;
};




#endif