#include "funciones_grupo.h"

int procesar_imagen(int argc, char* argv[])
{
    FILE *archivoEntrada;
    int status = TODO_OK;
    char *nombreArchivo = NULL;

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] != '-')
        {
            nombreArchivo = argv[i];
            break;
        }
    }

    if (!nombreArchivo)
    {
        printf("No se proporciono el nombre del archivo de imagen.\n");
        return ARCHIVO_NO_ENCONTRADO;
    }

    archivoEntrada = fopen(nombreArchivo, "rb");
    if (!archivoEntrada)
    {
        printf("Error al abrir el archivo de entrada: %s\n", nombreArchivo);
        return ARCHIVO_NO_ENCONTRADO;
    }

    for (int i = 1; i < argc; i++)
    {
        char *igual = strchr(argv[i], '=');
        if (igual != NULL)
        {
            *igual = '\0';
            const char *opcion = argv[i];
            const char *valor = igual+1;

            float porcentaje = atoi(valor);
            if(porcentaje<0 || porcentaje>100){
                break;
            }

            if (strcmp(opcion, "--tonalidad-roja") == 0)
            {
                status = tonalidadRojo(archivoEntrada, porcentaje);
            }
            else if (strcmp(opcion, "--tonalidad-verde") == 0)
            {
                status = tonalidadVerde(archivoEntrada);
            }
            else if (strcmp(opcion, "--aumentar-contraste") == 0)
            {
                status = aumentarContraste(archivoEntrada);
            }
        }
        else
        {
            if (strcmp(argv[i], "--negativo") == 0)
            {
                status = convertirNegativo(archivoEntrada);
            }
            else if (strcmp(argv[i], "--escala-de-grises") == 0)
            {
                status = blancoNegro(archivoEntrada);
            }
        }
    }

/*
    for(int i=0; i<argc; i++)
    {
        if(strcmp(argv[i],"--negativo")==0)
        {
            status= convertirNegativo(archivoEntrada);
        }
        else if (strcmp(argv[i],"--tonalidad-azul")==0)
        {
            status=tonalidadAzul(archivoEntrada);
        }
        else if (strcmp(argv[i],"--tonalidad-verde")==0)
        {
            status=tonalidadaVerde(archivoEntrada);
        }
        else if (strcmp(argv[i],"--tonalidad-roja")==0)
        {
            status=tonalidadaRojo(archivoEntrada,porcentaje);
        }
        else if (strcmp(argv[i],"--escala-de-grises")==0)
        {
            status=blancoNegro(archivoEntrada);
        }
        else if (strcmp(argv[i],"--aumentar-contraste")==0)
        {
            status=aumentarContraste(archivoEntrada);
        }
        else if (strcmp(argv[i],"--reducir-contraste")==0)
        {
            status=reducirContraste(archivoEntrada);
        }
        else if (strcmp(argv[i],"--recortar")==0)
        {
            status=recortarImagen(archivoEntrada);
        }
        else if (strcmp(argv[i],"--rotar-derecha")==0)
        {
            status=rotar90gradosDerecha(archivoEntrada);
        }
        else if (strcmp(argv[i],"--rotar-izquierda")==0)
        {
            status=rotar90gradosIzquierda(archivoEntrada);
        }
        else if (strcmp(argv[i],"--comodin")==0)
        {
            status=comodin(archivoEntrada);
        }
    }
    */

    if(status == 0)
    {
        printf("Todo OK");
    }
    fclose(archivoEntrada);
    return TODO_OK;
}

METADATAgral cargaCabecera(FILE* archivoEntrada, FILE* archivoSalida)
{
    METADATAgral metadata;

    fseek(archivoEntrada,0,SEEK_SET);
    fseek(archivoSalida,0,SEEK_SET);
    fread(&metadata.tipobm,sizeof(unsigned short),1,archivoEntrada);
    fwrite(&metadata.tipobm,sizeof(unsigned short),1,archivoSalida);

    fseek(archivoEntrada,2,SEEK_SET);
    fseek(archivoSalida,2,SEEK_SET);
    fread(&metadata.tamArchivo,sizeof(unsigned int),1,archivoEntrada);
    fwrite(&metadata.tamArchivo,sizeof(unsigned int),1,archivoSalida);;

    fseek(archivoEntrada,6,SEEK_SET);
    fseek(archivoSalida,6,SEEK_SET);
    fread(&metadata.reserva,sizeof(unsigned short),1,archivoEntrada);
    fwrite(&metadata.reserva,sizeof(unsigned short),1,archivoSalida);

    fseek(archivoEntrada,8,SEEK_SET);
    fseek(archivoSalida,8,SEEK_SET);
    fread(&metadata.reserva1,sizeof(unsigned short),1,archivoEntrada);
    fwrite(&metadata.reserva1,sizeof(unsigned short),1,archivoSalida);

    fseek(archivoEntrada,10,SEEK_SET);
    fseek(archivoSalida,10,SEEK_SET);
    fread(&metadata.comienzoImagen,sizeof(unsigned int),1,archivoEntrada);
    fwrite(&metadata.comienzoImagen,sizeof(unsigned int),1,archivoSalida);

    fseek(archivoEntrada,14,SEEK_SET);
    fseek(archivoSalida,14,SEEK_SET);
    fread(&metadata.tamEncabezado,sizeof(unsigned int),1,archivoEntrada);
    fwrite(&metadata.tamEncabezado,sizeof(unsigned int),1,archivoSalida);

    fseek(archivoEntrada,18,SEEK_SET);
    fseek(archivoSalida,18,SEEK_SET);
    fread(&metadata.ancho,sizeof(unsigned int),1,archivoEntrada);
    fwrite(&metadata.ancho,sizeof(unsigned int),1,archivoSalida);

    fseek(archivoEntrada,22,SEEK_SET);
    fseek(archivoSalida,22,SEEK_SET);
    fread(&metadata.alto,sizeof(unsigned int),1,archivoEntrada);
    fwrite(&metadata.alto,sizeof(unsigned int),1,archivoSalida);

    fseek(archivoEntrada,26,SEEK_SET);
    fseek(archivoSalida,26,SEEK_SET);
    fread(&metadata.planos,sizeof(unsigned short),1,archivoEntrada);
    fwrite(&metadata.planos,sizeof(unsigned short),1,archivoSalida);

    fseek(archivoEntrada,28,SEEK_SET);
    fseek(archivoSalida,28,SEEK_SET);
    fread(&metadata.tampuntos,sizeof(unsigned short),1,archivoEntrada);
    fwrite(&metadata.tampuntos,sizeof(unsigned short),1,archivoSalida);

    fseek(archivoEntrada,30,SEEK_SET);
    fseek(archivoSalida,30,SEEK_SET);
    fread(&metadata.compresion,sizeof(unsigned int),1,archivoEntrada);
    fwrite(&metadata.compresion,sizeof(unsigned int),1,archivoSalida);

    fseek(archivoEntrada,34,SEEK_SET);
    fseek(archivoSalida,34,SEEK_SET);
    fread(&metadata.tamimagen,sizeof(unsigned int),1,archivoEntrada);
    fwrite(&metadata.tamimagen,sizeof(unsigned int),1,archivoSalida);

    fseek(archivoEntrada,38,SEEK_SET);
    fseek(archivoSalida,38,SEEK_SET);
    fread(&metadata.resolucionhor,sizeof(unsigned int),1,archivoEntrada);
    fwrite(&metadata.resolucionhor,sizeof(unsigned int),1,archivoSalida);

    fseek(archivoEntrada,42,SEEK_SET);
    fseek(archivoSalida,42,SEEK_SET);
    fread(&metadata.resolucionvert,sizeof(unsigned int),1,archivoEntrada);
    fwrite(&metadata.resolucionvert,sizeof(unsigned int),1,archivoSalida);

    fseek(archivoEntrada,46,SEEK_SET);
    fseek(archivoSalida,46,SEEK_SET);
    fread(&metadata.tampaletacolores,sizeof(unsigned int),1,archivoEntrada);
    fwrite(&metadata.tampaletacolores,sizeof(unsigned int),1,archivoSalida);

    fseek(archivoEntrada,50,SEEK_SET);
    fseek(archivoSalida,50,SEEK_SET);
    fread(&metadata.contcolor,sizeof(unsigned int),1,archivoEntrada);
    fwrite(&metadata.contcolor,sizeof(unsigned int),1,archivoSalida);

    return metadata;
}

