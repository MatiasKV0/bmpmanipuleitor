#ifndef FUNCIONES_GRUPO_H_INCLUDED
#define FUNCIONES_GRUPO_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TODO_OK 0
#define ARCHIVO_NO_ENCONTRADO 10
#define NO_SE_PUEDE_CREAR_ARCHIVO 20
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define MAX_ANCHO 240
#define MAX_ALTO 360

#include "funciones_basso.h"
#include "funciones_ereno.h"
#include "funciones_villan.h"

// Ere�o, Tomas:
// DNI: 44513506
// Entrega: S�

// Basso, Tiziano Axel:
// DNI: 43916389
// Entrega: No

// Villan, Matias Nicolas:
// DNI: 46117338
// Entrega: No

typedef struct
{
    unsigned short tipobm;
    unsigned int tamArchivo;
    unsigned short reserva;
    unsigned short reserva1;
    unsigned int comienzoImagen;
    unsigned int tamEncabezado;
    unsigned int ancho;
    unsigned int alto;
    unsigned short planos;
    unsigned short tampuntos;
    unsigned int compresion;
    unsigned int tamimagen;
    unsigned int resolucionhor;
    unsigned int resolucionvert;
    unsigned int tampaletacolores;
    unsigned int contcolor;

}METADATAgral;

int procesar_imagen(int argc, char* argv[]);
METADATAgral cargaCabecera(FILE* archivoEntrada, FILE* archivoSalida);

typedef struct
{
    unsigned char pixel[3];
    unsigned int profundidad;
}t_pixel;

typedef struct
{
    unsigned int tamArchivo;
    unsigned int tamEncabezado;
    unsigned int comienzoImagen;
    unsigned int ancho;
    unsigned int alto;
    unsigned short profundidad;
}t_metadata;

typedef struct {
    unsigned char pixel [3];
} pixeles;

#endif // FUNCIONES_GRUPO_H_INCLUDED
