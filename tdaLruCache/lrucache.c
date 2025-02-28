#include "lrucache.h"

void crear_lrucache(tLruCache *pl, unsigned tamCache)
{
    pl->inicio = NULL;
    pl->tamCache = tamCache != 0 ? tamCache : 1;
}

int agregar_lrucache(tLruCache *pl, void *pd, unsigned tamInfo, int (*cmp)(void*, void*))
{
    tNodo *elim, *nue;
    tNodo **node = &(pl->inicio); //tNodo** == tLista*
    unsigned size = 0, rc = 1;
    if((nue = (tNodo*)malloc(sizeof(tNodo))) == NULL //reservo memoria
        || (nue->info = malloc(tamInfo)) == NULL)
    {
        free(nue);
        return NO_MEM;
    }

    if(*node)
    {
        while((rc = cmp((*node)->info, pd)) && (*node)->sig)
        {
            node = &(*node)->sig;
            size++;
        }
        size++; //Cuenta el ultimo que no entro al while porque sig == NULL
    }


    if(!rc) //Hay duplicado
    {
        //eliminar el duplicado
        //node apunta hacia el duplicado
        elim = *node;
        *node = (*node)->sig;
        free(elim->info);
        free(elim);
    }
    else
        size++; //El dato no existia por lo tanto insercion

    if(size > pl->tamCache) //Cache llena - Insercion
    {
        elim = *node;
        *node = NULL;
        free(elim->info);
        free(elim);
    }

    //pongo primero el nuevo
    memcpy(nue->info, pd, tamInfo);
    nue->tamInfo = tamInfo;
    nue->sig = pl->inicio;
    pl->inicio = nue;

    return OK;
}

int obtener_lrucache(tLruCache *pl, void *pd, unsigned dataMax, int (*cmp)(void*, void*))
{
    if(!pl->inicio)
        return NO_EXISTE;

    tNodo **node = &(pl->inicio);
    tNodo *aux;
    while(*node && cmp((*node)->info, pd))
        node = &(*node)->sig;

    if(*node)
    {
        //Copio la info
        memcpy(pd, (*node)->info, MIN(dataMax, (*node)->tamInfo));
        //Pongo el nodo primero
        aux = *node;
        *node = aux->sig;
        aux->sig = pl->inicio;
        pl->inicio = aux;
        return OK;
    }

    return NO_EXISTE;
}
int borrar_lrucache(tLruCache *pl, void *pd, unsigned dataMax, int (*cmp)(void*, void*))
{
    if(pl->inicio == NULL)
        return CACHE_VACIA;


    tNodo **node = &(pl->inicio);
    while(*node && cmp((*node)->info, pd))
        node = &(*node)->sig;

    if(*node)
    {
        tNodo *elim = *node;
        *node = elim->sig;
        memcpy(pd, elim->info, MIN(dataMax, elim->tamInfo));
        free(elim->info);
        free(elim);
        return OK;
    }
    return NO_EXISTE;
}

void vaciar_lrucache(tLruCache *pl)
{
    while(pl->inicio)
    {
        tNodo *elim = pl->inicio;
        pl->inicio = elim->sig;
        free(elim->info);
        free(elim);
    }
}
