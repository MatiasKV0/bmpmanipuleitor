#ifndef FUNCIONES_ERENO_H_INCLUDED
#define FUNCIONES_ERENO_H_INCLUDED
#include "funciones_grupo.h"

int tonalidadAzul(HEADER *header, FILE *archivo, float porcentaje, char *nomArch);
int tonalidadVerde(HEADER *header, FILE *archivo, float porcentaje, char *nomArch);
int recortePorcentaje(HEADER *header, FILE *archivo, float porcentaje, char *nomArch);
int aumentarContraste(HEADER *header, FILE *archivo, float porcentaje, char *nomArch);
int reducirContraste(HEADER *header, FILE *archivo, float porcentaje, char *nomArch);
int achicarImagen(HEADER *header, FILE *archivo, float porcentaje, char *nomArch);

int convertirNegativo(HEADER *header, FILE *archivo, char *nomArch);
int escalaGrises(HEADER *header, FILE *archivo, char *nomArch);

int rotar90gradosDerecha(HEADER *header, FILE *archivo, char *nomArch);
int rotar90gradosIzquierda(HEADER *header, FILE *archivo, char *nomArch);

int concatenarHorizontal(HEADER *header1, FILE *archivo1, char *nomArch1,
                         HEADER *header2, FILE *archivo2, char *nomArch2);
int concatenarVertical(HEADER *header1, FILE *archivo1, char *nomArch1,
                       HEADER *header2, FILE *archivo2, char *nomArch2);

#endif // FUNCIONES_ERENO_H_INCLUDED
