#include "logica.h"
#include "config.h"
#include <stdlib.h>
#include <time.h>
#include "raylib.h"


//variables y arreglos globales 
Zombi zombis[CANT_ZOMBIS];
Planta plantas[CANT_PLANTAS];
int solesJugador = 200;
Sol soles[CANT_SOLES];
float temporizadorSol = 0;
int plantaSeleccionada = -1;
Proyectil proyectiles[CANT_PROYECTILES];
int oleadaActual = 1;
int zombisRestantes = 0;
float temporizadorOleada = 0;
float temporizadorEntreOleadas = 0;
Cortacesped cortacespedes[FILAS];


void inicializarZombis(void) {
    srand(time(NULL)); //inicializa los numeros aleatorios

    for (int i = 0; i < CANT_ZOMBIS; i++) {
        zombis[i].x = 0;
        zombis[i].fila = -1;
        zombis[i].vida = 0; //comienzan inactivos; las oleadas los generan
        zombis[i].dano = DANO_ZOMBI;
    }
}


void moverZombis(float tiempo) {

    for (int i = 0; i < CANT_ZOMBIS; i++) {

        if (zombieVivo(i)) {
            // Si llega a la casa, se detiene
            if (zombis[i].x <= 0) {
                zombis[i].x = 0;
                continue;
            }
            // Si no hay una planta delante, avanza
            if (!hayPlantaDelante(zombis[i].fila, zombis[i].x)) {
                zombis[i].x -= 17 * tiempo;
            }
        }
    }
}


int zombieVivo(int numeroZombie){ //comprueba si un zombie sigue vivo segun la vida que le queda
    if(zombis[numeroZombie].vida>0){
        return 1;
    }
    else{
        return 0;
    }
}

void recibirDano(int numeroZombie, int cantidadDano){ //resta vida al zombie cuando recibe un proyectil
    if(zombieVivo(numeroZombie)){
        zombis[numeroZombie].vida -= cantidadDano;
         if (zombis[numeroZombie].vida < 0){ //evita que el zombie quede en negativo
        zombis[numeroZombie].vida = 0;
        }
    }
}


void inicializarPlantas(void){
    for(int i=0;i<CANT_PLANTAS; i++){
        plantas[i].tipo=-1; //todavia no elige 
        plantas[i].fila= -1; //sin posicion
        plantas[i].columna= -1; //sin posicion
        plantas[i].vida= 200;
        plantas[i].dano= 20;
        plantas[i].costoSoles = 0;
        plantas[i].produccionSoles = 0;
        plantas[i].cooldown=0;
        plantas[i].temporizador = 0;
        plantas[i].activa= 0; //todavia no esta colocada


    }

}

int colocarPlanta(int fila, int columna, int tipo){
    int costo;

    // Comprobar que la posicion este dentro del jardin
    if (fila < 0 || fila >= FILAS || columna < 0 || columna >= COLUMNAS) {
        return 0;
    }

    // Comprobar que el tipo de planta sea valido y obtener su costo
    if (tipo == TIPO_GUISANTE) {
        costo = COSTO_GUISANTE;
    }
    else if (tipo == TIPO_GIRASOL) {
        costo = COSTO_GIRASOL;
    }
    else if (tipo == TIPO_NUEZ) {
        costo = COSTO_NUEZ;
    }
    else {
        return 0;
    }

    // Comprobar que el jugador tenga suficientes soles antes de ocupar un espacio
    if (solesJugador < costo) {
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

            if(tipo==TIPO_GUISANTE){ //todas las carac. que tiene el guisante 
                plantas[i].vida=VIDA_GUISANTE;
                plantas[i].dano=DANO_GUISANTE;
                plantas[i].costoSoles = COSTO_GUISANTE;
                plantas[i].produccionSoles = PRODUCCION_GUISANTE;
                plantas[i].cooldown = COOLDOWN_GUISANTE;      

        }
            else if(tipo==TIPO_GIRASOL){ //todas las carac. que tiene el girasol
                plantas[i].vida=VIDA_GIRASOL;
                plantas[i].dano=DANO_GIRASOL;
                plantas[i].costoSoles = COSTO_GIRASOL;
                plantas[i].produccionSoles = PRODUCCION_GIRASOL;
                plantas[i].cooldown = COOLDOWN_GIRASOL;
            }
            else if(tipo==TIPO_NUEZ){ //todas las carac. que tiene la nuez
                plantas[i].vida=VIDA_NUEZ;
                plantas[i].dano=DANO_NUEZ;
                plantas[i].costoSoles = COSTO_NUEZ;
                plantas[i].produccionSoles = PRODUCCION_NUEZ;
                plantas[i].cooldown = 0;
            }
            else{
                return 0;
            }

            // Descontar el costo
            solesJugador -= plantas[i].costoSoles;

            plantas[i].activa = 1; //activa la planta para que se use 

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
    }
    temporizadorSol = 0;
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

            // Obtiene el centro del sol considerando el espacio de la casa
            Vector2 centro = {ANCHO_CASA + soles[i].x, soles[i].y};
            if (CheckCollisionPointCircle(mousePos, centro, RADIO_SOL)) {
                solesJugador += 25;
                soles[i].activo = 0;
            }
        }
    }
}

void actualizarSoles(float tiempo) {
    temporizadorSol += tiempo;

    if (temporizadorSol >= COOLDOWN_SOL) { //genera un sol cuando se cumple el cooldown
        generarSol();
        temporizadorSol = 0;
    }
}

int generarSolEn(float x, float y) { //genera un sol en una posicion especifica
    for (int i = 0; i < CANT_SOLES; i++) {
        if (soles[i].activo == 0) { //busca un espacio disponible para crear el sol
            soles[i].x = x;
            soles[i].y = y;
            soles[i].activo = 1;
            return 1;
        }
    }
    return 0;
}


void producirSoles(float tiempo) { //produccion de soles por el girasol
    for (int i = 0; i < CANT_PLANTAS; i++) {
        if (plantas[i].activa == 1 && plantas[i].tipo == TIPO_GIRASOL) { //comprueba que la planta este activa y sea un girasol
            plantas[i].temporizador += tiempo;
            if (plantas[i].temporizador >= plantas[i].cooldown) { //comprueba si ya paso el cooldwon para producir un sol
                //calcula el centro de la casilla donde esta el girasol
                float x = plantas[i].columna * ANCHO_CELDA + ANCHO_CELDA / 2;
                float y = ALTO_MENU + plantas[i].fila * ALTO_CELDA + ALTO_CELDA / 2;
                if (generarSolEn(x, y)) { //genera un sol en la posicion del girasol
                    plantas[i].temporizador = 0; //reinicia el temporizador si pudo crear el sol
                }
            }
        }
    }
}


void seleccionarPlanta(int mouseX, int mouseY) {
    Vector2 mousePos = {(float)mouseX, (float)mouseY}; //guarda la posicion del mouse en el vector 2

    //area de cada tarjeta 
    Rectangle tarjetaGuisante = {150, 10, 110, 80};
    Rectangle tarjetaGirasol = {270, 10, 110, 80};
    Rectangle tarjetaNuez = {390, 10, 110, 80};

    //comprueba sobre que tarjeta hizo clic el jugador 
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

    for (int i = 0; i < CANT_PROYECTILES; i++) { //busca un espacio disponible

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

void moverProyectiles(float tiempo) { //mueve los proyectiles hacia la derecha

    for (int i = 0; i < CANT_PROYECTILES; i++) {

        if (proyectiles[i].activo == 1) {

            proyectiles[i].x += 300 * tiempo;

            //desactiva el proyectil cuando se sale del jardin
            if (proyectiles[i].x > ANCHO_JARDIN) { 
                proyectiles[i].activo = 0;
            }
        }
    }
}

int hayZombieDelante(int fila, float x) {
    for (int i = 0; i < CANT_ZOMBIS; i++) {
        if (zombieVivo(i) && zombis[i].fila == fila && zombis[i].x > x && zombis[i].x < ANCHO_JARDIN){
            return 1;
        }
    }
    return 0;
}


void actualizarDisparos(float tiempo) {

    for (int i = 0; i < CANT_PLANTAS; i++) {

        if (plantas[i].activa == 1 && plantas[i].tipo == TIPO_GUISANTE) { //comprueba que la planta este activa y sea guisante 

            //calcula el centro de la casilla donde esta el guisante 
            float x = plantas[i].columna * ANCHO_CELDA + ANCHO_CELDA / 2;
            float y = ALTO_MENU + plantas[i].fila * ALTO_CELDA + ALTO_CELDA / 2;

            plantas[i].temporizador += tiempo;

            if (hayZombieDelante(plantas[i].fila, x)) { //comprueba si hay un zombie delante
                if (plantas[i].temporizador >= plantas[i].cooldown) { //comprueba si ya paso el cooldown
                    if (generarProyectil(x + 20, y, plantas[i].fila, plantas[i].dano)) {
                        plantas[i].temporizador = 0;
                    }
                }
            }
        }
    }
}



void detectarImpactos(void) {
    for (int i = 0; i < CANT_PROYECTILES; i++) {
        if (!proyectiles[i].activo) continue; //si el proyectil esta inactivo pasa al sgte
        for (int j = 0; j < CANT_ZOMBIS; j++) {
            //comprueba que ek zombie este vivo y que el proyectuk esta dentro de su rango
            if (zombieVivo(j) && proyectiles[i].fila == zombis[j].fila && proyectiles[i].x >= zombis[j].x &&proyectiles[i].x <= zombis[j].x + ANCHO_CELDA / 2) {
                recibirDano(j, proyectiles[i].dano); //resta vida al zombie 
                proyectiles[i].activo = 0; //desativa el proyectil luego del impacto
                break;
            }
        }
    }
}

int hayPlantaDelante(int fila, float x) {

    int plantaEncontrada = 0;
    float distanciaMenor = ANCHO_JARDIN + 100;

    for (int i = 0; i < CANT_PLANTAS; i++) {

        if (plantas[i].activa == 1 &&
            plantas[i].fila == fila &&
            plantas[i].columna * ANCHO_CELDA < x &&
            plantas[i].columna * ANCHO_CELDA + ANCHO_CELDA > x - 17) {

            float distancia = x - (plantas[i].columna * ANCHO_CELDA + ANCHO_CELDA);

            if (distancia < distanciaMenor) {
                distanciaMenor = distancia;
                plantaEncontrada = 1;
            }
        }
    }
    return plantaEncontrada;
}


void atacarPlanta(float tiempo) {
    for (int i = 0; i < CANT_ZOMBIS; i++) {
        if (zombieVivo(i)) {
            for (int j = 0; j < CANT_PLANTAS; j++) {
                if (plantas[j].activa == 1 &&
                    plantas[j].fila == zombis[i].fila &&
                    plantas[j].columna * ANCHO_CELDA < zombis[i].x &&
                    plantas[j].columna * ANCHO_CELDA + ANCHO_CELDA > zombis[i].x - 17) {
                    plantas[j].vida -= zombis[i].dano * tiempo;
                    if (plantas[j].vida <= 0) {
                        plantas[j].vida = 0;
                        plantas[j].activa = 0;
                    }
                    break;
                }
            }
        }
    }
}


void iniciarOleada(void) {

    zombisRestantes = ZOMBIS_POR_OLEADA * oleadaActual;
    temporizadorOleada = 0; //espera tres segundos antes del primer zombi
}

int generarZombi(void) {
    for (int i = 0; i < CANT_ZOMBIS; i++) {
        if (!zombieVivo(i)) {
            zombis[i].fila = rand() % FILAS;
            zombis[i].x = ANCHO_JARDIN + 50;
            zombis[i].vida = 325;
            zombis[i].dano = DANO_ZOMBI;

            return 1;
        }
    }
    return 0;
}

void actualizarOleadas(float tiempo) {

    // Todavia quedan zombis por generar
    if (zombisRestantes > 0) {

        temporizadorOleada += tiempo;

        if (temporizadorOleada >= TIEMPO_ENTRE_ZOMBIS) {

            if (generarZombi()) {
                zombisRestantes--;
                temporizadorOleada = 0;
            }
        }
    }

    // Ya se generaron todos los zombis de esta oleada
    else {

        int zombisVivos = 0;

        // Contamos cuántos zombis siguen vivos
        for (int i = 0; i < CANT_ZOMBIS; i++) {

            if (zombieVivo(i)) {
                zombisVivos++;
            }
        }

        // Si no queda ninguno, esperamos para la siguiente oleada
        if (zombisVivos == 0) {

            temporizadorEntreOleadas += tiempo;

            if (temporizadorEntreOleadas >= TIEMPO_ENTRE_OLEADAS) {

                oleadaActual++;

                iniciarOleada();

                temporizadorEntreOleadas = 0;
            }
        }
    }
}


void inicializarCortacespedes(void) {
    for (int i = 0; i < FILAS; i++) {
        cortacespedes[i].x = X_INI_CORTACESPED;
        cortacespedes[i].disponible = 1;
        cortacespedes[i].activo = 0;
    }
}

// se activa cuando un zombi vivo de su fila llega al borde del jardín
void activarCortacespedes(void) {
    for (int f = 0; f < FILAS; f++) {
        if (!cortacespedes[f].disponible) continue;
        for (int j = 0; j < CANT_ZOMBIS; j++) {
            if (zombieVivo(j) && zombis[j].fila == f && zombis[j].x <= 0) {
                cortacespedes[f].disponible = 0;
                cortacespedes[f].activo = 1;
                break;
            }
        }
    }
}

// avanza y elimina a los zombis de su fila que toca
void actualizarCortacespedes(float tiempo) {
    for (int i = 0; i < FILAS; i++) {

        if (cortacespedes[i].activo == 1) {

            // Elimina los zombis que encuentra en su fila
            for (int j = 0; j < CANT_ZOMBIS; j++) {
                if (zombieVivo(j) &&
                    zombis[j].fila == i &&
                    zombis[j].x <= cortacespedes[i].x + ANCHO_CELDA) {

                    zombis[j].vida = 0;
                }
            }

            // Avanza el cortacesped
            cortacespedes[i].x += 450 * tiempo;

            // Comprueba si salio del jardin
            if (cortacespedes[i].x > ANCHO_JARDIN) {
                cortacespedes[i].activo = 0;
            }
        }
    }
}





