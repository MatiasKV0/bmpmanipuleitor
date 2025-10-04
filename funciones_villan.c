#include "funciones_villan.h"

int tonalidadRojo(FILE* archivoEntrada, float porcentaje)
{
    FILE * archivoRojo;
    METADATAgral metadata;
    int i, j;

    archivoRojo = fopen("virus_tonalidad-roja.bmp","wb");
    if(!archivoRojo)
    {
        printf("Error al abrir el archivo de salida.");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    metadata = cargaCabecera(archivoEntrada,archivoRojo);

    fseek(archivoRojo,metadata.comienzoImagen,SEEK_SET);
    fseek(archivoEntrada,metadata.comienzoImagen,SEEK_SET);

    for(i=0;i<metadata.alto;i++)
      {
        for(j=0;j<metadata.ancho;j++)
        {
            t_pixel pixel;
            fread(&pixel, 3, 1, archivoEntrada);

            pixel.pixel[2] = min(255, pixel.pixel[2] + (porcentaje/100) * (pixel.pixel[2]));

            fwrite(&pixel, 3, 1, archivoRojo);
       }
      }

    fclose(archivoRojo);

    return TODO_OK;
}
