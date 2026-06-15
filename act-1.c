#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

// Definimos el límite de señales
#define MAX_SENIALES 5

// Variable global para contar las señales recibidas
int contador = 0;

void manejador(int signo) {
    contador++;
    
    if (contador < MAX_SENIALES) {
        printf("\nPID: %d | Señal recibida: %d/%d :)\n", getpid(), contador, MAX_SENIALES);
    } else {
        printf("\nPID: %d | Recibida la 5ta señal. Terminando programa.\n", getpid());
        exit(0); // Terminamos el proceso al llegar a 5
    }
}

int main() {
    // Registramos el manejador para SIGINT
    if (signal(SIGINT, manejador) == SIG_ERR) {
        printf("\nNo se puede capturar la señal: SIGINT\n");
        return 1;
    }
 
    printf("Programa iniciado (PID: %d). Presiona Ctrl+C 5 veces para salir.\n", getpid());
    
    while(1) {
        sleep(1);
    }

    return 0;
}
