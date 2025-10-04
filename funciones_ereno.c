#include "funciones_ereno.h"

int status;
float prom;
float prom2;
int min, max;
int brightness;
float factor;

int convertirNegativo(FILE* archivoEntrada)
{
    FILE * archivoNegativo;
    METADATAgral metadata;

    archivoNegativo = fopen("virus_negativo.bmp","wb");
    if(!archivoNegativo)
    {
        printf("Error al abrir el archivo de salida.");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    metadata = cargaCabecera(archivoEntrada,archivoNegativo);

    t_pixel pixel;
    int i, j;

    fseek(archivoNegativo,metadata.comienzoImagen,SEEK_SET);
    fseek(archivoEntrada,metadata.comienzoImagen,SEEK_SET);

    for(i=0;i<=metadata.alto;i++)
      {
        for(j=0;j<=metadata.ancho;j++)
        {
           fread(&pixel,sizeof(unsigned char), 3, archivoEntrada);

            pixel.pixel[0] = 255 - pixel.pixel[0] ;
            pixel.pixel[1] = 255 - pixel.pixel[1] ;
            pixel.pixel[2] = 255 - pixel.pixel[2];

           fwrite(&pixel,sizeof(unsigned char), 3, archivoNegativo);
       }
      }

    fclose(archivoNegativo);

    return TODO_OK;
}

int tonalidadAzul(FILE* archivoEntrada)
{
    FILE * archivoAzul;

    METADATAgral metadata;

    int i, j;

    archivoAzul = fopen("virus_tonalidad_azul.bmp","wb");
    if(!archivoAzul)
    {
        printf("Error al abrir el archivo de salida.");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    metadata = cargaCabecera(archivoEntrada,archivoAzul);

    fseek(archivoAzul,metadata.comienzoImagen,SEEK_SET);
    fseek(archivoEntrada,metadata.comienzoImagen,SEEK_SET);
    // modifico los pixeles
    for(i=0;i<metadata.alto;i++)
      {
        for(j=0;j<metadata.ancho;j++)
        {
            t_pixel pixel;

            fread(&pixel, 3, 1, archivoEntrada);

            // Ajustar la tonalidad azul
            pixel.pixel[0] = min(255, pixel.pixel[0] + 0.5 * (pixel.pixel[0]));

            // Escribir el p�xel modificado en el archivo de salida
            fwrite(&pixel, 3, 1, archivoAzul);
       }
      }

    fclose(archivoAzul);

    return TODO_OK;
}

int tonalidadVerde(FILE* archivoEntrada)
{
    FILE * archivoVerde;
    int i, j;
    METADATAgral metadata;

    archivoVerde = fopen("virus_tonalidad_verde.bmp","wb");
    if(!archivoVerde)
    {
        printf("Error al abrir el archivo de salida.");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }



    metadata = cargaCabecera(archivoEntrada,archivoVerde);

    fseek(archivoVerde,metadata.comienzoImagen,SEEK_SET);
    fseek(archivoEntrada,metadata.comienzoImagen,SEEK_SET);

    // modifico los pixeles
    for(i=0;i<=metadata.alto;i++)
      {
        for(j=0;j<=metadata.ancho;j++)
        {
            t_pixel pixel;
            fread(&pixel, 3, 1, archivoEntrada);

            // Ajustar la tonalidad verde
            pixel.pixel[1] = min(255, pixel.pixel[1] + 0.5 * (pixel.pixel[1]));

            // Escribir el p�xel modificado en el archivo de salida
            fwrite(&pixel, 3, 1, archivoVerde);
       }
      }

    fclose(archivoVerde);

    return TODO_OK;
}

int blancoNegro(FILE* archivoEntrada)
{
    FILE * archivoBlanconegro;
    METADATAgral metadata;
    int i, j;

    archivoBlanconegro = fopen("virus_escala_de_grises.bmp","wb");
    if(!archivoBlanconegro)
    {
        printf("Error al abrir el archivo de salida.");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    metadata = cargaCabecera(archivoEntrada,archivoBlanconegro);

    fseek(archivoBlanconegro,metadata.comienzoImagen,SEEK_SET);
    fseek(archivoEntrada,metadata.comienzoImagen,SEEK_SET);
    // modifico los pixeles
    for(i=0;i<metadata.alto;i++)
      {
        for(j=0;j<metadata.ancho;j++)
        {
          t_pixel pixel;

          fread(&pixel, 3, 1, archivoEntrada);

          prom = ( pixel.pixel[0] + pixel.pixel[1] + pixel.pixel[2] ) / 3;
            // Convertir en Blanco y Negro

            pixel.pixel[0] = prom;
            pixel.pixel[1] = prom;
            pixel.pixel[2] = prom;

            // Escribir el p�xel modificado en el archivo de salida
          fwrite(&pixel, 3, 1, archivoBlanconegro);
       }
      }

    fclose(archivoBlanconegro);

    return TODO_OK;
}

int aumentarContraste(FILE* archivoEntrada)
{
    FILE * archivoAumentarcontraste;
    METADATAgral metadata;
    int i, j;

    archivoAumentarcontraste = fopen("virus_aumentar_contraste.bmp","wb");
    if(!archivoAumentarcontraste)
    {
        printf("Error al abrir el archivo de salida.");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    metadata = cargaCabecera(archivoEntrada,archivoAumentarcontraste);

    fseek(archivoAumentarcontraste,metadata.comienzoImagen,SEEK_SET);
    fseek(archivoEntrada,metadata.comienzoImagen,SEEK_SET);
    t_pixel pixel;

    for(i=0;i<metadata.alto;i++)
      {
        for(j=0;j<metadata.ancho;j++)
        {
            fread(&pixel, sizeof(unsigned char), 3, archivoEntrada);

            prom2 = ( pixel.pixel[0] + pixel.pixel[1] + pixel.pixel[2] ) / 3;

            if(prom2<127)
            {
                    pixel.pixel[0] = max(0, pixel.pixel[0] * 0.75);
                    pixel.pixel[1] = max(0, pixel.pixel[1] * 0.75);
                    pixel.pixel[2] = max(0, pixel.pixel[2] * 0.75);
            }else
                 {
                        pixel.pixel[0] = min(255,max(127, pixel.pixel[0] * 1.25));
                        pixel.pixel[1] = min(255,max(127, pixel.pixel[1] * 1.25));
                        pixel.pixel[2] = min(255,max(127, pixel.pixel[2] * 1.25));
                 }
            // Escribir el p�xel modificado en el archivo de salida
            fwrite(&pixel, sizeof(unsigned char), 3, archivoAumentarcontraste);
         }
       }

    fclose(archivoAumentarcontraste);

    return TODO_OK;
}

int reducirContraste(FILE* archivoEntrada)
{
    FILE * archivoReducircontraste;
    METADATAgral metadata;
    int i, j;

    archivoReducircontraste = fopen("virus_reducir_contraste.bmp","wb");
    if(!archivoReducircontraste)
    {
        printf("Error al abrir el archivo de salida.");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    metadata = cargaCabecera(archivoEntrada,archivoReducircontraste);

    fseek(archivoReducircontraste,metadata.comienzoImagen,SEEK_SET);
    fseek(archivoEntrada,metadata.comienzoImagen,SEEK_SET);
    // modifico los pixeles
    t_pixel pixel;

    for(i=0;i<metadata.alto;i++)
      {
        for(j=0;j<metadata.ancho;j++)
        {
            fread(&pixel, sizeof(unsigned char), 3, archivoEntrada);

            prom2 = ( pixel.pixel[0] + pixel.pixel[1] + pixel.pixel[2] ) / 3;

            if(prom2>127)
            {
              pixel.pixel[0] = max(127, pixel.pixel[0] * 0.75);
              pixel.pixel[1] = max(127, pixel.pixel[1] * 0.75);
              pixel.pixel[2] = max(127, pixel.pixel[2] * 0.75);

            }else
                 {
                    pixel.pixel[0] = min(127, pixel.pixel[0] * 1.25);
                    pixel.pixel[1] = min(127, pixel.pixel[1] * 1.25);
                    pixel.pixel[2] = min(127, pixel.pixel[2] * 1.25);
                 }
            // Escribir el p�xel modificado en el archivo de salida
            fwrite(&pixel, sizeof(unsigned char), 3, archivoReducircontraste);
  }
       }

    fclose(archivoReducircontraste);

    return TODO_OK;
}

int recortarImagen(FILE* archivoEntrada)
{
    FILE * archivoRecortar;
    METADATAgral metadata;
    int i, j, anchorecorte, altorecorte,nuevoTamImagen,tamlinea;

    archivoRecortar = fopen("virus_recotar_imagen.bmp","wb");
    if(!archivoRecortar)
    {
        printf("Error al abrir el archivo de salida.");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    metadata = cargaCabecera(archivoEntrada,archivoRecortar);
    // modifico los pixeles
    t_pixel pixel;

    anchorecorte = metadata.ancho / 2;
    altorecorte = metadata.alto / 2;

    nuevoTamImagen = (anchorecorte * altorecorte * metadata.tampuntos) / 8;

    fseek(archivoRecortar,18,SEEK_SET);
    fwrite(&anchorecorte,sizeof(unsigned int),1,archivoRecortar);

    fseek(archivoRecortar,22,SEEK_SET);
    fwrite(&altorecorte,sizeof(unsigned int),1,archivoRecortar);

    fseek(archivoRecortar,34,SEEK_SET);
    fwrite(&nuevoTamImagen,sizeof(unsigned int),1,archivoRecortar);

    fseek(archivoRecortar,metadata.comienzoImagen,SEEK_SET);
    fseek(archivoEntrada,metadata.comienzoImagen,SEEK_SET);

    for(i=0;i<altorecorte;i++)
      {
        for(j=0;j<anchorecorte;j++)
        {
          fread(&pixel, sizeof(unsigned char), 3, archivoEntrada);

          fwrite(&pixel, sizeof(unsigned char), 3, archivoRecortar);
          tamlinea = metadata.ancho * 3;
       }
       fseek(archivoEntrada,tamlinea - (anchorecorte * 3),SEEK_CUR);
      }

    fclose(archivoRecortar);

    return TODO_OK;
}

int rotar90gradosDerecha(FILE* archivoEntrada)
{
    FILE * archivoRotarimagenderecha;
    METADATAgral metadata;
    int i, j, altoviejo, anchoviejo;

    archivoRotarimagenderecha = fopen("virus_rotar_imagen_derecha.bmp","wb");
    if(!archivoRotarimagenderecha)
    {
        printf("Error al abrir el archivo de salida.");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    metadata = cargaCabecera(archivoEntrada, archivoRotarimagenderecha);

    altoviejo = metadata.alto;
    anchoviejo = metadata.ancho;

    fseek(archivoRotarimagenderecha, 18, SEEK_SET);
    fwrite(&altoviejo, sizeof(int), 1, archivoRotarimagenderecha);

    fseek(archivoRotarimagenderecha, 22, SEEK_SET);
    fwrite(&anchoviejo, sizeof(int), 1, archivoRotarimagenderecha);

    pixeles matrizPixel[anchoviejo][altoviejo];
    pixeles copiamatrizPixel[altoviejo][anchoviejo];

    fseek(archivoEntrada,metadata.comienzoImagen, SEEK_SET);

    for(i = 0; i < altoviejo; i++)
    {
        for(j = 0; j < anchoviejo; j++)
        {
            fread(&copiamatrizPixel[i][j].pixel[0], sizeof(unsigned char), 1, archivoEntrada);
            fread(&copiamatrizPixel[i][j].pixel[1], sizeof(unsigned char), 1, archivoEntrada);
            fread(&copiamatrizPixel[i][j].pixel[2], sizeof(unsigned char), 1, archivoEntrada);
        }
    }

    fseek(archivoRotarimagenderecha,metadata.comienzoImagen, SEEK_SET);
    fseek(archivoEntrada,metadata.comienzoImagen, SEEK_SET);


    for(i = anchoviejo-1 ; i >= 0; i--)
    {
        for(j = 0 ; j <altoviejo; j++) // SE LE RESTA CONTANDO EL BYTE 0
        {
            matrizPixel[i][j].pixel[0] = copiamatrizPixel[j][i].pixel[0];
            matrizPixel[i][j].pixel[1] = copiamatrizPixel[j][i].pixel[1];
            matrizPixel[i][j].pixel[2] = copiamatrizPixel[j][i].pixel[2];

            fwrite(&matrizPixel[i][j], sizeof(unsigned char), 3, archivoRotarimagenderecha);
        }
    }

    fclose(archivoRotarimagenderecha);
    return TODO_OK;
}

int rotar90gradosIzquierda(FILE* archivoEntrada)
{
    FILE * archivoRotarizquierda;
    METADATAgral metadata;
    int i, j, altoviejo, anchoviejo;

    archivoRotarizquierda = fopen("virus_rotar_imagen_izquierda.bmp","wb");
    if(!archivoRotarizquierda)
    {
        printf("Error al abrir el archivo de salida.");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    metadata = cargaCabecera(archivoEntrada, archivoRotarizquierda);

    altoviejo = metadata.alto;
    anchoviejo = metadata.ancho;

    fseek(archivoRotarizquierda, 18, SEEK_SET);
    fwrite(&altoviejo, sizeof(int), 1, archivoRotarizquierda);

    fseek(archivoRotarizquierda, 22, SEEK_SET);
    fwrite(&anchoviejo, sizeof(int), 1, archivoRotarizquierda);

    pixeles matrizPixel[anchoviejo][altoviejo];
    pixeles copiamatrizPixel[altoviejo][anchoviejo];

    fseek(archivoEntrada,metadata.comienzoImagen, SEEK_SET);

    for(i = 0; i < altoviejo; i++)
    {
        for(j = 0; j < anchoviejo; j++)
        {
            fread(&copiamatrizPixel[i][j].pixel[0], sizeof(unsigned char), 1, archivoEntrada);
            fread(&copiamatrizPixel[i][j].pixel[1], sizeof(unsigned char), 1, archivoEntrada);
            fread(&copiamatrizPixel[i][j].pixel[2], sizeof(unsigned char), 1, archivoEntrada);
        }
    }

    fseek(archivoRotarizquierda,metadata.comienzoImagen, SEEK_SET);
    fseek(archivoEntrada,metadata.comienzoImagen, SEEK_SET);


    for(i = anchoviejo-1 ; i >= 0; i--)
    {
        for(j = altoviejo-1 ; j >= 0; j--) // SE LE RESTA CONTANDO EL BYTE 0
        {
            matrizPixel[i][j].pixel[0] = copiamatrizPixel[j][i].pixel[0];
            matrizPixel[i][j].pixel[1] = copiamatrizPixel[j][i].pixel[1];
            matrizPixel[i][j].pixel[2] = copiamatrizPixel[j][i].pixel[2];

            fwrite(&matrizPixel[i][j], sizeof(unsigned char), 3, archivoRotarizquierda);
        }
    }

    fclose(archivoRotarizquierda);
    return TODO_OK;
}

int comodin(FILE* archivoentrada)
{
    FILE * archivocomodin;
    METADATAgral metadata;
    int i, j;

    archivocomodin = fopen("virus_comodin.bmp","wb");
    if(!archivocomodin)
    {
        printf("Error al abrir el archivo de salida.");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    metadata = cargaCabecera(archivoentrada, archivocomodin);

    pixeles matrizpixel[metadata.alto][metadata.ancho];
    pixeles copiamatrizpixel[metadata.alto][metadata.ancho];

    fseek(archivoentrada,metadata.comienzoImagen, SEEK_SET);

    for(i = 0; i < metadata.alto; i++)
    {
        for(j = 0; j < metadata.ancho; j++)
        {
            fread(&copiamatrizpixel[i][j].pixel[0], sizeof(unsigned char), 1, archivoentrada);
            fread(&copiamatrizpixel[i][j].pixel[1], sizeof(unsigned char), 1, archivoentrada);
            fread(&copiamatrizpixel[i][j].pixel[2], sizeof(unsigned char), 1, archivoentrada);
        }
    }

    fseek(archivocomodin,metadata.comienzoImagen, SEEK_SET);
    fseek(archivoentrada,metadata.comienzoImagen, SEEK_SET);

    for(i = metadata.alto -1; i >= 0; i--)
    {
        for(j = metadata.ancho -1; j >= 0; j--) // SE LE RESTA CONTANDO EL BYTE 0
        {
            matrizpixel[i][j].pixel[0] = copiamatrizpixel[i][j].pixel[0];
            matrizpixel[i][j].pixel[1] = copiamatrizpixel[i][j].pixel[1];
            matrizpixel[i][j].pixel[2] = copiamatrizpixel[i][j].pixel[2];

            fwrite(&matrizpixel[i][j], sizeof(unsigned char), 3, archivocomodin);
        }
    }

    fclose(archivocomodin);
    return TODO_OK;
}
