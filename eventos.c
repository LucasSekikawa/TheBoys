#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "eventos.h"
#include "fprio.h"
#include "entidades.h"
#include "fila.h"
#include "lef.h"
#include "conjunto.h"

void evento_chega (struct Mundo *mundo, int tempo_atual, struct Evento *evento_atual)
{
    int id_base = evento_atual->idBase;
    int id_heroi = evento_atual->idHeroi;

    struct Base *base = mundo->Bases[id_base];
    struct Heroi *heroi = mundo->Herois[id_heroi];

    // Checa se o heroi esta morto
    if (heroi->Status == 0) {
        return;
    }

    int espera = 0;
    int tamanho_fila = fila_tamanho(base->Espera);

    // Atualiza a base do heroi
    heroi->Base = id_base;

    struct Evento *novo_evento = malloc(sizeof(struct Evento));
    if (novo_evento == NULL)
        return;

    // Checagens para ver se o heroi entra na fila
    if  ( (cjto_card(base->Presentes) < base->Lotacao) && (tamanho_fila == 0) ) 
        espera = 1;
    else
        if (heroi->Paciencia > ( 10 * tamanho_fila ))
            espera = 1;
        else
            espera = 0;

    if (espera)
    {
        // Atribuicao dos dados do evento ESPERA       
        novo_evento->idBase = base->ID;
        novo_evento->idHeroi = heroi->ID;
        novo_evento->idMissao = ID_INEXISTENTE;

        if (fprio_insere(mundo->lef, novo_evento, EV_ESPERA, tempo_atual) == -1) 
        {
            free(novo_evento);
            printf("Erro ao inserir evento na LEF\n");
        }

        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ESPERA\n", tempo_atual, id_heroi, 
               novo_evento->idBase, cjto_card(mundo->Bases[id_base]->Presentes), 
               mundo->Bases[id_base]->Lotacao);
        return;
    }
    // Caso ele nao espere, preenche os dados do evento DESISTE
    novo_evento->idBase = base->ID;
    novo_evento->idHeroi = heroi->ID;
    novo_evento->idMissao = ID_INEXISTENTE;

    if (fprio_insere(mundo->lef, novo_evento, EV_DESISTE, tempo_atual) == -1)
    {
        free(novo_evento);
        printf("Erro ao inserir evento na LEF\n");
    }

    printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) DESISTE\n", tempo_atual, id_heroi, 
           novo_evento->idBase, cjto_card(mundo->Bases[id_base]->Presentes), 
           mundo->Bases[id_base]->Lotacao);
    return;
}

void evento_espera (struct Mundo *mundo, int tempo_atual, struct Evento *evento_atual)
{
    int id_base = evento_atual->idBase;
    int id_heroi = evento_atual->idHeroi;

    struct Base *base = mundo->Bases[id_base];
    struct Heroi *heroi = mundo->Herois[id_heroi];

    struct Evento *novo_evento = malloc(sizeof(struct Evento));
    if (novo_evento == NULL)
        return;

    // Guarda quantos herois estavam na fila antes da insercao
    int tamanho_antes = base->Espera->num;
    
    // Tenta inserir o heroi na fila
    int tamanho_depois = fila_insere(base->Espera, heroi->ID);
    
    // Caso a insercao tenha dado certo
    if (tamanho_depois > 0)  
    {
        // Preenche as informacoes do evento AVISA
        novo_evento->idBase = base->ID;
        novo_evento->idHeroi = heroi->ID;
        novo_evento->idMissao = ID_INEXISTENTE;

        // Tenta inserir na LEF 
        if (fprio_insere(mundo->lef, novo_evento, EV_AVISA, tempo_atual) == -1) 
        {
            free(novo_evento); 
            printf("Erro ao inserir evento na LEF\n");
            return;
        }

        // Atualiza o tamanho da fila max da base apos a insercao
        if (tamanho_depois > base->Fila_max)
        {
            base->Fila_max = tamanho_depois;
        }

        printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", tempo_atual, id_heroi, id_base, tamanho_antes);
        
        return;
    }

    free(novo_evento);
    return; 
}

void evento_desiste (struct Mundo *mundo, int tempo_atual, struct Evento *evento_atual)
{
    int id_base = evento_atual->idBase;
    int id_heroi = evento_atual->idHeroi;

    struct Base *base = mundo->Bases[id_base];
    struct Heroi *heroi = mundo->Herois[id_heroi];

    struct Evento *novo_evento = malloc(sizeof(struct Evento));
    if (novo_evento == NULL)
        return;

    // Salva a base original
    int base_origem = heroi->Base;
    
    // Atualiza o valor da base atual
    heroi->Base = ID_INEXISTENTE;

    int base_destino = rand() % mundo->NBASES;

    // Atribui os dados do novo evento VIAJA
    novo_evento->idHeroi = heroi->ID;
    // Coloca o id da base de origem em idBase
    novo_evento->idBase = base_origem;       
    // Coloca o id da base destino no idMissao 
    novo_evento->idMissao = base_destino;    

    if (fprio_insere(mundo->lef, novo_evento, EV_VIAJA, tempo_atual) == -1)
    {
        free(novo_evento);
        printf("Erro ao inserir evento na LEF\n");
        return;
    } 

    printf("%6d: DESIST HEROI %d BASE %d e vai viajar para outra.\n", tempo_atual, heroi->ID, base->ID);
}

void evento_viaja(struct Mundo *mundo, int tempo_atual, struct Evento *evento_atual)
{
    int id_heroi = evento_atual->idHeroi;
    struct Heroi *heroi = mundo->Herois[id_heroi];
    
    if (heroi->Status == 0) {
        return; 
    }
    
    int id_base_origem = evento_atual->idBase;      
    int id_base_destino = evento_atual->idMissao;  
    
    struct Base *base_origem = mundo->Bases[id_base_origem];
    struct Base *base_destino = mundo->Bases[id_base_destino];

    struct Evento *novo_evento = malloc(sizeof(struct Evento));
    if (novo_evento == NULL)
        return;

    // Calcula distância entre as duas bases
    int x_origem = base_origem->local.x;
    int y_origem = base_origem->local.y;
    int x_destino = base_destino->local.x;
    int y_destino = base_destino->local.y;
    
    // Calcula a distancia e depois transforma em int
    double distancia_d = sqrt(pow(x_destino - x_origem, 2.0) + pow(y_destino - y_origem, 2.0));
    int distancia = (int)(distancia_d);

    // Calcula a duracao da viagem
    int duracao = distancia / heroi->Velocidade; 
    if (duracao == 0) duracao = 1;

    // Atribui os dados do novo evento CHEGA
    novo_evento->idHeroi = heroi->ID;
    novo_evento->idBase = id_base_destino;  
    novo_evento->idMissao = ID_INEXISTENTE;

    if (fprio_insere(mundo->lef, novo_evento, EV_CHEGA, tempo_atual + duracao) == -1) 
    {
        free(novo_evento);
        printf("Erro ao inserir evento na LEF\n");
        return;
    }

    printf("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", 
           tempo_atual, id_heroi, id_base_origem, id_base_destino, 
           distancia, heroi->Velocidade, tempo_atual + duracao);
}


void evento_avisa (struct Mundo *mundo, int tempo_atual, struct Evento *evento_atual)
{
    int id_heroi_retirado;
    int id_base = evento_atual->idBase;
    
    struct Base *base = mundo->Bases[id_base];
    
    // Checa se ha vagas na base e se ha heroi esperando na fila
    while ((cjto_card(base->Presentes) < base->Lotacao) && (base->Espera->num > 0))
    {
        // Retira o primeiro heroi da fila e armazena seu id
        if (fila_retira(base->Espera, &id_heroi_retirado) == 0) 
        {
            printf("Erro na retirada da fila.");
            return;
        }

        // Adiciona o heroi retirado da fila na base
        cjto_insere(base->Presentes, id_heroi_retirado);

        // Criacao do evento apenas se as condicoes anteriores forem cumpridas
        struct Evento *novo_evento = malloc(sizeof(struct Evento));
        if (novo_evento == NULL)
            return; 

        // Preenchimento das informacoes do evento ENTRA 
        novo_evento->idBase = base->ID;
        novo_evento->idHeroi = id_heroi_retirado;
        novo_evento->idMissao = ID_INEXISTENTE;

        // Insercao do evento na lef
        if (fprio_insere(mundo->lef, novo_evento, EV_ENTRA, tempo_atual) == -1)
        {
            free(novo_evento); 
            return;
        }
        
        printf("%6d: AVISA PORTEIRO BASE %d ADMITE %d\n", tempo_atual, base->ID, id_heroi_retirado);
    }
}

void evento_entra(struct Mundo *mundo, int tempo_atual, struct Evento *evento_atual)
{
    int id_base = evento_atual->idBase;
    int id_heroi = evento_atual->idHeroi;

    struct Base *base = mundo->Bases[id_base];
    struct Heroi *heroi = mundo->Herois[id_heroi];

    // Checa se o heroi esta morto
    if (heroi->Status == 0) {
        return;
    }

    struct Evento *novo_evento = malloc(sizeof(struct Evento));
    if (novo_evento == NULL)
        return;

    // Atribuicao do novo evento SAI
    novo_evento->idHeroi = heroi->ID;
    novo_evento->idBase = base->ID;
    novo_evento->idMissao = ID_INEXISTENTE;

    int tempo_permanencia_base = 15 + heroi->Paciencia * (rand() % 20 + 1);

    if (fprio_insere(mundo->lef, novo_evento, EV_SAI, tempo_atual+tempo_permanencia_base) == -1)
    {
        free(novo_evento);
        printf("Erro ao inserir evento na LEF\n");
        return;
    } 

    printf("%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %d\n",  tempo_atual, id_heroi, 
           id_base, cjto_card(base->Presentes), base->Lotacao, 
           tempo_atual + tempo_permanencia_base);
}

void evento_sai(struct Mundo *mundo, int tempo_atual, struct Evento *evento_atual)
{
    int id_base = evento_atual->idBase;
    int id_heroi = evento_atual->idHeroi;

    struct Base *base = mundo->Bases[id_base];
    struct Heroi *heroi = mundo->Herois[id_heroi];

    if (heroi->Status == 0) {
        return;
    }

    struct Evento *novo_evento_viagem = malloc(sizeof(struct Evento));
    struct Evento *novo_evento_avisa = malloc(sizeof(struct Evento));

    if (novo_evento_viagem == NULL) 
    {
        if (novo_evento_avisa != NULL) 
            free(novo_evento_avisa);
        return;
    }
    if (novo_evento_avisa == NULL) 
    {
        free(novo_evento_viagem); 
        return;
    }

    int base_origem_viagem = heroi->Base;
    
    // Heroi e retirado da base
    cjto_retira(base->Presentes, heroi->ID);
    heroi->Base = ID_INEXISTENTE;

    int base_destino = rand() % mundo->NBASES;

    // Atribuicao dos dados do novo evento AVISA
    novo_evento_avisa->idBase = base->ID;
    novo_evento_avisa->idHeroi = ID_INEXISTENTE;
    novo_evento_avisa->idMissao = ID_INEXISTENTE;

    // Atribuicao dos dados do novo evento VIAGEM
    novo_evento_viagem->idBase = base_origem_viagem; 
    novo_evento_viagem->idHeroi = heroi->ID;
    novo_evento_viagem->idMissao = base_destino;     

    if (fprio_insere(mundo->lef, novo_evento_viagem, EV_VIAJA, tempo_atual) == -1)
    {
        free(novo_evento_viagem); 
        free(novo_evento_avisa); 
        printf("Erro ao inserir VIAJA na LEF\n");
        return;
    }

    if (fprio_insere(mundo->lef, novo_evento_avisa, EV_AVISA, tempo_atual) == -1)
    {
        free(novo_evento_avisa); 
        printf("Erro ao inserir AVISA na LEF\n");
        return;
    }
    
    printf("%6d: SAI HEROI %2d BASE %d (%2d/%2d)\n", tempo_atual, id_heroi, id_base, 
           cjto_card(mundo->Bases[id_base]->Presentes), mundo->Bases[id_base]->Lotacao);
}


void evento_morre(struct Mundo *mundo, int tempo_atual, struct Evento *evento_atual)
{

    int id_base = evento_atual->idBase;
    int id_heroi = evento_atual->idHeroi;
    int id_missao = evento_atual->idMissao;


    struct Base *base = mundo->Bases[id_base];
    struct Heroi *heroi = mundo->Herois[id_heroi];


    struct Evento *novo_evento= malloc(sizeof(struct Evento));
    if (novo_evento == NULL) 
    {
        printf("Erro ao inserir evento na LEF\n");
        return; 
    }

    // Atualiza o status do herói e libera uma vaga na base
    cjto_retira(base->Presentes, heroi->ID); 
    heroi->Status = 0;
    heroi->Base = ID_INEXISTENTE;

    // Preenche as informacoes do evento AVISA
    novo_evento->idBase = base->ID;
    novo_evento->idHeroi = heroi->ID; 
    novo_evento->idMissao = ID_INEXISTENTE;
    
    // Agenda o evento AVISA
    if (fprio_insere(mundo->lef, novo_evento, EV_AVISA, tempo_atual) == -1) 
    {
        free(novo_evento);  
        printf("Erro ao inserir evento na LEF\n");
        return;
    }

    printf("%6d: MORRE HEROI %2d MISSAO %d\n", tempo_atual, heroi->ID, id_missao);
}


void evento_missao(struct Mundo *mundo, int tempo_atual, struct Evento *evento_atual)
{
    if (evento_atual == NULL) {
        printf("Erro ao receber evento atual\n");
        return; 
    }
    
    int id_missao = evento_atual->idMissao;
    struct Missao *missao = mundo->Missoes[id_missao];

    // Variaveis para encontrar a base apta mais proxima da missao
    int id_BMP_apta = ID_INEXISTENTE; 
    int distancia_min_apta = -1;

    // Variaveis para encontrar a base mais proxima da missao
    int id_BMP = ID_INEXISTENTE; 
    int distancia_BMP = -1; 

    mundo->Missoes[id_missao]->tentativas++;
    mundo->Total_Tentativas++;

    printf("%6d: MISSAO %d TENT %d HAB REQ: [", tempo_atual, id_missao, 
           mundo->Missoes[id_missao]->tentativas);
    cjto_imprime(mundo->Missoes[id_missao]->Habilidades);
    printf("]\n"); 

    // Percorre todas as bases procurando pela mais proxima 
    for (int i = 0; i < mundo->NBASES; i++)
    {
        struct Base *base_atual = mundo->Bases[i];
        int nova_distancia = calcula_distancia(base_atual, missao);

        printf("%6d: MISSAO %d BASE %d DIST %d HEROIS [ ", tempo_atual, id_missao, i, nova_distancia);
        cjto_imprime(mundo->Bases[i]->Presentes);
        printf(" ]\n"); 

        // Checa se a base nao esta vazia
        if (cjto_card(base_atual->Presentes) > 0) 
        {
            // Condicao usada para o caso de uso de composto V
            if (id_BMP == ID_INEXISTENTE || nova_distancia < distancia_BMP)
            {
                distancia_BMP = nova_distancia;
                id_BMP = i;
            }

            // Procura pela base apta mais proxima
            if (equipe_apta(mundo, tempo_atual, base_atual, missao)) 
            {
                if (id_BMP_apta == ID_INEXISTENTE || nova_distancia < distancia_min_apta)
                {
                    distancia_min_apta = nova_distancia;
                    id_BMP_apta = i;
                }
            } 
        }
    }

    // Checa se existe equipe apta
    if (id_BMP_apta != ID_INEXISTENTE)
    {
        missao->status = 1;
        mundo->Missoes_Cumpridas++;
        mundo->Bases[id_BMP_apta]->Missoes_Cumpridas++;
        
        // Incrementa experiencia de todos herois da base
        incrementa_experiencia(mundo, id_BMP_apta, ID_INEXISTENTE);
        
        printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: [", tempo_atual, id_missao, id_BMP_apta);
        cjto_imprime(mundo->Bases[id_BMP_apta]->Presentes);
        printf("]\n"); 

        return;
    }

    // Checa se e possivel usar o composto V
    if (mundo->NCompostosV > 0 && tempo_atual % 2500 == 0 && id_BMP != ID_INEXISTENTE)
    {            
        // Verifica se a base mais próxima tem heróis
        if (cjto_card(mundo->Bases[id_BMP]->Presentes) > 0) 
        {
            int id_heroi_sacrificio = mais_experiente(mundo, id_BMP); 

            if (id_heroi_sacrificio != ID_INEXISTENTE) 
            {
                mundo->NCompostosV--;
                missao->status = 1;
                mundo->Missoes_Cumpridas++;
                
                // Incrementa contador de missões da base
                mundo->Bases[id_BMP]->Missoes_Cumpridas++;

                printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: [", tempo_atual, id_missao, id_BMP);
                cjto_imprime(mundo->Bases[id_BMP]->Presentes);
                printf("]\n"); 

                struct Evento *novo_evento = malloc(sizeof(struct Evento));
                if (novo_evento == NULL)
                {
                    printf("Erro ao alocar espaco para evento");
                    return;
                }

                // Atribui os dados para o novo evento MORRE
                novo_evento->idBase = id_BMP; 
                novo_evento->idMissao = missao->ID;
                novo_evento->idHeroi = id_heroi_sacrificio; 

                if (fprio_insere(mundo->lef, novo_evento, EV_MORRE, tempo_atual) == -1)
                {
                    printf("Erro ao inserir evento na LEF");
                    free(novo_evento);
                    return;
                }
                
                // Incrementa xp para todos menos para o heroi sacrificado
                incrementa_experiencia(mundo, id_BMP, id_heroi_sacrificio); 
                return; 
            }
        }
    }

    // Em caso de missao impossivel
    struct Evento *novo_evento = malloc(sizeof(struct Evento));
    if (novo_evento == NULL)
    {
        printf("Erro ao alocar espaco para evento");
        return;
    }

    // Preenche os dados do novo evento MISSAO
    novo_evento->idBase = ID_INEXISTENTE;
    novo_evento->idMissao = missao->ID;
    novo_evento->idHeroi = ID_INEXISTENTE;

    fprio_insere(mundo->lef, novo_evento, EV_MISSAO, tempo_atual + 24*60);

    printf("%6d: MISSAO %d IMPOSSIVEL\n", tempo_atual, id_missao);
}


int calcula_distancia(struct Base *base, struct Missao *missao)
{
    int x_base = base->local.x;
    int y_base = base->local.y;
    int x_missao = missao->local.x;
    int y_missao = missao->local.y;

    double distancia_double = sqrt(pow(x_missao - x_base, 2) + pow(y_missao - y_base, 2));

    // Conversão para inteiro com truncamento
    int distancia = (int)(distancia_double);
    return distancia;

}

int equipe_apta(struct Mundo *mundo, int tempo_atual, struct Base *base, struct Missao *missao)
{
    // Checa se base nao esta vazia
    if (cjto_card(base->Presentes) == 0)
        return 0;

    // Aloca o conjunto de união
    struct cjto_t *cjto_uniao_final = cjto_cria(mundo->NHABILIDADES); 
    if (cjto_uniao_final == NULL) 
        return 0; 
    
    // Itera os ids de todos os herois presentes na base
    for (int id_heroi = 0; id_heroi < mundo->NHEROIS; id_heroi++) 
    {
        if (cjto_pertence(base->Presentes, id_heroi)) 
        {
            struct Heroi *heroi = mundo->Herois[id_heroi];
            
            // Ignora heróis mortos
            if (heroi->Status == 0) 
                continue;
            
            
            struct cjto_t *habilidades_heroi = heroi->Habilidades;

            printf("%6d: MISSAO %d HAB HEROI %2d: [ ", tempo_atual, missao->ID, id_heroi);
            cjto_imprime(habilidades_heroi);
            printf(" ]\n"); 
            
            // Adiciona todas as habilidades do herói ao conjunto união
            for (int k = 0; k < mundo->NHABILIDADES; k++)
                if (cjto_pertence(habilidades_heroi, k))
                    cjto_insere(cjto_uniao_final, k);
        }
    }
    
    // Checa se o conjunto uniao final contem as habilidades necessarias
    int apta = cjto_contem(cjto_uniao_final, missao->Habilidades);

    printf("%6d: MISSAO %d UNIAO HAB BASE %d: [ ", tempo_atual, missao->ID, base->ID);
    cjto_imprime(cjto_uniao_final);
    printf(" ]\n"); 

    // Destroi o conjunto auxiliar
    cjto_destroi(cjto_uniao_final);

    return apta; 
}

void incrementa_experiencia(struct Mundo *mundo, int id_base, int id_excluir)
{
    struct Base *base = mundo->Bases[id_base];

    // Checa todos herois do mundo procurando pelos que estavam na base
    for (int id_heroi=0; id_heroi<mundo->NHEROIS; id_heroi++)
    {
        // Caso esteja, aumenta em 1 sua experiencia
        if (cjto_pertence(base->Presentes, id_heroi))
        {
            struct Heroi *heroi = mundo->Herois[id_heroi];
            // Checa se o heroi esta vivo e se ele nao foi heroi marcado para morrer
            if ((heroi->Status != 0) && (heroi->ID != id_excluir))
                heroi->Experiencia++;
        }
    }
}

int mais_experiente(struct Mundo *mundo, int id_base)
{
    struct Base *base = mundo->Bases[id_base];

    int id_mais_experiente = ID_INEXISTENTE;
    int maior_experiencia = -1;

    // Checa todos herois do mundo procurando pelos que estavam na base
    for (int id_heroi = 0; id_heroi < mundo->NHEROIS; id_heroi++)
    {
        if (cjto_pertence(base->Presentes, id_heroi))
        {
            struct Heroi *heroi = mundo->Herois[id_heroi];
            
            // Checa se o heroi esta vivo
            if (heroi->Status != 0) 
            {
                // Checa a experiencia atual com a maior experiencia encontrada
                if (heroi->Experiencia > maior_experiencia)
                {
                    maior_experiencia = heroi->Experiencia;
                    id_mais_experiente = id_heroi;
                }
            }
        }
    }
    return id_mais_experiente;
}

void evento_fim(struct Mundo *mundo, int tempo_atual)
{ 
    printf("%6d: FIM\n\n", tempo_atual);

    for (int i = 0; i < mundo->NHEROIS; i++) 
        imprime_estatisticas_heroi(mundo->Herois[i]);
    printf("\n");

    for (int i = 0; i < mundo->NBASES; i++)  
        imprime_estatisticas_base(mundo->Bases[i]); 
    printf("\n");

    imprime_estatisticas_globais(mundo);
}

void imprime_estatisticas_heroi(struct Heroi *heroi)
{
    if (heroi->Status == 1)
    {
        printf("HEROI %2d VIVO PAC %3d VEL %4d EXP %4d HABS ", heroi->ID, heroi->Paciencia, heroi->Velocidade, heroi->Experiencia);
        printf("[");
        cjto_imprime(heroi->Habilidades);
        printf("]\n");

        return;         
    }

    printf("HEROI %2d MORTO PAC %3d VEL %4d EXP %4d HABS ", heroi->ID, heroi->Paciencia, heroi->Velocidade, heroi->Experiencia);
    printf("[");
    cjto_imprime(heroi->Habilidades);
    printf("]\n");   
}

void imprime_estatisticas_base(struct Base *base)
{
    printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n", base->ID, base->Lotacao, base->Fila_max, base->Missoes_Cumpridas);
}

void imprime_estatisticas_globais(struct Mundo *mundo)
{
    printf("EVENTOS TRATADOS: %d\n", mundo->Eventos_Tratados);


    float taxa_sucesso = (float)mundo->Missoes_Cumpridas * 100.0f / mundo->NMISSOES;
    printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n", mundo->Missoes_Cumpridas, mundo->NMISSOES, taxa_sucesso);

    int min_tentativas = mundo->Missoes[0]->tentativas;
    int max_tentativas = mundo->Missoes[0]->tentativas;
    
    // Percorre todas as missoes para saber o min e max de tentativas e depois calcula a media de todas
    for (int i = 1; i < mundo->NMISSOES; i++)
    {
        if (mundo->Missoes[i]->tentativas < min_tentativas)
            min_tentativas = mundo->Missoes[i]->tentativas;
        if (mundo->Missoes[i]->tentativas > max_tentativas)
            max_tentativas = mundo->Missoes[i]->tentativas;    
    }
    
    float media = (float)mundo->Total_Tentativas / mundo->NMISSOES;
    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n", min_tentativas, max_tentativas, media);



    int num_mortos = 0;
    for (int j = 0; j < mundo->NHEROIS; j++)
      if (mundo->Herois[j]->Status == 0) 
        num_mortos++;

    float taxa_mortalidade = (float)num_mortos * 100.0f / mundo->NHEROIS;
    printf("TAXA MORTALIDADE: %.1f%%\n", taxa_mortalidade);
}