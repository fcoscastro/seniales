#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

// estados del semáforo
#define ROJO 0
#define VERDE 1
#define AMARILLO 2

// variable para almacenar el estado actual del semaforo
int estado_semaforo = ROJO;

// Manejador de señales para el proceso hijo
void manejador_semaforo(int senial) {
    if (senial == SIGUSR1) {
        estado_semaforo = VERDE;
    } else if (senial == SIGUSR2) {
        estado_semaforo = AMARILLO;
    } else if (senial == SIGINT) {
        estado_semaforo = ROJO;
    }
}

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Error al crear el proceso (fork)");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { 
 
        // Registramos el manejador para las tres señales
        signal(SIGUSR1, manejador_semaforo);
        signal(SIGUSR2, manejador_semaforo);
        signal(SIGINT, manejador_semaforo);

        // Bucle para mostrar continuamente el estado
        while(1) {
            switch(estado_semaforo) {
                case VERDE:
                    printf("[\033[0;32mVERDE\033[0m]    El tráfico fluye...\n");
                    break;
                case AMARILLO:
                    printf("[\033[0;33mAMARILLO\033[0m] Precaución...\n");
                    break;
                case ROJO:
                    printf("[\033[0;31mROJO\033[0m]     Alto total.\n");
                    break;
            }
            // Retardo de 1 segundo para no saturar la terminal
            sleep(1); 
        }

    } else { 
        // Ejecutaremos 3 ciclos completos del semáforo
        for (int ciclo = 1; ciclo <= 3; ciclo++) {
            printf("\n--- INICIANDO CICLO %d ---\n", ciclo);

            // Cambiar a VERDE
            kill(pid, SIGUSR1);
            sleep(3); // Mantiene el verde por 3 segundos

            // Cambiar a AMARILLO
            kill(pid, SIGUSR2);
            sleep(1); // Mantiene el amarillo por 1 segundo

            // Cambiar a ROJO
            kill(pid, SIGINT);
            sleep(2); // Mantiene el rojo por 2 segundos
        }

        // Una vez terminados los ciclos, cerramos el semáforo
        printf("\nPadre: Apagando semáforo...\n");
        kill(pid, SIGTERM); // Enviamos señal de terminación al hijo
        wait(NULL);         // Esperamos a que el hijo termine correctamente para evitar procesos zombie
        printf("Padre: Ejecución finalizada.\n");
    }

    return 0;
}
