#include "config.h"

#include "logica.h"

ConfiguracionJuego configuracion_juego;
TipoZombi tipos_zombi[NUM_TIPOS_ZOMBI];
TipoPlanta tipos_planta[NUM_TIPOS_PLANTA];
ConfiguracionOleada configuracion_oleada;

int filas_jardin = 5;
int columnas_jardin = 9;

void inicializarConfiguracion(void) {
    //Juego
    ConfiguracionJuego *c = &configuracion_juego;

    c->ancho_celda = 80;
    c->alto_celda = 90;
    c->alto_menu = 100;

    c->ancho_img_casa = 100;
    c->ancho_carril = 80;

    c->alto_cortacesped = 55;
    c->posicion_cortacesped = -72;
    c->vel_cortacesped = 450;

    c->radio_sol = 18;
    c->cooldown_sol = 8.0f;

    //Zombis
    tipos_zombi[ZOMBI_NORMAL] = (TipoZombi) {
        .vida = 325, .dano = 50, .velocidad = 17
    };
    tipos_zombi[ZOMBI_FLAG] = (TipoZombi) {

        .vida = 325, .dano = 75, .velocidad = 22
    };
    tipos_zombi[ZOMBI_CONO] = (TipoZombi) {
        .vida = 600, .dano = 100, .velocidad = 9
    };

    // Plantas
    tipos_planta[TIPO_GUISANTE] = (TipoPlanta) {
        .vida = 300, .dano = 20, .costo = 100, .produccion = 0, .cooldown = 1.5f
    };
    tipos_planta[TIPO_GIRASOL] = (TipoPlanta) {
        .vida = 300, .dano = 0, .costo = 50, .produccion = 0, .cooldown = 10.0f
    };
    tipos_planta[TIPO_NUEZ] = (TipoPlanta) {
        .vida = 4000, .dano = 0, .costo = 50, .produccion = 0, .cooldown = 0.0f
    };
    tipos_planta[TIPO_CACTUS] = (TipoPlanta) {
        .vida = 250, .dano = 25, .costo = 125, .produccion = 0, .cooldown = 1.5f
    };

    //Oleadas
    configuracion_oleada = (ConfiguracionOleada) {
        .zombis_por_oleada = 5,
        .tiempo_entre_zombis = 3.0f,
        .tiempo_entre_oleadas = 5.0f,
        .max_oleadas = 5
    };
}

// calcula los valores que dependen del tamaño del mapa
void calcularDerivadas(void) {
    ConfiguracionJuego *c = &configuracion_juego;

    c->ancho_jardin = columnas_jardin * c->ancho_celda;
    c->alto_jardin = filas_jardin * c->alto_celda;
    c->ancho_casa = c->ancho_img_casa + c->ancho_carril;

    // si el mapa es mas grande, habrá mas soles
    float base = c->cooldown_sol;
    float cooldown = base * 45 / (float)(filas_jardin * columnas_jardin);
    if (cooldown > base) cooldown = base;
    if (cooldown < 1.0f) cooldown = 1.0f;
    c->cooldown_sol = cooldown;
}

void crear_zombi_id(int id, Zombi *zombi) { // inicializa las características de un zombi según su tipo
    zombi->vida = tipos_zombi[id].vida;
    zombi->dano = tipos_zombi[id].dano;
    zombi->velocidad = tipos_zombi[id].velocidad;
    zombi->tipo = id;
}

void crear_planta_id(int id, Planta *planta) { // inicializa las características de una planta según su tipo
    planta->vida = tipos_planta[id].vida;
    planta->dano = tipos_planta[id].dano;
    planta->costoSoles = tipos_planta[id].costo;
    planta->cooldown = tipos_planta[id].cooldown;
}
