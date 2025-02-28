#include "tweener.h"

int cmpFeedId( void *d1,  void *d2)
{
    tPosteo *p1 = (tPosteo*)d1;
    tPosteo *p2 = (tPosteo*)d2;
    return p1->id - p2->id;
}


int crearLotesPrueba(char * path_Usuarios, char * path_Posteos, int dispersion)
{
    FILE* fbUsuarios = fopen(path_Usuarios, "wb");

    FILE* fbPosteos = fopen(path_Posteos, "wb");


    tPosteo posteo;
    for (int i = 0 ; i < CANT_POSTEOS ; i++)
    {
        posteo.id = i;
        strcpy(posteo.texto, "Este es el posteo #");
        char num[10];
        strcat(posteo.texto, itoa(i, num, 10));
        fwrite(&posteo, sizeof(tPosteo), 1, fbPosteos);
    }
    fclose(fbPosteos);
//    printf("Archivo de posteos generado correctamente.\n");

    tUsuario usuario;
    for(int i = 0 ; i < CANT_USUARIOS ; i++)
    {
        strcpy(usuario.usuario, "Usuario_");
        char num[10];
        strcat(usuario.usuario, itoa(i, num, 10));
        for(int j = 0 ; j < MAX_FEED ; j++)
        {
            usuario.feed[j] = rand() % (CANT_POSTEOS / dispersion);
        }
        fwrite(&usuario, sizeof(tUsuario), 1, fbUsuarios);
    }
    fclose(fbUsuarios);
//    printf("Archivo de usuarios generado correctamente.\n");
    return 0;
}

int procesarFeeds(const char * path_Usuarios,const char * path_Posteos)
{
    FILE *fbUsuarios = fopen(path_Usuarios, "rb");
    if(fbUsuarios == NULL)
    {
        printf("Error al abrir el archivo de usuarios.\n");
        return -1;
    }
    FILE *fbPosteos = fopen(path_Posteos, "rb");
    if(fbPosteos == NULL)
    {
        printf("Error al abrir el archivo de posteos.\n");
        fclose(fbUsuarios);
        return -1;
    }

    tUsuario usuario;
    tPosteo posteo;

    fread(&usuario, sizeof(tUsuario), 1, fbUsuarios);
    while(feof(fbUsuarios) == 0)
    {
        //printf("Procesando usuario %s\n", usuario.usuario);
        for(int i = 0 ; i < MAX_FEED ; i++)
        {
            posteo.id = usuario.feed[i];
            fseek(fbPosteos, posteo.id * sizeof(tPosteo), SEEK_SET);
            fread(&posteo, sizeof(tPosteo), 1, fbPosteos);
        }
        fread(&usuario, sizeof(tUsuario), 1, fbUsuarios);
    }

    fclose(fbUsuarios);
    fclose(fbPosteos);
    return 0;
}


int procesarFeedsLruCache(const char * path_Usuarios, const char * path_Posteos, unsigned cacheSize)
{
    FILE *fbUsuarios = fopen(path_Usuarios, "rb");
    if(fbUsuarios == NULL)
    {
        printf("Error al abrir el archivo de usuarios.\n");
        return -1;
    }
    FILE *fbPosteos = fopen(path_Posteos, "rb");
    if(fbPosteos == NULL)
    {
        printf("Error al abrir el archivo de posteos.\n");
        fclose(fbUsuarios);
        return -1;
    }

    tLruCache cache;
    crear_lrucache(&cache, cacheSize);

    tUsuario usuario;
    tPosteo posteo;

    fread(&usuario, sizeof(tUsuario), 1, fbUsuarios);
    while(feof(fbUsuarios) == 0)
    {
        //printf("Procesando usuario %s\n", usuario.usuario);
        for(int i = 0 ; i < MAX_FEED ; i++)
        {
            posteo.id = usuario.feed[i];
            if(obtener_lrucache(&cache, &posteo, sizeof(tPosteo), cmpFeedId) == NO_EXISTE)
            {
                fseek(fbPosteos, posteo.id * sizeof(tPosteo), SEEK_SET);
                fread(&posteo, sizeof(tPosteo), 1, fbPosteos);
                agregar_lrucache(&cache, &posteo, sizeof(tPosteo), cmpFeedId);
            }
//            else
//                printf("Posteo en cache\n");
        }
        fread(&usuario, sizeof(tUsuario), 1, fbUsuarios);
    }

    fclose(fbUsuarios);
    fclose(fbPosteos);
    vaciar_lrucache(&cache);
    return 0;
}

int procesarFeedsSecuencial(const char * path_Usuarios,const char * path_Posteos)
{
    FILE *fbUsuarios = fopen(path_Usuarios, "rb");
    if(fbUsuarios == NULL)
    {
        printf("Error al abrir el archivo de usuarios.\n");
        return -1;
    }
    FILE *fbPosteos = fopen(path_Posteos, "rb");
    if(fbPosteos == NULL)
    {
        printf("Error al abrir el archivo de posteos.\n");
        fclose(fbUsuarios);
        return -1;
    }

    tUsuario usuario;
    tPosteo posteo;

    fread(&usuario, sizeof(tUsuario), 1, fbUsuarios);
    while(feof(fbUsuarios) == 0)
    {
        //printf("Procesando usuario %s\n", usuario.usuario);
        for(int i = 0 ; i < MAX_FEED ; i++)
        {
            rewind(fbPosteos);
            fread(&posteo, sizeof(tPosteo), 1, fbPosteos);
            while(!feof(fbPosteos) && usuario.feed[i] > posteo.id)
                fread(&posteo, sizeof(tPosteo), 1, fbPosteos);
        }
        fread(&usuario, sizeof(tUsuario), 1, fbUsuarios);
    }

    fclose(fbUsuarios);
    fclose(fbPosteos);
    return 0;
}

int procesarFeedsLruCacheSecuencial(const char * path_Usuarios, const char * path_Posteos, unsigned cacheSize)
{
        FILE *fbUsuarios = fopen(path_Usuarios, "rb");
    if(fbUsuarios == NULL)
    {
        printf("Error al abrir el archivo de usuarios.\n");
        return -1;
    }
    FILE *fbPosteos = fopen(path_Posteos, "rb");
    if(fbPosteos == NULL)
    {
        printf("Error al abrir el archivo de posteos.\n");
        fclose(fbUsuarios);
        return -1;
    }

    tLruCache cache;
    crear_lrucache(&cache, cacheSize);

    tUsuario usuario;
    tPosteo posteo;

    fread(&usuario, sizeof(tUsuario), 1, fbUsuarios);
    while(feof(fbUsuarios) == 0)
    {
        //printf("Procesando usuario %s\n", usuario.usuario);
        for(int i = 0 ; i < MAX_FEED ; i++)
        {
            posteo.id = usuario.feed[i];
            if(obtener_lrucache(&cache, &posteo, sizeof(tPosteo), cmpFeedId) == NO_EXISTE)
            {
                rewind(fbPosteos);
                fread(&posteo, sizeof(tPosteo), 1, fbPosteos);
                while(!feof(fbPosteos) && usuario.feed[i] > posteo.id)
                    fread(&posteo, sizeof(tPosteo), 1, fbPosteos);

                agregar_lrucache(&cache, &posteo, sizeof(tPosteo), cmpFeedId);
            }
//            else
//                printf("Posteo en cache\n");
        }
        fread(&usuario, sizeof(tUsuario), 1, fbUsuarios);
    }

    fclose(fbUsuarios);
    fclose(fbPosteos);
    vaciar_lrucache(&cache);
    return 0;
}
