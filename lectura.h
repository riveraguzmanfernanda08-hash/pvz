#ifndef LECTURA_H
#define LECTURA_H

// valores validos de las celdas de la matriz del mapa
#define CELDA_PASTO 0       // celda libre donde se puede plantar
#define celda_obstaculo 1   // no se puede plantar sobre esto
#define celda_max_valor 1   // mayor valor permitido en el archivo

// matriz del jardin cargada desde el archivo (NULL si no se ha cargado)
extern int **mapa_jardin;

// funciones MAPA
int **leerMapa(const char *nombreArchivo, int *filas, int *columnas);
void mostrarMapa(int **mapa, int filas, int columnas);
void liberarMapa(int **mapa, int filas);
int esObstaculo(int fila, int columna);

// genera un mapa aleatorio y lo guarda en un archivo (porcentaje de obstaculos del 0 al 100)
int generarMapa(const char *nombreArchivo, int filas, int columnas, int porcentajeObstaculos);

// lee config.txt y sobreescribe los valores por defecto. retorna 1 si ok, 0 si error
int leerConfiguracion(const char *nombreArchivo);

#endif
