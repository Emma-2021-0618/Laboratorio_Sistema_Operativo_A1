# UCATECI
- Sistemas operativo 2
- Lizandro JOse Ramirez
- Encuentro 7 Teoria

# Autores
- Enmanuel Sanchez Rodriguez 2021-0618
- Albert Francisco Hernandez Sanchez 2019-0126
  
<br>

# Semáforos en Linux (Programación concurrente).


El concepto de semáforos fue el primero en solucionar problemas de sincronización sin espera activa. Lo inventó Edsger W. Dijkstra a finales de la década de 1960 ([Dijkstra74]), aunque las primeras ideas aparecieron a principios de la misma década ([Dijkstra35]). Está inspirado en las señales visuales ferroviarias[3] que indican si un tren está habilitado para entrar en una vía. Es una construcción sencilla y eficiente que permite solucionar problemas genéricos de sincronización entre procesos.
En el video se realiza un programa que consta de dos docmuentos en lenguaje C, uno Crea, espera y señaliza. Y otro tiene el codigo del semáforo.

<br>

# # Semaforo.c

En este primer documento se ilustra cómo se puede usar semáforos para sincronizar procesos en un sistema operativo Unix. El programa crea varios procesos hijos y los hace competir por acceso a una sección crítica, la cual solo puede ser ejecutada por un proceso a la vez.

<br>

En estos primeros codigos vemos como son agragadas las librerías necesarias para la creación de procesos y semáforos.
También se define una constante "NPROCS" igual a 3, que indica el número de procesos hijos que se crearán. Luego se declara una función "proceso" y una variable "semexmut".

<br>

Dentro del "mai" hay algunas variables necesarias para el proceso y se utiliza la función "srand" para inicializar el generador de números aleatorios con el PID del proceso actual.

<br>

~~~~~~~~~~~~~~~~~~
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "semaphores.h"

#define NPROCS 3

int proceso(int n);

int semexmut;

int main(){
int i;
int p;
int status;

srand(getpid());
~~~~~~~~~~~~~~~~~~

<br>

La función "createsem" crea un semáforo compartido identificado por la clave 0x1234 y lo inicializa con un valor de 1. Este semáforo se utiliza para proteger la sección crítica que se encuentra en la función "proceso".

<br>

Luego es utilizado un bucle "For" para crear los procesos hijos. En cada iteración del bucle, se llama a la función "fork" para crear un nuevo proceso. Si el valor de retorno de "fork" es 0, entonces se trata de un proceso hijo y se llama a la función "proceso" con un índice igual a "i". De lo contrario, si el valor de retorno es mayor que 0, entonces se trata del proceso padre y continúa con el bucle para crear los demás procesos hijos.

<br>

~~~~~~~~~~~~~~~~~~
semexmut=createsem(0x1234,1); // Crear un semáforo con un identificador único

for (i = 0; i < NPROCS; i++) // Crear varios procesos hijos
{
    p=fork();
    if (p==0)
        proceso(i);    
}
~~~~~~~~~~~~~~~~~~

<br>

Después de crear los procesos hijos, el proceso padre espera a que todos los procesos hijos terminen ejecución utilizando la función "wait". Luego, se utiliza la función "erasesem" para eliminar el semáforo.

<br>

~~~~~~~~~~~~~~~~~~
for (i = 0; i < NPROCS; i++) // Esperar a que terminen todos los procesos hijos
{
    wait(&status);
}

erasesem(semexmut); // Eliminar el semáforo
~~~~~~~~~~~~~~~~~~

<br>

En esta ultima parte la función "proceso" es la función que realiza la tarea principal del programa. Cada proceso hijo ejecuta esta función 10 veces. En cada iteración, el proceso espera un tiempo aleatorio utilizando la función "sleep". Luego, espera a que el semáforo esté disponible utilizando la función "semwait". Una vez que el semáforo está disponible, el proceso imprime un mensaje en la salida estándar y espera otro tiempo aleatorio antes de liberar el semáforo utilizando la función "semsignal". Después de 10 iteraciones, el proceso hijo sale de la función "proceso" utilizando la función "exit".

Finalmente, la función "main" espera a que todos los procesos hijos terminen utilizando la función "wait" y luego elimina el semáforo utilizando la función "erasesem".

<br>

~~~~~~~~~~~~~~~~~~
}

int proceso(int n){
    int i;
    for (i = 0; i < 10; i++) // Realizar 10 iteraciones
    {
        sleep(rand()%3); // Esperar un tiempo aleatorio
        semwait(semexmut); // Esperar a que el semáforo esté disponible
        printf("Proceso %d entra a la seccion critica -",n);
        fflush(rand()%3); // Limpiar el búfer de salida de datos
        sleep(rand()%3); // Esperar un tiempo aleatorio
        printf("Proceso %d sale de la seccion critica\n", n);
        semsignal(semexmut); // Liberar el semáforo
    }
    exit(0); // Salir del proceso hijo
}
~~~~~~~~~~~~~~~~~~

<br>

# # semaphores.h

<br>

Estas son las declaraciones de encabezado que se utilizan para definir las funciones de semáforos de System V. 
Y define la macro "SEM_ID" como int. Esto significa que el identificador del semáforo se manejará como un entero en lugar de una estructura.

<br>

~~~~~~~~~~~~~~~~~~
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#define SEM_ID int
~~~~~~~~~~~~~~~~~~

<br>

La funcion "semid" descrementa el semaforo.
Si el valor del semáforo es cero, la función se bloquea hasta que el valor del semáforo sea mayor que cero. 
La función utiliza la estructura "sembuf" para especificar la operación a realizar en el semáforo.

<br>

~~~~~~~~~~~~~~~~~~
void semwait(int semid){

    struct sembuf s;
    s.sem_num=0;
    s.sem_op=-1;
    s.sem_flg=SEM_UNDO;

    SEMOP(semid,&s,1);
    return;
}
~~~~~~~~~~~~~~~~~~

<br>

Esta función crea un nuevo semáforo y devuelve su identificador. La función utiliza "semget" para crear el semáforo y "semctl" para establecer su valor inicial.

<br>

~~~~~~~~~~~~~~~~~~
void semsignal(int semid){
    
    struct sembuf s;
    s.sem_num=0;
    s.sem_op=1;
    s.sem_flg=SEM_UNDO;

    SEMOP(semid,&s,1);
    return;   
}
~~~~~~~~~~~~~~~~~~

<br>

Esta función elimina el semáforo identificado por "semid". La función utiliza "semctl" para eliminar el semáforo.

<br>

~~~~~~~~~~~~~~~~~~
void erasesem(int semid){
    semctl(semid,0,IPC_RMID,0);
    return;
}
~~~~~~~~~~~~~~~~~~
