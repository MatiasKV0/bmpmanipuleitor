#ifndef FUNCIONES_ERENO_H_INCLUDED
#define FUNCIONES_ERENO_H_INCLUDED
#include "funciones_grupo.h"

int convertirNegativo(FILE* archivoEntrada);
int blancoNegro(FILE* archivoEntrada);

int aumentarContraste(FILE* archivoEntrada);
int reducirContraste(FILE* archivoEntrada);
int tonalidadAzul(FILE* archivoEntrada);
int tonalidadVerde(FILE* archivoEntrada);
int recortarImagen(FILE* archivoEntrada);

int rotar90gradosDerecha(FILE* archivoEntrada);
int rotar90gradosIzquierda(FILE* archivoEntrada);

int comodin(FILE* archivoEntrada);

#endif // FUNCIONES_ERENO_H_INCLUDED
