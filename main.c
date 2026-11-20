#include <stdio.h>
#include "visual.h"
#include "logica.h"
#include "raylib.h"
#include "config.h"

Music musicaFondo; //Variable que guarda la musica

// Reinicia todos los elementos de la partida
void reiniciarPartida(void) {
    //Inicializar todos los elementos
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

    iniciarOleada(); //Comenenzar desde la primera oleada

    // Reiniciar soles
    solesJugador = 200;

    // Reiniciar estados de la partida
    juegoPerdido = 0;
    juegoGanado = 0;
}


int main(void) {
    inicializarConfiguracion();
    iniciarVentana();   //Inicializar la ventana 
    InitAudioDevice(); //Inicializar audio

    //Cargar y reproducir música
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

    iniciarOleada(); //Comenzar primera oleada

    while (!WindowShouldClose()) { //Mantener el juego abierto mientras usuario no cierre la ventana

        UpdateMusicStream(musicaFondo);// Actualizar música

        float tiempo = GetFrameTime(); // Obtener el tiempo transcurrido entre fotogramas

        // ACTUALIZAR EL JUEGO
        if (juegoPerdido == 0 && juegoGanado == 0) { //Mientras la partida no ha terminado

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
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { //Comprobar si el usuario presiono clic izquierdo del mouse

                Vector2 mousePos = GetMousePosition(); //Obtener la posicion del mouse

                recogerSol(mousePos);// Recoger soles

                seleccionarPlanta(GetMouseX(), GetMouseY()); // Seleccionar planta o pala

                // Comprobar si el clic fue dentro del jardín
                if (mousePos.x >= configuracion_juego.ancho_casa && mousePos.x < configuracion_juego.ancho_casa + configuracion_juego.ancho_jardin &&
                    mousePos.y >= configuracion_juego.alto_menu && mousePos.y < configuracion_juego.alto_menu + configuracion_juego.alto_jardin) {
                    
                        // Calcular la fila y columna donde se hizo clic
                    int fila = (mousePos.y - configuracion_juego.alto_menu) / configuracion_juego.alto_celda;
                    int columna = (mousePos.x - configuracion_juego.ancho_casa) / configuracion_juego.ancho_celda;

                    // Si está seleccionada la pala
                    if (palaSeleccionada == 1) {

                        quitarPlanta(fila, columna); //Quitar planta de la casilla seleccionada

                    }

                    // Si está seleccionada una planta
                    else if (plantaSeleccionada != -1) {

                        if (colocarPlanta(fila, columna, plantaSeleccionada)) { //Coloca la planta en la casilla seleccionada 
                            plantaSeleccionada = -1; //Dejar de seleccionar
                        }
                    }
                }
            }
        }


        // DETECTAR CLIC EN VOLVER A JUGAR
        else {

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { //Ver si hizo clic

                Vector2 mousePos = GetMousePosition(); //Obtener la posicion del mouse

                Rectangle boton = { //Posicion y tamaño del boton
                    (configuracion_juego.ancho_casa + configuracion_juego.ancho_jardin - 240) / 2, 380,240,60 };

                if (CheckCollisionPointRec(mousePos, boton)) { //Ver si el clic fue en el boton
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

            if (zombieVivo(i)) { //Ver si el zombie sigue vivo

                //Calcular la posicion del zombie 
                int xZombie = (int)(configuracion_juego.ancho_casa + zombis[i].x);
                int yZombie = configuracion_juego.alto_menu + zombis[i].fila * configuracion_juego.alto_celda;

                // Comprobar el tipo de zombi
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

            DrawRectangle( //Oscurece la pantalla
                0, 0,
                configuracion_juego.ancho_casa + configuracion_juego.ancho_jardin,
                configuracion_juego.alto_menu + configuracion_juego.alto_jardin,

                Fade(BLACK, 0.65f)
            );

            DrawText( //Mostar el mensaje
                "GAME OVER",
                (configuracion_juego.ancho_casa + configuracion_juego.ancho_jardin - MeasureText("GAME OVER", 60)) / 2,
                300,
                60,
                RED
            );
        }


        // PANTALLA DE VICTORIA
        else if (juegoGanado == 1) {

            DrawRectangle( //Oscurece la pantalla
                0, 0,
                configuracion_juego.ancho_casa + configuracion_juego.ancho_jardin,
                configuracion_juego.alto_menu + configuracion_juego.alto_jardin,
                Fade(BLACK, 0.65f)
            );

            DrawText( //Muestra el mensaje
                "¡VICTORIA!",
                (configuracion_juego.ancho_casa + configuracion_juego.ancho_jardin - MeasureText("¡VICTORIA!", 60)) / 2,

                270,
                60,
                GOLD
            );

            DrawText( //Muestra el mensaje
                "Has protegido tu casa",
                (configuracion_juego.ancho_casa + configuracion_juego.ancho_jardin - MeasureText("Has protegido tu casa", 25)) / 2,
                340,
                25,
                WHITE
            );
        }


        // BOTON VOLVER A JUGAR
        if (juegoPerdido == 1 || juegoGanado == 1) { //Mostar cuando la partida termine

            Rectangle boton = { //Posicion y tamaño del boton
                (configuracion_juego.ancho_casa + configuracion_juego.ancho_jardin - 240) / 2,
                380,
                240,
                60
            };

            DrawRectangleRec(boton, DARKGREEN); //Fonso del boton
            DrawRectangleLinesEx(boton, 3, WHITE); //Borde del boton

            DrawText( //Mostrar mensaje
                "VOLVER A JUGAR",
                boton.x + (boton.width - MeasureText("VOLVER A JUGAR", 20)) / 2,
                boton.y + 20,
                20,
                WHITE
            );
        }
        //Termina el dibujo de los distintos elementos 
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

    return 0;
}
