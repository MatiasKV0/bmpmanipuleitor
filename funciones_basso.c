#include "funciones_basso.h"

int tonalidadColumnasSecuencial(HEADER* header, FILE* archivo, float porcentaje, char* nomArch)
{
    char nomArchSalida[128] = "VIRUS_tonalidad-columnas-";
    char numero[20];

    sprintf(numero, "%.0f_", porcentaje);
    strcat(nomArchSalida, numero);
    strcat(nomArchSalida, nomArch);

    FILE *archivoSalida = fopen(nomArchSalida, "wb");
    if (!archivoSalida)
    {
        printf("Error al abrir el archivo de salida.\n");
        return ERROR_ARCH;
    }

    if (!cargarHeader(archivoSalida, header))
    {
        fclose(archivoSalida);
        return ERROR_ARCH;
    }

    fseek(archivo, header->comienzoImagen, SEEK_SET);
    fseek(archivoSalida, header->comienzoImagen, SEEK_SET);

    // 🔹 Calcular columnas y asegurar mínimo 1
    int cantColumnas = (int)(porcentaje + 0.5f);
    if (cantColumnas < 1)
        cantColumnas = 1;

    float anchoPorColumna = header->ancho / (float)cantColumnas; // división REAL
    int flagTonalidad = 0; // 0=roja, 1=verde, 2=azul

    for (unsigned int i = 0; i < header->alto; i++)
    {
        flagTonalidad = 0;
        for (unsigned int j = 0; j < header->ancho; j++)
        {
            t_pixel pixel;
            fread(&pixel, 3, 1, archivo);

            // 🔸 Calcular índice de columna según posición horizontal
            int indiceColumna = (int)(j / anchoPorColumna);

            // Repetir patrón de color cada 3 columnas
            flagTonalidad = indiceColumna % 3;

            unsigned char b = pixel.pixel[0];
            unsigned char g = pixel.pixel[1];
            unsigned char r = pixel.pixel[2];

            // 🔸 Aumentar intensidad del color correspondiente
            switch (flagTonalidad)
            {
                case 0: // rojo
                    r = (unsigned char)((r * 1.5f) > 255 ? 255 : r * 1.5f);
                    break;
                case 1: // verde
                    g = (unsigned char)((g * 1.5f) > 255 ? 255 : g * 1.5f);
                    break;
                case 2: // azul
                    b = (unsigned char)((b * 1.5f) > 255 ? 255 : b * 1.5f);
                    break;
            }

            pixel.pixel[0] = b;
            pixel.pixel[1] = g;
            pixel.pixel[2] = r;

            fwrite(&pixel, 3, 1, archivoSalida);
        }

        // Manejo del padding (alineación de filas)
        int padding = (4 - (header->ancho * 3) % 4) % 4;
        unsigned char pad[3] = {0, 0, 0};
        fread(pad, 1, padding, archivo);
        fwrite(pad, 1, padding, archivoSalida);
    }

    fclose(archivoSalida);
    return EXITO;
}

void manejarPadding(HEADER* header, FILE* archivoEntrada, FILE* archivoSalida)
{
    const int BYTES_POR_PIXEL = 3;
    int rowBytes = header->ancho * BYTES_POR_PIXEL;
    int padding = (4 - (rowBytes % 4)) % 4;
    unsigned char padZeros[3] = {0, 0, 0};

    if (padding)
    {
        fseek(archivoEntrada, padding, SEEK_CUR);
        fwrite(padZeros, 1, padding, archivoSalida);
    }
}


int limitar_rango_bmp(int valor)
{
    if (valor < 0) return 0;
    if (valor > 255) return 255;
    return valor;
}

