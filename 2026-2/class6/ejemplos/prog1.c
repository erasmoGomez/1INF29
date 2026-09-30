#define _GNU_SOURCE     /* necesario para pthread_yield (extensión de GNU) */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>    /* biblioteca de hilos POSIX */
#include <sched.h>

/*
 * La función de un hilo SOLO puede recibir un parámetro
 * (un único puntero void*). Pero muchas veces queremos pasarle
 * varios datos. Agrupamos todos los datos dentro de una "struct" y
 * pasamos un puntero a esa struct. Aquí pasamos 2 datos:
 *   - id           -> número del hilo
 *   - repeticiones -> cuántas veces debe imprimir su mensaje
 */
typedef struct {
    int id;
    int repeticiones;
} DatosHilo;


void *tarea_del_hilo(void *arg)
{
    /* Convertimos el puntero genérico al tipo real: DatosHilo* */
    DatosHilo *datos = (DatosHilo *) arg;

    for (int i = 1; i <= datos->repeticiones; i++) {
        printf("   [Hilo %d] mensaje %d de %d\n", 
                datos->id, 
                i, 
                datos->repeticiones);

        /* pthread_yield(): sugiere ceder el CPU a otro hilo. */
        //pthread_yield();
        sched_yield(); // POSIX
    }
    printf("   [Hilo %d] Termino.\n", datos->id);

    /* pthread_exit(): termina SOLO este hilo. */
    pthread_exit(NULL);
}

int main(void)
{
    int cantidad = 3;
    pthread_t   hilos[3];
    DatosHilo   datos[3];
    pthread_attr_t atributos;

    /* pthread_attr_init(): crea la estructura de atributos
     * con los valores por defecto del sistema. */
    pthread_attr_init(&atributos);

    /* Vamos a ilustrar cómo modificar uno de los atributos del hilo */
    /* Cada hilo tiene su propia "pila" (memoria para sus variables
     * locales y llamadas a funciones). Aquí leemos el valor por
     * defecto y luego lo cambiamos, para ver que SI se modifica. */
    /* Todos los hilos creados con estos atributos usaran esta pila. */
    size_t tam_pila;
    pthread_attr_getstacksize(&atributos, &tam_pila);
    printf("[Main] Tamano de pila por DEFECTO: %zu bytes\n", tam_pila);
    pthread_attr_setstacksize(&atributos, 1024 * 1024);
    pthread_attr_getstacksize(&atributos, &tam_pila);
    printf("[Main] Tamano de pila MODIFICADO: %zu bytes\n\n", tam_pila);

    printf("[Main] Voy a crear %d hilos.\n", cantidad);
    for (int i = 0; i < cantidad; i++) {
        datos[i].id           = i + 1;
        datos[i].repeticiones = i + 1;
        if (pthread_create(&hilos[i], &atributos,
                           tarea_del_hilo, &datos[i]) != 0) {
            printf("[Main] Error al crear el hilo %d\n", i + 1);
            exit(1);
        }
    }

    /* pthread_join(): esperamos a que cada hilo termine. */
    for (int i = 0; i < cantidad; i++) {
        pthread_join(hilos[i], NULL);
        printf("[Main] El hilo %d ya termino.\n", i + 1);
    }

    /* pthread_attr_destroy(): liberamos los atributos. */
    pthread_attr_destroy(&atributos);
    printf("[Main] Todos los hilos terminaron. Fin del programa.\n");
    return 0;
}