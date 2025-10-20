#ifndef FUNCIONES_GRUPO_H_INCLUDED
#define FUNCIONES_GRUPO_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define EXITO 0
#define ERROR_ARGS 1
#define ERROR_ARCH 2
#define ERROR_MEM 3
#define ERROR_BMP 4
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define MAX_ANCHO 240
#define MAX_ALTO 360

typedef struct {
    unsigned short tipobm;           // "BM"
    unsigned int tamArchivo;         // Tamaño total del archivo
    unsigned short reserva;          // Reservado (0)
    unsigned short reserva1;         // Reservado (0)
    unsigned int comienzoImagen;     // Offset de los datos de imagen
    unsigned int tamEncabezado;      // Tamaño del encabezado de info (40)
    int ancho;                       // Ancho de la imagen
    int alto;                        // Alto de la imagen
    unsigned short planos;           // Siempre 1
    unsigned short tampuntos;        // Bits por píxel (24 normalmente)
    unsigned int compresion;         // 0 = sin compresión
    unsigned int tamimagen;          // Tamaño de los datos de imagen
    int resolucionhor;               // Resolución horizontal
    int resolucionvert;              // Resolución vertical
    unsigned int tampaletacolores;   // Colores en la paleta (0 si no aplica)
    unsigned int contcolor;          // Colores importantes (0 si todos)
} HEADER;

#include "funciones_villan.h"
#include "funciones_basso.h"
#include "funciones_ereno.h"

// Ereño, Tomas:
// DNI: 44513506
// Entrega: Si

// Basso, Tiziano Axel:
// DNI: 43916389
// Entrega: No

// Villan, Matias Nicolas:
// DNI: 46117338
// Entrega: No

int procesar_imagen(int argc, char* argv[]);
HEADER cargaCabecera(FILE* archivoEntrada, FILE* archivoNegativo);

typedef struct
{
    unsigned char pixel[3];
}t_pixel;

#endif // FUNCIONES_GRUPO_H_INCLUDED
