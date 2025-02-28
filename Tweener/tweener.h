#ifndef TWEENER_H_INCLUDED
#define TWEENER_H_INCLUDED

#include <stdio.h>
#include <time.h>
#include "../tdaLruCache/lrucache.h"

#define CANT_POSTEOS 50000
#define CANT_USUARIOS 200
#define MAX_FEED 20

#define DISPERSION_NULA 100
#define DISPERSION_BAJA 10
#define DISPERSION_MEDIA 2
#define DISPERSION_ALTA 1


typedef struct
{
    int id;
    char texto[141];
} tPosteo;

typedef struct
{
    char usuario[15];
    int feed[MAX_FEED];
} tUsuario;

int crearLotesPrueba(char * path_Usuarios, char * path_Posteos, int dispersion);

int procesarFeeds(const char * path_Usuarios,const char * path_Posteos);
int procesarFeedsLruCache(const char * path_Usuarios, const char * path_Posteos, unsigned cacheSize);
int procesarFeedsSecuencial(const char * path_Usuarios,const char * path_Posteos);
int procesarFeedsLruCacheSecuencial(const char * path_Usuarios, const char * path_Posteos, unsigned cacheSize);


#endif // TWEENER_H_INCLUDED
