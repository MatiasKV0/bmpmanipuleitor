#include "funciones_grupo.h"

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

    bool hayConcatenacion = false;

    //--------------------
    // ARGUMENTOS
    //--------------------

    if (argc == 1)
    {
        printf("[ERROR] No se ingresaron parametros.\n");
        printf("Use '--help' para ver las opciones disponibles.\n");
        return ERROR_ARGS;
    }

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
            hayConcatenacion = true;
        else if (strcmp(argv[i], "--concatenar-vertical") == 0)
            hayConcatenacion = true;
        else if (argv[i][0] != '-')
        {
            if (!nombreArchivo)
                nombreArchivo  = argv[i];                  // primero
            else if (!nombreArchivo2)
                nombreArchivo2 = argv[i];                  // segundo
            else
            {
                printf("[ERROR] Se han ingresado demasiados Archivos.\n");
                return ERROR_ARGS;
            }
        }
    }

    // --Help
    if (mostrarAyuda)
    {
        imprimirHelp();
        return EXITO;
    }

    if (!nombreArchivo)
    {
        printf("[ERROR] No se proporciono el nombre del archivo.\n");
        return ERROR_ARCH;
    }

    if (hayConcatenacion)
    {
        if (!nombreArchivo2)
        {
            printf("[ERROR] La funcion concatenar requiere dos imagenes.\n");
            return ERROR_ARGS;
        }
    }
    else
    {
        if (nombreArchivo2)
        {
            printf("[ERROR] Solo la funcion concatenar acepta dos imagenes.\n");
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

    //--------------------------
    // ABRIR PRIMER ARCHIVO
    //--------------------------
    if (modoVerbose) printf("[INFO] Cargando archivo: %s\n", nombreArchivo);
    archivoEntrada = fopen(nombreArchivo, "rb");
    if (!archivoEntrada)
    {
        printf("[ERROR] No se pudo abrir el archivo: %s\n", nombreArchivo);
        return ERROR_ARCH;
    }

    if (modoVerbose) printf("[INFO] Validando header BMP...\n");
    if (!leerHeader(archivoEntrada, &header))
    {
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
        if (!archivoEntrada2)
        {
            printf("[ERROR] No se pudo abrir el archivo: %s\n", nombreArchivo2);
            fclose(archivoEntrada);
            return ERROR_ARCH;
        }
        if (modoVerbose) printf("[INFO] Validando header BMP...\n");
        if (!leerHeader(archivoEntrada2, &header2))
        {
            printf("[ERROR] No se pudo leer el encabezado BMP.\n");
            fclose(archivoEntrada2);
            fclose(archivoEntrada);
            return ERROR_BMP;
        }
    }

    //--------------------------
    // --validar
    //--------------------------
    if (soloValidar) printf("\nValidando %s...\n", nombreArchivo);
    status = verificarArchivo(archivoEntrada, &header, soloValidar);
    if (soloValidar && hayConcatenacion) printf("\nValidando %s...\n", nombreArchivo2);
    if(hayConcatenacion) status = verificarArchivo(archivoEntrada2, &header2, soloValidar);
    if (modoVerbose && !soloValidar)
    {
        printf("[INFO] '%s' ARCHIVO VALIDO - Dimensiones: %dx%d, Tamano: %u bytes\n",
               nombreArchivo,header.ancho, header.alto, header.tamArchivo);
        if(hayConcatenacion)
        {
            printf("[INFO] '%s' ARCHIVO VALIDO - Dimensiones: %dx%d, Tamano: %u bytes\n",
                   nombreArchivo2,header2.ancho, header2.alto, header2.tamArchivo);
        }
    }

    if (status != 0)
    {
        if (archivoEntrada2) fclose(archivoEntrada2);
        fclose(archivoEntrada);
        return status;
    }

    // --info
    if (mostrarInfo)
    {
        imprimirInfo(&header,nombreArchivo);
        if(archivoEntrada2) imprimirInfo(&header2,nombreArchivo2);
    }

    //-------------------
    // FILTROS
    //-------------------
    eliminarDuplicados(&argc, argv);
    int CantArch = 0;

    for (int i = 1; i < argc; i++)
    {
        const char *opcion = argv[i];
        if(!strcmp(opcion,nombreArchivo)) continue;
        if(hayConcatenacion && !strcmp(opcion,nombreArchivo2)) continue;
        if(!strcmp(argv[i],"--validar") || !strcmp(argv[i],"--verbose") || !strcmp(argv[i],"--info")) continue;

        char *igual = strchr(opcion, '=');
        if (igual)
        {
            *igual = '\0';
            const char *valor  = igual + 1;
            float porcentaje   = (float)atoi(valor);

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
            else if (strcmp(opcion, "--tonalidad-azul") == 0)
            {
                if (modoVerbose) printf("[INFO] Aplicando filtro: tonalidad azul (%d%%)\n", (int)porcentaje);
                status = tonalidadAzul(&header, archivoEntrada, porcentaje, nombreArchivo);
                CantArch++;
            }
            else if (strcmp(opcion, "--tonalidad-verde") == 0)
            {
                if (modoVerbose) printf("[INFO] Aplicando filtro: tonalidad verde (%d%%)\n", (int)porcentaje);
                status = tonalidadVerde(&header, archivoEntrada, porcentaje, nombreArchivo);
                CantArch++;
            }
            else if (strcmp(opcion, "--recortar") == 0)
            {
                if (modoVerbose) printf("[INFO] Aplicando filtro: Recortar Imagen (%d%%)\n", (int)porcentaje);
                status = recortePorcentaje(&header, archivoEntrada, porcentaje, nombreArchivo);
                CantArch++;
            }
            else if (strcmp(opcion, "--aumentar-contraste") == 0)
            {
                if (modoVerbose) printf("[INFO] Aplicando filtro: Aumentar contraste\n");
                status = aumentarContraste(&header, archivoEntrada, porcentaje, nombreArchivo);
                CantArch++;
            }
            else if (strcmp(opcion, "--reducir-contraste") == 0)
            {
                if (modoVerbose) printf("[INFO] Aplicando filtro: Reducir contraste\n");
                status = reducirContraste(&header, archivoEntrada, porcentaje, nombreArchivo);
                CantArch++;
            }
            else if (strcmp(opcion, "--achicar") == 0)
            {
                if (modoVerbose) printf("[INFO] Aplicando filtro: Achicar imagen\n");
                status = achicarImagen(&header, archivoEntrada, porcentaje, nombreArchivo);
                CantArch++;
            }
            else if(strcmp(opcion, "--comodin") == 0)
            {
                if (porcentaje < 2 || porcentaje > 10)
                {
                    printf("[ERROR] Parametro invalido en %s. Ingrese valores entre [2-10]\n", opcion);
                    status = ERROR_ARGS;
                    continue;
                }
                if (modoVerbose) printf("[INFO] Aplicando filtro: Encolumnar colores\n");
                status = tonalidadColumnasSecuencial(&header, archivoEntrada, porcentaje, nombreArchivo);
                CantArch++;
            }
            else
            {
                printf("[ERROR] No existe el argumento '%s'.\n",opcion);
                status = ERROR_ARGS;
            }
        }
        else if (!strcmp(opcion, "--negativo"))
        {
            if (modoVerbose) printf("[INFO] Aplicando filtro: negativo\n");
            status = convertirNegativo(&header, archivoEntrada, nombreArchivo);
            CantArch++;
        }
        else if (!strcmp(opcion, "--escala-de-grises"))
        {
            if (modoVerbose) printf("[INFO] Aplicando filtro: escala de grises\n");
            status = escalaGrises(&header, archivoEntrada, nombreArchivo);
            CantArch++;
        }
        else if (!strcmp(opcion, "--espejar-horizontal"))
        {
            if (modoVerbose) printf("[INFO] Aplicando filtro: espejar horizontalmente\n");
            status = espejarHorizontal(&header, archivoEntrada, nombreArchivo);
            CantArch++;
        }
        else if (!strcmp(opcion, "--espejar-vertical"))
        {
            if (modoVerbose) printf("[INFO] Aplicando filtro: espejar verticalmente\n");
            status = espejarVertical(&header, archivoEntrada, nombreArchivo);
            CantArch++;
        }
        else if (!strcmp(opcion, "--rotar-derecha"))
        {
            if (modoVerbose) printf("[INFO] Aplicando filtro: Rotar Derecha \n");
            status = rotar90gradosDerecha(&header, archivoEntrada, nombreArchivo);
            CantArch++;
        }
        else if (!strcmp(opcion, "--rotar-izquierda"))
        {
            if (modoVerbose) printf("[INFO] Aplicando filtro: Rotar Izquierda \n");
            status = rotar90gradosIzquierda(&header, archivoEntrada, nombreArchivo);
            CantArch++;
        }
        else if (!strcmp(opcion, "--concatenar-horizontal"))
        {
            if (modoVerbose) printf("[INFO] Aplicando filtro: Concatenar Horizontal \n");
            status = concatenarHorizontal(&header, archivoEntrada, nombreArchivo,
                                          &header2, archivoEntrada2, nombreArchivo2);
            CantArch++;
        }
        else if (!strcmp(opcion, "--concatenar-vertical"))
        {
            if (modoVerbose) printf("[INFO] Aplicando filtro: Concatenar Vertical \n");
            status = concatenarVertical(&header, archivoEntrada, nombreArchivo,
                                        &header2, archivoEntrada2, nombreArchivo2);
            CantArch++;
        }
        else
        {
            printf("[ERROR] No existe el argumento '%s'.\n",opcion);
            status = ERROR_ARGS;
        }
    }

    if (!mostrarInfo && !soloValidar && CantArch == 0)
    {
        printf("[ERROR] No se especifico ninguna accion o filtro valido para aplicar a '%s'.\n", nombreArchivo);
        printf("Use '--help' para ver las opciones disponibles.\n");
        fclose(archivoEntrada);
        if (archivoEntrada2) fclose(archivoEntrada2);
        return ERROR_ARGS;
    }

    if (modoVerbose)
    {
        if (CantArch == 1) printf("[INFO] Proceso finalizado - %d archivo generado.\n", CantArch);
        else               printf("[INFO] Proceso finalizado - %d archivos generados.\n", CantArch);
    }
    else if(status == EXITO || CantArch>0)
    {
        printf("Proceso finalizado exitosamente.\n");
    }

    if (archivoEntrada)
        fclose(archivoEntrada);
    if (archivoEntrada2)
        fclose(archivoEntrada2);
    return status;
}
