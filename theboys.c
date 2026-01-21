// programa principal do projeto "The Boys - 2024/2"
// Autor: Lucas Mazurkievicz Sekikawa, GRR 20255726

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "entidades.h" 
#include "inicia.h"
#include "destroi.h" 
#include "fprio.h"     
#include "conjunto.h"  
#include "fila.h"
#include "lef.h"
#include "destroi.h"

int main ()
{
    struct Mundo *mundo = cria_mundo();

    inicia_lef(mundo);

    destroi_mundo(mundo);

    return 0;
}