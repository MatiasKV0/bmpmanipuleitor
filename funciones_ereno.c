#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "funciones_ereno.h"
//--------------------- FUNCIONES ---------------------

// FUNCIONES: Generales.
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

// FUNCIONES: Filtros con Parametros (0-100%)
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

int tonalidadAzul(HEADER* header, FILE* archivo, float porcentaje, char* nomArch)
{

    char nomArchSalida[100] = "virus_tonalidad-azul-";
    char numero[20];

    sprintf(numero, "%.0f_", porcentaje);
    strcat(nomArchSalida, numero);
    strcat(nomArchSalida, nomArch);

    FILE *archivoAzul = fopen(nomArchSalida, "wb");
    if (!archivoAzul)
    {
        printf("Error al abrir el archivo de salida.\n");
        return ERROR_ARCH;
    }

    if (!cargarHeader(archivoAzul, header))
    {
        fclose(archivoAzul);
        return ERROR_ARCH;
    }

    fseek(archivo, header->comienzoImagen, SEEK_SET);
    fseek(archivoAzul, header->comienzoImagen, SEEK_SET);

    for (unsigned int i = 0; i < header->alto; i++)
    {
        for (unsigned int j = 0; j < header->ancho; j++)
        {
            t_pixel pixel;
            fread(&pixel, 3, 1, archivo);

            pixel.pixel[0] = (unsigned char) (
                (pixel.pixel[0] + (porcentaje / 100) * pixel.pixel[0]) > 255
                ? 255
                : pixel.pixel[0] + (porcentaje / 100) * pixel.pixel[0]
            );

            fwrite(&pixel, 3, 1, archivoAzul);
        }
    }

    fclose(archivoAzul);
    return EXITO;
}

int tonalidadVerde(HEADER* header, FILE* archivo, float porcentaje, char* nomArch)
{

    char nomArchSalida[100] = "virus_tonalidad-verde-";
    char numero[20];

    sprintf(numero, "%.0f_", porcentaje);
    strcat(nomArchSalida, numero);
    strcat(nomArchSalida, nomArch);

    FILE *archivoVerde = fopen(nomArchSalida, "wb");
    if (!archivoVerde)
    {
        printf("Error al abrir el archivo de salida.\n");
        return ERROR_ARCH;
    }

    if (!cargarHeader(archivoVerde, header))
    {
        fclose(archivoVerde);
        return ERROR_ARCH;
    }

    fseek(archivo, header->comienzoImagen, SEEK_SET);
    fseek(archivoVerde, header->comienzoImagen, SEEK_SET);

    for (unsigned int i = 0; i < header->alto; i++)
    {
        for (unsigned int j = 0; j < header->ancho; j++)
        {
            t_pixel pixel;
            fread(&pixel, 3, 1, archivo);

            pixel.pixel[1] = (unsigned char) (
                (pixel.pixel[1] + (porcentaje / 100) * pixel.pixel[1]) > 255
                ? 255
                : pixel.pixel[1] + (porcentaje / 100) * pixel.pixel[1]
            );

            fwrite(&pixel, 3, 1, archivoVerde);
        }
    }

    fclose(archivoVerde);
    return EXITO;
}

int recortePorcentaje(HEADER* header, FILE* archivo, float porcentaje, char* nomArch)
{

    char nomArchSalida[128] = "virus_recorte-";
    char numero[20];
    sprintf(numero, "%.0f_", porcentaje);
    strcat(nomArchSalida, numero);
    strcat(nomArchSalida, nomArch);

    FILE* archivoRecorte = fopen(nomArchSalida, "wb");
    if (!archivoRecorte) {
        printf("Error al abrir el archivo de salida.\n");
        return ERROR_ARCH;
    }

    // Copio headers originales al archivo de salida
    if (!cargarHeader(archivoRecorte, header)) {
        fclose(archivoRecorte);
        return ERROR_ARCH;
    }

    // Asumimos BMP 24b sin compresión (3 bytes por píxel)
    const int bytesPorPixel = 3;

    // Nuevo tamaño (porcentaje aplicado a ambas dimensiones)
    unsigned int nuevoAncho = (unsigned int)(header->ancho * (porcentaje / 100.0f));
    unsigned int nuevoAlto  = (unsigned int)(header->alto  * (porcentaje / 100.0f));
    if (nuevoAncho == 0) nuevoAncho = 1;
    if (nuevoAlto  == 0) nuevoAlto  = 1;

    // Row sizes y paddings
    int rowSizeOrigNoPad = (int)header->ancho * bytesPorPixel;
    int paddingOrig      = (4 - (rowSizeOrigNoPad % 4)) % 4;
    int rowSizeOrigPad   = rowSizeOrigNoPad + paddingOrig;

    int rowSizeNewNoPad  = (int)nuevoAncho * bytesPorPixel;
    int paddingNew       = (4 - (rowSizeNewNoPad % 4)) % 4;
    int rowSizeNewPad    = rowSizeNewNoPad + paddingNew;

    // biSizeImage nuevo
    unsigned int biSizeImage = (unsigned int)rowSizeNewPad * (unsigned int)nuevoAlto;

    // Actualizo campos del header de salida
    fseek(archivoRecorte, 18, SEEK_SET); fwrite(&nuevoAncho,  sizeof(unsigned int), 1, archivoRecorte);
    fseek(archivoRecorte, 22, SEEK_SET); fwrite(&nuevoAlto,   sizeof(unsigned int), 1, archivoRecorte);
    fseek(archivoRecorte, 34, SEEK_SET); fwrite(&biSizeImage, sizeof(unsigned int), 1, archivoRecorte);

    // Posiciono datos
    fseek(archivo, header->comienzoImagen, SEEK_SET);
    fseek(archivoRecorte, header->comienzoImagen, SEEK_SET);

    // Escribir filas recortadas desde el “origen”
    unsigned char padZeros[3] = {0,0,0};
    t_pixel pixel;

    for (unsigned int i = 0; i < nuevoAlto; i++) {
        // Escribo sólo los primeros nuevoAncho píxeles de la fila original
        for (unsigned int j = 0; j < nuevoAncho; j++) {
            fread(&pixel, 1, 3, archivo);
            fwrite(&pixel, 1, 3, archivoRecorte);
        }

        // Salto el resto de la fila original (píxeles descartados + padding original)
        long saltar = (long)(rowSizeOrigPad - (int)(nuevoAncho * bytesPorPixel));
        fseek(archivo, saltar, SEEK_CUR);

        // Padding de la fila nueva
        if (paddingNew) fwrite(padZeros, 1, paddingNew, archivoRecorte);
    }

    fclose(archivoRecorte);
    return EXITO;
}

int aumentarContraste(HEADER* header, FILE* archivo, float porcentaje, char* nomArch)
{

    char nomArchSalida[128] = "virus_aumentar-contraste-";
    char numero[20];

    sprintf(numero, "%.0f_", porcentaje);
    strcat(nomArchSalida, numero);
    strcat(nomArchSalida, nomArch);

    FILE *archivoContraste = fopen(nomArchSalida, "wb");
    if (!archivoContraste)
    {
        printf("Error al abrir el archivo de salida.\n");
        return ERROR_ARCH;
    }

    if (!cargarHeader(archivoContraste, header))
    {
        fclose(archivoContraste);
        return ERROR_ARCH;
    }

    fseek(archivo, header->comienzoImagen, SEEK_SET);
    fseek(archivoContraste, header->comienzoImagen, SEEK_SET);

    t_pixel pixel;

    float factor = 1.0f + (porcentaje / 100.0f);

    for (unsigned int i = 0; i < header->alto; i++)
    {
        for (unsigned int j = 0; j < header->ancho; j++)
        {
            fread(&pixel, 3, 1, archivo);

            // Aumentar contraste canal a canal respecto a 128
            for (int c = 0; c < 3; c++)
            {
                float v  = (float)pixel.pixel[c];
                float nv = (v - 128.0f) * factor + 128.0f;

                if (nv < 0.0f)   nv = 0.0f;
                if (nv > 255.0f) nv = 255.0f;

                pixel.pixel[c] = (unsigned char)(nv);
            }

            fwrite(&pixel, 3, 1, archivoContraste);
        }
    }

    fclose(archivoContraste);
    return EXITO;
}

int reducirContraste(HEADER* header, FILE* archivo, float porcentaje, char* nomArch)
{

    if (porcentaje < 0.0f) porcentaje = 0.0f;
    if (porcentaje > 100.0f) porcentaje = 100.0f;
    const float MIN_FACTOR = 0.05f;
    float factor = 1.0f - (porcentaje / 100.0f);
    if (factor < MIN_FACTOR) factor = MIN_FACTOR;

    char nomArchSalida[128] = "virus_reducir-contraste-";
    char numero[20];
    sprintf(numero, "%.0f_", porcentaje);
    strcat(nomArchSalida, numero);
    strcat(nomArchSalida, nomArch);

    FILE *archivoReducido = fopen(nomArchSalida, "wb");
    if (!archivoReducido) {
        printf("Error al abrir el archivo de salida.\n");
        return ERROR_ARCH;
    }

    if (!cargarHeader(archivoReducido, header)) {
        fclose(archivoReducido);
        return ERROR_ARCH;
    }

    // Posicionar al inicio de datos
    fseek(archivo, header->comienzoImagen, SEEK_SET);
    fseek(archivoReducido, header->comienzoImagen, SEEK_SET);

    // --- Padding por fila (24 bpp → 3 bytes/pixel; cada fila múltiplo de 4) ---
    const int bytesPorPixel = 3;
    int rowSizeNoPad = (int)header->ancho * bytesPorPixel;
    int padding      = (4 - (rowSizeNoPad % 4)) % 4;
    unsigned char padZeros[3] = {0,0,0};

    t_pixel pixel;

    for (unsigned int i = 0; i < header->alto; i++) {
        for (unsigned int j = 0; j < header->ancho; j++) {
            fread(&pixel, 1, 3, archivo);

            // Reducir contraste respecto al gris medio 128
            for (int c = 0; c < 3; c++) {
                float v  = (float)pixel.pixel[c];
                float nv = (v - 128.0f) * factor + 128.0f;

                if (nv < 0.0f)   nv = 0.0f;
                if (nv > 255.0f) nv = 255.0f;
                pixel.pixel[c] = (unsigned char)nv;
            }

            fwrite(&pixel, 1, 3, archivoReducido);
        }

        // Saltar padding de la fila original y escribir padding en la nueva
        if (padding) fseek(archivo, padding, SEEK_CUR);
        if (padding) fwrite(padZeros, 1, padding, archivoReducido);
    }

    fclose(archivoReducido);
    return EXITO;
}

int achicarImagen(HEADER* header, FILE* archivo, float porcentaje, char* nomArch)
{
    const int BPP = 3; // 24 bpp (B,G,R)

    // Nombre de salida
    char nomArchSalida[128] = "virus_achicar-";
    char numero[20];
    sprintf(numero, "%.0f_", porcentaje);
    strcat(nomArchSalida, numero);
    strcat(nomArchSalida, nomArch);

    FILE* archivoOut = fopen(nomArchSalida, "wb");
    if (!archivoOut) { printf("Error al abrir el archivo de salida.\n"); return ERROR_ARCH; }

    // Copiar headers base
    if (!cargarHeader(archivoOut, header)) { fclose(archivoOut); return ERROR_ARCH; }

    // Dimensiones nuevas
    unsigned int nuevoAncho = (unsigned int)(header->ancho * (porcentaje / 100.0f));
    unsigned int nuevoAlto  = (unsigned int)(header->alto  * (porcentaje / 100.0f));
    if (nuevoAncho == 0) nuevoAncho = 1;
    if (nuevoAlto  == 0) nuevoAlto  = 1;

    // *** SIN PADDING ***
    int rowOrig = (int)header->ancho * BPP;     // sin relleno
    int rowNew  = (int)nuevoAncho   * BPP;      // sin relleno

    unsigned int bfOffBits  = header->comienzoImagen;  // típicamente 54
    unsigned int biSizeImage = (unsigned int)rowNew * (unsigned int)nuevoAlto; // sin relleno
    unsigned int bfSize      = bfOffBits + biSizeImage;

    // Actualizar cabecera en archivo destino (offsets estándar BMP v3)
    fseek(archivoOut, 18, SEEK_SET); fwrite(&nuevoAncho,   sizeof(unsigned int), 1, archivoOut);
    fseek(archivoOut, 22, SEEK_SET); fwrite(&nuevoAlto,    sizeof(unsigned int), 1, archivoOut);
    fseek(archivoOut, 34, SEEK_SET); fwrite(&biSizeImage,  sizeof(unsigned int), 1, archivoOut);
    fseek(archivoOut,  2, SEEK_SET); fwrite(&bfSize,       sizeof(unsigned int), 1, archivoOut);

    // Posicionar datos
    fseek(archivo,    bfOffBits, SEEK_SET);
    fseek(archivoOut, bfOffBits, SEEK_SET);

    // Buffer para una fila fuente (sin relleno)
    unsigned char* srcRow = (unsigned char*)malloc((size_t)rowOrig);
    if (!srcRow) { fclose(archivoOut); return false; }

    // Reescalado (vecino más cercano) SIN padding en lectura/escritura
    for (unsigned int yOut = 0; yOut < nuevoAlto; yOut++) {
        unsigned int ySrc = (unsigned int)(((unsigned long long)yOut * header->alto) / nuevoAlto);
        if (ySrc >= header->alto) ySrc = header->alto - 1;

        // Seek absoluto a la fila fuente y leerla completa (sin relleno)
        long offFila = (long)bfOffBits + (long)ySrc * (long)rowOrig;
        fseek(archivo, offFila, SEEK_SET);
        fread(srcRow, 1, (size_t)rowOrig, archivo);

        // Escribir fila destino
        for (unsigned int xOut = 0; xOut < nuevoAncho; xOut++) {
            unsigned int xSrc = (unsigned int)(((unsigned long long)xOut * header->ancho) / nuevoAncho);
            if (xSrc >= header->ancho) xSrc = header->ancho - 1;

            size_t idx = (size_t)xSrc * BPP;
            fwrite(&srcRow[idx], 1, BPP, archivoOut);
        }
    }

    free(srcRow);
    fclose(archivoOut);
    return EXITO;
}

// FUNCIONES: Filtros Basicos.

int convertirNegativo(HEADER* header, FILE* archivo, char* nomArch)
{
    const int BYTES_POR_PIXEL = 3; // 24 bpp (B,G,R)

    // Nombre de salida: virus_negativo-<nomArch>
    char nomArchSalida[128] = "virus_negativo-";
    strcat(nomArchSalida, nomArch);

    FILE* archivoNegativo = fopen(nomArchSalida, "wb");
    if (!archivoNegativo) {
        printf("Error al abrir el archivo de salida.\n");
        return ERROR_ARCH;
    }

    // Copiar headers al archivo de salida
    if (!cargarHeader(archivoNegativo, header)) {
        fclose(archivoNegativo);
        return ERROR_ARCH;
    }

    // Posicionar al inicio de los datos en ambos archivos
    fseek(archivo,          header->comienzoImagen, SEEK_SET);
    fseek(archivoNegativo,  header->comienzoImagen, SEEK_SET);

    // --- Padding por fila (24 bpp: cada fila múltiplo de 4 bytes) ---
    int rowNoPad = header->ancho * BYTES_POR_PIXEL;
    int padding  = (4 - (rowNoPad % 4)) % 4;
    unsigned char padZeros[3] = {0,0,0};

    t_pixel pixel;

    for (unsigned int i = 0; i < (unsigned int)header->alto; i++) {
        for (unsigned int j = 0; j < (unsigned int)header->ancho; j++) {
            fread(&pixel, 1, 3, archivo);

            // Invertir canales (B, G, R)
            pixel.pixel[0] = (unsigned char)(255 - pixel.pixel[0]); // B
            pixel.pixel[1] = (unsigned char)(255 - pixel.pixel[1]); // G
            pixel.pixel[2] = (unsigned char)(255 - pixel.pixel[2]); // R

            fwrite(&pixel, 1, 3, archivoNegativo);
        }

        // Saltar padding de entrada y escribir padding de salida (mismo ancho mismo padding)
        if (padding) fseek(archivo, padding, SEEK_CUR);
        if (padding) fwrite(padZeros, 1, padding, archivoNegativo);
    }

    fclose(archivoNegativo);
    return EXITO;
}

int escalaGrises(HEADER* header, FILE* archivo, char* nomArch)
{
    const int BYTES_POR_PIXEL = 3; // 24 bpp (B,G,R)

    // Nombre de salida: virus_escala-grises-<nomArch>
    char nomArchSalida[128] = "virus_escala-grises-";
    strcat(nomArchSalida, nomArch);

    FILE* archivoGris = fopen(nomArchSalida, "wb");
    if (!archivoGris) {
        printf("Error al abrir el archivo de salida.\n");
        return ERROR_ARCH;
    }

    if (!cargarHeader(archivoGris, header)) {
        fclose(archivoGris);
        return ERROR_ARCH;
    }

    // Posicionamiento al inicio de datos
    fseek(archivo,     header->comienzoImagen, SEEK_SET);
    fseek(archivoGris, header->comienzoImagen, SEEK_SET);

    // Padding por fila (múltiplo de 4 bytes)
    int rowNoPad = header->ancho * BYTES_POR_PIXEL;
    int padding  = (4 - (rowNoPad % 4)) % 4;
    unsigned char padZeros[3] = {0,0,0};

    t_pixel pixel;

    for (unsigned int i = 0; i < (unsigned int)header->alto; i++) {
        for (unsigned int j = 0; j < (unsigned int)header->ancho; j++) {
            fread(&pixel, 1, 3, archivo);

            // Promedio simple (B,G,R) -> gris
            unsigned char prom = (unsigned char)(
                ( (unsigned int)pixel.pixel[0] +
                  (unsigned int)pixel.pixel[1] +
                  (unsigned int)pixel.pixel[2] ) / 3
            );

            pixel.pixel[0] = prom; // B
            pixel.pixel[1] = prom; // G
            pixel.pixel[2] = prom; // R

            fwrite(&pixel, 1, 3, archivoGris);
        }
        // Saltar padding de entrada y escribir padding de salida
        if (padding) fseek(archivo, padding, SEEK_CUR);
        if (padding) fwrite(padZeros, 1, padding, archivoGris);
    }

    fclose(archivoGris);
    return EXITO;
}

int espejarHorizontal(HEADER* header, FILE* archivo, char*nomArch)
{

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

int espejarVertical(HEADER* header, FILE* archivo, char*nomArch)
{

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

//FUNCIONES: Rotaciones.
int rotar90gradosDerecha(HEADER* header, FILE* archivo, char* nomArch)
{
    const int BPP = 3; // 24 bpp (B,G,R)

    // Nombre de salida
    char nomArchSalida[128] = "virus_rotar-derecha-";
    strcat(nomArchSalida, nomArch);

    FILE* archivoOut = fopen(nomArchSalida, "wb");
    if (!archivoOut) { printf("Error al abrir el archivo de salida.\n"); return ERROR_ARCH; }

    // Copiar headers base
    if (!cargarHeader(archivoOut, header)) { fclose(archivoOut); return ERROR_ARCH; }

    // Dimensiones originales
    unsigned int AnchoViejo = header->ancho;
    unsigned int AltoViejo = header->alto;

    // Row sizes y padding (origen)
    int rowOrigNoPad = (int)AnchoViejo * BPP;
    int padOrig      = (4 - (rowOrigNoPad % 4)) % 4;
    int rowOrigPad   = rowOrigNoPad + padOrig;

    // Dimensiones nuevas (rotadas 90° derecha)
    unsigned int AnchoNuevo = AltoViejo;
    unsigned int AltoNuevo = AnchoViejo;

    // Row sizes y padding (destino)
    int rowNewNoPad = (int)AnchoNuevo * BPP;
    int padNew      = (4 - (rowNewNoPad % 4)) % 4;
    int rowNewPad   = rowNewNoPad + padNew;

    // Recalcular tamaños en header destino
    unsigned int newComImagen = header->comienzoImagen;    // 54
    unsigned int newTamImagen = (unsigned int)(rowNewPad * (unsigned int)AltoNuevo);
    unsigned int newTamArchivo = newComImagen + newTamImagen;

    // Actualizar header ya escrito en archivoOut
    fseek(archivoOut, 18, SEEK_SET); fwrite(&AnchoNuevo, sizeof(unsigned int), 1, archivoOut); // ancho
    fseek(archivoOut, 22, SEEK_SET); fwrite(&AltoNuevo, sizeof(unsigned int), 1, archivoOut); // alto
    fseek(archivoOut, 34, SEEK_SET); fwrite(&newTamImagen, sizeof(unsigned int), 1, archivoOut); // newTamImagen
    fseek(archivoOut,  2, SEEK_SET); fwrite(&newTamArchivo, sizeof(unsigned int), 1, archivoOut); // newTamArchivo

    // Posicionar datos
    fseek(archivo,    newComImagen, SEEK_SET);
    fseek(archivoOut, newComImagen, SEEK_SET);

    // --- Buffers dinámicos ---
    // Matriz fuente y destino en RAM con origen "arriba-izquierda" (top)
    t_pixel* srcTop = (t_pixel*)malloc((size_t)AnchoViejo * AltoViejo * sizeof(t_pixel));
    t_pixel* dstTop = (t_pixel*)malloc((size_t)AnchoNuevo * AltoNuevo * sizeof(t_pixel));
    if (!srcTop || !dstTop) { free(srcTop); free(dstTop); fclose(archivoOut); return ERROR_MEM; }

    // Buffer para leer una fila de origen (bytes crudos BGR)
    unsigned char* rowBuf = (unsigned char*)malloc((size_t)rowOrigNoPad);
    if (!rowBuf) { free(srcTop); free(dstTop); fclose(archivoOut); return ERROR_MEM; }

    // --- Leer ORIGEN a srcTop con coordenadas top-left ---
    // El archivo BMP con H>0 es bottom-up: yFila=0 es la fila inferior.
    // Convertimos a yTop = H-1 - yFila para tener (0,0) arriba-izquierda en RAM.
    for (unsigned int yFila = 0; yFila < AltoViejo; yFila++) {
        // seek absoluto a la fila
        long offFila = (long)newComImagen + (long)yFila * (long)rowOrigPad;
        fseek(archivo, offFila, SEEK_SET);
        fread(rowBuf, 1, (size_t)rowOrigNoPad, archivo);

        unsigned int yTop = AltoViejo - 1 - yFila; //Se convierte a TOP-LEFT.
        t_pixel* rowDst = &srcTop[yTop * AnchoViejo];
        for (unsigned int x = 0; x < AnchoViejo; x++) {
            size_t idx = (size_t)x * BPP;
            rowDst[x].pixel[0] = rowBuf[idx + 0]; // B
            rowDst[x].pixel[1] = rowBuf[idx + 1]; // G
            rowDst[x].pixel[2] = rowBuf[idx + 2]; // R
        }
    }

    // --- Rotar 90° derecha en RAM (top-left coords) ---
    // Fórmula (top-left): dst(x',y') = src(y', H-1 - x')
    // En índices [y][x]: dstTop[y'][x'] = srcTop[H-1 - x'][y']
    for (unsigned int yPrime = 0; yPrime < AltoNuevo; yPrime++) {     // AltoNuevo = W
        for (unsigned int xPrime = 0; xPrime < AnchoNuevo; xPrime++) { // AnchoNuevo = H
            dstTop[yPrime * AnchoNuevo + xPrime] = srcTop[(AltoViejo - 1 - xPrime) * AnchoViejo + yPrime];
        }
    }

    // --- Escribir DESTINO desde dstTop en orden bottom-up ---
    unsigned char zeros[3] = {0,0,0};
    for (unsigned int yFilaOut = 0; yFilaOut < AltoNuevo; yFilaOut++) {
        unsigned int yTop = AltoNuevo - 1 - yFilaOut; // convertir top->bottom
        t_pixel* rowSrc = &dstTop[yTop * AnchoNuevo];

        // volcar la fila (solo píxeles)
        for (unsigned int x = 0; x < AnchoNuevo; x++) {
            fwrite(&rowSrc[x], 1, BPP, archivoOut);
        }
        // padding
        if (padNew) fwrite(zeros, 1, padNew, archivoOut);
    }

    free(rowBuf);
    free(dstTop);
    free(srcTop);
    fclose(archivoOut);
    return EXITO;
}

int rotar90gradosIzquierda(HEADER* header, FILE* archivo, char* nomArch)
{
    const int BPP = 3; // 24 bpp (B,G,R)

    // Nombre de salida
    char nomArchSalida[128] = "virus_rotar-izquierda-";
    strcat(nomArchSalida, nomArch);

    FILE* archivoOut = fopen(nomArchSalida, "wb");
    if (!archivoOut) { printf("Error al abrir el archivo de salida.\n"); return ERROR_ARCH; }

    // Copiar headers base
    if (!cargarHeader(archivoOut, header)) { fclose(archivoOut); return ERROR_ARCH; }

    // Dimensiones originales
    unsigned int AnchoViejo = header->ancho;
    unsigned int AltoViejo  = header->alto;

    // Row sizes y padding (origen)
    int rowOrigNoPad = (int)AnchoViejo * BPP;
    int padOrig      = (4 - (rowOrigNoPad % 4)) % 4;
    int rowOrigPad   = rowOrigNoPad + padOrig;

    // Dimensiones nuevas (rotadas 90° izquierda)
    unsigned int AnchoNuevo = AltoViejo;
    unsigned int AltoNuevo  = AnchoViejo;

    // Row sizes y padding (destino)
    int rowNewNoPad = (int)AnchoNuevo * BPP;
    int padNew      = (4 - (rowNewNoPad % 4)) % 4;
    int rowNewPad   = rowNewNoPad + padNew;

    // Recalcular tamaños en header destino
    unsigned int newComImagen  = header->comienzoImagen; //54
    unsigned int newTamImagen  = (unsigned int)(rowNewPad * (unsigned int)AltoNuevo);
    unsigned int newTamArchivo = newComImagen + newTamImagen;

    // Actualizar header ya escrito en archivoOut
    fseek(archivoOut, 18, SEEK_SET); fwrite(&AnchoNuevo,   sizeof(unsigned int), 1, archivoOut); // ancho
    fseek(archivoOut, 22, SEEK_SET); fwrite(&AltoNuevo,    sizeof(unsigned int), 1, archivoOut); // alto
    fseek(archivoOut, 34, SEEK_SET); fwrite(&newTamImagen, sizeof(unsigned int), 1, archivoOut); // biSizeImage
    fseek(archivoOut,  2, SEEK_SET); fwrite(&newTamArchivo,sizeof(unsigned int), 1, archivoOut); // bfSize

    // Posicionar datos
    fseek(archivo,    newComImagen, SEEK_SET);
    fseek(archivoOut, newComImagen, SEEK_SET);

    // --- Buffers dinámicos ---
    t_pixel* srcTop = (t_pixel*)malloc((size_t)AnchoViejo * AltoViejo * sizeof(t_pixel));
    t_pixel* dstTop = (t_pixel*)malloc((size_t)AnchoNuevo * AltoNuevo * sizeof(t_pixel));
    if (!srcTop || !dstTop) { free(srcTop); free(dstTop); fclose(archivoOut); return ERROR_MEM; }

    unsigned char* rowBuf = (unsigned char*)malloc((size_t)rowOrigNoPad);
    if (!rowBuf) { free(srcTop); free(dstTop); fclose(archivoOut); return ERROR_MEM; }

    // --- Leer ORIGEN a srcTop con coordenadas top-left ---
    // (BMP con AltoViejo>0 es bottom-up; mapeamos a top-left en RAM)
    for (unsigned int yFila = 0; yFila < AltoViejo; yFila++) {
        long offFila = (long)newComImagen + (long)yFila * (long)rowOrigPad;
        fseek(archivo, offFila, SEEK_SET);
        fread(rowBuf, 1, (size_t)rowOrigNoPad, archivo);

        unsigned int yTop = AltoViejo - 1 - yFila; // bottom-up -> top-left
        t_pixel* rowDst = &srcTop[yTop * AnchoViejo];

        for (unsigned int x = 0; x < AnchoViejo; x++) {
            size_t idx = (size_t)x * BPP;
            rowDst[x].pixel[0] = rowBuf[idx + 0]; // B
            rowDst[x].pixel[1] = rowBuf[idx + 1]; // G
            rowDst[x].pixel[2] = rowBuf[idx + 2]; // R
        }
    }

    // --- Rotar 90° izquierda en RAM (top-left) ---
    // Fórmula (CCW): dst(x',y') = src(W-1 - y', x')
    // En índices [y][x]: dstTop[y'][x'] = srcTop[x'][AnchoViejo - 1 - y']
    for (unsigned int yPrime = 0; yPrime < AltoNuevo; yPrime++) {      // AltoNuevo = W
        for (unsigned int xPrime = 0; xPrime < AnchoNuevo; xPrime++) { // AnchoNuevo = H
            dstTop[yPrime * AnchoNuevo + xPrime] =
                srcTop[xPrime * AnchoViejo + (AnchoViejo - 1 - yPrime)];
        }
    }

    // --- Escribir DESTINO desde dstTop en orden bottom-up ---
    static const unsigned char zeros[3] = {0,0,0};
    for (unsigned int yFilaOut = 0; yFilaOut < AltoNuevo; yFilaOut++) {
        unsigned int yTop = AltoNuevo - 1 - yFilaOut; // top -> bottom
        t_pixel* rowSrc = &dstTop[yTop * AnchoNuevo];

        for (unsigned int x = 0; x < AnchoNuevo; x++) {
            fwrite(&rowSrc[x], 1, BPP, archivoOut);
        }
        if (padNew) fwrite(zeros, 1, padNew, archivoOut);
    }

    free(rowBuf);
    free(dstTop);
    free(srcTop);
    fclose(archivoOut);
    return EXITO;
}

//FUNCIONES: Concatenar.

int concatenarHorizontal(HEADER* header1, FILE* archivo1, char* nomArch1,HEADER* header2, FILE* archivo2, char* nomArch2)
{
    const int BPP = 3; // 24 bpp (B,G,R)
    const unsigned char ZERO = 0;

    // >>> COLOR DE RELLENO (ni negro ni blanco) <<<
    const unsigned char FILL_B = 128; // Azul marino
    const unsigned char FILL_G = 0;
    const unsigned char FILL_R = 0;

    // Validaciones mínimas
    if (!header1 || !archivo1 || !header2 || !archivo2 || !nomArch1 || !nomArch2)
        return ERROR_ARCH;
    if (header1->tampuntos != 24 || header2->tampuntos != 24) {
        printf("Solo se soportan BMP de 24 bpp.\n");
        return ERROR_ARCH;
    }

    // Nombre de salida
    char nomArchSalida[512] = "virus_concatenar-horizontal-";
    strncat(nomArchSalida, nomArch1, sizeof(nomArchSalida)-1 - strlen(nomArchSalida));
    strncat(nomArchSalida, "-",       sizeof(nomArchSalida)-1 - strlen(nomArchSalida));
    strncat(nomArchSalida, nomArch2, sizeof(nomArchSalida)-1 - strlen(nomArchSalida));

    FILE* archivoOut = fopen(nomArchSalida, "wb");
    if (!archivoOut) { printf("Error al abrir el archivo de salida.\n"); return ERROR_ARCH; }

    // Copiar headers base desde la primera imagen
    if (!cargarHeader(archivoOut, header1)) { fclose(archivoOut); return ERROR_ARCH; }

    // Dimensiones y paddings de origen
    unsigned int W1 = (unsigned int)header1->ancho;
    unsigned int H1 = (unsigned int)header1->alto;
    unsigned int W2 = (unsigned int)header2->ancho;
    unsigned int H2 = (unsigned int)header2->alto;

    int rowNoPad1 = (int)W1 * BPP;
    int pad1      = (4 - (rowNoPad1 % 4)) % 4;
    int rowPad1   = rowNoPad1 + pad1;

    int rowNoPad2 = (int)W2 * BPP;
    int pad2      = (4 - (rowNoPad2 % 4)) % 4;
    int rowPad2   = rowNoPad2 + pad2;

    // Dimensiones destino (alineación arriba)
    unsigned int W = W1 + W2;
    unsigned int H = (H1 > H2) ? H1 : H2;

    // Row y padding destino
    int rowNoPad = (int)W * BPP;
    int padNew   = (4 - (rowNoPad % 4)) % 4;
    int rowPad   = rowNoPad + padNew;

    // Offsets: usar el de cada imagen para leer y el de la 1ra para escribir
    unsigned int offsDatosOut = header1->comienzoImagen;  // salida
    unsigned int offsDatos1   = header1->comienzoImagen;  // leer img1
    unsigned int offsDatos2   = header2->comienzoImagen;  // leer img2

    // Recalcular tamaños en header destino
    unsigned int biSizeImage = (unsigned int)H * (unsigned int)rowPad;
    unsigned int bfSize      = offsDatosOut + biSizeImage;

    // Actualizar header YA escrito en archivoOut (BITMAPINFOHEADER / BITMAPFILEHEADER)
    fseek(archivoOut, 18, SEEK_SET); fwrite(&W,           sizeof(unsigned int), 1, archivoOut); // ancho
    fseek(archivoOut, 22, SEEK_SET); fwrite(&H,           sizeof(unsigned int), 1, archivoOut); // alto
    fseek(archivoOut, 34, SEEK_SET); fwrite(&biSizeImage, sizeof(unsigned int), 1, archivoOut); // biSizeImage
    fseek(archivoOut,  2, SEEK_SET); fwrite(&bfSize,      sizeof(unsigned int), 1, archivoOut); // bfSize

    // Posicionar punteros al comienzo de datos de cada archivo
    fseek(archivo1,  offsDatos1,   SEEK_SET);
    fseek(archivo2,  offsDatos2,   SEEK_SET);
    fseek(archivoOut, offsDatosOut, SEEK_SET);

    // Buffers de trabajo por fila
    unsigned char* row1 = (unsigned char*)malloc((size_t)rowNoPad1);
    unsigned char* row2 = (unsigned char*)malloc((size_t)rowNoPad2);
    unsigned char* dst  = (unsigned char*)malloc((size_t)rowNoPad);
    if (!row1 || !row2 || !dst) {
        free(row1); free(row2); free(dst);
        fclose(archivoOut);
        return ERROR_MEM;
    }

    // Concatenación fila por fila (BOTTOM-UP en archivo)
    for (unsigned int yOut = 0; yOut < H; yOut++) {
        // yTop es la fila lógica desde arriba
        unsigned int yTop = H - 1 - yOut;

        // --- Origen 1 ---
        if (yTop < H1) {
            long off1 = (long)offsDatos1 + (long)(H1 - 1 - yTop) * (long)rowPad1;
            fseek(archivo1, off1, SEEK_SET);
            if (fread(row1, 1, (size_t)rowNoPad1, archivo1) != (size_t)rowNoPad1) {
                free(row1); free(row2); free(dst); fclose(archivoOut); return ERROR_ARCH;
            }
        } else {
            // Relleno con color cuando falta la fila en img1
            for (unsigned int x = 0; x < W1; x++) {
                size_t idx = (size_t)x * 3;
                row1[idx + 0] = FILL_B;
                row1[idx + 1] = FILL_G;
                row1[idx + 2] = FILL_R;
            }
        }

        // --- Origen 2 ---
        if (yTop < H2) {
            long off2 = (long)offsDatos2 + (long)(H2 - 1 - yTop) * (long)rowPad2;
            fseek(archivo2, off2, SEEK_SET);
            if (fread(row2, 1, (size_t)rowNoPad2, archivo2) != (size_t)rowNoPad2) {
                free(row1); free(row2); free(dst); fclose(archivoOut); return ERROR_ARCH;
            }
        } else {
            // Relleno con color cuando falta la fila en img2
            for (unsigned int x = 0; x < W2; x++) {
                size_t idx = (size_t)x * 3;
                row2[idx + 0] = FILL_B;
                row2[idx + 1] = FILL_G;
                row2[idx + 2] = FILL_R;
            }
        }

        // --- Construir fila destino: row1 || row2 ---
        memcpy(dst,             row1, (size_t)rowNoPad1);
        memcpy(dst + rowNoPad1, row2, (size_t)rowNoPad2);

        // --- Escribir fila destino ---
        if (fwrite(dst, 1, (size_t)rowNoPad, archivoOut) != (size_t)rowNoPad) {
            free(row1); free(row2); free(dst); fclose(archivoOut); return ERROR_ARCH;
        }
        // padding destino
        for (int p = 0; p < padNew; p++) fwrite(&ZERO, 1, 1, archivoOut);
    }

    free(row1); free(row2); free(dst);
    fclose(archivoOut);
    return EXITO;
}

int concatenarVertical(HEADER* header1, FILE* archivo1, char* nomArch1,HEADER* header2, FILE* archivo2, char* nomArch2)
{
    const int BPP = 3; // 24 bpp (B,G,R)

    const unsigned char FILL_B = 128; // Azul marino (BGR)
    const unsigned char FILL_G = 70;
    const unsigned char FILL_R = 0;

    // Nombre de salida
    char nomArchSalida[256] = "virus_concatenar-vertical-";
    strncat(nomArchSalida, nomArch1, sizeof(nomArchSalida)-1 - strlen(nomArchSalida));
    strncat(nomArchSalida, "-",       sizeof(nomArchSalida)-1 - strlen(nomArchSalida));
    strncat(nomArchSalida, nomArch2, sizeof(nomArchSalida)-1 - strlen(nomArchSalida));

    FILE* archivoOut = fopen(nomArchSalida, "wb");
    if (!archivoOut) { printf("Error al abrir el archivo de salida.\n"); return ERROR_ARCH; }

    // Copiar headers base (de la 1ra imagen)
    if (!cargarHeader(archivoOut, header1)) { fclose(archivoOut); return ERROR_ARCH; }

    // Dimensiones originales
    unsigned int W1 = (unsigned int)header1->ancho;
    unsigned int H1 = (unsigned int)header1->alto;
    unsigned int W2 = (unsigned int)header2->ancho;
    unsigned int H2 = (unsigned int)header2->alto;

    // Row sizes y padding (orígenes)
    int rowNoPad1 = (int)W1 * BPP;
    int pad1      = (4 - (rowNoPad1 % 4)) % 4;
    int rowPad1   = rowNoPad1 + pad1;

    int rowNoPad2 = (int)W2 * BPP;
    int pad2      = (4 - (rowNoPad2 % 4)) % 4;
    int rowPad2   = rowNoPad2 + pad2;

    // Dimensiones nuevas (vertical): ancho = max, alto = suma
    unsigned int W = (W1 > W2) ? W1 : W2;
    unsigned int H = H1 + H2;

    // Row size y padding (destino)
    int rowNoPad = (int)W * BPP;
    int padNew   = (4 - (rowNoPad % 4)) % 4;
    int rowPad   = rowNoPad + padNew;

    // ---- USAR EL OFFSET CORRECTO PARA CADA ARCHIVO ----
    unsigned int offsDatosOut = header1->comienzoImagen; // salida
    unsigned int offsDatos1   = header1->comienzoImagen; // lectura img1
    unsigned int offsDatos2   = header2->comienzoImagen; // lectura img2

    // Recalcular tamaños en header destino
    unsigned int biSizeImage = (unsigned int)H * (unsigned int)rowPad;  // datos con padding por fila
    unsigned int bfSize      = offsDatosOut + biSizeImage;

    // Actualizar header ya escrito en archivoOut
    fseek(archivoOut, 18, SEEK_SET); fwrite(&W,           sizeof(unsigned int), 1, archivoOut); // ancho
    fseek(archivoOut, 22, SEEK_SET); fwrite(&H,           sizeof(unsigned int), 1, archivoOut); // alto
    fseek(archivoOut, 34, SEEK_SET); fwrite(&biSizeImage, sizeof(unsigned int), 1, archivoOut); // biSizeImage
    fseek(archivoOut,  2, SEEK_SET); fwrite(&bfSize,      sizeof(unsigned int), 1, archivoOut); // bfSize

    // Posicionar datos
    fseek(archivo1,  offsDatos1,   SEEK_SET);
    fseek(archivo2,  offsDatos2,   SEEK_SET);
    fseek(archivoOut, offsDatosOut, SEEK_SET);

    // Buffers por fila
    unsigned char* row1 = (unsigned char*)malloc((size_t)rowNoPad1);
    unsigned char* row2 = (unsigned char*)malloc((size_t)rowNoPad2);
    unsigned char* dst  = (unsigned char*)malloc((size_t)rowNoPad);
    if (!row1 || !row2 || !dst) {
        free(row1); free(row2); free(dst);
        fclose(archivoOut);
        return ERROR_MEM;
    }
    static const unsigned char ZERO = 0;

    // Escribir destino en orden bottom-up
    for (unsigned int yOut = 0; yOut < H; yOut++) {
        unsigned int yTop = H - 1 - yOut;  // fila lógica desde arriba

        // Inicializar fila destino con COLOR de relleno (no negro/blanco)
        for (unsigned int x = 0; x < W; x++) {
            size_t idx = (size_t)x * BPP;
            dst[idx + 0] = FILL_B; // B
            dst[idx + 1] = FILL_G; // G
            dst[idx + 2] = FILL_R; // R
        }

        if (yTop < H1) {
            // Fila de la imagen 1 (arriba)
            long off1 = (long)offsDatos1 + (long)(H1 - 1 - yTop) * (long)rowPad1; // bottom-up en archivo
            fseek(archivo1, off1, SEEK_SET);
            if (fread(row1, 1, (size_t)rowNoPad1, archivo1) != (size_t)rowNoPad1) {
                free(row1); free(row2); free(dst); fclose(archivoOut); return ERROR_ARCH;
            }
            // Alineación por ANCHO a la izquierda (x0=0)
            unsigned int x0 = 0;
            memcpy(dst + (size_t)x0 * BPP, row1, (size_t)rowNoPad1);
        } else {
            // Fila de la imagen 2 (abajo)
            unsigned int y2Top = yTop - H1;
            long off2 = (long)offsDatos2 + (long)(H2 - 1 - y2Top) * (long)rowPad2;
            fseek(archivo2, off2, SEEK_SET);
            if (fread(row2, 1, (size_t)rowNoPad2, archivo2) != (size_t)rowNoPad2) {
                free(row1); free(row2); free(dst); fclose(archivoOut); return ERROR_ARCH;
            }
            unsigned int x0 = 0; // izquierda
            memcpy(dst + (size_t)x0 * BPP, row2, (size_t)rowNoPad2);
        }

        // Escribir fila destino y su padding
        if (fwrite(dst, 1, (size_t)rowNoPad, archivoOut) != (size_t)rowNoPad) {
            free(row1); free(row2); free(dst); fclose(archivoOut); return ERROR_ARCH;
        }
        for (int p = 0; p < padNew; p++) fwrite(&ZERO, 1, 1, archivoOut);
    }

    free(row1); free(row2); free(dst);
    fclose(archivoOut);
    return EXITO;
}
