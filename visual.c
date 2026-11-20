
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
Texture2D zombieCono;
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



void iniciarVentana() { // Inicializa la ventana del juego usando las dimensiones de la configuración
    InitWindow(
        configuracion_juego.ancho_casa + configuracion_juego.ancho_jardin,
        configuracion_juego.alto_menu + filas_jardin * configuracion_juego.alto_celda,
        "Plants vs zombies"
    );

    MaximizeWindow(); // Maximiza la ventana del juego
}


void cerrarVentana() { // Cierra la ventana del juego
    CloseWindow();
}



void cargarJardin(void) { // Carga la imagen del jardín
    jardin = LoadTexture("assets/pastito_solo_pasto.png");
    SetTextureFilter(jardin, TEXTURE_FILTER_BILINEAR); // Mejora la calidad
}


void liberarJardin(void) { // Libera la memoria utilizada por el jardín
    UnloadTexture(jardin);
}



void cargarLapida(void) { // Carga la imagen de la lápida
    lapida = LoadTexture("assets/lapida.png");
    if (lapida.id != 0) SetTextureFilter(lapida, TEXTURE_FILTER_BILINEAR); // Mejora la calidad
}

// Libera la memoria utilizada por la lápida
void liberarLapida(void) {
    if (lapida.id != 0)
        UnloadTexture(lapida);
}


// Carga la imagen de la casa
void cargarCasa(void) {
    casa = LoadTexture("assets/casa.png");
    SetTextureFilter(casa, TEXTURE_FILTER_BILINEAR);
}

// Libera la memoria utilizada por la casa
void liberarCasa(void) {
    UnloadTexture(casa);
}


// Carga la imagen del cortacésped
void cargarCortacesped(void) {
    cortacesped = LoadTexture("assets/cortacesped.png");
    SetTextureFilter(cortacesped, TEXTURE_FILTER_BILINEAR);
}

// Libera la memoria utilizada por el cortacésped
void liberarCortacesped(void) {
    UnloadTexture(cortacesped);
}


// Carga la imagen del zombi normal
void cargarZombieNormal(void) {
    zombieNormal = LoadTexture("assets/zombie_normal.png");
    SetTextureFilter(zombieNormal, TEXTURE_FILTER_POINT);
}

// Libera la memoria utilizada por el zombi normal
void liberarZombieNormal(void) {
    UnloadTexture(zombieNormal);
}


// Carga la imagen del zombi bandera
void cargarZombieFlag(void) {
    zombieFlag = LoadTexture("assets/zombie_flag.png");
    SetTextureFilter(zombieFlag, TEXTURE_FILTER_BILINEAR);
}

// Libera la memoria utilizada por el zombi bandera
void liberarZombieFlag(void) {
    UnloadTexture(zombieFlag);
}


// Carga la imagen del zombi con cono
void cargarZombieCono(void) {
    zombieCono = LoadTexture("assets/zombie_cono.png");
    SetTextureFilter(zombieCono, TEXTURE_FILTER_POINT);
}

// Libera la memoria utilizada por el zombi con cono
void liberarZombieCono(void) {
    UnloadTexture(zombieCono);
}


// Carga la imagen de la pala
void cargarPala(void) {
    pala = LoadTexture("assets/pala.png");
    SetTextureFilter(pala, TEXTURE_FILTER_BILINEAR);
}

// Libera la memoria utilizada por la pala
void liberarPala(void) {
    UnloadTexture(pala);
}


// Carga las imágenes de las notas
void cargarNotasOleadas(void) {

    notasOleadas[0] = LoadTexture("assets/nota_oleada1.png");
    notasOleadas[1] = LoadTexture("assets/nota_oleada2.png");
    notasOleadas[2] = LoadTexture("assets/nota_oleada3.png");
    notasOleadas[3] = LoadTexture("assets/nota_oleada4.png");
    notasOleadas[4] = LoadTexture("assets/nota_oleada5.png");

    // Recorre las cinco imágenes para ver si se cargaron bien
    for (int i = 0; i < 5; i++) {

        if (notasOleadas[i].id != 0) {

            SetTextureFilter(notasOleadas[i], TEXTURE_FILTER_BILINEAR);
            printf("Nota %d: Cargada correctamente\n", i + 1);

        } else {

            printf("Nota %d: ERROR al cargar la imagen\n", i + 1);

        }
    }
}

// Libera la memoria utilizada por las notas 
void liberarNotasOleadas(void) {

    for (int i = 0; i < 5; i++) {
        if (notasOleadas[i].id != 0) {
            UnloadTexture(notasOleadas[i]);
        }
    }
}


// Carga las imágenes de las plantas, los soles y sus tarjetas
void cargarPlantas(void) {
    girasol = LoadTexture("assets/girasol.png");
    nuez = LoadTexture("assets/nuez.png");
    cactus = LoadTexture("assets/cactus.png");
    lanzaguisantes = LoadTexture("assets/lanzaguisantes.png");

    sol = LoadTexture("assets/Sun.png");

    cardGuisante = LoadTexture("assets/peashooterCard.png");
    cardGirasol = LoadTexture("assets/sunflowerCard.png");
    cardNuez = LoadTexture("assets/wallnutCard.png");
    cardCactus = LoadTexture("assets/cactusCard.png");

    // Aplica un filtro para mejorar la apariencia de las imágenes
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

// Libera la memoria de las imágenes cargadas para las plantas y sus tarjetas
void liberarPlantas(void) {
    UnloadTexture(girasol);
    UnloadTexture(nuez);
    UnloadTexture(cactus);
    UnloadTexture(lanzaguisantes);
    UnloadTexture(sol);
    UnloadTexture(cardGuisante);
    UnloadTexture(cardGirasol);
    UnloadTexture(cardNuez);
    UnloadTexture(cardCactus);
}


void dibujarJardin() {

    // Dibuja el fondo del menú superior
    DrawRectangle(0, 0, configuracion_juego.ancho_casa + configuracion_juego.ancho_jardin, configuracion_juego.alto_menu, BROWN);

    
    DrawTexturePro( // Dibuja la textura del jardín ajustándola a sus dimensiones 
        jardin,
        (Rectangle){0, 0, jardin.width, jardin.height},
        (Rectangle){configuracion_juego.ancho_casa, configuracion_juego.alto_menu, configuracion_juego.ancho_jardin, configuracion_juego.alto_jardin},
        (Vector2){0, 0},
        0,
        WHITE
    );

   
    for (int i = 0; i < filas_jardin; i++) { // Recorre todas las filas y columnas del jardín para buscar obstáculos
        for (int j = 0; j < columnas_jardin; j++) {

            if (!esObstaculo(i, j)) continue;

            // Calcula la posición y el tamaño de la celda que contiene el obstáculo
            Rectangle celda = {
                (float)(configuracion_juego.ancho_casa + j * configuracion_juego.ancho_celda),
                (float)(configuracion_juego.alto_menu + i * configuracion_juego.alto_celda),
                (float)configuracion_juego.ancho_celda,
                (float)configuracion_juego.alto_celda
            };

            // Comprueba si la imagen de la lápida se cargó correctamente
            if (lapida.id != 0) {

                // Calcula la escala para que la lápida entre en la celda sin deformarse
                float escala = fminf(celda.width / lapida.width, celda.height / lapida.height);

                // Calcula la posición para centrar la lápida dentro de la celda
                Rectangle destino = {
                    celda.x + (celda.width - lapida.width * escala) / 2,
                    celda.y + (celda.height - lapida.height * escala) / 2,
                    lapida.width * escala,
                    lapida.height * escala
                };

                // Dibuja la lápida en el jardín
                DrawTexturePro(lapida, (Rectangle){0, 0, lapida.width, lapida.height},
                               destino, (Vector2){0, 0}, 0, WHITE);

            } else {

                // Si no se cargó la imagen, dibuja un rectángulo como reemplazo
                DrawRectangle(celda.x + 8, celda.y + 8, celda.width - 16, celda.height - 16, DARKGRAY);
            }
        }
    }
}


void dibujarCasa(void) {

    // Define la posición y el tamaño que tendrá la imagen de la casa
    Rectangle destino = {
        0,
        configuracion_juego.alto_menu,
        configuracion_juego.ancho_img_casa,
        configuracion_juego.alto_jardin
    };

    // Define qué parte de la imagen original se utilizará
    Rectangle origen = {
        0, 0,
        casa.width,
        casa.height
    };

    // Dibuja la casa utilizando las dimensiones definidas
    DrawTexturePro(
        casa,
        origen,
        destino,
        (Vector2){0, 0},
        0,
        WHITE
    );
}


void dibujarCortacespedes(void) {

    // Calcula el tamaño que tendrá la imagen del cortacésped
    float alto = configuracion_juego.alto_cortacesped;
    float ancho = alto * cortacesped.width / cortacesped.height;

   
    for (int i = 0; i < filas_jardin; i++) { // Recorre las filas del jardín

        // Si el cortacésped no está disponible ni activo, no se dibuja
        if (!cortacespedes[i].disponible && !cortacespedes[i].activo) {
            continue;
        }

        // Calcula la posición del cortacésped según la fila en la que se encuentra
        Rectangle destino = {
            configuracion_juego.ancho_casa + cortacespedes[i].x,
            configuracion_juego.alto_menu + i * configuracion_juego.alto_celda + (configuracion_juego.alto_celda - alto) / 2,
            ancho,
            alto
        };

        // Define la parte de la imagen original que se utilizará, recortando 4 píxeles del borde derecho
        Rectangle origen = {
            0, 0,
            cortacesped.width - 4,
            cortacesped.height
        };

       
        DrawTexturePro(  // Dibuja el cortacésped
            cortacesped,
            origen,
            destino,
            (Vector2){0, 0},
            0,
            WHITE
        );
    }
}


void dibujarCarril(void) {

    // Dibuja el fondo del carril
    DrawRectangle(
        configuracion_juego.ancho_img_casa,
        configuracion_juego.alto_menu,
        configuracion_juego.ancho_carril,
        configuracion_juego.alto_jardin,
        BEIGE
    );

    // Dibuja las líneas que separan las filas del jardín
    for (int i = 0; i <= filas_jardin; i++) {

        DrawLine(
            configuracion_juego.ancho_img_casa,
            configuracion_juego.alto_menu + i * configuracion_juego.alto_celda,
            configuracion_juego.ancho_casa,
            configuracion_juego.alto_menu + i * configuracion_juego.alto_celda,
            (Color){70, 70, 75, 255}
        );
    }

    // Dibuja el borde que separa el carril del jardín
    DrawLine(
        configuracion_juego.ancho_casa,
        configuracion_juego.alto_menu,
        configuracion_juego.ancho_casa,
        configuracion_juego.alto_menu + configuracion_juego.alto_jardin,
        BLACK
    );
}


void dibujarMenu(void) {

    DrawRectangle(0, 0, 1000, configuracion_juego.alto_menu, BROWN);    // Dibuja el fondo del menú
    DrawRectangleRounded((Rectangle){15, 15, 120, 70}, 0.2f, 8, (Color){55, 45, 35, 230});     // Dibuja el recuadro del contador de soles

    // Define la parte de la imagen del sol que se utilizará y su posición en el menú
    Rectangle origenSol = {0, 0, (float)sol.width, (float)sol.height};
    Rectangle destinoSol = {25, 30, 40, 40};

    
    DrawTexturePro(sol, origenSol, destinoSol, (Vector2){0, 0}, 0, WHITE); // Dibuja el sol en el contador

    // Convierte la cantidad de soles disponibles en texto para mostrarla en pantalla
    char textoSoles[20];
    sprintf(textoSoles, "%d", solesJugador);
    DrawText(textoSoles, 75, 32, 30, GOLD);

    // Dibuja la tarjeta del lanzaguisantes
    DrawTexturePro(
        cardGuisante,
        (Rectangle){0, 0, (float)cardGuisante.width, (float)cardGuisante.height},
        (Rectangle){150, 10, 110, 80},
        (Vector2){0, 0},
        0,
        WHITE
    );

    // Resalta la tarjeta si el lanzaguisantes está seleccionado
    if (plantaSeleccionada == TIPO_GUISANTE) {
        DrawRectangleLinesEx((Rectangle){150, 10, 110, 80}, 4, GOLD);
    } else {
        DrawRectangleLines(150, 10, 110, 80, BLACK);
    }

    // Dibuja la tarjeta del girasol
    DrawTexturePro(
        cardGirasol,
        (Rectangle){0, 0, (float)cardGirasol.width, (float)cardGirasol.height},
        (Rectangle){270, 10, 110, 80},
        (Vector2){0, 0},
        0,
        WHITE
    );

    // Resalta la tarjeta si el girasol está seleccionado
    if (plantaSeleccionada == TIPO_GIRASOL) {
        DrawRectangleLinesEx((Rectangle){270, 10, 110, 80}, 4, GOLD);
    } else {
        DrawRectangleLines(270, 10, 110, 80, BLACK);
    }

    // Dibuja la tarjeta de la nuez
    DrawTexturePro(
        cardNuez,
        (Rectangle){0, 0, (float)cardNuez.width, (float)cardNuez.height},
        (Rectangle){390, 10, 110, 80},
        (Vector2){0, 0},
        0,
        WHITE
    );

    // Resalta la tarjeta si la nuez está seleccionada
    if (plantaSeleccionada == TIPO_NUEZ) {
        DrawRectangleLinesEx((Rectangle){390, 10, 110, 80}, 4, GOLD);
    } else {
        DrawRectangleLines(390, 10, 110, 80, BLACK);
    }

    // Dibuja la tarjeta del cactus
    DrawTexturePro(
        cardCactus,
        (Rectangle){0, 0, (float)cardCactus.width, (float)cardCactus.height},
        (Rectangle){510, 10, 110, 80},
        (Vector2){0, 0},
        0,
        WHITE
    );

    // Resalta la tarjeta si el cactus está seleccionado
    if (plantaSeleccionada == TIPO_CACTUS) {
        DrawRectangleLinesEx((Rectangle){510, 10, 110, 80}, 4, GOLD);
    } else {
        DrawRectangleLines(510, 10, 110, 80, BLACK);
    }

    // Dibuja la tarjeta de la pala
    DrawTexturePro(
        pala,
        (Rectangle){0, 0, (float)pala.width, (float)pala.height},
        (Rectangle){630, 10, 110, 80},
        (Vector2){0, 0},
        0,
        WHITE
    );

    // Resalta la tarjeta si la pala está seleccionada
    if (palaSeleccionada == 1) {
        DrawRectangleLinesEx((Rectangle){630, 10, 110, 80}, 4, GOLD);
    } else {
        DrawRectangleLines(630, 10, 110, 80, BLACK);
    }
}

void dibujarZombie(int x, int y) {

    // Define la parte de la imagen original que se utilizará
    Rectangle origen = {
        0, 0,
        (float)zombieNormal.width,
        (float)zombieNormal.height
    };

    // Define la posición y el tamaño del zombi en pantalla
    Rectangle destino = {
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

    // Define la parte de la imagen que se utilizará
    Rectangle origen = {
        0, 0,
        zombieFlag.width,
        zombieFlag.height
    };

    // Define la posición y el tamaño del zombi con bandera
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

void dibujarZombieCono(int x, int y) {

    // Define la parte de la imagen que se utilizará
    Rectangle origen = {
        0, 0,
        (float)zombieCono.width,
        (float)zombieCono.height
    };

    // Define la posición y el tamaño del zombi con cono
    Rectangle destino = {
        x, y + 5,
        50, 80
    };

    DrawTexturePro(
        zombieCono,
        origen,
        destino,
        (Vector2){0, 0},
        0,
        WHITE
    );
}


void dibujarPlantas(void) {
    for (int i = 0; i < CANT_PLANTAS; i++) {  // Recorre el arreglo que contiene las plantas

        if (plantas[i].activa == 1) { // Comprueba si la planta está activa

            // Calcula la posición de la planta según su fila y columna
            int x = configuracion_juego.ancho_casa + plantas[i].columna * configuracion_juego.ancho_celda;
            int y = configuracion_juego.alto_menu + plantas[i].fila * configuracion_juego.alto_celda;

            // Define la posición y el tamaño que tendrá la planta
            Rectangle destino = {
                x + 5,
                y + 5,
                70,
                80
            };

            Rectangle origen;

            // Ve el tipo de planta para seleccionar la imagen correspondiente
            if (plantas[i].tipo == TIPO_GUISANTE) {

                origen = (Rectangle){0, 0, lanzaguisantes.width, lanzaguisantes.height};

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

                origen = (Rectangle){0, 0, girasol.width, girasol.height};

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

                origen = (Rectangle){0, 0, nuez.width, nuez.height};

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

                origen = (Rectangle){0, 0, cactus.width, cactus.height};

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

    for (int i = 0; i < CANT_SOLES; i++) {  // Recorre todos los soles disponibles

        if (soles[i].activo == 1) { // Comprueba si el sol está activo

            // Calcula la posición del sol dentro de la ventana
            int x = configuracion_juego.ancho_casa + (int)soles[i].x;
            int y = (int)soles[i].y;

            // Dibuja el sol centrado en las coordenadas indicadas
            DrawTexture(sol, x - sol.width / 2, y - sol.height / 2, WHITE);
        }
    }
}


void dibujarProyectiles(void) {
    for (int i = 0; i < CANT_PROYECTILES; i++) {    // Recorre todos los proyectiles disponibles

        if (proyectiles[i].activo == 1) {  // Comprueba si el proyectil está activo

            // Dibuja el proyectil como un círculo verde
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
    if (mostrarNotaOleada == 1) { // Ve si se debe mostrar la nota de inicio de la oleada

        int indiceNota = oleadaActual - 1; // Obtiene la posición de la imagen correspondiente a la oleada actual

        // Comprueba que el índice esté dentro del arreglo y que la imagen exista
        if (indiceNota >= 0 && indiceNota < 5 && notasOleadas[indiceNota].id != 0) {

            Texture2D nota = notasOleadas[indiceNota];

            float escala = 0.5f; // Reduce el tamaño original de la imagen al 50 %

            float ancho = nota.width * escala;
            float alto = nota.height * escala;

            // Calcula las coordenadas para centrar la nota en el jardín
            float x = (configuracion_juego.ancho_casa +
                       configuracion_juego.ancho_jardin -
                       ancho) / 2.0f;

            float y = (configuracion_juego.alto_menu +
                       configuracion_juego.alto_jardin -
                       alto) / 2.0f;

            // Define la parte de la imagen original que se utilizará
            Rectangle origen = {
                0, 0,
                (float)nota.width,
                (float)nota.height
            };

            // Define la posición y el tamaño final de la imagen
            Rectangle destino = {
                x, y,
                ancho, alto
            };

            // Dibuja la nota de la oleada en el centro del jardín
            DrawTexturePro(
                nota,
                origen,
                destino,
                (Vector2){0, 0},
                0,
                WHITE
            );
        }
    }

    int zombisPorDerrotar = zombisRestantes;    // Obtiene la cantidad de zombis que quedan por derrotar
    for (int i = 0; i < CANT_ZOMBIS; i++) {  // Recorre los zombis para contar los que siguen vivos
        if (zombieVivo(i)) {
            zombisPorDerrotar++;
        }
    }
        // Convierte la cantidad de zombis restantes en texto
        char textoZombis[30];
        sprintf(textoZombis, "Zombis: %d", zombisPorDerrotar);
        DrawText(textoZombis, 760, 15, 20, WHITE);

        // Convierte el número de la oleada actual en texto
        char textoOleada[30];
        sprintf(textoOleada, "Oleada: %d", oleadaActual);
        DrawText(textoOleada, 760, 55, 20, WHITE);
}
