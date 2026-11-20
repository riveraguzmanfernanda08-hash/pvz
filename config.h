#pragma once

#include "constantes.h"
#include "logica.h"

typedef struct {
    int ancho_celda;
    int alto_celda;
    int alto_menu;

    int ancho_jardin;   // derivada: columnas * ancho_celda
    int alto_jardin;    // derivada: filas * alto_celda

    int ancho_img_casa;
    int ancho_carril;
    int ancho_casa;     // derivada: ancho_img_casa + ancho_carril

    int alto_cortacesped;
    int posicion_cortacesped;
    int vel_cortacesped;

    int radio_sol;
    float cooldown_sol;
} ConfiguracionJuego;

typedef struct {
    int vida;
    int dano;
    int velocidad;
} TipoZombi;

typedef struct {
    int vida;
    int dano;
    int costo;
    int produccion;
    float cooldown;
} TipoPlanta;

typedef struct {
    int zombis_por_oleada;
    float tiempo_entre_zombis;
    float tiempo_entre_oleadas;
    int max_oleadas;
} ConfiguracionOleada;

enum {
    ZOMBI_NORMAL,
    ZOMBI_FLAG,
    ZOMBI_SIN_CABEZA,
    NUM_TIPOS_ZOMBI
};

enum {
    TIPO_GUISANTE,
    TIPO_GIRASOL,
    TIPO_NUEZ,
    TIPO_CACTUS,
    NUM_TIPOS_PLANTA
};

extern ConfiguracionJuego configuracion_juego;
extern TipoZombi tipos_zombi[NUM_TIPOS_ZOMBI];
extern TipoPlanta tipos_planta[NUM_TIPOS_PLANTA];
extern ConfiguracionOleada configuracion_oleada;

void inicializarConfiguracion(void);
void calcularDerivadas(void);

void crear_zombi_id(int id, Zombi *zombi);
void crear_planta_id(int id, Planta *planta);
