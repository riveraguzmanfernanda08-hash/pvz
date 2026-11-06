#include "visual.h"
#include "logica.h"
#include "raylib.h"
#include "config.h"

int main() {
    iniciarVentana();
    inicializarZombis();
    inicializarPlantas();
    inicializarSoles();
    inicializarProyectiles();
 
    colocarPlanta(2, 3, TIPO_GIRASOL); //prueba

    while (!WindowShouldClose()) {
        moverZombis(GetFrameTime());
        actualizarSoles(GetFrameTime());
        producirSoles(GetFrameTime());
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        recogerSol(mousePos);
        seleccionarPlanta(GetMouseX(), GetMouseY());
        if (plantaSeleccionada != -1 && mousePos.y >= ALTO_MENU) {

        int fila = (mousePos.y - ALTO_MENU) / ALTO_CELDA;
        int columna = mousePos.x / ANCHO_CELDA;

        if (colocarPlanta(fila, columna, plantaSeleccionada)) {
        plantaSeleccionada = -1;
        }
        }
        }


        BeginDrawing();
        ClearBackground(RAYWHITE);
        dibujarJardin();
        dibujarMenu();
        dibujarPlantas();
        dibujarSoles();
        dibujarProyectiles();
        moverProyectiles(GetFrameTime());

        for (int i = 0; i < CANT_ZOMBIS; i++) {
            if(zombieVivo(i)){
            int yZombie = ALTO_MENU + zombis[i].fila * ALTO_CELDA;

            dibujarZombie((int)zombis[i].x, yZombie);
            }
        }

        EndDrawing();
    }

    cerrarVentana();

    return 0;
}
