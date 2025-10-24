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

void imprimirHelp()
{
    printf("BMPMANIPULEITOR - Manipulador de imagenes BMP 24 bits\n\n");
    printf("GRUPO: VIRUS\n");
    printf("Integrantes:\n");
    printf("1. 44513506 - ERENO, Tomas\n");
    printf("2. 43916389 - BASSO, Tiziano Axel\n");
    printf("3. 46117338 - VILLAN, Matias Nicolas\n\n");
    printf("Uso: bmpmanipuleitor.exe [OPCIONES] archivo.bmp\n");
    printf("EJEMPLOS:\n");
    printf("  bmpmanipuleitor.exe --negativo foto.bmp\n");
    printf("  bmpmanipuleitor.exe --info imagen.bmp --validar\n");
    printf("  bmpmanipuleitor.exe foto.bmp --verbose --escala-de-grises --aumentar-contraste=25\n");
    printf("  bmpmanipuleitor.exe --concatenar-horizontal img1.bmp img2.bmp\n");
    printf("  bmpmanipuleitor.exe img1.bmp --concatenar-vertical img2.bmp\n");
    printf("  bmpmanipuleitor.exe img1.bmp img2.bmp --concatenar-horizontal\n\n");

    printf("UTILIDADES:\n");
    printf("  --help       Mostrar esta ayuda\n");
    printf("  --info       Mostrar informacion tecnica del BMP\n");
    printf("  --validar    Validar archivo\n");
    printf("  --verbose    Activar modo informativo detallado\n\n");

    printf("FILTROS BASICOS:\n");
    printf("  --negativo                Invertir colores\n");
    printf("  --escala-de-grises        Convertir a escala de grises promediando RGB\n");
    printf("  --espejar-horizontal      Voltear imagen horizontalmente\n");
    printf("  --espejar-vertical        Voltear imagen verticalmente\n\n");

    printf("FILTROS CON PARAMETROS (0-100%%):\n");
    printf("  --aumentar-contraste=X    Aumenta el contraste en un X%%\n");
    printf("  --reducir-contraste=X     Reduce el contraste en un X%%\n");
    printf("  --tonalidad-azul=X        Aumenta en un X%% la intensidad del color azul\n");
    printf("  --tonalidad-verde=X       Aumenta en un X%% la intensidad del color verde\n");
    printf("  --tonalidad-roja=X        Aumenta en un X%% la intensidad del color rojo\n");
    printf("  --recortar=X              Mantiene solo X%% de la imagen original\n");
    printf("  --achicar=X               Reescalando la imagen al X%%\n\n");

    printf("ROTACIONES:\n");
    printf("  --rotar-derecha           Rota la imagen 90 grados en sentido horario\n");
    printf("  --rotar-izquierda         Rota la imagen 90 grados en sentido antihorario\n\n");

    printf("CONCATENACIONES:\n");
    printf("  --concatenar-horizontal   Concatena dos imagenes lado a lado\n");
    printf("  --concatenar-vertical     Concatena dos imagenes una sobre otra\n");
    printf("     * Si las imagenes tienen distinto alto/ancho, la menor se rellena\n\n");

    printf("COMODIN:\n");
    printf("  --comodin=X               Imprime columnas de colores\n\n");

    printf("NOTAS:\n");
    printf("  * Solo los filtros de concatenacion requieren dos imagenes.\n");
    printf("  * Los demas filtros operan sobre la primer imagen.\n\n");
}

void imprimirInfo(HEADER* header, char* nomArch)
{
    printf("\nINFORMACION DEL ARCHIVO\n");
    printf("Archivo: %s\n", nomArch);
    printf("Tamano del archivo: %u bytes\n", header->tamArchivo);
    printf("Dimensiones: %dx%d pixeles\n", header->ancho, header->alto);
    printf("Profundidad de color: %d bits\n", header->tampuntos);
    printf("Compresion: %s\n", (header->compresion == 0) ? "No comprimido" : "Comprimido");
    printf("Offset de datos: %u bytes\n", header->comienzoImagen);
    printf("Tamano de imagen: %u bytes\n", header->tamimagen);
    int padding = (4 - (header->ancho * 3) % 4) % 4;
    printf("Padding por fila: %d bytes\n\n", padding);
}

int verificarArchivo(FILE* arch, HEADER* header, bool imprimir)
{
    unsigned char firma[2];
    size_t leidos = fread(firma, sizeof(unsigned char), 2, arch);
    if (leidos < 2)
    {
        printf("[ERROR]: Archivo vacío o no válido.\n");
        fclose(arch);
        return ERROR_BMP;
    }

    if (!(firma[0] == 'B' && firma[1] == 'M'))
    {
        printf("[ERROR]: Firma BMP inválida.\n");
        fclose(arch);
        return ERROR_BMP;
    }

    if(imprimir) printf("Signature BMP valido.\n");

    if (header->tampuntos != 24)
    {
        printf("[ERROR]: Profundidad de color incorrecta (%d bits, esperado 24 bits)\n", header->tampuntos);
        fclose(arch);
        return ERROR_BMP;
    }

    if(imprimir) printf("Profundidad de 24 bits confirmada.\n");

    if (header->compresion != 0)
    {
        printf("[ERROR]: Compresión detectada (%u)\n", header->compresion);
        fclose(arch);
        return ERROR_BMP;
    }

    if(imprimir) printf("Compresion: No comprimido.\n");

    if (header->ancho <= 0 || header->alto <= 0)
    {
        printf("[ERROR]: Dimensiones no válidas (%dx%d)\n", header->ancho, header->alto);
        fclose(arch);
        return ERROR_BMP;
    }

    if(imprimir) printf("ARCHIVO VALIDO - Listo para procesar.\n\n");

    rewind(arch);
    return EXITO;
}

int tonalidadRojo(HEADER* header, FILE* archivo, float porcentaje, char* nomArch)
{

    char nomArchSalida[128] = "VIRUS_tonalidad-roja-";
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

int espejarHorizontal(HEADER* header, FILE* archivo, char*nomArch)
{

    char nomArchSalida[128] = "VIRUS_espejar-horizontal_";
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
    if (!fila)
    {
        fclose(archivoEspejo);
        return ERROR_MEM;
    }

    for (unsigned int i = 0; i < header->alto; i++)
    {
        fread(fila, sizeof(t_pixel), header->ancho, archivo);
        fread(paddingBytes, 1, padding, archivo);

        for (int j = header->ancho - 1; j >= 0; j--)
        {
            fwrite(&fila[j], sizeof(t_pixel), 1, archivoEspejo);
        }
        fwrite(paddingBytes, 1, padding, archivoEspejo);
    }


    free(fila);
    fclose(archivoEspejo);
    return EXITO;
}

int espejarVertical(HEADER* header, FILE* archivo, char* nomArch)
{
    char nomArchSalida[128] = "VIRUS_espejar-vertical_";
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

    int padding = (4 - (header->ancho * 3) % 4) % 4;
    unsigned char paddingBytes[3] = {0, 0, 0};

    t_pixel *fila = malloc(sizeof(t_pixel) * header->ancho);
    if (!fila)
    {
        fclose(archivoEspejo);
        return ERROR_MEM;
    }

    for (int i = 0; i < header->alto; i++)
    {
        long offsetFila = header->comienzoImagen + (long)i * (header->ancho * sizeof(t_pixel) + padding);
        fseek(archivo, offsetFila, SEEK_SET);

        fread(fila, sizeof(t_pixel), header->ancho, archivo);
        fread(paddingBytes, 1, padding, archivo);

        long offsetDestino = header->comienzoImagen + (long)(header->alto - 1 - i) * (header->ancho * sizeof(t_pixel) + padding);
        fseek(archivoEspejo, offsetDestino, SEEK_SET);

        fwrite(fila, sizeof(t_pixel), header->ancho, archivoEspejo);
        fwrite(paddingBytes, 1, padding, archivoEspejo);
    }

    free(fila);
    fclose(archivoEspejo);
    return EXITO;
}

char* archivoSinExtension(const char* nom) {
    char *punto = strrchr(nom, '.');
    size_t len = (punto) ? (size_t)(punto - nom) : strlen(nom);

    char *nuevo = malloc(len + 1);
    if (!nuevo) return NULL;

    strncpy(nuevo, nom, len);
    nuevo[len] = '\0';
    return nuevo;
}

void eliminarDuplicados(int *argc, char *argv[])
{
    for (int i = 1; i < *argc; i++)
    {
        for (int j = i + 1; j < *argc; )
        {
            if (strcmp(argv[i], argv[j]) == 0)
            {
                for (int k = j; k < *argc - 1; k++)
                    argv[k] = argv[k + 1];

                (*argc)--;
            }
            else
            {
                j++;
            }
        }
    }
}

