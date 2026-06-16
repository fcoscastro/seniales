#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// Utilizamos volatile sig_atomic_t para que el contador sea seguro dentro del manejador
volatile sig_atomic_t contador = 0;

void manejador(int senial){
    if (senial == SIGUSR1) {
        contador++;
        printf("Hijo: Señal del padre recibida (Van %d)\n", contador);
    } else if (senial == SIGTERM) {
        // Cuando el padre termine de enviar las 10, enviará SIGTERM
        printf("\nHijo: Proceso terminado. Recibí un total de %d señales.\n", contador);
        exit(0);
    }
}

int main(){
    int pid = fork();
    
    if(pid == 0){ // Código del proceso HIJO
        // Registramos las señales que el hijo va a capturar
        signal(SIGUSR1, manejador);
        signal(SIGTERM, manejador);
        
        while(1) {
            pause(); // El hijo espera las señales
        }
    } else { // Código del proceso PADRE
        // El padre envía exactamente 10 señales
        for(int i = 0; i < 10; i++){
            sleep(1);
            kill(pid, SIGUSR1);
        }
        
        // Esperamos un segundo extra para asegurar que el hijo procesó la última
        sleep(1);
        
        printf("Padre: He terminado de enviar las 10 señales. Cerrando hijo...\n");
        // Le enviamos SIGTERM para que el hijo imprima el resultado final y termine
        kill(pid, SIGTERM); 
    }
    
    return 0;
}
