#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <string.h>

// Variable para almacenar el tiempo de inicio
time_t tiempo_inicio;

// Manejador para SIGALRM (cada 5 segundos)
void manejador_alerta(int sig) {
    char *mensaje = "5 segundos mas!!!\n";
    write(STDOUT_FILENO, mensaje, strlen(mensaje));
}

// Manejador para SIGINT (Ctrl+C para salir)
void manejador_salida(int sig) {
    time_t tiempo_final = time(NULL);
    double segundos_transcurridos = difftime(tiempo_final, tiempo_inicio);
    
    printf("\n\n--- Programa terminado ---\n");
    printf("Tiempo total de ejecución: %.0f segundos.\n", segundos_transcurridos);
    exit(0);
}

int main() {
    // 1. Guardamos el tiempo de inicio
    tiempo_inicio = time(NULL);

    // 2. Registramos los manejadores
    signal(SIGALRM, manejador_alerta);
    signal(SIGINT, manejador_salida); // Capturamos Ctrl+C para calcular el tiempo

    printf("Programa iniciado (PID: %d). Presiona Ctrl+C para salir.\n", getpid());

    while(1) {
        alarm(5);
        pause();
    }

    return 0;
}
