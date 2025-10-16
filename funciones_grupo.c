#include "funciones_grupo.h"
#include "funciones_villan.h"

int procesar_imagen(int argc, char* argv[])
{

    FILE *archivoEntrada = NULL;
    HEADER header;
    int status = EXITO;

    bool modoVerbose = false;
    bool soloValidar = false;
    bool mostrarInfo = false;
    bool mostrarAyuda = false;

    char *nombreArchivo = NULL;

    //--------------------
    //ARGUMENTOS
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
        else if (argv[i][0] != '-')
            nombreArchivo = argv[i];
    }

    //--------------------------
    // UTILIDADES
    //--------------------------

    // --verbose
    if (modoVerbose)
    {
        printf("[INFO] Iniciando bmpmanipuleitor...\n");
        printf("[INFO] Argumentos detectados: ");
        for (int i = 1; i < argc; i++)
        {
            printf("%s ",argv[i]);
        }
        putchar('\n');
    }

    // --help
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
        printf("bmpmanipuleitor.exe --negativo foto.bmp\n");
        printf("bmpmanipuleitor.exe --info imagen.bmp --validar\n");
        printf("bmpmanipuleitor.exe foto.bmp --verbose --escala-de-grises --aumentar-contraste=25\n\n");
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
        printf("  --comodin <Explicacion personalizada>\n");
        return EXITO;
    }

    if (!nombreArchivo)
    {
        printf("[ERROR] No se proporciono el nombre del archivo.\n");
        return ERROR_ARCH;
    }

    // Abrir archivo
    if(modoVerbose) printf("[INFO] Cargando archivo: %s\n",nombreArchivo);
    archivoEntrada = fopen(nombreArchivo, "rb");
    if (!archivoEntrada)
    {
        printf("[ERROR] No se pudo abrir el archivo: %s\n", nombreArchivo);
        return ERROR_ARCH;
    }

    // Leer header
    if(modoVerbose) printf("[INFO] Validando header BMP...\n");
    if (!leerHeader(archivoEntrada, &header))
    {
        printf("[ERROR] No se pudo leer el encabezado BMP.\n");
        fclose(archivoEntrada);
        return ERROR_BMP;
    }

    // --validar
    if (soloValidar) printf("Validando %s...\n", nombreArchivo);
    status = verificarArchivo(archivoEntrada,&header,soloValidar);
    if(modoVerbose && !soloValidar) printf("[INFO] ARCHIVO VALIDO - Dimensiones: %dx%d, Tamano: %d bytes\n",header.ancho, header.alto, header.tamArchivo);
    if(status!=0)return status;

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
        fclose(archivoEntrada);
        return EXITO;
    }

    //-------------------
    // FILTROS
    //-------------------

    int CantArch=0;

    for (int i = 1; i < argc; i++)
    {
        char *igual = strchr(argv[i], '=');
        if (igual)
        {
            *igual = '\0';
            const char *opcion = argv[i];
            const char *valor = igual + 1;
            float porcentaje = atoi(valor);

            if (porcentaje < 0 || porcentaje > 100)
            {
                printf("[ERROR] Parametro invalido en %s.\n", opcion);
                status = ERROR_ARGS;
                continue;
            }

            if (strcmp(opcion, "--tonalidad-roja") == 0)
            {
                if (modoVerbose) printf("[INFO] Aplicando filtro: tonalidad roja (%d%%)\n", (int)porcentaje);
                status = tonalidadRojo(&header, archivoEntrada, porcentaje, nombreArchivo);
                CantArch++;
            }
            else if (strcmp(opcion, "--aumentar-contraste") == 0)
            {
                if (modoVerbose) printf("[INFO] Aplicando filtro: contraste\n");
                // status = aumentarContraste(&header, archivoEntrada, porcentaje);
                CantArch++;
            }
        }
        else if (strcmp(argv[i], "--negativo") == 0)
        {
            if (modoVerbose) printf("[INFO] Aplicando filtro: negativo\n");
            // status = convertirNegativo(&header, archivoEntrada);
            CantArch++;
        }
        else if (strcmp(argv[i], "--escala-de-grises") == 0)
        {
            if (modoVerbose) printf("[INFO] Aplicando filtro: escala de grises\n");
            // status = blancoNegro(&header, archivoEntrada);
            CantArch++;
        }
        else if(strcmp(argv[i],"--espejar-horizontal")==0){
            if(modoVerbose) printf("[INFO] Aplicando filtro: espejar horizontalmente\n");
            status = espejarHorizontal(&header, archivoEntrada, nombreArchivo);
            CantArch++;
        }
        else if(strcmp(argv[i],"--espejar-vertical")==0){
            if(modoVerbose) printf("[INFO] Aplicando filtro: espejar verticalmente\n");
            status = espejarVertical(&header, archivoEntrada, nombreArchivo);
            CantArch++;
        }
    }

    if (modoVerbose)
    {
        if(CantArch==1){
            printf("[INFO] Proceso finalizado - %d archivo generado\n",CantArch);
        }
        else{
            printf("[INFO] Proceso finalizado - %d archivos generados\n",CantArch);
        }
    }
    else{
        printf("Proceso finalizado exitosamente.\n");
    }
    fclose(archivoEntrada);
    return status;
}
