#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

#define NUM_THREADS 4
#define NUM_POINTS 100000000

sem_t sem;
long long inside_circle = 0;//circle points global variable
long long total_points =0;//total points global variable
 
//create random points_thread function
void* points(void* arg)
{
	long long points_inside = 0;
	double x,y;
	unsigned int rand_state = rand();
	
	for(long long i=0; i<NUM_POINTS/NUM_THREADS; i++)
	{
		x = (double)rand_r(&rand_state) / RAND_MAX*2.0 -1.0;
		y = (double)rand_r(&rand_state) / RAND_MAX*2.0 -1.0;
		if(x*x + y*y <=1.0)
		{
			points_inside++;
		}
	}
	
	//global variable update use semaphore
	sem_wait(&sem);
	inside_circle += points_inside;
	total_points += NUM_POINTS / NUM_THREADS;
	sem_post(&sem);
	
	return NULL;
}
	

void multi()
{
	int suc1, suc2;
	clock_t start, end;
	double time_used;
	
    pthread_t threads[NUM_THREADS];
    	srand(time(NULL));

    	
    // Initialize the semaphore
    if (sem_init(&sem, 0, 1) != 0) {
        perror("sem_init: failed");
        exit(EXIT_FAILURE);
    }
    
    	start = clock();
    for (int i = 0; i < NUM_THREADS; i++) {
	suc1 = pthread_create(&threads[i], NULL, points, NULL);
        if (suc1 != 0) {
            perror("pthread_create: failed");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < NUM_THREADS; i++) {
	suc2 = pthread_join(threads[i], NULL);
        if(suc2 != 0) {
            perror("pthread_join: failed");
            exit(EXIT_FAILURE);
        }
    }
        end = clock();
	double pi_estimate = 4.0 * inside_circle / total_points;
	printf("Multi-thread value of pi : %f\n", pi_estimate);
    	time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    	printf("Multi-threading time: %f seconds\n", time_used);
	sem_destroy(&sem);

}

void single()
{
	long long points_inside = 0;
	double x,y;
	srand(time(NULL));
	clock_t start, end;
	double time_used;
	start = clock();
	unsigned int rand_state = rand();
	
	for(long long i=0; i<NUM_POINTS; i++)
	{
		x = (double)rand_r(&rand_state) / RAND_MAX*2.0 -1.0;
		y = (double)rand_r(&rand_state) / RAND_MAX*2.0 -1.0;
		if(x*x + y*y <=1.0)
		{
			points_inside++;
		}
	}
	end = clock();	
	double pi_estimate = 4.0 * points_inside/ NUM_POINTS;
	printf("Single-thread value of pi : %f\n", pi_estimate);
    	time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    	printf("Multi-threading time: %f seconds\n", time_used);
}
