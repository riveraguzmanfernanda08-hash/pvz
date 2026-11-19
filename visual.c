#include "visual.h"
#include "raylib.h"
#include "config.h"
#include "logica.h"
#include <stdio.h>

Texture2D jardin;
Texture2D casa;
Texture2D cortacesped;
Texture2D zombieFlag;
Texture2D zombieNormal;
Texture2D zombieSinCabeza;
Texture2D pala;

Texture2D girasol;
Texture2D nuez;
Texture2D cactus;
Texture2D lanzaguisantes;
Texture2D sol;

Texture2D cardGuisante;
Texture2D cardGirasol;
Texture2D cardNuez;

void iniciarVentana() { //inicializar ventana del juego con sus dimensiones y titulo
    InitWindow(
        ANCHO_CASA + ANCHO_JARDIN,
        ALTO_MENU + FILAS * ALTO_CELDA,
        "Plants vs zombies"
    );

    MaximizeWindow(); //maximizar ventana 
}

void cerrarVentana() { //cierra ventana
    CloseWindow();
}


// para que cargue el pasto
void cargarJardin(void) {
    jardin = LoadTexture("assets/pastito_solo_pasto.png");
    SetTextureFilter(jardin, TEXTURE_FILTER_BILINEAR);
}

void liberarJardin(void) {
    UnloadTexture(jardin);
}

void cargarCasa(void) {
    casa = LoadTexture("assets/casa.png");
    SetTextureFilter(casa, TEXTURE_FILTER_BILINEAR); //mejora la calidad de la textura 
}

void liberarCasa(void) {
    UnloadTexture(casa);
}


void cargarCortacesped(void) {
    cortacesped = LoadTexture("assets/cortacesped.png");
    SetTextureFilter(cortacesped, TEXTURE_FILTER_BILINEAR);
}

void liberarCortacesped(void) {
    UnloadTexture(cortacesped);
}


void cargarZombieNormal(void) {
    zombieNormal = LoadTexture("assets/zombie_normal.png");
    SetTextureFilter(zombieNormal, TEXTURE_FILTER_POINT);

}

void liberarZombieNormal(void) {
    UnloadTexture(zombieNormal);
}


void cargarZombieFlag(void) {
    zombieFlag = LoadTexture("assets/zombie_flag.png");
    SetTextureFilter(zombieFlag, TEXTURE_FILTER_BILINEAR);
}

void liberarZombieFlag(void) {
    UnloadTexture(zombieFlag);
}

void cargarZombieSinCabeza(void) {
    zombieSinCabeza = LoadTexture("assets/zombie_sin_cabeza.png");
    SetTextureFilter(zombieSinCabeza, TEXTURE_FILTER_POINT);
}

void liberarZombieSinCabeza(void) {
    UnloadTexture(zombieSinCabeza);
}


void cargarPala(void) {
    pala = LoadTexture("assets/pala.png");
    SetTextureFilter(pala, TEXTURE_FILTER_BILINEAR);
}

void liberarPala(void) {
    UnloadTexture(pala);
}

void cargarPlantas(void) {
    girasol= LoadTexture("assets/girasol.png");
    nuez= LoadTexture("assets/nuez.png");
    cactus= LoadTexture("assets/cactus.png");
    lanzaguisantes= LoadTexture("assets/lanzaguisantes.png");
    sol = LoadTexture("assets/Sun.png");

    cardGuisante = LoadTexture("assets/peashooterCard.png");
    cardGirasol  = LoadTexture("assets/sunflowerCard.png");
    cardNuez     = LoadTexture("assets/wallnutCard.png");

    SetTextureFilter(girasol, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(nuez, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(cactus, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(lanzaguisantes, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(sol, TEXTURE_FILTER_BILINEAR);

    SetTextureFilter(cardGuisante, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(cardGirasol, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(cardNuez, TEXTURE_FILTER_BILINEAR);
}

void liberarPlantas(void) {
    UnloadTexture(girasol);
    UnloadTexture(nuez);
    UnloadTexture(cactus);
    UnloadTexture(lanzaguisantes);
    UnloadTexture(sol);
    UnloadTexture(cardGuisante);
    UnloadTexture(cardGirasol);
    UnloadTexture(cardNuez);
}


//tamaño de la imagen del jardin
void dibujarJardin() {

    DrawRectangle(0, 0, ANCHO_CASA + ANCHO_JARDIN, ALTO_MENU, BROWN);

    DrawTexturePro(
        jardin,
        (Rectangle){0, 0, jardin.width, jardin.height},
        (Rectangle){ANCHO_CASA, ALTO_MENU, ANCHO_JARDIN, ALTO_JARDIN},
        (Vector2){0, 0},
        0,
        WHITE
    );
}

void dibujarCasa(void) {

    Rectangle destino = {  // Define la posicion y el tamaño de destino de la imagen
        0,
        ALTO_MENU,
        ANCHO_IMG_CASA,
        ALTO_JARDIN
    };

    Rectangle origen = {
        0, 0,
        casa.width,
        casa.height
    };

    DrawTexturePro(  // Dibuja la imagen al tamaño indicado
        casa,
        origen,
        destino,
        (Vector2){0, 0},
        0,
        WHITE
    );
}


void dibujarCortacespedes(void) {

    float alto = ALTO_CORTACESPED;
    float ancho = alto * cortacesped.width / cortacesped.height;

    for (int i = 0; i < FILAS; i++) {

        if (!cortacespedes[i].disponible && !cortacespedes[i].activo) { // Si el cortacesped no esta disponible ni activo, no se dibuja
            continue;
        }

        Rectangle destino = { //Define posicion y tamaño del cortacesped
            ANCHO_CASA + cortacespedes[i].x,
            ALTO_MENU + i * ALTO_CELDA + (ALTO_CELDA - alto) / 2,
            ancho,
            alto
        };

        // Recortar 4 px del borde derecho de la imagen 
        Rectangle origen = {
            0, 0,
            cortacesped.width - 4,
            cortacesped.height
        };

        DrawTexturePro(
            cortacesped,
            origen,
            destino,
            (Vector2){0, 0},
            0,
            WHITE
        );
    }
}


void dibujarCarril(void) { //Es el carril que esta entre la casa y el jardin

    DrawRectangle(
        ANCHO_IMG_CASA,
        ALTO_MENU,
        ANCHO_CARRIL,
        ALTO_JARDIN,
        BEIGE
    );

    // Lineas divisorias entre filas
    for (int i = 0; i <= FILAS; i++) {

        DrawLine(
            ANCHO_IMG_CASA,
            ALTO_MENU + i * ALTO_CELDA,
            ANCHO_CASA,
            ALTO_MENU + i * ALTO_CELDA,
            (Color){70, 70, 75, 255}
        );
    }

    // Borde del lado del jardin
    DrawLine(
        ANCHO_CASA,
        ALTO_MENU,
        ANCHO_CASA,
        ALTO_MENU + ALTO_JARDIN,
        BLACK
    );
}


// carga el menu de arriba (para seleccionar plantas, pala)
void dibujarMenu(void) {
DrawRectangle(0, 0, 1000, ALTO_MENU, BROWN); //fondo del menu
// coloca la imagen de sol
Rectangle origenSol = { 0.0f, 0.0f, (float)sol.width, (float)sol.height };
Rectangle destinoSol = { 42.0f - 20.0f, 47.0f - 20.0f, 40.0f, 40.0f };
DrawTexturePro(sol, origenSol, destinoSol, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);

//convierte los soles a texto
char textoSoles[20];
sprintf(textoSoles, "%d", solesJugador);
DrawText(textoSoles, 70, 35, 25, DARKBROWN);

    // TARJETA DEL GUISANTE
    DrawTexturePro(
        cardGuisante,
        (Rectangle){0, 0, (float)cardGuisante.width, (float)cardGuisante.height},
        (Rectangle){150, 10, 110, 80},
        (Vector2){0, 0},
        0,
        WHITE
    );
    if (plantaSeleccionada == TIPO_GUISANTE) {
        DrawRectangleLinesEx((Rectangle){150, 10, 110, 80}, 4, GOLD);
    } else {
        DrawRectangleLines(150, 10, 110, 80, BLACK);
    }

    // TARJETA DEL GIRASOL
    DrawTexturePro(
        cardGirasol,
        (Rectangle){0, 0, (float)cardGirasol.width, (float)cardGirasol.height},
        (Rectangle){270, 10, 110, 80},
        (Vector2){0, 0},
        0,
        WHITE
    );
    if (plantaSeleccionada == TIPO_GIRASOL) {
        DrawRectangleLinesEx((Rectangle){270, 10, 110, 80}, 4, GOLD);
    } else {
        DrawRectangleLines(270, 10, 110, 80, BLACK);
    }

    // TARJETA DE LA NUEZ
    DrawTexturePro(
        cardNuez,
        (Rectangle){0, 0, (float)cardNuez.width, (float)cardNuez.height},
        (Rectangle){390, 10, 110, 80},
        (Vector2){0, 0},
        0,
        WHITE
    );
    if (plantaSeleccionada == TIPO_NUEZ) {
        DrawRectangleLinesEx((Rectangle){390, 10, 110, 80}, 4, GOLD);
    } else {
        DrawRectangleLines(390, 10, 110, 80, BLACK);
    }

    // TARJETA DEL CACTUS
    DrawRectangle(510, 10, 110, 80, BEIGE);
    DrawText("Cactus", 520, 20, 15, DARKGREEN);
    DrawText("125", 550, 65, 20, BLACK);

    DrawTexturePro(
        cactus,
        (Rectangle){0, 0, cactus.width, cactus.height},
        (Rectangle){550, 35, 45, 30},
        (Vector2){0, 0},
        0,
        WHITE
    );
    //Se resalta cuando es seleccionada
    if (plantaSeleccionada == TIPO_CACTUS) {
        DrawRectangleLinesEx((Rectangle){510, 10, 110, 80}, 4, GOLD);
    } else {
        DrawRectangleLines(510, 10, 110, 80, BLACK);
    }

    // TARJETA DE LA PALA
    DrawTexturePro(
        pala,
        (Rectangle){0, 0, (float)pala.width, (float)pala.height},
        (Rectangle){630, 10, 110, 80},
        (Vector2){0, 0},
        0,
        WHITE
    );
    if (palaSeleccionada == 1) {
        DrawRectangleLinesEx((Rectangle){630, 10, 110, 80}, 4, GOLD);
    } else {
        DrawRectangleLines(630, 10, 110, 80, BLACK);
    }

}



void dibujarZombie(int x, int y) {

    Rectangle origen = {
        0, 0,
        (float)zombieNormal.width,
        (float)zombieNormal.height
    };

    Rectangle destino = {   //Define la posicion y el tamaño del zombie 
        x, y + 5,
        50, 80
    };

    DrawTexturePro(
        zombieNormal,
        origen,
        destino,
        (Vector2){0, 0},
        0,
        WHITE
    );
}

void dibujarZombieFlag(int x, int y) {

    Rectangle origen = {
        0, 0,
        zombieFlag.width,
        zombieFlag.height
    };

    Rectangle destino = {
        x, y,
        100, 100
    };

    DrawTexturePro(
        zombieFlag,
        origen,
        destino,
        (Vector2){0, 0},
        0,
        WHITE
    );
}


void dibujarZombieSinCabeza(int x, int y) {

    Rectangle origen = {
        0, 0,
        (float)zombieSinCabeza.width,
        (float)zombieSinCabeza.height
    };

    Rectangle destino = {
        x, y + 5,
        50, 80
    };

    DrawTexturePro(
        zombieSinCabeza,
        origen,
        destino,
        (Vector2){0, 0},
        0,
        WHITE
    );
}



void dibujarPlantas(void) {

    // Recorrer todas las plantas activas
    for (int i = 0; i < CANT_PLANTAS; i++) {

        if (plantas[i].activa == 1) {
            //Calcula la posicion de la planta segun su fila y columna
            int x = ANCHO_CASA + plantas[i].columna * ANCHO_CELDA;
            int y = ALTO_MENU + plantas[i].fila * ALTO_CELDA;

            // Posicion y tamaño de las plantas
            Rectangle destino = {
                x + 5,
                y + 5,
                70,
                80
            };

            Rectangle origen;
            //Comprueba el tipo de planta
            if (plantas[i].tipo == TIPO_GUISANTE) {

                origen = (Rectangle){
                    0, 0,
                    lanzaguisantes.width,
                    lanzaguisantes.height
                };

                DrawTexturePro(
                    lanzaguisantes,
                    origen,
                    destino,
                    (Vector2){0, 0},
                    0,
                    WHITE
                );

            }
            else if (plantas[i].tipo == TIPO_GIRASOL) {

                origen = (Rectangle){
                    0, 0,
                    girasol.width,
                    girasol.height
                };

                DrawTexturePro(
                    girasol,
                    origen,
                    destino,
                    (Vector2){0, 0},
                    0,
                    WHITE
                );

            }
            else if (plantas[i].tipo == TIPO_NUEZ) {

                origen = (Rectangle){
                    0, 0,
                    nuez.width,
                    nuez.height
                };

                DrawTexturePro(
                    nuez,
                    origen,
                    destino,
                    (Vector2){0, 0},
                    0,
                    WHITE
                );

            }
            else if (plantas[i].tipo == TIPO_CACTUS) {

                origen = (Rectangle){
                    0, 0,
                    cactus.width,
                    cactus.height
                };

                DrawTexturePro(
                    cactus,
                    origen,
                    destino,
                    (Vector2){0, 0},
                    0,
                    WHITE
                );
            }
        }
    }
}


void dibujarSoles(void) {
    for (int i = 0; i < CANT_SOLES; i++) { // Recorre todos los soles
        if (soles[i].activo == 1) { // Ve si esta activo
            // Se obtiene la posicion del sol
            int x = ANCHO_CASA + (int)soles[i].x;
            int y = (int)soles[i].y;

            DrawTexture(sol, x - sol.width / 2, y - sol.height / 2, WHITE);
        }
    }
}


void dibujarProyectiles(void) {
    //Recorre todos los proyectiles
    for (int i = 0; i < CANT_PROYECTILES; i++) {

        if (proyectiles[i].activo == 1) { //Ve si esta activo

            DrawCircle(
                ANCHO_CASA + (int)proyectiles[i].x,
                (int)proyectiles[i].y,
                7,
                LIME
            );
        }
    }
}


void dibujarOleada(void) {
    //Convierte la oleada a texto
    char textoOleada[30];
    sprintf(textoOleada, "Oleada: %d", oleadaActual);
    DrawText(textoOleada, 760, 35, 25, WHITE);
}
