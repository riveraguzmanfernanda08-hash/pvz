#include <stdio.h>
#include <stdlib.h>
#include "lectura.h"

int **leerMapa(const char *nombreArchivo, int *filas, int *columnas) {

    FILE *archivo = fopen(nombreArchivo, "r");

    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        return NULL;
    }

    // Leer las dimensiones del mapa
    if (fscanf(archivo, "%d %d", filas, columnas) != 2) {
        printf("Error: no se pudieron leer las dimensiones del mapa\n");
        fclose(archivo);
        return NULL;
    }

    // Comprobar que las dimensiones sean positivas
    if (*filas <= 0 || *columnas <= 0) {
        printf("Error: las dimensiones del mapa deben ser positivas\n");
        fclose(archivo);
        return NULL;
    }

    // Reservar memoria para las filas
    int **mapa = malloc(*filas * sizeof(int *));

    if (mapa == NULL) {
        fclose(archivo);
        return NULL;
    }

    // Reservar memoria para cada columna
    for (int i = 0; i < *filas; i++) {

        mapa[i] = malloc(*columnas * sizeof(int));

        if (mapa[i] == NULL) {

            for (int j = 0; j < i; j++) {
                free(mapa[j]);
            }

            free(mapa);
            fclose(archivo);
            return NULL;
        }
    }

    // Leer y comprobar los valores de la matriz
    for (int i = 0; i < *filas; i++) {

        for (int j = 0; j < *columnas; j++) {

            if (fscanf(archivo, "%d", &mapa[i][j]) != 1) {
                printf("Error: faltan valores en el mapa\n");

                for (int k = 0; k < *filas; k++) {
                    free(mapa[k]);
                }

                free(mapa);
                fclose(archivo);
                return NULL;
            }

            if (mapa[i][j] != 0) {
                printf("Error: valor no valido en el mapa\n");

            for (int k = 0; k < *filas; k++) {
                free(mapa[k]);
            }

            free(mapa);
            fclose(archivo);
            return NULL;
            }
        }
    }

    fclose(archivo);

    return mapa;
}

// Mostrar la matriz por pantalla
void mostrarMapa(int **mapa, int filas, int columnas) {

    for (int i = 0; i < filas; i++) {

        for (int j = 0; j < columnas; j++) {
            printf("%d ", mapa[i][j]);
        }

        printf("\n");
    }
}

// Liberar la memoria de la matriz
void liberarMapa(int **mapa, int filas) {

    if (mapa == NULL) {
        return;
    }

    for (int i = 0; i < filas; i++) {
        free(mapa[i]);
    }

    free(mapa);
}
