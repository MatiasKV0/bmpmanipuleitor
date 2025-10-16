#include "funciones_villan.h"

bool leerHeader(FILE* archivo, HEADER* header)
{
    if (archivo == NULL || header == NULL)
        return false;

    size_t leidos = 0;
    leidos += fread(&header->tipobm, sizeof(unsigned short), 1, archivo);
    leidos += fread(&header->tamArchivo, sizeof(unsigned int), 1, archivo);
    leidos += fread(&header->reserva, sizeof(unsigned short), 1, archivo);
    leidos += fread(&header->reserva1, sizeof(unsigned short), 1, archivo);
    leidos += fread(&header->comienzoImagen, sizeof(unsigned int), 1, archivo);
    leidos += fread(&header->tamEncabezado, sizeof(unsigned int), 1, archivo);
    leidos += fread(&header->ancho, sizeof(unsigned int), 1, archivo);
    leidos += fread(&header->alto, sizeof(unsigned int), 1, archivo);
    leidos += fread(&header->planos, sizeof(unsigned short), 1, archivo);
    leidos += fread(&header->tampuntos, sizeof(unsigned short), 1, archivo);
    leidos += fread(&header->compresion, sizeof(unsigned int), 1, archivo);
    leidos += fread(&header->tamimagen, sizeof(unsigned int), 1, archivo);
    leidos += fread(&header->resolucionhor, sizeof(unsigned int), 1, archivo);
    leidos += fread(&header->resolucionvert, sizeof(unsigned int), 1, archivo);
    leidos += fread(&header->tampaletacolores, sizeof(unsigned int), 1, archivo);
    leidos += fread(&header->contcolor, sizeof(unsigned int), 1, archivo);

    rewind(archivo);

    if (leidos != 16)
        return false;

    return true;
}

bool cargarHeader(FILE* archivo, HEADER* header)
{
    if (archivo == NULL || header == NULL)
        return false;

    size_t escritos = 0;
    escritos += fwrite(&header->tipobm, sizeof(unsigned short), 1, archivo);
    escritos += fwrite(&header->tamArchivo, sizeof(unsigned int), 1, archivo);
    escritos += fwrite(&header->reserva, sizeof(unsigned short), 1, archivo);
    escritos += fwrite(&header->reserva1, sizeof(unsigned short), 1, archivo);
    escritos += fwrite(&header->comienzoImagen, sizeof(unsigned int), 1, archivo);
    escritos += fwrite(&header->tamEncabezado, sizeof(unsigned int), 1, archivo);
    escritos += fwrite(&header->ancho, sizeof(unsigned int), 1, archivo);
    escritos += fwrite(&header->alto, sizeof(unsigned int), 1, archivo);
    escritos += fwrite(&header->planos, sizeof(unsigned short), 1, archivo);
    escritos += fwrite(&header->tampuntos, sizeof(unsigned short), 1, archivo);
    escritos += fwrite(&header->compresion, sizeof(unsigned int), 1, archivo);
    escritos += fwrite(&header->tamimagen, sizeof(unsigned int), 1, archivo);
    escritos += fwrite(&header->resolucionhor, sizeof(unsigned int), 1, archivo);
    escritos += fwrite(&header->resolucionvert, sizeof(unsigned int), 1, archivo);
    escritos += fwrite(&header->tampaletacolores, sizeof(unsigned int), 1, archivo);
    escritos += fwrite(&header->contcolor, sizeof(unsigned int), 1, archivo);

    rewind(archivo);

    if (escritos != 16)
        return false;

    return true;
}

int verificarArchivo(FILE* arch, HEADER* header, bool imprimir)
{
    unsigned char firma[2];
    size_t leidos = fread(firma, sizeof(unsigned char), 2, arch);
    if (leidos < 2) {
        printf("[ERROR]: Archivo vacío o no válido.\n");
        fclose(arch);
        return ERROR_BMP;
    }

    if (!(firma[0] == 'B' && firma[1] == 'M')) {
        printf("[ERROR]: Firma BMP inválida.\n");
        fclose(arch);
        return ERROR_BMP;
    }

    if(imprimir) printf("Signature BMP valido\n");

    if (header->tampuntos != 24) {
        printf("[ERROR]: Profundidad de color incorrecta (%d bits, esperado 24 bits)\n", header->tampuntos);
        fclose(arch);
        return ERROR_BMP;
    }

    if(imprimir) printf("Profundidad de 24 bits confirmada\n");

    if (header->compresion != 0) {
        printf("[ERROR]: Compresión detectada (%u)\n", header->compresion);
        fclose(arch);
        return ERROR_BMP;
    }

    if(imprimir) printf("Compresion: No comprimido\n");

    if (header->ancho <= 0 || header->alto <= 0) {
        printf("[ERROR]: Dimensiones no válidas (%dx%d)\n", header->ancho, header->alto);
        fclose(arch);
        return ERROR_BMP;
    }

    if(imprimir) printf("ARCHIVO VALIDO - Listo para procesar\n");

    rewind(arch);
    return EXITO;
}

int tonalidadRojo(HEADER* header, FILE* archivo, float porcentaje, char* nomArch)
{

    char nomArchSalida[100] = "virus_tonalidad-roja-";
    char numero[20];

    sprintf(numero, "%.0f_", porcentaje);
    strcat(nomArchSalida, numero);
    strcat(nomArchSalida, nomArch);

    FILE *archivoRojo = fopen(nomArchSalida, "wb");
    if (!archivoRojo)
    {
        printf("Error al abrir el archivo de salida.\n");
        return ERROR_ARCH;
    }

    if (!cargarHeader(archivoRojo, header))
    {
        fclose(archivoRojo);
        return ERROR_ARCH;
    }

    fseek(archivo, header->comienzoImagen, SEEK_SET);
    fseek(archivoRojo, header->comienzoImagen, SEEK_SET);

    for (unsigned int i = 0; i < header->alto; i++)
    {
        for (unsigned int j = 0; j < header->ancho; j++)
        {
            t_pixel pixel;
            fread(&pixel, 3, 1, archivo);

            pixel.pixel[2] = (unsigned char) (
                (pixel.pixel[2] + (porcentaje / 100) * pixel.pixel[2]) > 255
                ? 255
                : pixel.pixel[2] + (porcentaje / 100) * pixel.pixel[2]
            );

            fwrite(&pixel, 3, 1, archivoRojo);
        }
    }

    fclose(archivoRojo);
    return EXITO;
}

int espejarHorizontal(HEADER* header, FILE* archivo, char*nomArch){

    char nomArchSalida[100] = "virus_espejar-horizontal-";
    strcat(nomArchSalida, nomArch);

    FILE *archivoEspejo = fopen(nomArchSalida, "wb");
    if (!archivoEspejo)
    {
        printf("Error al abrir el archivo de salida.\n");
        return ERROR_ARCH;
    }

    if (!cargarHeader(archivoEspejo, header))
    {
        fclose(archivoEspejo);
        return ERROR_ARCH;
    }

    fseek(archivo, header->comienzoImagen, SEEK_SET);
    fseek(archivoEspejo, header->comienzoImagen, SEEK_SET);

    unsigned char paddingBytes[3] = {0, 0, 0};
    int padding = (4 - (header->ancho * 3) % 4) % 4;
    t_pixel *fila = malloc(sizeof(t_pixel)*header->ancho);
     if (!fila) {
        fclose(archivoEspejo);
        return ERROR_MEM;
    }

    for (unsigned int i = 0; i < header->alto; i++) {
        fread(fila, sizeof(t_pixel), header->ancho, archivo);
        fread(paddingBytes, 1, padding, archivo); // Saltar padding original

        // Escribir fila invertida
        for (int j = header->ancho - 1; j >= 0; j--) {
            fwrite(&fila[j], sizeof(t_pixel), 1, archivoEspejo);
        }
        fwrite(paddingBytes, 1, padding, archivoEspejo); // Mantener padding
    }


    free(fila);
    fclose(archivoEspejo);
    return EXITO;
}

int espejarVertical(HEADER* header, FILE* archivo, char*nomArch){

    char nomArchSalida[100] = "virus_espejar-vertical-";
    strcat(nomArchSalida, nomArch);

    FILE *archivoEspejo = fopen(nomArchSalida, "wb");
    if (!archivoEspejo)
    {
        printf("Error al abrir el archivo de salida.\n");
        return ERROR_ARCH;
    }

    if (!cargarHeader(archivoEspejo, header))
    {
        fclose(archivoEspejo);
        return ERROR_ARCH;
    }

    fseek(archivo, header->comienzoImagen, SEEK_SET);
    fseek(archivoEspejo, header->comienzoImagen, SEEK_SET);

    unsigned char paddingBytes[3] = {0, 0, 0};
    int padding = (4 - (header->alto * 3) % 4) % 4;
    t_pixel *fila = malloc(sizeof(t_pixel)*header->alto);
     if (!fila) {
        fclose(archivoEspejo);
        return ERROR_MEM;
    }

    for (unsigned int i = 0; i < header->ancho; i++) {
        fread(fila, sizeof(t_pixel), header->alto, archivo);
        fread(paddingBytes, 1, padding, archivo); // Saltar padding original

        // Escribir fila invertida
        for (int j = header->alto - 1; j >= 0; j--) {
            fwrite(&fila[j], sizeof(t_pixel), 1, archivoEspejo);
        }
        fwrite(paddingBytes, 1, padding, archivoEspejo); // Mantener padding
    }


    free(fila);
    fclose(archivoEspejo);
    return EXITO;
}
