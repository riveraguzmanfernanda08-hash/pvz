#include "visual.h"
#include "logica.h"
#include "raylib.h"
#include "config.h"

int main() {
    iniciarVentana();
    cargarCasa();
    cargarCortacesped();

    inicializarZombis();
    inicializarPlantas();
    inicializarSoles();
    inicializarProyectiles();
    inicializarCortacespedes();

    iniciarOleada();

    while (!WindowShouldClose()) {

        float tiempo = GetFrameTime();

        moverZombis(tiempo);
        moverZombis(tiempo);
        activarCortacespedes();
        actualizarCortacespedes(tiempo);
        atacarPlanta(tiempo);
        actualizarOleadas(tiempo);

        actualizarSoles(tiempo);
        producirSoles(tiempo);

        actualizarDisparos(tiempo);
        moverProyectiles(tiempo);
        detectarImpactos();

        // Detectar clic del mouse
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mousePos = GetMousePosition();
            recogerSol(mousePos);
            seleccionarPlanta(GetMouseX(), GetMouseY());
            if (plantaSeleccionada != -1 &&
                mousePos.x >= ANCHO_CASA &&
                mousePos.x < ANCHO_CASA + ANCHO_JARDIN &&
                mousePos.y >= ALTO_MENU &&
                mousePos.y < ALTO_MENU + ALTO_JARDIN) {

                int fila = (mousePos.y - ALTO_MENU) / ALTO_CELDA;
                int columna = (mousePos.x - ANCHO_CASA) / ANCHO_CELDA;

                if (colocarPlanta(fila, columna, plantaSeleccionada)) {
                    plantaSeleccionada = -1;
                }
            }
        } // Aqui termina el clic del mouse.

        // Dibujar el juego
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

        for (int i = 0; i < CANT_ZOMBIS; i++) {

            if (zombieVivo(i)) {

                int yZombie = ALTO_MENU + zombis[i].fila * ALTO_CELDA;

                dibujarZombie((int)(ANCHO_CASA + zombis[i].x), yZombie);
            }
        }

        EndDrawing();
    }

    liberarCasa();
    liberarCortacesped();
    cerrarVentana();

    return 0;
}
