#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "visual.h"
#include "logica.h"
#include "raylib.h"
#include "config.h"
#include "lectura.h"
#include <string.h>

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

int main(int argc, char *argv[]) {

    const char *archivoMapa = "mapa.txt";
    const char *archivoConfig = "config.txt";

    // modo generador: ./pvz --generar <filas> <columnas> <archivo> [%obstaculos]
    if (argc >= 5 && strcmp(argv[1], "--generar") == 0) {
        int porcentaje = (argc >= 6) ? atoi(argv[5]) : 15;
        if (generarMapa(argv[4], atoi(argv[2]), atoi(argv[3]), porcentaje)) {
            printf("Mapa generado en '%s'\n", argv[4]);
            return 0;
        }
        return 1;
    }

    // uso normal: ./pvz [mapa.txt] [config.txt]
    if (argc >= 2) archivoMapa = argv[1];
    if (argc >= 3) archivoConfig = argv[2];

    int filas;
    int columnas;

    mapa_jardin = leerMapa(archivoMapa, &filas, &columnas);

    if (mapa_jardin == NULL) {
        printf("No se pudo cargar el mapa\n");
        return 1;
    }
    filas_jardin = filas;
    columnas_jardin = columnas;
    printf("mapa cargado correctamente: %d filas x %d columnas\n", filas, columnas);
    inicializarConfiguracion();

    if (!leerConfiguracion(archivoConfig)) {
        printf("Configuracion no valida o ausente: se usan los valores por defecto\n");
        inicializarConfiguracion();
    }

    calcularDerivadas();

    iniciarVentana();
    InitAudioDevice();

    // Cargar y reproducir música
    musicaFondo = LoadMusicStream("assets/musica.wav");
    SetMusicVolume(musicaFondo, 0.4f);
    PlayMusicStream(musicaFondo);

    cargarJardin();
    cargarLapida();
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
                if (mousePos.x >= configuracion_juego.ancho_casa &&
                    mousePos.x < configuracion_juego.ancho_casa + configuracion_juego.ancho_jardin &&
                    mousePos.y >= configuracion_juego.alto_menu &&
                    mousePos.y < configuracion_juego.alto_menu + configuracion_juego.alto_jardin) {

                    // Calcular la fila y columna donde se hizo clic
                    int fila = (mousePos.y - configuracion_juego.alto_menu) / configuracion_juego.alto_celda;
                int columna = (mousePos.x - configuracion_juego.ancho_casa) / configuracion_juego.ancho_celda;

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
                    (configuracion_juego.ancho_casa + configuracion_juego.ancho_jardin - 240) / 2,
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

                int xZombie = (int)(configuracion_juego.ancho_casa + zombis[i].x);
                int yZombie = configuracion_juego.alto_menu + zombis[i].fila * configuracion_juego.alto_celda;

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
                configuracion_juego.ancho_casa + configuracion_juego.ancho_jardin,
                configuracion_juego.alto_menu + configuracion_juego.alto_jardin,
                Fade(BLACK, 0.65f)
            );

            DrawText(
                "GAME OVER",
                (configuracion_juego.ancho_casa + configuracion_juego.ancho_jardin - MeasureText("GAME OVER", 60)) / 2,
                     300,
                     60,
                     RED
            );
        }

        // PANTALLA DE VICTORIA
        else if (juegoGanado == 1) {

            DrawRectangle(
                0, 0,
                configuracion_juego.ancho_casa + configuracion_juego.ancho_jardin,
                configuracion_juego.alto_menu + configuracion_juego.alto_jardin,
                Fade(BLACK, 0.65f)
            );

            DrawText(
                "¡VICTORIA!",
                (configuracion_juego.ancho_casa + configuracion_juego.ancho_jardin - MeasureText("¡VICTORIA!", 60)) / 2,
                     270,
                     60,
                     GOLD
            );

            DrawText(
                "Has protegido tu casa",
                (configuracion_juego.ancho_casa + configuracion_juego.ancho_jardin - MeasureText("Has protegido tu casa", 25)) / 2,
                     340,
                     25,
                     WHITE
            );
        }

        // BOTON VOLVER A JUGAR
        if (juegoPerdido == 1 || juegoGanado == 1) {

            Rectangle boton = {
                (configuracion_juego.ancho_casa + configuracion_juego.ancho_jardin - 240) / 2,
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
    liberarLapida();
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

    liberarMapa(mapa_jardin, filas);
    mapa_jardin = NULL;

    return 0;
}
