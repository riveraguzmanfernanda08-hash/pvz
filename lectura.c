#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lectura.h"
#include "constantes.h"
#include "config.h"

// matriz del jardin
int **mapa_jardin = NULL;

/*
 * MAPA!!
 * lee el mapa desde un archivo de texto
 * retorna la matriz o NULL si hubo algun error.
 */
int **leerMapa(const char *nombreArchivo, int *filas, int *columnas) {

    FILE *archivo = fopen(nombreArchivo, "r");

    if (archivo == NULL) {
        printf("no se pudo abrir el archivo de mapa '%s'\n", nombreArchivo);
        return NULL;
    }

    // leer las dimensiones del mapa
    if (fscanf(archivo, "%d %d", filas, columnas) != 2) {
        printf("no se pudieron leer las dimensiones del mapa\n");
        fclose(archivo);
        return NULL;
    }

    // comprobar que las dimensiones esten dentro de los limites
    if (*filas <= 0 || *columnas <= 0 ||
        *filas > max_filas || *columnas > max_columnas) {
        printf("dimensiones invalidas (filas 1-%d, columnas 1-%d)\n",
               max_filas, max_columnas);
        fclose(archivo);
        return NULL;
    }

    // reservar memoria para las filas (calloc deja los punteros en NULL, lo que permite liberar de forma segura si algo falla a la mitad)
    int **mapa = calloc(*filas, sizeof(int *));

    if (mapa == NULL) {
        fclose(archivo);
        return NULL;
    }

    // reservar memoria para cada fila
    for (int i = 0; i < *filas; i++) {
        mapa[i] = malloc(*columnas * sizeof(int));
        if (mapa[i] == NULL) {
            liberarMapa(mapa, *filas);
            fclose(archivo);
            return NULL;
        }
    }

    // leer y comprobar los valores de la matriz
    for (int i = 0; i < *filas; i++) {
        for (int j = 0; j < *columnas; j++) {
            if (fscanf(archivo, "%d", &mapa[i][j]) != 1) {
                printf("faltan valores en el mapa (fila %d, columna %d)\n", i + 1, j + 1);
                liberarMapa(mapa, *filas);
                fclose(archivo);
                return NULL;
            }

            if (mapa[i][j] < 0 || mapa[i][j] > celda_max_valor) {
                printf("valor no valido (%d) en fila %d, columna %d\n",
                       mapa[i][j], i + 1, j + 1);
                liberarMapa(mapa, *filas);
                fclose(archivo);
                return NULL;
            }
        }
    }
    fclose(archivo);
    return mapa;
}

// mostrar la matriz por pantalla
void mostrarMapa(int **mapa, int filas, int columnas) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            printf("%d ", mapa[i][j]);
        }
        printf("\n");
    }
}

// liberar la memoria de la matriz
void liberarMapa(int **mapa, int filas) {

    if (mapa == NULL) {
        return;
    }

    for (int i = 0; i < filas; i++) {
        free(mapa[i]);   // free(NULL) es seguro
    }
    free(mapa);
}

// indica si la celda (fila, columna) es un obstaculo
int esObstaculo(int fila, int columna) {
    if (mapa_jardin == NULL || fila < 0 || fila >= filas_jardin || columna < 0 || columna >= columnas_jardin) {
        return 0;
    }
    return mapa_jardin[fila][columna] == celda_obstaculo;
}

// genera un mapa aleatorio y lo escribe en un archivo con el mismo formato que leerMapa.
// la primera columna siempre queda libre para no bloquear el avance de las plantas.
// retorna 1 si se guardo correctamente, 0 si hubo error.
int generarMapa(const char *nombreArchivo, int filas, int columnas, int porcentajeObstaculos) {
    if (filas <= 0 || columnas <= 0 || filas > max_filas || columnas > max_columnas ||
        porcentajeObstaculos < 0 || porcentajeObstaculos > 100) {
        printf("parametros de generacion invalidos\n");
        return 0;
    }

    FILE *archivo = fopen(nombreArchivo, "w");

    if (archivo == NULL) {
        printf("no se pudo crear '%s'\n", nombreArchivo);
        return 0;
    }
    srand((unsigned int)time(NULL));
    fprintf(archivo, "%d %d\n", filas, columnas);

    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            int valor = CELDA_PASTO;
            if (j > 0 && rand() % 100 < porcentajeObstaculos) {
                valor = celda_obstaculo;
            }
            fprintf(archivo, "%d%c", valor, (j == columnas - 1) ? '\n' : ' ');
        }
    }
    fclose(archivo);
    return 1;
}

/*
 *  CONFIGURACION!!
 */
// quita espacios y saltos de linea al inicio y final de una cadena
static char *recortar(char *texto) {
    while (*texto == ' ' || *texto == '\t') texto++;
    size_t largo = strlen(texto);
    while (largo > 0 && (texto[largo - 1] == '\n' || texto[largo - 1] == '\r' ||
                         texto[largo - 1] == ' '  || texto[largo - 1] == '\t')) {
        texto[--largo] = '\0';
    }
    return texto;
}

// convierte el nombre de un zombi del archivo a su indice (o -1 si no existe)
static int indiceZombi(const char *nombre) {
    if (strcmp(nombre, "zombi_normal") == 0)      return ZOMBI_NORMAL;
    if (strcmp(nombre, "zombi_flag") == 0)        return ZOMBI_FLAG;
    if (strcmp(nombre, "zombi_cono") == 0)        return ZOMBI_CONO;
    return -1;
}

// convierte el nombre de una planta del archivo a su indice (o -1 si no existe)
static int indicePlanta(const char *nombre) {
    if (strcmp(nombre, "guisante") == 0) return TIPO_GUISANTE;
    if (strcmp(nombre, "girasol") == 0)  return TIPO_GIRASOL;
    if (strcmp(nombre, "nuez") == 0)     return TIPO_NUEZ;
    if (strcmp(nombre, "cactus") == 0)   return TIPO_CACTUS;
    return -1;
}

// asigna un parametro "clave valor" de la seccion juego.
// retorna 1 si la clave existe, 0 si no.
static int asignarParametroJuego(const char *clave, float valor) {
    ConfiguracionJuego *c = &configuracion_juego;

    if (strcmp(clave, "ancho_celda") == 0)
        c->ancho_celda = (int)valor;
    else if (strcmp(clave, "alto_celda") == 0)
        c->alto_celda = (int)valor;
    else if (strcmp(clave, "alto_menu") == 0)
        c->alto_menu = (int)valor;
    else if (strcmp(clave, "ancho_img_casa") == 0)
        c->ancho_img_casa = (int)valor;
    else if (strcmp(clave, "ancho_carril") == 0)
        c->ancho_carril = (int)valor;
    else if (strcmp(clave, "alto_cortacesped") == 0)
        c->alto_cortacesped = (int)valor;
    else if (strcmp(clave, "posicion_cortacesped") == 0)
        c->posicion_cortacesped = (int)valor;
    else if (strcmp(clave, "vel_cortacesped") == 0)
        c->vel_cortacesped = (int)valor;
    else if (strcmp(clave, "radio_sol") == 0)
        c->radio_sol = (int)valor;
    else if (strcmp(clave, "cooldown_sol") == 0)
        c->cooldown_sol = valor;
    else return 0;

    return 1;
}

// asigna un parametro "clave valor" de la seccion oleadas.
static int asignarParametroOleada(const char *clave, float valor) {
    ConfiguracionOleada *o = &configuracion_oleada;

    if (strcmp(clave, "zombis_por_oleada") == 0)
        o->zombis_por_oleada = (int)valor;
    else if (strcmp(clave, "tiempo_entre_zombis") == 0)
        o->tiempo_entre_zombis = valor;
    else if (strcmp(clave, "tiempo_entre_oleadas") == 0)
        o->tiempo_entre_oleadas = valor;
    else if (strcmp(clave, "max_oleadas") == 0)
        o->max_oleadas = (int)valor;
    else return 0;

    return 1;
}

// comprueba que la configuracion leida tenga valores positivos y no negativos
static int configuracionValida(void) {
    ConfiguracionJuego *c = &configuracion_juego;
    ConfiguracionOleada *o = &configuracion_oleada;

    if (c->ancho_celda <= 0 || c->alto_celda <= 0 || c->alto_menu <= 0 || c->ancho_img_casa <= 0 || c->ancho_carril < 0 ||c->alto_cortacesped <= 0 || c->vel_cortacesped <= 0 || c->radio_sol <= 0 || c->cooldown_sol <= 0) {
        printf("valores de juego invalidos en la configuracion\n");
        return 0;
    }

    for (int i = 0; i < NUM_TIPOS_ZOMBI; i++) {
        if (tipos_zombi[i].vida <= 0 || tipos_zombi[i].dano < 0 || tipos_zombi[i].velocidad <= 0) {
            printf("valores de zombi %d invalidos en la configuracion\n", i);
            return 0;
        }
    }

    for (int i = 0; i < NUM_TIPOS_PLANTA; i++) {
        if (tipos_planta[i].vida <= 0 || tipos_planta[i].dano < 0 || tipos_planta[i].costo < 0 || tipos_planta[i].cooldown < 0) {
            printf("valores de planta %d invalidos en la configuracion\n", i);
            return 0;
        }
    }

    if (o->zombis_por_oleada <= 0 || o->max_oleadas <= 0 || o->tiempo_entre_zombis <= 0 || o->tiempo_entre_oleadas < 0) {
        printf("valores de oleadas invalidos en la configuracion\n");
        return 0;
    }
    return 1;
}

// lee el archivo de configuracion. Debe llamarse DESPUES de inicializarConfiguracion(), ya que los parametros que falten en el archivo conservan su valor por defecto.
// retorna 1 si se leyo correctamente, 0 si hubo error
int leerConfiguracion(const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "r");

    if (archivo == NULL) {
        printf("no se pudo abrir '%s'\n", nombreArchivo);
        return 0;
    }

    char linea[256];
    char seccion[32] = "";
    int numeroLinea = 0;

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        numeroLinea++;
        char *texto = recortar(linea);
        // ignorar lineas vacias y comentarios
        if (texto[0] == '\0' || texto[0] == '#') {
            continue;
        }

        // cambio de seccion
        if (strcmp(texto, "juego") == 0 || strcmp(texto, "zombis") == 0 || strcmp(texto, "plantas") == 0 || strcmp(texto, "oleadas") == 0) {
            strcpy(seccion, texto);
            continue;
        }
        char clave[64];

        if (sscanf(texto, "%63s", clave) != 1) {
            continue;
        }

        if (strcmp(seccion, "juego") == 0 || strcmp(seccion, "oleadas") == 0) {
            float valor;

            if (sscanf(texto, "%*s %f", &valor) != 1) {
                printf("error en config. falta el valor de '%s'\n", clave);
                fclose(archivo);
                return 0;
            }

            int ok = (strcmp(seccion, "juego") == 0)
                   ? asignarParametroJuego(clave, valor)
                   : asignarParametroOleada(clave, valor);

            if (!ok) {
                printf("aviso config. parametro desconocido '%s'\n", clave);
            }
        }
        else if (strcmp(seccion, "zombis") == 0) {

            int id = indiceZombi(clave);
            int vida, dano, velocidad;

            if (id < 0) {
                printf("aviso config. zombi desconocido '%s'\n", clave);
            }
            else if (sscanf(texto, "%*s %d %d %d", &vida, &dano, &velocidad) != 3) {
                printf("error en config. '%s' necesita vida dano velocidad\n", clave);
                fclose(archivo);
                return 0;
            }
            else {
                tipos_zombi[id].vida = vida;
                tipos_zombi[id].dano = dano;
                tipos_zombi[id].velocidad = velocidad;
            }
        }
        else if (strcmp(seccion, "plantas") == 0) {
            int id = indicePlanta(clave);
            int vida, dano, costo, produccion;
            float cooldown;

            if (id < 0) {
                printf("aviso config. planta desconocida '%s'\n", clave);
            }
            else if (sscanf(texto, "%*s %d %d %d %d %f",
                            &vida, &dano, &costo, &produccion, &cooldown) != 5) {
                printf("error en config. '%s' necesita vida daño costo produccion cooldown\n", clave);
                fclose(archivo);
                return 0;
            }
            else {
                tipos_planta[id].vida = vida;
                tipos_planta[id].dano = dano;
                tipos_planta[id].costo = costo;
                tipos_planta[id].produccion = produccion;
                tipos_planta[id].cooldown = cooldown;
            }
        }
        else {
            printf("aviso config. linea fuera de una seccion: '%s'\n", texto);
        }
    }
    fclose(archivo);
    return configuracionValida();
}
