#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<pthread.h>

#define INTERACTION 100
#define THREADS 100

pthread_spinlock_t lock;

void *work(void *id){
	double fpA = 3.14;
	double fpB = 2.12;
	double fpC;
	double final = 5.26;

	int i, l, z;

#ifdef DEBUG
	printf("thread %lx\n", id);
#endif
	for (i = INTERACTION; i ; i--) {
		l = pthread_spin_lock(&lock);

		fpC = fpA + fpB;	
		double d = rand();

		for (z =0 ; z < 1024*1024*1024; z++){
			pthread_spin_lock(&lock);
			pthread_spin_unlock(&lock);
		}

		l = pthread_spin_unlock(&lock);

		if (final != fpC) {
			printf("%f %f\n", final, fpC);

			asm (".long 0x0"); // causes a core dump
		}

	}

	pthread_exit(NULL);
}

int main(){
	pthread_t threads[THREADS];
	long t;

	while(true) {
		for(t=0; t<THREADS; t++){
			pthread_create(&threads[t],NULL, work, (void *) t);
		}
		
		for(t=0; t<THREADS; t++){
			pthread_join(threads[t], NULL);
		}
	}
}
