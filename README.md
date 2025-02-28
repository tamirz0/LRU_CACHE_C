# LRU_CACHE_C
Implementacion de una Least Recently Used Cache en C
En este trabajo se busca demostrar la utilidad de una LRU Cache a traves de una recreacion de la misma sobre una imitacion de Red Social nombrada "Tweener".

La idea es mediante un archivo posts y usuarios (que contiene los id de los posts en su feed) lograr acceder al archivo posts de forma mas veloz a traves de guardar en "Memoria" (LRU) 
con distintas variaciones de apariciones de posts en los feeds de los usuarios, desde una variacion baja (siempre los mismos posts) a una alta (posts siempre distintos) y demostrar a 
traves de la utilizacion de la libreria time.h los tiempos de ejecucion diferentes. 
La implementacion de la Cache sera a traves de una Lista simplemente enlazada con un tamaño limitado en su creacion.
