/*
TP LRU CACHE



*/

#include <stdio.h>
#include "tweener.h"

#define archivo_usuarios "usuarios.dat"
#define archivo_posteos "posteos.dat"

void calculoTiempos(const char* path_Usuarios, const char* path_Posteos);
void calculoTiemposSecuencial(const char* path_Usuarios, const char* path_Posteos);


int main()
{
    printf("\n===LOTE DE PRUEBA CON DISPERSION NULA===\n");
    crearLotesPrueba(archivo_usuarios, archivo_posteos, DISPERSION_NULA);
    puts("-Acceso directo (FSEEK)");
    calculoTiempos(archivo_usuarios, archivo_posteos);
    puts("-Acceso secuencial");
    calculoTiemposSecuencial(archivo_usuarios, archivo_posteos);

    printf("\n===LOTE DE PRUEBA CON DISPERSION BAJA===\n");
    crearLotesPrueba(archivo_usuarios, archivo_posteos, DISPERSION_BAJA);
    puts("-Acceso directo (FSEEK)");
    calculoTiempos(archivo_usuarios, archivo_posteos);
    puts("-Acceso secuencial");
    calculoTiemposSecuencial(archivo_usuarios, archivo_posteos);

    printf("\n===LOTE DE PRUEBA CON DISPERSION MEDIA===\n");
    crearLotesPrueba(archivo_usuarios, archivo_posteos, DISPERSION_MEDIA);
    puts("-Acceso directo (FSEEK)");
    calculoTiempos(archivo_usuarios, archivo_posteos);
    puts("-Acceso secuencial");
    calculoTiemposSecuencial(archivo_usuarios, archivo_posteos);

    printf("\n===LOTE DE PRUEBA CON DISPERSION ALTA===\n");
    crearLotesPrueba(archivo_usuarios, archivo_posteos, DISPERSION_ALTA);
    puts("-Acceso directo (FSEEK)");
    calculoTiempos(archivo_usuarios, archivo_posteos);
    puts("-Acceso secuencial");
    calculoTiemposSecuencial(archivo_usuarios, archivo_posteos);

    return 0;
}

void calculoTiempos(const char* path_Usuarios, const char* path_Posteos)
{
    int vTamanios[] = {5, 10, 20, 100};
    double tiempo_inicio = clock();
    procesarFeeds(path_Usuarios, path_Posteos);
    double tiempo_final = (double)(clock() - tiempo_inicio) / CLOCKS_PER_SEC;
    printf("Tiempo en procesar todos los usuarios (FSEEK): %f\n",tiempo_final);


    for(int i = 0 ; i < sizeof(vTamanios) / sizeof(int) ; i++)
    {
        tiempo_inicio = clock();
        procesarFeedsLruCache(path_Usuarios, path_Posteos, vTamanios[i]);
        tiempo_final = (double)(clock() - tiempo_inicio) / CLOCKS_PER_SEC;

        printf("Tiempo en procesar todos los usuarios (TAMCACHE = %d): %f\n",vTamanios[i],tiempo_final);
    }
}

void calculoTiemposSecuencial(const char* path_Usuarios, const char* path_Posteos)
{
    int vTamanios[] = {5, 10, 20, 100};
    double tiempo_inicio = clock();
    procesarFeedsSecuencial(path_Usuarios, path_Posteos);
    double tiempo_final = (double)(clock() - tiempo_inicio) / CLOCKS_PER_SEC;
    printf("Tiempo en procesar todos los usuarios (SECUENCIAL): %f\n",tiempo_final);

    for(int i = 0 ; i < sizeof(vTamanios) / sizeof(int) ; i++)
    {
        tiempo_inicio = clock();
        procesarFeedsLruCacheSecuencial(path_Usuarios, path_Posteos, vTamanios[i]);
        tiempo_final = (double)(clock() - tiempo_inicio) / CLOCKS_PER_SEC;

        printf("Tiempo en procesar todos los usuarios (TAMCACHE = %d): %f\n",vTamanios[i],tiempo_final);
    }
}

