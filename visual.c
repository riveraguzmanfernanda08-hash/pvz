#include "visual.h"
#include "raylib.h"
#include "config.h"
#include "logica.h"
#include "constantes.h"
#include <stdio.h>
#include <math.h>
#include "lectura.h"


Texture2D jardin;
Texture2D casa;
Texture2D notasOleadas[5];
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
Texture2D lapida;

Texture2D cardGuisante;
Texture2D cardGirasol;
Texture2D cardNuez;
Texture2D cardCactus;

void iniciarVentana() { //inicializar ventana del juego con sus dimensiones y titulo
    InitWindow(
        configuracion_juego.ancho_casa + configuracion_juego.ancho_jardin,
        configuracion_juego.alto_menu + filas_jardin * configuracion_juego.alto_celda,

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

void cargarLapida(void) {
    lapida = LoadTexture("assets/lapida.png");
    if (lapida.id != 0) SetTextureFilter(lapida, TEXTURE_FILTER_BILINEAR);
}

void liberarLapida(void) {
    if (lapida.id != 0)
        UnloadTexture(lapida);
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

void cargarNotasOleadas(void) {

    notasOleadas[0] = LoadTexture("assets/nota_oleada1.png");
    notasOleadas[1] = LoadTexture("assets/nota_oleada2.png");
    notasOleadas[2] = LoadTexture("assets/nota_oleada3.png");
    notasOleadas[3] = LoadTexture("assets/nota_oleada4.png");
    notasOleadas[4] = LoadTexture("assets/nota_oleada5.png");

    for (int i = 0; i < 5; i++) {
        if (notasOleadas[i].id != 0) {
            SetTextureFilter(notasOleadas[i], TEXTURE_FILTER_BILINEAR);
        }
    }
}

void liberarNotasOleadas(void) {

    for (int i = 0; i < 5; i++) {
        if (notasOleadas[i].id != 0) {
            UnloadTexture(notasOleadas[i]);
        }
    }
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
    cardCactus   = LoadTexture("assets/cactusCard.png");

    SetTextureFilter(girasol, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(nuez, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(cactus, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(lanzaguisantes, TEXTURE_FILTER_BILINEAR);

    SetTextureFilter(sol, TEXTURE_FILTER_BILINEAR);

    SetTextureFilter(cardGuisante, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(cardGirasol, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(cardNuez, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(cardCactus, TEXTURE_FILTER_BILINEAR);
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

void dibujarJardin() {
    DrawRectangle(0, 0, configuracion_juego.ancho_casa + configuracion_juego.ancho_jardin, configuracion_juego.alto_menu, BROWN);

    DrawTexturePro(
        jardin,
        (Rectangle){0, 0, jardin.width, jardin.height},
        (Rectangle){configuracion_juego.ancho_casa, configuracion_juego.alto_menu, configuracion_juego.ancho_jardin, configuracion_juego.alto_jardin},
        (Vector2){0, 0},
        0,
        WHITE
    );

        for (int i = 0; i < filas_jardin; i++) {
        for (int j = 0; j < columnas_jardin; j++) {

            if (!esObstaculo(i, j)) continue;

            Rectangle celda = {
                (float)(configuracion_juego.ancho_casa + j * configuracion_juego.ancho_celda),
                (float)(configuracion_juego.alto_menu + i * configuracion_juego.alto_celda),
                (float)configuracion_juego.ancho_celda,
                (float)configuracion_juego.alto_celda
            };

            if (lapida.id != 0) {
                float escala = fminf(celda.width / lapida.width, celda.height / lapida.height);
                Rectangle destino = {
                    celda.x + (celda.width - lapida.width * escala) / 2,
                    celda.y + (celda.height - lapida.height * escala) / 2,
                    lapida.width * escala, lapida.height * escala
                };
                DrawTexturePro(lapida, (Rectangle){0, 0, lapida.width, lapida.height},
                               destino, (Vector2){0, 0}, 0, WHITE);
            } else {
                DrawRectangle(celda.x + 8, celda.y + 8, celda.width - 16, celda.height - 16, DARKGRAY);
            }
        }
    }
}

void dibujarCasa(void) {

    Rectangle destino = {  // Define la posicion y el tamaño de destino de la imagen
        0,
        configuracion_juego.alto_menu,
        configuracion_juego.ancho_img_casa,
        configuracion_juego.alto_jardin
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

    float alto = configuracion_juego.alto_cortacesped;
    float ancho = alto * cortacesped.width / cortacesped.height;

    for (int i = 0; i < filas_jardin; i++) {

        if (!cortacespedes[i].disponible && !cortacespedes[i].activo) { // Si el cortacesped no esta disponible ni activo, no se dibuja
            continue;
        }

        Rectangle destino = { //Define posicion y tamaño del cortacesped
            configuracion_juego.ancho_casa + cortacespedes[i].x,
            configuracion_juego.alto_menu + i * configuracion_juego.alto_celda + (configuracion_juego.alto_celda - alto) / 2,
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
        configuracion_juego.ancho_img_casa,
        configuracion_juego.alto_menu,
        configuracion_juego.ancho_carril,
        configuracion_juego.alto_jardin,
        BEIGE
    );

    // Lineas divisorias entre filas
    for (int i = 0; i <= filas_jardin; i++) {

        DrawLine(
            configuracion_juego.ancho_img_casa,
            configuracion_juego.alto_menu + i * configuracion_juego.alto_celda,
            configuracion_juego.ancho_casa,
            configuracion_juego.alto_menu + i * configuracion_juego.alto_celda,
            (Color){70, 70, 75, 255}
        );
    }

    // Borde del lado del jardin
    DrawLine(
        configuracion_juego.ancho_casa,
        configuracion_juego.alto_menu,
        configuracion_juego.ancho_casa,
        configuracion_juego.alto_menu + configuracion_juego.alto_jardin,
        BLACK
    );
}



// carga el menu de arriba (para seleccionar plantas, pala)
void dibujarMenu(void) {
DrawRectangle(0, 0, 1000, configuracion_juego.alto_menu, BROWN); //fondo del menu
// coloca la imagen de sol
// Contador de soles
DrawRectangleRounded((Rectangle){15, 15, 120, 70}, 0.2f, 8, (Color){55, 45, 35, 230});

Rectangle origenSol = { 0, 0, (float)sol.width, (float)sol.height };
Rectangle destinoSol = { 25, 30, 40, 40};
DrawTexturePro(sol, origenSol, destinoSol, (Vector2){0, 0}, 0, WHITE);
//convierte los soles en texto
char textoSoles[20];
sprintf(textoSoles, "%d", solesJugador);
DrawText(textoSoles, 75, 32, 30, GOLD);

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
    DrawTexturePro(
        cardCactus,
        (Rectangle){0, 0, (float)cardCactus.width, (float)cardCactus.height},
        (Rectangle){510, 10, 110, 80},
        (Vector2){0, 0},
        0,
        WHITE
    );

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

			int x = configuracion_juego.ancho_casa + plantas[i].columna * configuracion_juego.ancho_celda;
            int y = configuracion_juego.alto_menu + plantas[i].fila * configuracion_juego.alto_celda;

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
            int x = configuracion_juego.ancho_casa + (int)soles[i].x;
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
                configuracion_juego.ancho_casa + (int)proyectiles[i].x,
                (int)proyectiles[i].y,
                7,
                LIME
            );
        }
    }
}


void dibujarOleada(void) {

    // Mostrar la nota de la oleada
    if (mostrarNotaOleada == 1) {

        int indiceNota = oleadaActual - 1;

        // Comprobar que exista una imagen para esta oleada
        if (indiceNota >= 0 && indiceNota < 5 &&
            notasOleadas[indiceNota].id != 0) {

            Texture2D nota = notasOleadas[indiceNota];

            // Centrar la imagen en la pantalla
            float x = (configuracion_juego.ancho_casa +
                       configuracion_juego.ancho_jardin -
                       nota.width) / 2.0f;

            float y = (configuracion_juego.alto_menu +
                       configuracion_juego.alto_jardin -
                       nota.height) / 2.0f;

            DrawTexture(nota, (int)x, (int)y, WHITE);
        }
    }

    int zombisPorDerrotar = zombisRestantes;

    // Contar los zombis que ya aparecieron y siguen vivos
    for (int i = 0; i < CANT_ZOMBIS; i++) {
        if (zombieVivo(i)) {
            zombisPorDerrotar++;
        }
    }

    // Mostrar zombis restantes
    char textoZombis[30];
    sprintf(textoZombis, "Zombis: %d", zombisPorDerrotar);
    DrawText(textoZombis, 760, 15, 20, WHITE);

    // Mostrar oleada actual
    char textoOleada[30];
    sprintf(textoOleada, "Oleada: %d", oleadaActual);
    DrawText(textoOleada, 760, 55, 20, WHITE);
}
