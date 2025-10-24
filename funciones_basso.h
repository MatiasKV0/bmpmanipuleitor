#ifndef FUNCIONES_BASSO_H_INCLUDED
#define FUNCIONES_BASSO_H_INCLUDED
#include "funciones_grupo.h"

int limitar_rango_bmp(int valor);
int tonalidadColumnasSecuencial(HEADER* header, FILE* archivo, float cantColumnas, char* nomArch);
void manejarPadding(HEADER* header, FILE* archivoEntrada, FILE* archivoSalida);

#endif // FUNCIONES_BASSO_H_INCLUDED
