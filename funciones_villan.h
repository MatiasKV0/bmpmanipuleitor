#ifndef FUNCIONES_VILLAN_H_INCLUDED
#define FUNCIONES_VILLAN_H_INCLUDED
#include "funciones_grupo.h"

int verificarArchivo(FILE* arch, HEADER* header, bool imprimir);
bool leerHeader(FILE* archivo,HEADER* header);
bool cargarHeader(FILE* archivo, HEADER* header);

void imprimirHelp();
void imprimirInfo(HEADER* header, char* nomArch);
char* archivoSinExtension(const char* nom);
void eliminarDuplicados(int *argc, char *argv[]);

int tonalidadRojo(HEADER* header, FILE* archivo, float porcentaje, char* nomArch);
int espejarHorizontal(HEADER* header, FILE* archivo, char*nomArch);
int espejarVertical(HEADER* header, FILE* archivo, char*nomArch);

#endif // FUNCIONES_VILLAN_H_INCLUDED
