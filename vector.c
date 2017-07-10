#include<altivec.h>
#include<stdio.h>
#include<stdbool.h>
#include<pthread.h>

#define INTERACTION 100
#define THREADS 100

pthread_spinlock_t lock;

void *work(void *id){
	vector signed int vecA = {1,1,1,1};
	vector signed int vecB = {4,3,2,1};
	vector signed int final = {0xa, 8, 6, 4};

	register vector signed int vecC, vecD, vecE;
	register vector bool d;

	int i, l, z;

#ifdef DEBUG
	printf("thread %lx\n", id);
#endif
	for (i = INTERACTION; i ; i--) {
		l = pthread_spin_lock(&lock);

		vecC = vec_add(vecA, vecB);
		vecD = vec_add(vecC, vecB);
		vecE = vec_add(vecA, vecD);

		for (z =0 ; z < 1024*1024*1024; z++){
			pthread_spin_lock(&lock);
			pthread_spin_unlock(&lock);
		}

		d = vec_cmpeq(vecE, final);

		l = pthread_spin_unlock(&lock);

		if (!d[0] + !d[1] + !d[2] + !d[3] != 0 ) {
			printf("%lx %lx %lx %lx\n", vecE[0], vecE[1], vecE[2], vecE[3]);
			printf("%lx %lx %lx %lx\n", final[0], final[1], final[2], final[3]);
			printf("%lx %lx %lx %lx\n", d[0], d[1], d[2], d[3]);

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
