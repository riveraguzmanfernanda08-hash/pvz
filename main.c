
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

    mapa_jardin = leerMapa(archivoMapa, &filas, &columnas); //carga el mapa desde el archivo indicado

    if (mapa_jardin == NULL) { 
        printf("No se pudo cargar el mapa\n");
        return 1;
    }

    filas_jardin = filas;
    columnas_jardin = columnas;

    printf("mapa cargado correctamente: %d filas x %d columnas\n", filas, columnas);

    inicializarConfiguracion(); // inicializa y carga la configuración del juego

    if (!leerConfiguracion(archivoConfig)) {
        printf("Configuracion no valida o ausente: se usan los valores por defecto\n");
        inicializarConfiguracion();
    }

    calcularDerivadas();

    iniciarVentana();
    InitAudioDevice();

    //cargar y reproducir música
    musicaFondo = LoadMusicStream("assets/musica.wav");
    SetMusicVolume(musicaFondo, 0.4f);
    PlayMusicStream(musicaFondo);

    //carga las imagenes
    cargarJardin();
    cargarLapida();
    cargarCasa();
    cargarPlantas();
    cargarCortacesped();
    cargarZombieFlag();
    cargarZombieNormal();
    cargarZombieCono();
    cargarPala();
    cargarNotasOleadas();

    // Inicializar elementos del juego
    inicializarZombis();
    inicializarPlantas();
    inicializarSoles();
    inicializarProyectiles();
    inicializarCortacespedes();

    iniciarOleada();

    while (!WindowShouldClose()) { //mantiene el juego funcionando hasta que se cierre la ventana

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

            // detectar clic del mouse
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

                Vector2 mousePos = GetMousePosition();

                recogerSol(mousePos);

                seleccionarPlanta(GetMouseX(), GetMouseY());

                // Comprobar si el clic fue dentro del jardín
                if (mousePos.x >= configuracion_juego.ancho_casa && mousePos.x < configuracion_juego.ancho_casa + configuracion_juego.ancho_jardin &&
                    mousePos.y >= configuracion_juego.alto_menu && mousePos.y < configuracion_juego.alto_menu + configuracion_juego.alto_jardin) {

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

        // detectar clic en volver a jugar
        else {

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { //ve si se hizo clic dentro del botón de reinicio

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

        BeginDrawing(); //dibuja el juego

        ClearBackground(RAYWHITE);

        //dibuja los elementos del juego
        dibujarJardin();
        dibujarCasa();
        dibujarCarril();
        dibujarCortacespedes();

        dibujarMenu();
        dibujarOleada();
        dibujarPlantas();
        dibujarSoles();
        dibujarProyectiles();

        //dibujar zombis
        for (int i = 0; i < CANT_ZOMBIS; i++) {

            if (zombieVivo(i)) {
                //calcula la posición del zombi según su fila y coordenada horizontal
                int xZombie = (int)(configuracion_juego.ancho_casa + zombis[i].x);
                int yZombie = configuracion_juego.alto_menu + zombis[i].fila * configuracion_juego.alto_celda;

                //dibuja la imagen correspondiente al tipo de zombi
                if (zombis[i].tipo == ZOMBI_NORMAL) {
                    dibujarZombie(xZombie, yZombie);
                }
                else if (zombis[i].tipo == ZOMBI_FLAG) {
                    dibujarZombieFlag(xZombie, yZombie);
                }
                else if (zombis[i].tipo == ZOMBI_CONO) {
                    dibujarZombieCono(xZombie, yZombie);
                }
            }
        }

        
        dibujarOleada();

        //pantalla de derrota
        if (juegoPerdido == 1) {

            DrawRectangle(
                0, 0,
                configuracion_juego.ancho_casa + configuracion_juego.ancho_jardin,
                configuracion_juego.alto_menu + configuracion_juego.alto_jardin,
                Fade(BLACK, 0.65f) //oscurece la pantalla
            );

            DrawText(
                "GAME OVER",
                (configuracion_juego.ancho_casa + configuracion_juego.ancho_jardin - MeasureText("GAME OVER", 60)) / 2,
                300,
                60,
                RED
            );
        }

        //pantalla de victoria
        else if (juegoGanado == 1) {

            DrawRectangle(
                0, 0,
                configuracion_juego.ancho_casa + configuracion_juego.ancho_jardin,
                configuracion_juego.alto_menu + configuracion_juego.alto_jardin,
                Fade(BLACK, 0.65f) //oscurece la pantalla
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

        //boton volver a jugar
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

    //libera las imagenes usadas
    liberarJardin();
    liberarLapida();
    liberarCasa();
    liberarPlantas();
    liberarCortacesped();
    liberarZombieFlag();
    liberarZombieNormal();
    liberarZombieCono();
    liberarPala();
    liberarNotasOleadas();

    //libera música y audio
    StopMusicStream(musicaFondo);
    UnloadMusicStream(musicaFondo);
    CloseAudioDevice();

    cerrarVentana();

    //libera la memoria utilizada por el mapa
    liberarMapa(mapa_jardin, filas);
    mapa_jardin = NULL;

    return 0;
}
