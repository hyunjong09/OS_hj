#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

#define NUM_THREADS 4
#define NUM_POINTS 100000000

pthread_mutex_t mutex;
sem_t sem;
long long inside_circle = 0;
long long total_points = 0;
 
void* points_no_sync(void* arg)
{
    long long points_inside = 0;
    double x, y;
    unsigned int rand_state = rand();
    
    for(long long i = 0; i < NUM_POINTS / NUM_THREADS; i++)
    {
        x = (double)rand_r(&rand_state) / RAND_MAX * 2.0 - 1.0;
        y = (double)rand_r(&rand_state) / RAND_MAX * 2.0 - 1.0;
        if(x * x + y * y <= 1.0)
        {
            points_inside++;
        }
    }
    
    // Update global variable without synchronization
    inside_circle += points_inside;
    total_points += NUM_POINTS / NUM_THREADS;
    
    return NULL;
}


void* points_with_mutex(void* arg)
{
    long long points_inside = 0;
    double x, y;
    unsigned int rand_state = rand();
    
    for(long long i = 0; i < NUM_POINTS / NUM_THREADS; i++)
    {
        x = (double)rand_r(&rand_state) / RAND_MAX * 2.0 - 1.0;
        y = (double)rand_r(&rand_state) / RAND_MAX * 2.0 - 1.0;
        if(x * x + y * y <= 1.0)
        {
            points_inside++;
        }
    }
    
    // Synchronized update with mutex
    pthread_mutex_lock(&mutex);
    inside_circle += points_inside;
    total_points += NUM_POINTS / NUM_THREADS;
    pthread_mutex_unlock(&mutex);
    
    return NULL;
}

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
	

void run_simulation(void* (*simulation_func)(void*), const char* method_name)
{
    pthread_t threads[NUM_THREADS];
    inside_circle = 0;
    total_points = 0;

    clock_t start = clock();
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, simulation_func, NULL);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    clock_t end = clock();
    
    double pi_estimate = 4.0 * inside_circle / total_points;
    printf("%s value of pi: %f\n", method_name, pi_estimate);
    double time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("%s computation time: %f seconds\n", method_name, time_used);
}

void multi()
{
    srand(time(NULL));
    pthread_mutex_init(&mutex, NULL);
    sem_init(&sem, 0, 1);
	
    printf("Running simulation with no synchronization...\n");
    run_simulation(points_no_sync, "No synchronization");

    printf("\nRunning simulation with mutex...\n");
    run_simulation(points_with_mutex, "Mutex");

    printf("\nRunning simulation with semaphore...\n");
    run_simulation(points_with_semaphore, "Semaphore");

    pthread_mutex_destroy(&mutex);
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
