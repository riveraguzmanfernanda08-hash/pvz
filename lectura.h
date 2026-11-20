#ifndef LECTURA_H
#define LECTURA_H

int **leerMapa(const char *nombreArchivo, int *filas, int *columnas);

void mostrarMapa(int **mapa, int filas, int columnas);

void liberarMapa(int **mapa, int filas);

#endif