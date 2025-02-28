#ifndef LRUCACHE_H_INCLUDED
#define LRUCACHE_H_INCLUDED
//LRU Cache
//Least Recently Used Cache
//Implementado en lista simplemente enlazada
#include <stdlib.h>
#include <string.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define OK 0
#define NO_MEM 1
#define CACHE_VACIA 2
#define NO_EXISTE 3





typedef struct sNodo
{
    void *info;
    unsigned tamInfo;
    struct sNodo *sig;
}tNodo;

typedef struct{
    tNodo *inicio;
    unsigned tamCache;
}tLruCache;


void crear_lrucache(tLruCache *pl, unsigned cacheSize);
int agregar_lrucache(tLruCache *pl, void *pd, unsigned dataSize, int (*cmp)(void*, void*));
int obtener_lrucache(tLruCache *pl, void *pd, unsigned dataMax, int (*cmp)(void*, void*));
int borrar_lrucache(tLruCache *pl, void *pd, unsigned dataMax, int (*cmp)(void*, void*));
void vaciar_lrucache(tLruCache *pl);


#endif // LRUCACHE_H_INCLUDED
