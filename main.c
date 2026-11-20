
#include <stdio.h>
#include "visual.h"
#include "logica.h"
#include "raylib.h"
#include "config.h"
#include "lectura.h"

Music musicaFondo;

// Reinicia todos los elementos de la partida
void reiniciarPartida(void) {

    inicializarZombis();
    inicializarPlantas();
    inicializarSoles();
    inicializarProyectiles();
    inicializarCortacespedes();

    // Reiniciar valores de las oleadas
    oleadaActual = 1;
    zombisRestantes = 0;
    temporizadorOleada = 0;
    temporizadorEntreOleadas = 0;

    // Reiniciar selección
    palaSeleccionada = 0;
    plantaSeleccionada = -1;

    iniciarOleada();

    // Reiniciar soles
    solesJugador = 200;

    // Reiniciar estados de la partida
    juegoPerdido = 0;
    juegoGanado = 0;
}

int main(void) {

    int filas;
    int columnas;

    int **mapa = leerMapa("mapa.txt", &filas, &columnas);

    if (mapa == NULL) {
        printf("No se pudo cargar el mapa\n");
        return 1;
    }

    printf("Mapa cargado correctamente:\n");
    printf("Filas: %d\n", filas);
    printf("Columnas: %d\n", columnas);

    mostrarMapa(mapa, filas, columnas);

    // Inicializar configuración del juego
    inicializarConfiguracion();

    iniciarVentana();
    InitAudioDevice();

    // Cargar y reproducir música
    musicaFondo = LoadMusicStream("assets/musica.mp3");
    SetMusicVolume(musicaFondo, 0.4f);
    PlayMusicStream(musicaFondo);

    cargarJardin();
    cargarCasa();
    cargarPlantas();
    cargarCortacesped();
    cargarZombieFlag();
    cargarZombieNormal();
    cargarZombieSinCabeza();
    cargarPala();

    // Inicializar elementos del juego
    inicializarZombis();
    inicializarPlantas();
    inicializarSoles();
    inicializarProyectiles();
    inicializarCortacespedes();

    iniciarOleada();

    while (!WindowShouldClose()) {

        UpdateMusicStream(musicaFondo);

        float tiempo = GetFrameTime();

        // ACTUALIZAR EL JUEGO
        if (juegoPerdido == 0 && juegoGanado == 0) {

            moverZombis(tiempo);

            activarCortacespedes();
            actualizarCortacespedes(tiempo);

            comprobarDerrota();

            atacarPlanta(tiempo);
            actualizarOleadas(tiempo);

            actualizarSoles(tiempo);
            producirSoles(tiempo);

            actualizarDisparos(tiempo);
            moverProyectiles(tiempo);
            detectarImpactos();

            // DETECTAR CLIC DEL MOUSE
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

                Vector2 mousePos = GetMousePosition();

                recogerSol(mousePos);

                seleccionarPlanta(GetMouseX(), GetMouseY());

                // Comprobar si el clic fue dentro del jardín
                if (mousePos.x >= ANCHO_CASA &&
                    mousePos.x < ANCHO_CASA + ANCHO_JARDIN &&
                    mousePos.y >= ALTO_MENU &&
                    mousePos.y < ALTO_MENU + ALTO_JARDIN) {

                    int fila = (mousePos.y - ALTO_MENU) / ALTO_CELDA;
                    int columna = (mousePos.x - ANCHO_CASA) / ANCHO_CELDA;

                    // Si está seleccionada la pala
                    if (palaSeleccionada == 1) {

                        quitarPlanta(fila, columna);

                    }

                    // Si está seleccionada una planta
                    else if (plantaSeleccionada != -1) {

                        if (colocarPlanta(fila, columna, plantaSeleccionada)) {
                            plantaSeleccionada = -1;
                        }
                    }
                }
            }
        }

        // DETECTAR CLIC EN VOLVER A JUGAR
        else {

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

                Vector2 mousePos = GetMousePosition();

                Rectangle boton = {
                    (ANCHO_CASA + ANCHO_JARDIN - 240) / 2,
                    380,
                    240,
                    60
                };

                if (CheckCollisionPointRec(mousePos, boton)) {
                    reiniciarPartida();
                }
            }
        }

        // DIBUJAR EL JUEGO
        BeginDrawing();

        ClearBackground(RAYWHITE);

        dibujarJardin();
        dibujarCasa();
        dibujarCarril();
        dibujarCortacespedes();

        dibujarMenu();
        dibujarOleada();
        dibujarPlantas();
        dibujarSoles();
        dibujarProyectiles();

        // DIBUJAR ZOMBIS
        for (int i = 0; i < CANT_ZOMBIS; i++) {

            if (zombieVivo(i)) {

                int xZombie = (int)(ANCHO_CASA + zombis[i].x);
                int yZombie = ALTO_MENU + zombis[i].fila * ALTO_CELDA;

                if (zombis[i].tipo == ZOMBI_NORMAL) {
                    dibujarZombie(xZombie, yZombie);
                }
                else if (zombis[i].tipo == ZOMBI_FLAG) {
                    dibujarZombieFlag(xZombie, yZombie);
                }
                else if (zombis[i].tipo == ZOMBI_SIN_CABEZA) {
                    dibujarZombieSinCabeza(xZombie, yZombie);
                }
            }
        }

        // PANTALLA DE DERROTA
        if (juegoPerdido == 1) {

            DrawRectangle(
                0, 0,
                ANCHO_CASA + ANCHO_JARDIN,
                ALTO_MENU + ALTO_JARDIN,
                Fade(BLACK, 0.65f)
            );

            DrawText(
                "GAME OVER",
                (ANCHO_CASA + ANCHO_JARDIN - MeasureText("GAME OVER", 60)) / 2,
                300,
                60,
                RED
            );
        }

        // PANTALLA DE VICTORIA
        else if (juegoGanado == 1) {

            DrawRectangle(
                0, 0,
                ANCHO_CASA + ANCHO_JARDIN,
                ALTO_MENU + ALTO_JARDIN,
                Fade(BLACK, 0.65f)
            );

            DrawText(
                "¡VICTORIA!",
                (ANCHO_CASA + ANCHO_JARDIN - MeasureText("¡VICTORIA!", 60)) / 2,
                270,
                60,
                GOLD
            );

            DrawText(
                "Has protegido tu casa",
                (ANCHO_CASA + ANCHO_JARDIN - MeasureText("Has protegido tu casa", 25)) / 2,
                340,
                25,
                WHITE
            );
        }

        // BOTON VOLVER A JUGAR
        if (juegoPerdido == 1 || juegoGanado == 1) {

            Rectangle boton = {
                (ANCHO_CASA + ANCHO_JARDIN - 240) / 2,
                380,
                240,
                60
            };

            DrawRectangleRec(boton, DARKGREEN);
            DrawRectangleLinesEx(boton, 3, WHITE);

            DrawText(
                "VOLVER A JUGAR",
                boton.x + (boton.width - MeasureText("VOLVER A JUGAR", 20)) / 2,
                boton.y + 20,
                20,
                WHITE
            );
        }

        EndDrawing();
    }

    // LIBERAR RECURSOS
    liberarJardin();
    liberarCasa();
    liberarPlantas();
    liberarCortacesped();
    liberarZombieFlag();
    liberarZombieNormal();
    liberarZombieSinCabeza();
    liberarPala();

    // Liberar música y audio
    StopMusicStream(musicaFondo);
    UnloadMusicStream(musicaFondo);
    CloseAudioDevice();

    cerrarVentana();

    liberarMapa(mapa, filas);

    return 0;
}
