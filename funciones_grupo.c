#include "funciones_grupo.h"
#include "funciones_villan.h"

int procesar_imagen(int argc, char* argv[])
{
    FILE *archivoEntrada  = NULL;
    FILE *archivoEntrada2 = NULL;

    HEADER header, header2;
    int status = EXITO;

    bool modoVerbose   = false;
    bool soloValidar   = false;
    bool mostrarInfo   = false;
    bool mostrarAyuda  = false;

    char *nombreArchivo  = NULL;   // 1er BMP
    char *nombreArchivo2 = NULL;   // 2do BMP

    bool hacerConcatH = false;     // horizontal
    bool hacerConcatV = false;     // vertical

    //--------------------
    // ARGUMENTOS
    //--------------------
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--help") == 0)
            mostrarAyuda = true;
        else if (strcmp(argv[i], "--verbose") == 0)
            modoVerbose = true;
        else if (strcmp(argv[i], "--validar") == 0)
            soloValidar = true;
        else if (strcmp(argv[i], "--info") == 0)
            mostrarInfo = true;
        else if (strcmp(argv[i], "--concatenar-horizontal") == 0)
            hacerConcatH = true;
        else if (strcmp(argv[i], "--concatenar-vertical") == 0)
            hacerConcatV = true;
        else if (argv[i][0] != '-')
        {
            if (!nombreArchivo)
                nombreArchivo  = argv[i];                  // primero
            else if (!nombreArchivo2)
                nombreArchivo2 = argv[i];                  // segundo
            else {
                printf("[ERROR] Demasiados archivos. Solo se admiten dos.\n");
                return ERROR_ARGS;
            }
        }
    }

    //Relacionado a Concatenacion
    if (hacerConcatH && hacerConcatV) {
        printf("[ERROR] Elegí solo una operación: --concatenar-horizontal o --concatenar-vertical\n");
        return ERROR_ARGS;
    }

    bool hayConcatenacion = (hacerConcatH || hacerConcatV);

    if (!nombreArchivo) {
        printf("[ERROR] No se proporcionó el nombre del archivo.\n");
        return ERROR_ARCH;
    }

    if (hayConcatenacion) {
        if (!nombreArchivo2) {
            printf("[ERROR] La función CONCATENAR requiere DOS archivos: img1.bmp img2.bmp\n");
            printf("Ejemplos:\n");
            printf("  bmpmanipuleitor.exe --concatenar-horizontal imagen1.bmp imagen2.bmp\n");
            printf("  bmpmanipuleitor.exe imagen1.bmp --concatenar-horizontal imagen2.bmp\n");
            printf("  bmpmanipuleitor.exe imagen1.bmp imagen2.bmp --concatenar-horizontal\n");
            return ERROR_ARGS;
        }
        if (strcmp(nombreArchivo, nombreArchivo2) == 0) {
            printf("[ERROR] Los archivos de entrada no pueden ser el mismo.\n");
            return ERROR_ARGS;
        }
    } else {
        if (nombreArchivo2) {
            printf("[ERROR] Solo los filtros de concatenación aceptan dos imágenes.\n");
            printf("Uso Ejemplo Horizontal: bmpmanipuleitor.exe --concatenar-horizontal img1.bmp img2.bmp\n");
            return ERROR_ARGS;
        }
    }

    //--------------------------
    // UTILIDADES
    //--------------------------

    // --Verbose

    if (modoVerbose)
    {
        printf("[INFO] Iniciando bmpmanipuleitor...\n");
        printf("[INFO] Argumentos detectados: ");
        for (int i = 1; i < argc; i++) printf("%s ", argv[i]);
        putchar('\n');
    }

    // --Help
    if (mostrarAyuda)
    {
        printf("BMPMANIPULEITOR - Manipulador de imagenes BMP 24 bits\n");
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
        printf("  --validar    Validar archivo sin aplicar filtros\n");
        printf("  --verbose    Activar modo informativo detallado\n\n");
        printf("FILTROS:\n");
        printf("  --negativo                 Aplica filtro negativo\n");
        printf("  --escala-de-grises         Convierte a blanco y negro\n");
        printf("  --tonalidad-roja=<0-100>   Ajusta tonalidad roja\n");
        printf("  --aumentar-contraste=<n>   Incrementa contraste en %%\n");
        printf("  --concatenar-horizontal    Concatena img1 + img2 (lado a lado)\n");
        printf("  --concatenar-vertical      Concatena img1\n");
        printf("                             encima de img2 (una arriba de otra)\n");
        return EXITO;
    }

    //--------------------------
    // ABRIR PRIMER ARCHIVO
    //--------------------------
    if (modoVerbose) printf("[INFO] Cargando archivo: %s\n", nombreArchivo);
    archivoEntrada = fopen(nombreArchivo, "rb");
    if (!archivoEntrada) {
        printf("[ERROR] No se pudo abrir el archivo: %s\n", nombreArchivo);
        return ERROR_ARCH;
    }

    if (modoVerbose) printf("[INFO] Validando header BMP...\n");
    if (!leerHeader(archivoEntrada, &header)) {
        printf("[ERROR] No se pudo leer el encabezado BMP.\n");
        fclose(archivoEntrada);
        return ERROR_BMP;
    }

    //--------------------------
    // ABRIR SEGUNDO ARCHIVO (Solo si hay concatenacion)
    //--------------------------
    if (hayConcatenacion)
    {
        if (modoVerbose) printf("[INFO] Cargando archivo 2: %s\n", nombreArchivo2);
        archivoEntrada2 = fopen(nombreArchivo2, "rb");
        if (!archivoEntrada2) {
            printf("[ERROR] No se pudo abrir: %s\n", nombreArchivo2);
            fclose(archivoEntrada);
            return ERROR_ARCH;
        }
        if (!leerHeader(archivoEntrada2, &header2)) {
            printf("[ERROR] No se pudo leer encabezado del segundo BMP.\n");
            fclose(archivoEntrada2);
            fclose(archivoEntrada);
            return ERROR_BMP;
        }
    }

    //--------------------------
    // --validar
    //--------------------------
    if (soloValidar) printf("Validando %s...\n", nombreArchivo);
    status = verificarArchivo(archivoEntrada, &header, soloValidar);
    if (modoVerbose && !soloValidar)
        printf("[INFO] ARCHIVO VALIDO - Dimensiones: %dx%d, Tamano: %u bytes\n",
               header.ancho, header.alto, header.tamArchivo);
    if (status != 0) {
        if (archivoEntrada2) fclose(archivoEntrada2);
        fclose(archivoEntrada);
        return status;
    }

    // --info
    if (mostrarInfo)
    {
        printf("\nINFORMACION DEL ARCHIVO\n");
        printf("Archivo: %s\n", nombreArchivo);
        printf("Tamano del archivo: %u bytes\n", header.tamArchivo);
        printf("Dimensiones: %dx%d pixeles\n", header.ancho, header.alto);
        printf("Profundidad de color: %d bits\n", header.tampuntos);
        printf("Compresion: %s\n", (header.compresion == 0) ? "No comprimido" : "Comprimido");
        printf("Offset de datos: %u bytes\n", header.comienzoImagen);
        printf("Tamano de imagen: %u bytes\n", header.tamimagen);
        int padding = (4 - (header.ancho * 3) % 4) % 4;
        printf("Padding por fila: %d bytes\n", padding);
        if (archivoEntrada2) fclose(archivoEntrada2);
        fclose(archivoEntrada);
        return EXITO;
    }

    //-------------------
    // FILTROS
    //-------------------
    int CantArch = 0;

    for (int i = 1; i < argc; i++)
    {
        char *igual = strchr(argv[i], '=');
        if (igual)
        {
            *igual = '\0';
            const char *opcion = argv[i];
            const char *valor  = igual + 1;
            float porcentaje   = (float)atoi(valor);

            if (porcentaje < 0 || porcentaje > 100)
            {
                printf("[ERROR] Parametro invalido en %s.\n", opcion);
                status = ERROR_ARGS;
                continue;
            }

            if (strcmp(opcion, "--tonalidad-roja") == 0) {
                if (modoVerbose) printf("[INFO] Aplicando filtro: tonalidad roja (%d%%)\n", (int)porcentaje);
                status = tonalidadRojo(&header, archivoEntrada, porcentaje, nombreArchivo);
                CantArch++;
            }
            else if (strcmp(opcion, "--tonalidad-azul") == 0) {
                if (modoVerbose) printf("[INFO] Aplicando filtro: tonalidad azul (%d%%)\n", (int)porcentaje);
                status = tonalidadAzul(&header, archivoEntrada, porcentaje, nombreArchivo);
                CantArch++;
            }
            else if (strcmp(opcion, "--tonalidad-verde") == 0) {
                if (modoVerbose) printf("[INFO] Aplicando filtro: tonalidad verde (%d%%)\n", (int)porcentaje);
                status = tonalidadVerde(&header, archivoEntrada, porcentaje, nombreArchivo);
                CantArch++;
            }
            else if (strcmp(opcion, "--recortar-imagen") == 0) {
                if (modoVerbose) printf("[INFO] Aplicando filtro: Recortar Imagen (%d%%)\n", (int)porcentaje);
                status = recortePorcentaje(&header, archivoEntrada, porcentaje, nombreArchivo);
                CantArch++;
            }
            else if (strcmp(opcion, "--aumentar-contraste") == 0) {
                if (modoVerbose) printf("[INFO] Aplicando filtro: Aumentar contraste\n");
                status = aumentarContraste(&header, archivoEntrada, porcentaje, nombreArchivo);
                CantArch++;
            }
            else if (strcmp(opcion, "--reducir-contraste") == 0) {
                if (modoVerbose) printf("[INFO] Aplicando filtro: Reducir contraste\n");
                status = reducirContraste(&header, archivoEntrada, porcentaje, nombreArchivo);
                CantArch++;
            }
            else if (strcmp(opcion, "--achicar") == 0) {
                if (modoVerbose) printf("[INFO] Aplicando filtro: Achicar imagen\n");
                status = achicarImagen(&header, archivoEntrada, porcentaje, nombreArchivo);
                CantArch++;
            }
        }
        else if (strcmp(argv[i], "--negativo") == 0) {
            if (modoVerbose) printf("[INFO] Aplicando filtro: negativo\n");
            status = convertirNegativo(&header, archivoEntrada, nombreArchivo);
            CantArch++;
        }
        else if (strcmp(argv[i], "--escala-de-grises") == 0) {
            if (modoVerbose) printf("[INFO] Aplicando filtro: escala de grises\n");
            status = escalaGrises(&header, archivoEntrada, nombreArchivo);
            CantArch++;
        }
        else if (strcmp(argv[i], "--espejar-horizontal") == 0) {
            if (modoVerbose) printf("[INFO] Aplicando filtro: espejar horizontalmente\n");
            status = espejarHorizontal(&header, archivoEntrada, nombreArchivo);
            CantArch++;
        }
        else if (strcmp(argv[i], "--espejar-vertical") == 0) {
            if (modoVerbose) printf("[INFO] Aplicando filtro: espejar verticalmente\n");
            status = espejarVertical(&header, archivoEntrada, nombreArchivo);
            CantArch++;
        }
        else if (strcmp(argv[i], "--rotar-derecha") == 0) {
            if (modoVerbose) printf("[INFO] Aplicando filtro: Rotar Derecha \n");
            status = rotar90gradosDerecha(&header, archivoEntrada, nombreArchivo);
            CantArch++;
        }
        else if (strcmp(argv[i], "--rotar-izquierda") == 0) {
            if (modoVerbose) printf("[INFO] Aplicando filtro: Rotar Izquierda \n");
            status = rotar90gradosIzquierda(&header, archivoEntrada, nombreArchivo);
            CantArch++;
        }
        else if (strcmp(argv[i], "--concatenar-horizontal") == 0) {
            if (modoVerbose) printf("[INFO] Aplicando filtro: Concatenar Horizontal \n");
            status = concatenarHorizontal(&header, archivoEntrada, nombreArchivo,
                                          &header2, archivoEntrada2, nombreArchivo2);
            CantArch++;
        }
        else if (strcmp(argv[i], "--concatenar-vertical") == 0) {
            if (modoVerbose) printf("[INFO] Aplicando filtro: Concatenar Vertical \n");
            status = concatenarVertical(&header, archivoEntrada, nombreArchivo,
                                        &header2, archivoEntrada2, nombreArchivo2);
            CantArch++;
        }
    }

    if (modoVerbose) {
        if (CantArch == 1) printf("[INFO] Proceso finalizado - %d archivo generado\n", CantArch);
        else               printf("[INFO] Proceso finalizado - %d archivos generados\n", CantArch);
    } else {
        printf("Proceso finalizado exitosamente.\n");
    }

    if (archivoEntrada)
        fclose(archivoEntrada);
    if (archivoEntrada2)
        fclose(archivoEntrada2);
    return status;
}
