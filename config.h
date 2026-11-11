#ifndef CONFIG_H
#define CONFIG_H

#define FILAS 5
#define COLUMNAS 9

#define ANCHO_CELDA 80
#define ALTO_CELDA 90
#define ALTO_MENU 100

#define ANCHO_JARDIN (COLUMNAS * ANCHO_CELDA)
#define ALTO_JARDIN (FILAS * ALTO_CELDA)

#define TIPO_GUISANTE 0
#define TIPO_GIRASOL 1
#define TIPO_NUEZ 2

#define VIDA_GUISANTE 300
#define DANO_GUISANTE 25
#define COSTO_GUISANTE 100
#define PRODUCCION_GUISANTE 0
#define COOLDOWN_GUISANTE 1.5

#define VIDA_GIRASOL 300
#define DANO_GIRASOL 0
#define COSTO_GIRASOL 50
#define PRODUCCION_GIRASOL 25
#define COOLDOWN_GIRASOL 10.0

#define VIDA_NUEZ 4000
#define DANO_NUEZ 0
#define COSTO_NUEZ 50
#define PRODUCCION_NUEZ 0

#define RADIO_SOL 18
#define COOLDOWN_SOL 8.0

#define DANO_ZOMBI 100
#define ZOMBIS_POR_OLEADA 5
#define TIEMPO_ENTRE_ZOMBIS 3.0
#define TIEMPO_ENTRE_OLEADAS 5.0

// ---- Zona izquierda: casa + carril de cortacéspedes ----
#define ANCHO_IMG_CASA  100                              // NUEVO: solo la imagen
#define ANCHO_CARRIL    80                               // NUEVO: columna gris
#define ANCHO_CASA      (ANCHO_IMG_CASA + ANCHO_CARRIL)  // CAMBIADO: era 150

#define ALTO_CORTACESPED   55
#define X_INI_CORTACESPED  (-72)   // dentro del carril (de -80 a 0)
#define VEL_CORTACESPED    300

#endif



