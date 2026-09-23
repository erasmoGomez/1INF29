#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int id;
    int repeticiones;
} DatosHilo;

/* Variable compartida que indica qué hilo puede trabajar */
int turno = 1;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condicion = PTHREAD_COND_INITIALIZER;


void *tarea_del_hilo(void *arg)
{
    DatosHilo *datos = (DatosHilo *) arg;

    pthread_mutex_lock(&mutex);

    /* Si todavía no es mi turno, me bloqueo */
    while (turno != datos->id) {
        pthread_cond_wait(&condicion, &mutex);
    }

    /* Ya es mi turno */
    for (int i = 1; i <= datos->repeticiones; i++) {
        printf("   [Hilo %d] mensaje %d de %d\n",
               datos->id,
               i,
               datos->repeticiones);
    }

    printf("   [Hilo %d] Termino.\n", datos->id);

    /* Le toca al siguiente hilo */
    turno++;

    /* Despertamos a los hilos que estén esperando */
    pthread_cond_broadcast(&condicion);

    pthread_mutex_unlock(&mutex);

    return NULL;
}


int main(void)
{
    int cantidad = 3;

    pthread_t hilos[3];
    DatosHilo datos[3];
    pthread_attr_t atributos;

    pthread_attr_init(&atributos);

    size_t tam_pila;

    pthread_attr_getstacksize(&atributos, &tam_pila);
    printf("[Main] Tamano de pila por DEFECTO: %zu bytes\n", tam_pila);

    pthread_attr_setstacksize(&atributos, 1024 * 1024);

    pthread_attr_getstacksize(&atributos, &tam_pila);
    printf("[Main] Tamano de pila MODIFICADO: %zu bytes\n\n", tam_pila);

    printf("[Main] Voy a crear %d hilos.\n", cantidad);

    for (int i = 0; i < cantidad; i++) {

        datos[i].id = i + 1;
        datos[i].repeticiones = i + 1;

        if (pthread_create(&hilos[i],
                           &atributos,
                           tarea_del_hilo,
                           &datos[i]) != 0) {

            printf("[Main] Error al crear el hilo %d\n", i + 1);
            exit(1);
        }
    }

    for (int i = 0; i < cantidad; i++) {

        pthread_join(hilos[i], NULL);

        printf("[Main] El hilo %d ya termino.\n", i + 1);
    }

    pthread_attr_destroy(&atributos);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condicion);

    printf("[Main] Todos los hilos terminaron. Fin del programa.\n");

    return 0;
}s