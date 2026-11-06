
#ifndef LOGICA_H
#define LOGICA_H
#include "raylib.h"

#define CANT_ZOMBIS 5
#define CANT_PLANTAS 45
#define CANT_SOLES 10


typedef struct {
    int fila;
    float x;
    int vida;
    int dano;
} Zombi;

typedef struct{
    int tipo;
    int fila;
    int columna;
    int vida;
    int dano;
    int costoSoles;
    int produccionSoles;
    float cooldown;
    float temporizador;
    int activa;
} Planta;

typedef struct {
    float x;
    float y;
    int activo;
    float cooldown;
} Sol;

typedef struct {
    float x;
    float y;
    int fila;
    int dano;
    int activo;
} Proyectil;



extern Zombi zombis[CANT_ZOMBIS];
extern Planta plantas[CANT_PLANTAS];
extern int solesJugador;
extern Sol soles[CANT_SOLES];
extern float temporizadorSol;
extern int plantaSeleccionada;
extern Proyectil proyectiles[CANT_PROYECTILES];


//zombie
void inicializarZombis(void);
void moverZombis(float tiempo);
int zombieVivo(int numeroZombie);
void recibirDano(int numeroZombie, int cantidadDano);

//plantas
void inicializarPlantas(void);
int colocarPlanta(int fila, int columna, int tipo);


//sol
void inicializarSoles(void);
void generarSol(void);
void recogerSol(Vector2 mousePos);
void actualizarSoles(float tiempo);
int generarSolEn(float x, float y);
void producirSoles(float tiempo);


//tarjetas
void seleccionarPlanta(int mouseX, int mouseY);

//disparos
void inicializarProyectiles(void);
int generarProyectil(float x, float y, int fila, int dano);
void moverProyectiles(float tiempo);

#endif

