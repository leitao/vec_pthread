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
	register vector signed int vecC;
	register vector signed int vecD;
	register vector signed int vecE;
	register vector bool d;
	vector signed int final = {0xa, 8, 6, 4};

	int i;
	int l;

	l = pthread_spin_lock(&lock);

#ifdef DEBUG
	printf("thread %lx\n", id);
#endif

	for (i = INTERACTION; i ; i--) {
		vecC = vec_add(vecA, vecB);
		vecD = vec_add(vecC, vecB);
		vecE = vec_add(vecA, vecD);

		d = vec_cmpeq(vecE, final);


		if (!d[0] + !d[1] + !d[2] + !d[3] != 0 ) {
			printf("%lx %lx %lx %lx\n", vecE[0], vecE[1], vecE[2], vecE[3]);
			printf("%lx %lx %lx %lx\n", final[0], final[1], final[2], final[3]);
			printf("%lx %lx %lx %lx\n", d[0], d[1], d[2], d[3]);
		}
	}

	l = pthread_spin_unlock(&lock);
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
