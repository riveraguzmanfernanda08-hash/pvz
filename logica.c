#include "logica.h"
#include "config.h"
#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include <math.h>


//variables y arreglos globales 
Zombi zombis[CANT_ZOMBIS]; //guarda todos los zombis
Planta plantas[CANT_PLANTAS]; //guarda todas las plantas
int solesJugador = 200; //cantidad de soles al iniciar
Sol soles[CANT_SOLES];  //guaarda los soles que aparecen en el jardin
float temporizadorSol = 0; //temporizador para generar soles
int plantaSeleccionada = -1; 
Proyectil proyectiles[CANT_PROYECTILES]; //guarda todos los proyectiles 
int oleadaActual = 1;
int zombisRestantes = 0;
float temporizadorOleada = 0;
float temporizadorEntreOleadas = 0;
Cortacesped cortacespedes[FILAS]; //guarda los cortacespedes
int juegoPerdido = 0;
int zombisFlagPendientes = 0;
int zombisSinCabezaPendientes = 0;
int palaSeleccionada = 0;
int juegoGanado = 0;



void inicializarZombis(void) {
    srand(time(NULL)); //inicializa los numeros aleatorios

    for (int i = 0; i < CANT_ZOMBIS; i++) { //recorre todod los espacios del arreglo de zombies
        crear_zombi_id(ZOMBI_NORMAL, &zombis[i]);
        zombis[i].x = 0;
        zombis[i].fila = -1;
        zombis[i].vida = 0; //comienzan inactivos; las oleadas los generan
    }
}


void moverZombis(float tiempo) { //actualiza la posicion de los zombies para que avancen
    for (int i = 0; i < CANT_ZOMBIS; i++) { //recorre todos los zombies 

        if (zombieVivo(i)) { //ve si esta vivo
            // Si llega a la casa, se detiene
            if (zombis[i].x <= 0) {
                zombis[i].x = 0;
                continue;
            }

            float velocidad = zombis[i].velocidad;


            // Si no hay una planta delante, avanza
            if (!hayPlantaDelante(zombis[i].fila, zombis[i].x)) {
                zombis[i].x -= velocidad * tiempo;
            }
        }
    }
}


int zombieVivo(int numeroZombie){ //comprueba si un zombie sigue vivo segun la vida que le queda
    if(zombis[numeroZombie].vida>0){ //si es mayor a 0, sigue vivo
        return 1;
    }
    else{ //si su vida es 0 o menor, ya esta muerto
        return 0;
    }
}

void recibirDano(int numeroZombie, int cantidadDano){ //resta vida al zombie cuando recibe un proyectil
    if(zombieVivo(numeroZombie)){ //ve si el zombie sigue vivo
        zombis[numeroZombie].vida -= cantidadDano; //resta la vida al zombie
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
        plantas[i].cooldown=0;
        plantas[i].temporizador = 0;
        plantas[i].activa= 0; //todavia no esta colocada


    }

}

int colocarPlanta(int fila, int columna, int tipo) { // coloca una planta en la fila y columna indicada
    // Comprobar que la posicion este dentro del jardin
    if(fila < 0 || fila >= FILAS ||
        columna < 0 || columna >= COLUMNAS) {
        return 0;
    }

    // Validar el tipo y obtener los datos base (incluido el costo)
    if(tipo < 0 || tipo >= NUM_TIPOS_PLANTA) {
        return 0;
    }

    Planta nueva = {0};
    crear_planta_id(tipo, &nueva);

    // Comprobar que el jugador tenga suficientes soles
    if(solesJugador < nueva.costoSoles) {
        return 0;
    }

    // Comprobar que la casilla no este ocupada
    for(int i = 0; i < CANT_PLANTAS; i++) {

        if (plantas[i].activa == 1 &&
            plantas[i].fila == fila &&
            plantas[i].columna == columna) {
            return 0;
        }
    }

    // Buscar un espacio disponible
    for(int i = 0; i < CANT_PLANTAS; i++) {
        if (plantas[i].activa == 0) {
            plantas[i] = nueva;

            plantas[i].tipo = tipo;
            plantas[i].fila = fila;
            plantas[i].columna = columna;
            plantas[i].temporizador = 0;

            plantas[i].activa = 1;

            solesJugador -= nueva.costoSoles;
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

void generarSol(void) {  //genera un sol en una posicion aleatoria del jardin
    for (int i = 0; i < CANT_SOLES; i++) {
        if (soles[i].activo == 0) { //busca un espacio libre en el arreglo.
            soles[i].x = 50 + rand() % (configuracion_juego.ancho_jardin - 100); //Genera una posicion aleatoria dentro del jardin.
            soles[i].y = configuracion_juego.alto_menu + 30 + rand() % (configuracion_juego.alto_jardin - 60);


            soles[i].activo = 1; //Activa el sol para que pueda dibujarse.
            return;
        }
    }
}




void recogerSol(Vector2 mousePos) {
    for (int i = 0; i < CANT_SOLES; i++) {
        if (soles[i].activo == 1) {

            // Obtiene el centro del sol considerando el espacio de la casa
            Vector2 centro = {configuracion_juego.ancho_casa + soles[i].x, soles[i].y};
            if (CheckCollisionPointCircle(mousePos, centro, configuracion_juego.radio_sol)) { //comprueba si el mouse esta dentro del circulo del sol
                solesJugador += 25; //aumenta los soles del jugador
                soles[i].activo = 0; //desactiva el sol despues de recogerlo
            }
        }
    }
}

void actualizarSoles(float tiempo) {
    temporizadorSol += tiempo;

    if (temporizadorSol >= configuracion_juego.cooldown_sol) { //genera un sol cuando se cumple el cooldown

        generarSol();
        temporizadorSol = 0; //reinicia el temporizador
    }
}

int generarSolEn(float x, float y) { //genera un sol en una posicion especifica
    for (int i = 0; i < CANT_SOLES; i++) {
        if (soles[i].activo == 0) { //busca un espacio disponible para crear el sol
            soles[i].x = x;
            soles[i].y = y;
            soles[i].activo = 1;
            return 1; //indica que el sol se pudo generar 
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
                float x = plantas[i].columna * configuracion_juego.ancho_celda + configuracion_juego.ancho_celda / 2;
                float y = configuracion_juego.alto_menu + plantas[i].fila * configuracion_juego.alto_celda + configuracion_juego.alto_celda / 2;

                if (generarSolEn(x, y)) { //genera un sol en la posicion del girasol
                    plantas[i].temporizador = 0; //reinicia el temporizador si pudo crear el sol
                }
            }
        }
    }
}




void seleccionarPlanta(int mouseX, int mouseY) { 
    Vector2 mousePos = {(float)mouseX, (float)mouseY};
    // Define el area de cada tarjeta del menu
    Rectangle tarjetaGuisante = {150, 10, 110, 80};
    Rectangle tarjetaGirasol = {270, 10, 110, 80};
    Rectangle tarjetaNuez = {390, 10, 110, 80};
    Rectangle tarjetaCactus = {510, 10, 110, 80};
    Rectangle tarjetaPala = {630, 10, 110, 80};

    // Comprueba si el jugador hizo clic en la tarjeta
    if (CheckCollisionPointRec(mousePos, tarjetaGuisante)) {
        plantaSeleccionada = TIPO_GUISANTE;
        palaSeleccionada = 0;
    }
    else if (CheckCollisionPointRec(mousePos, tarjetaGirasol)) {
        plantaSeleccionada = TIPO_GIRASOL;
        palaSeleccionada = 0;
    }
    else if (CheckCollisionPointRec(mousePos, tarjetaNuez)) {
        plantaSeleccionada = TIPO_NUEZ;
        palaSeleccionada = 0;
    }
    else if (CheckCollisionPointRec(mousePos, tarjetaCactus)) {
        plantaSeleccionada = TIPO_CACTUS;
        palaSeleccionada = 0;
    }
    else if (CheckCollisionPointRec(mousePos, tarjetaPala)) {
        plantaSeleccionada = -1;
        palaSeleccionada = 1;
    }
}


void quitarPlanta(int fila, int columna) { //quita una planta de la casilla indicada

    for (int i = 0; i < CANT_PLANTAS; i++) {

        // Busca una planta activa que este en la fila y columna indicadas
        if (plantas[i].activa == 1 &&
            plantas[i].fila == fila &&
            plantas[i].columna == columna) {

            plantas[i].activa = 0; //desactiva la planta
            palaSeleccionada = 0; //deselecciona la pala

            return;
        }
    }

    // Si no había planta, la pala sigue seleccionada
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

        if (proyectiles[i].activo == 0) { //busca un espacio disponible

            proyectiles[i].x = x;
            proyectiles[i].y = y;
            proyectiles[i].fila = fila;
            proyectiles[i].dano = dano;
            proyectiles[i].activo = 1;

            return 1; //indica que el proyectil se pudo generar
        }
    }

    return 0; //indica que no habia espacio disponible 
}

void moverProyectiles(float tiempo) { //mueve los proyectiles hacia la derecha

    for (int i = 0; i < CANT_PROYECTILES; i++) {

        if (proyectiles[i].activo == 1) {

            proyectiles[i].x += 300 * tiempo; //actualiza la posicion del proyectil

            //desactiva el proyectil cuando se sale del jardin
            if (proyectiles[i].x > configuracion_juego.ancho_jardin) {
                proyectiles[i].activo = 0;
            }
        }
    }
}

int hayZombieDelante(int fila, float x) {
    for (int i = 0; i < CANT_ZOMBIS; i++) {
          //Ve si el zombie que esta vivo, en la misma fila y delante de la planta
        if (zombieVivo(i) && zombis[i].fila == fila && zombis[i].x > x && zombis[i].x < configuracion_juego.ancho_jardin){
            return 1; //hay un zombie delante
        }
    }
    return 0; //no hay zombie delante
}



void actualizarDisparos(float tiempo) {

    for (int i = 0; i < CANT_PLANTAS; i++) {

        // Comprueba que la planta este activa y pueda disparar
        if (plantas[i].activa == 1 && (plantas[i].tipo == TIPO_GUISANTE || plantas[i].tipo == TIPO_CACTUS)) {

            // Calcula el centro de la casilla
            float x = plantas[i].columna * configuracion_juego.ancho_celda + configuracion_juego.ancho_celda / 2;
            float y = configuracion_juego.alto_menu + plantas[i].fila * configuracion_juego.alto_celda + configuracion_juego.alto_celda / 2;

            plantas[i].temporizador += tiempo; //actualiza el tiempo de espera

            // Comprueba si hay un zombie delante
            if (hayZombieDelante(plantas[i].fila, x)) {

                // Comprueba si ya paso el cooldown
                if (plantas[i].temporizador >= plantas[i].cooldown) {

                    if (generarProyectil(x + 20, y, plantas[i].fila, plantas[i].dano)) { //genera un proyectil desde la posicion de la planta
                        plantas[i].temporizador = 0; //reinicia el temporizador
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
            if (zombieVivo(j) && proyectiles[i].fila == zombis[j].fila && proyectiles[i].x >= zombis[j].x &&proyectiles[i].x <= zombis[j].x + configuracion_juego.ancho_celda / 2) {
                recibirDano(j, proyectiles[i].dano); //resta vida al zombie 
                proyectiles[i].activo = 0; //desativa el proyectil luego del impacto
                break; //termina de buscar el proyectil
            }
        }
    }
}

int hayPlantaDelante(int fila, float x) {

    int plantaEncontrada = 0;
    float distanciaMenor = configuracion_juego.ancho_jardin + 100;

    for (int i = 0; i < CANT_PLANTAS; i++) {

        // Ve que la planta este activa, en la misma fila y que este delante del zombie
        if (plantas[i].activa == 1 && plantas[i].fila == fila &&
            plantas[i].columna * configuracion_juego.ancho_celda < x && plantas[i].columna * configuracion_juego.ancho_celda + configuracion_juego.ancho_celda > x - 17) {

            float distancia = x - (plantas[i].columna * configuracion_juego.ancho_celda + configuracion_juego.ancho_celda); //calcula la distancia entre el zombie y la planta

            if (distancia < distanciaMenor) { //guarda la planta encontrada que esta mas cerca 
                distanciaMenor = distancia;
                plantaEncontrada = 1;
            }
        }
    }
    return plantaEncontrada; //retorna 1 si encontro una planta y 0 si no
}


void atacarPlanta(float tiempo) {
    for (int i = 0; i < CANT_ZOMBIS; i++) {
        if (zombieVivo(i)) { //ve que el zombie este vivo
            for (int j = 0; j < CANT_PLANTAS; j++) { 

                //Ve que la planta este activa, en la misma fila  y que el zombie este en contacto con ella
                if (plantas[j].activa == 1 && plantas[j].fila == zombis[i].fila &&
                    plantas[j].columna * configuracion_juego.ancho_celda < zombis[i].x && plantas[j].columna * configuracion_juego.ancho_celda + configuracion_juego.ancho_celda > zombis[i].x - 17) {
                    plantas[j].vida -= zombis[i].dano * tiempo; //resta vida a la planta segun el daño del zombie y el tiempo transcurrido
                    if (plantas[j].vida <= 0) { //ve si se quedo sin vida
                        plantas[j].vida = 0;
                        plantas[j].activa = 0; //desactiva la planta si esta murio
                    }
                    break; //zombie deja de buscar plantas
                }
            }
        }
    }
}


void iniciarOleada(void) {
    zombisRestantes = configuracion_oleada.zombis_por_oleada * oleadaActual;

    // Calcula cuantos zombis bandera apareceran
    zombisFlagPendientes = (oleadaActual + 2) / 3;

    // Calcula cuantos zombis sin cabeza apareceran desde la oleada 2
    zombisSinCabezaPendientes = oleadaActual / 2;

    // Evita que haya mas zombis especiales que zombis disponibles
    if (zombisFlagPendientes > zombisRestantes) {
        zombisFlagPendientes = zombisRestantes;
    }

    if (zombisSinCabezaPendientes >
        zombisRestantes - zombisFlagPendientes) {
        zombisSinCabezaPendientes =
            zombisRestantes - zombisFlagPendientes;
    }

    temporizadorOleada = 0; //reinicia el tiempo de la oleada
}

int generarZombi(void) {
    for(int i = 0; i < CANT_ZOMBIS; i++) {
        if(!zombieVivo(i)) { // busca un espacio disponible
            int fila = 0;
            int espacioDisponible = 0;
            float posicionX = configuracion_juego.ancho_jardin + 50;

            // Buscar una fila donde haya espacio para aparecer
            for(int intento = 0; intento < 20; intento++) {

                fila = rand() % FILAS; // selecciona una fila aleatoria
                espacioDisponible = 1;

                for(int j = 0; j < CANT_ZOMBIS; j++) { // ve si hay otro zombi cerca de esa posicion

                    if(zombieVivo(j) && zombis[j].fila == fila) {

                        if(fabsf(zombis[j].x - posicionX) < 100) {
                            espacioDisponible = 0;
                            break;
                        }
                    }
                }

                if(espacioDisponible) { // si hay espacio termina la busqueda

                    break;
                }
            }

            if(!espacioDisponible) { // si no hay espacio, no genera el zombi
                return 0;
            }

            // Elegir al azar el tipo de zombi segun los pendientes
            int cantidadEspeciales =
            zombisFlagPendientes + zombisSinCabezaPendientes;

            int seleccion = rand() % zombisRestantes;
            int tipo;

            if(seleccion < zombisFlagPendientes) {
                tipo = ZOMBI_FLAG;
                zombisFlagPendientes--;
            } else if(seleccion < cantidadEspeciales) {
                tipo = ZOMBI_SIN_CABEZA;
                zombisSinCabezaPendientes--;
            } else {
                tipo = ZOMBI_NORMAL;
            }

            // Crear el zombi (vida, dano, velocidad, tipo) y ubicarlo
            Zombi nuevo = {0};
            crear_zombi_id(tipo, &nuevo);
            nuevo.fila = fila;
            nuevo.x = posicionX;

            zombis[i] = nuevo;

            return 1; // el zombi se genero correctamente
        }
    }

    return 0; // no se pudo generar el zombi
}


void actualizarOleadas(float tiempo) {
    if (zombisRestantes > 0) { //ve si todavia quedan zombies por generar

        temporizadorOleada += tiempo;

        if (temporizadorOleada >= configuracion_oleada.tiempo_entre_zombis) { //ve si ya paso el tiempo entre la aparicion de zombies


            if (generarZombi()) { //intenta generar un zombi
                zombisRestantes--;
                temporizadorOleada = 0; //reinicia el temporizador 
            }
        }
    }

    // Ya se generaron todos los zombis de esta oleada
    else {

        int zombisVivos = 0;

        for (int i = 0; i < CANT_ZOMBIS; i++) { //cuenta cuantos zombies estan vivos

            if (zombieVivo(i)) {
                zombisVivos++;
            }
        }

        // Si no queda ninguno, esperamos
        if (zombisVivos == 0) {

            temporizadorEntreOleadas += tiempo;

            if (temporizadorEntreOleadas >= configuracion_oleada.tiempo_entre_oleadas) {

                // Comprobamos si terminamos la ultima oleada
                if (oleadaActual >= configuracion_oleada.max_oleadas) {

                    juegoGanado = 1;
                    return;
                }

                oleadaActual++;//aumenta el numero de oleada y la inicia

                iniciarOleada();

                temporizadorEntreOleadas = 0; //reincia el temporizador entre oleadas
            }
        }
    }
}


void inicializarCortacespedes(void) {
    for (int i = 0; i < FILAS; i++) {
        cortacespedes[i].x = configuracion_juego.posicion_cortacesped;
        cortacespedes[i].disponible = 1;
        cortacespedes[i].activo = 0;
    }
}

// se activa cuando un zombi vivo de su fila llega al borde del jardín
void activarCortacespedes(void) {
    for (int f = 0; f < FILAS; f++) {
        if (!cortacespedes[f].disponible) continue;//si el cortacesped ya no esta disponible, pasa a la siguiente fila
        for (int j = 0; j < CANT_ZOMBIS; j++) { //ve si un zombie vivo de esa fila llego al borde del jardin
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

        if (cortacespedes[i].activo == 1) { //ve si el cortacesped esta activo

            // Elimina los zombis que encuentra en su fila
            for (int j = 0; j < CANT_ZOMBIS; j++) {
                if (zombieVivo(j) &&
                    zombis[j].fila == i &&
                    zombis[j].x <= cortacespedes[i].x + configuracion_juego.ancho_celda) {


                    zombis[j].vida = 0; //elimina al zombie que toca el cortacesped
                }
            }

            cortacespedes[i].x += configuracion_juego.vel_cortacesped * tiempo; // Avanza el cortacesped hacia la derecha

            // Comprueba si salio del jardin
            if (cortacespedes[i].x > configuracion_juego.ancho_jardin) {
                cortacespedes[i].activo = 0; //se desactiva
            }
        }
    }
}

void comprobarDerrota(void) {
    for (int i = 0; i < CANT_ZOMBIS; i++) {
        if (zombieVivo(i) && zombis[i].x <= 0) { //Ve si el zombie esta vivo y llego al borde del jardin

            int fila = zombis[i].fila;

             // Ve que la fila sea valida y que su cortacesped no se este activo
            if (fila >= 0 && fila < FILAS && cortacespedes[fila].disponible == 0 &&
                cortacespedes[fila].activo == 0) {

                juegoPerdido = 1; //activa la pantalla de derrota
                break;
            }
        }
    }
}
