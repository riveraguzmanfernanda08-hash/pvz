#include "visual.h"
#include "raylib.h"
#include "config.h"
#include "logica.h"
#include <stdio.h>


void iniciarVentana(){
    InitWindow(COLUMNAS * ANCHO_CELDA, FILAS * ALTO_CELDA, "Plants vs zombies"); //crear ventana con tamaño y nombre
    MaximizeWindow();
}
 
void cerrarVentana(){
    CloseWindow();//cierra la ventana
}

void dibujarJardin(){
    DrawRectangle(0,0,ANCHO_JARDIN,ALTO_MENU,BROWN);
    for (int i = 0; i < FILAS; i++) {
    for (int j = 0; j < COLUMNAS; j++) {
        int x = j*ANCHO_CELDA; //convertir en coordenadas
        int y = ALTO_MENU + i * ALTO_CELDA;
        Color color;

    if ((i + j) % 2 == 0) {
        color = GREEN;
    } else {
        color = DARKGREEN;
    }
        DrawRectangle(x,y,ANCHO_CELDA,ALTO_CELDA,color);
        DrawRectangleLines(x, y, ANCHO_CELDA, ALTO_CELDA, BLACK);
        
    }
}
}

void dibujarMenu(void) {
    DrawRectangle(0, 0, 1000, ALTO_MENU, BROWN);
    DrawCircle(42, 47, 20, GOLD);
    char textoSoles[20];
    sprintf(textoSoles, "%d", solesJugador);
    DrawText(textoSoles, 70, 35, 25, DARKBROWN);
    
    // Tarjeta del guisante
    DrawRectangle(150, 10, 110, 80, BEIGE); 
    DrawText("Guisante", 160, 20, 15, DARKGREEN);
    DrawText("100", 190, 60, 20, BLACK);
    DrawCircle(205, 48, 13, GREEN);
    DrawCircle(216, 43, 5, DARKGREEN);
    if (plantaSeleccionada == TIPO_GUISANTE) {
        DrawRectangleLinesEx((Rectangle){150, 10, 110, 80}, 4, GOLD);
    }
    else {
        DrawRectangleLines(150, 10, 110, 80, BLACK);
    }




    // Tarjeta del girasol
    DrawRectangle(270, 10, 110, 80, BEIGE); 
    DrawText("Girasol", 280, 20, 15, DARKGREEN);
    DrawText("50", 315, 60, 20, BLACK);
    DrawCircle(325, 48, 13, YELLOW);
    DrawCircle(325, 48, 6, ORANGE);
    // Borde del girasol
    if (plantaSeleccionada == TIPO_GIRASOL) {
        DrawRectangleLinesEx((Rectangle){270, 10, 110, 80}, 4, GOLD);
    }
    else {
        DrawRectangleLines(270, 10, 110, 80, BLACK);
    }



    // Tarjeta de la nuez
    DrawRectangle(390, 10, 110, 80, BEIGE); 
    DrawText("Nuez", 400, 20, 15, DARKGREEN);
    DrawText("50", 435, 60, 20, BLACK);
    DrawCircle(445, 48, 15, BROWN);
    if (plantaSeleccionada == TIPO_NUEZ) {
        DrawRectangleLinesEx((Rectangle){390, 10, 110, 80}, 4, GOLD);
    }
    else {
        DrawRectangleLines(390, 10, 110, 80, BLACK);
    }








}

void dibujarZombie(int x, int y){
    // Cabeza
    DrawCircle(x + 20, y + 15, 15, LIME);

    // Ojos
    DrawCircle(x + 15, y + 12, 3, WHITE);
    DrawCircle(x + 25, y + 12, 3, WHITE);

    // Pupilas
    DrawCircle(x + 15, y + 12, 1, BLACK);
    DrawCircle(x + 25, y + 12, 1, BLACK);

    // Cuerpo
    DrawRectangle(x + 7, y + 30, 26, 35, GRAY);

    // Brazos
    DrawLine(x + 7, y + 35, x - 5, y + 50, LIME);
    DrawLine(x + 33, y + 35, x + 45, y + 45, LIME);

    // Piernas
    DrawLine(x + 13, y + 65, x + 8, y + 82, DARKGRAY);
    DrawLine(x + 27, y + 65, x + 32, y + 82, DARKGRAY);
}

void dibujarPlantas(void){
    for(int i=0;i<CANT_PLANTAS;i++){
        if(plantas[i].activa==1){
            int x=plantas[i].columna*ANCHO_CELDA;
            int y = ALTO_MENU + plantas[i].fila * ALTO_CELDA;

            if (plantas[i].tipo == TIPO_GUISANTE) {
            DrawRectangle(x + 35, y + 45, 10, 30, DARKGREEN);
            DrawCircle(x + 40, y + 45, 20, GREEN);
            DrawEllipse(x + 23, y + 55, 15, 7, LIME);

            }
            else if (plantas[i].tipo == TIPO_GIRASOL) {
            DrawCircle(x + 40, y + 45, 20, YELLOW);
            }
            else if (plantas[i].tipo == TIPO_NUEZ) {
                DrawCircle(x + 40, y + 45, 20, BROWN);
            }
        }
    }

}


void dibujarSoles(void){
    for (int i = 0; i < CANT_SOLES; i++) {
        if(soles[i].activo==1){
            DrawCircle((int)soles[i].x, (int)soles[i].y, 18, GOLD);
            DrawCircle((int)soles[i].x, (int)soles[i].y, 12, YELLOW);
        }
    }
}

void dibujarProyectiles(void) {

    for (int i = 0; i < CANT_PROYECTILES; i++) {

        if (proyectiles[i].activo == 1) {

            DrawCircle(
                (int)proyectiles[i].x, //posicion horizontal
                (int)proyectiles[i].y, //posicion vertical
                7, //radio
                LIME //color
            );
        }
    }
}