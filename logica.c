#include "logica.h"
#include "config.h"
#include <stdlib.h>
#include <time.h>
#include "raylib.h"





Zombi zombis[CANT_ZOMBIS];
Planta plantas[CANT_PLANTAS];
int solesJugador = 150;
Sol soles[CANT_SOLES];
float temporizadorSol = 0;
int plantaSeleccionada = -1;
Proyectil proyectiles[CANT_PROYECTILES];


void inicializarZombis(void) {
    srand(time(NULL));

    for (int i = 0; i < CANT_ZOMBIS; i++) {
        zombis[i].x = COLUMNAS * ANCHO_CELDA + i * 100;
        zombis[i].fila = rand() % FILAS;
        zombis[i].vida = 325;
        zombis[i].dano = 20;
    }
}

void moverZombis(float tiempo) {
    for (int i = 0; i < CANT_ZOMBIS; i++) {
        zombis[i].x = zombis[i].x - 17 * tiempo;
    }
}

int zombieVivo(int numeroZombie){
    if(zombis[numeroZombie].vida>0){
        return 1;
    }
    else{
        return 0;
    }
}

void recibirDano(int numeroZombie, int cantidadDano){
    if(zombieVivo(numeroZombie)){
        zombis[numeroZombie].vida -= cantidadDano;
         if (zombis[numeroZombie].vida < 0){
        zombis[numeroZombie].vida = 0;
        }
    }
}


void inicializarPlantas(void){
    for(int i=0;i<CANT_PLANTAS; i++){
        plantas[i].tipo=-1; //todavia no elige 
        plantas[i].fila= -1; //sin posicion
        plantas[i].columna= -1; //sin posicion
        plantas[i].vida= 300;
        plantas[i].dano= 20;
        plantas[i].costoSoles = 0;
        plantas[i].produccionSoles = 0;
        plantas[i].cooldown=0;
        plantas[i].temporizador = 0;
        plantas[i].activa= 0; //todavia no esta colocada


    }

}

int colocarPlanta(int fila, int columna, int tipo){
    // Comprobar que la posicion este dentro del jardin
    if (fila < 0 || fila >= FILAS || columna < 0 || columna >= COLUMNAS) {
        return 0;
    }
    // Comprobar que la casilla no este ocupada
    for(int i=0; i<CANT_PLANTAS;i++){

        if(plantas[i].activa==1 && plantas[i].fila==fila && plantas[i].columna==columna){
            return 0;
        }
    }
    // Buscar un espacio disponible
    for(int i = 0; i < CANT_PLANTAS; i++){

        if(plantas[i].activa == 0){

            plantas[i].tipo=tipo;
            plantas[i].fila=fila;
            plantas[i].columna=columna;
            plantas[i].temporizador = 0;

            if(tipo==TIPO_GUISANTE){
                plantas[i].vida=VIDA_GUISANTE;
                plantas[i].dano=DANO_GUISANTE;
                plantas[i].costoSoles = COSTO_GUISANTE;
                plantas[i].produccionSoles = PRODUCCION_GUISANTE;
                plantas[i].cooldown = COOLDOWN_GUISANTE;      

        }
            else if(tipo==TIPO_GIRASOL){
                plantas[i].vida=VIDA_GIRASOL;
                plantas[i].dano=DANO_GIRASOL;
                plantas[i].costoSoles = COSTO_GIRASOL;
                plantas[i].produccionSoles = PRODUCCION_GIRASOL;
                plantas[i].cooldown = COOLDOWN_GIRASOL;
            }
            else if(tipo==TIPO_NUEZ){
                plantas[i].vida=VIDA_NUEZ;
                plantas[i].dano=DANO_NUEZ;
                plantas[i].costoSoles = COSTO_NUEZ;
                plantas[i].produccionSoles = PRODUCCION_NUEZ;
                plantas[i].cooldown = 0;
            }
            else{
                return 0;
            }

            // Comprobar si el jugador tiene suficientes soles
            if (solesJugador < plantas[i].costoSoles) {
                return 0;
            }

            // Descontar el costo
            solesJugador -= plantas[i].costoSoles;

            plantas[i].activa = 1;

            return 1;
        }
    }

    return 0;
}


void inicializarSoles(void) {
    for (int i = 0; i < CANT_SOLES; i++) {
        soles[i].x = 0;
        soles[i].y = 0;
        soles[i].activo = 0;
        temporizadorSol = 0;
    }
}

void generarSol(void) {
    for (int i = 0; i < CANT_SOLES; i++) {
        if (soles[i].activo == 0) { //busca un espacio libre en el arreglo.
            soles[i].x = 50 + rand() % (ANCHO_JARDIN - 100); //Genera una posicion aleatoria dentro del jardin.
            soles[i].y = ALTO_MENU + 30 + rand() % (ALTO_JARDIN - 60);

            soles[i].activo = 1; //Activa el sol para que pueda dibujarse.
            return;
        }
    }
}



void recogerSol(Vector2 mousePos) {
    for (int i = 0; i < CANT_SOLES; i++) {
        if (soles[i].activo == 1) {
            Vector2 centro = {soles[i].x, soles[i].y};
            if (CheckCollisionPointCircle(mousePos, centro, RADIO_SOL)) {
                solesJugador += 25;
                soles[i].activo = 0;
            }
        }
    }
}

void actualizarSoles(float tiempo) {
    temporizadorSol += tiempo;

    if (temporizadorSol >= COOLDOWN_SOL) {
        generarSol();
        temporizadorSol = 0;
    }
}

int generarSolEn(float x, float y) {
    for (int i = 0; i < CANT_SOLES; i++) {
        if (soles[i].activo == 0) {
            soles[i].x = x;
            soles[i].y = y;
            soles[i].activo = 1;
            return 1;
        }
    }
    return 0;
}


void producirSoles(float tiempo) {
    for (int i = 0; i < CANT_PLANTAS; i++) {
        if (plantas[i].activa == 1 && plantas[i].tipo == TIPO_GIRASOL) {
            plantas[i].temporizador += tiempo;
            if (plantas[i].temporizador >= plantas[i].cooldown) {
                float x = plantas[i].columna * ANCHO_CELDA + ANCHO_CELDA / 2;
                float y = ALTO_MENU + plantas[i].fila * ALTO_CELDA + ALTO_CELDA / 2;
                generarSolEn(x, y);
                plantas[i].temporizador = 0;
            }
        }
    }
}


void seleccionarPlanta(int mouseX, int mouseY) {
    Vector2 mousePos = {(float)mouseX, (float)mouseY};

    Rectangle tarjetaGuisante = {150, 10, 110, 80};
    Rectangle tarjetaGirasol = {270, 10, 110, 80};
    Rectangle tarjetaNuez = {390, 10, 110, 80};

    if (CheckCollisionPointRec(mousePos, tarjetaGuisante)) {
        plantaSeleccionada = TIPO_GUISANTE;
    }
    else if (CheckCollisionPointRec(mousePos, tarjetaGirasol)) {
        plantaSeleccionada = TIPO_GIRASOL;
    }
    else if (CheckCollisionPointRec(mousePos, tarjetaNuez)) {
        plantaSeleccionada = TIPO_NUEZ;
    }
}

void inicializarProyectiles(void) {
    for (int i = 0; i < CANT_PROYECTILES; i++) {
        proyectiles[i].x = 0;
        proyectiles[i].y = 0;
        proyectiles[i].fila = -1;
        proyectiles[i].dano = 0;
        proyectiles[i].activo = 0;
    }
}

int generarProyectil(float x, float y, int fila, int dano) {

    for (int i = 0; i < CANT_PROYECTILES; i++) {

        if (proyectiles[i].activo == 0) {

            proyectiles[i].x = x;
            proyectiles[i].y = y;
            proyectiles[i].fila = fila;
            proyectiles[i].dano = dano;
            proyectiles[i].activo = 1;

            return 1;
        }
    }

    return 0;
}

void moverProyectiles(float tiempo) {

    for (int i = 0; i < CANT_PROYECTILES; i++) {

        if (proyectiles[i].activo == 1) {

            proyectiles[i].x += 300 * tiempo;

            if (proyectiles[i].x > ANCHO_JARDIN) {
                proyectiles[i].activo = 0;
            }
        }
    }
}