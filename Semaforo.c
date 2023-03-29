#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphores.h>

#define NPROCS 3

int proceso(int n);

int semexmut;

int main(){
int i;
int p;
int status;

srand(getpid());
semexmut=createsem(0x1234,1);

for (i = 0; i < NPROCS; i++)
{
    p=fork();
    if (p==0)
        proceso(i);    
}
for (i = 0; i < NPROCS; i++)
{
    wait(&status);
}

erasesem(semexmut);
}

int proceso(int n){
    int i;
    for (i = 0; i < 10; i++)
    {
        sleep(rand()%3);
        semwait(semexmut);
        printf("Proceso %d entra a la seccion critica -",n);
        fflush(rand()%3);
        sleep(rand()%3);
        printf("Proceso %d sale de la seccion critica\n", n);
        semsignal(semexmut);
    }
    exit(0);
}




