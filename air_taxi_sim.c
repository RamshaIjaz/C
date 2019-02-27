/*
 ----------------- COMP 310/ECSE 427 Winter 2018 -----------------
 Dimitri Gallos
 Assignment 2 skeleton
 
 -----------------------------------------------------------------
 I declare that the awesomeness below is a genuine piece of work
 and falls under the McGill code of conduct, to the best of my knowledge.
 -----------------------------------------------------------------
 */

//Please enter your name and McGill ID below
//Name: Ramsha Ijaz
//McGill ID: 260665762

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <semaphore.h>

int BUFFER_SIZE = 100; //size of queue
int int_max = 2147483647;
int int_min = -2147483648;

sem_t Mutex;
sem_t empty_Count;
sem_t full_Count;

// A structure to represent a queue
struct Queue
{
    int front, rear, size;
    unsigned capacity;
    int *array;
};

// function to create a queue of given capacity.
// It initializes size of queue as 0
struct Queue *createQueue(unsigned capacity)
{
    struct Queue *queue = (struct Queue *)malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1; // This is important, see the enqueue
    queue->array = (int *)malloc(queue->capacity * sizeof(int));
    return queue;
}

// Queue is full when size becomes equal to the capacity
int isFull(struct Queue *queue)
{
    return ((queue->size) >= queue->capacity);
}

// Queue is empty when size is 0
int isEmpty(struct Queue *queue)
{
    return (queue->size == 0);
}

// Function to add an item to the queue.
// It changes rear and size
void enqueue(struct Queue *queue, int item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
    printf("%d enqueued to queue\n", item);
}

// Function to remove an item from queue.
// It changes front and size
int dequeue(struct Queue *queue)
{
    if (isEmpty(queue))
        return int_min;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

// Function to get front of queue
int front(struct Queue *queue)
{
    if (isEmpty(queue))
        return int_min;
    return queue->array[queue->front];
}

// Function to get rear of queue
int rear(struct Queue *queue)
{
    if (isEmpty(queue))
        return int_min;
    return queue->array[queue->rear];
}

void print(struct Queue *queue)
{
    if (queue->size == 0)
    {
        return;
    }

    for (int i = queue->front; i < queue->front + queue->size; i++)
    {

        printf(" Element at position %d is %d \n ", i % (queue->capacity), queue->array[i % (queue->capacity)]);
    }
}

struct Queue *queue;

/*Producer Function: Simulates an Airplane arriving and dumping 5-10 passengers to the taxi platform */
void *FnAirplane(void *pl_id)
{
    int passenger, pass_id, plane_id;

    plane_id = (*(int *)pl_id) * 100000; //getting the plane id/plane number

    while (1)
    {

        passenger = rand() % 10 + 5; //randomly generates 5-10 passengers
        printf("Airplane %i arrives with %i passengers\n", (*(int *)pl_id), passenger);
        for (int i = 0; i < passenger; i++) //loops for the number of passengers generated
        {

            pass_id = 1000000 + plane_id + i; //creating passenger id

            printf("Passenger %i of airplane %i arrives to platform\n", i, (*(int *)pl_id));

            if (sem_trywait(&empty_Count) == -1) //checks if queue is full and breaks the for loop to allow next batch
            {
                printf("Platform is full: Rest of passengers of plane %i take the bus\n", *(int *)pl_id);
                break;
            }
            else
            {
                sem_wait(&Mutex);        //decrement Mutex to enter critical section
                enqueue(queue, pass_id); //enqueing a passenger
                sem_post(&Mutex);        //incrementing Mutex to give turn to next thread
                sem_post(&full_Count);   //incrementing full_Count to indicate no# of full spots on platform
            }
        }
        usleep(1000000); //(1 hr = 1 sec = 10^6 microseconds)
    }
}

/* Consumer Function: simulates a taxi that takes n time to take a passenger home and come back to the airport */
void *FnTaxi(void *tax_id)
{
    int time, taxi_id;
    taxi_id = *(int *)tax_id;

    while (1)
    {

        printf("Taxi driver %i arrives\n", taxi_id);
        if (sem_trywait(&full_Count) == -1) //checks if queue is empty and then waits for queue to get full
        {
            printf("Taxi driver %i waits for passengers to enter the platform\n", taxi_id); //when queue is empty
            sem_wait(&full_Count);                                                          //blocks the thread for the queue to fill up
        }

        sem_wait(&Mutex); //decrementing Mutex to enter critical section
        int pass_id = dequeue(queue);
        sem_post(&Mutex);       //incrementing Mutex for giving turn to next thread
        sem_post(&empty_Count); //incrementing empty_Count to indicate no# of empty spots on platform
        printf("Taxi driver %i picked up client %i from the platform\n", taxi_id, pass_id);
        time = rand() % 21 + 10;     //sleeping for 10-30 min
        usleep(time * 1000000 / 60); //(1 hr = 1 sec = 1 min * 10^6 microseconds/60 min )
    }
}

int main(int argc, char *argv[])
{

    int num_airplanes;
    int num_taxis;
    int i;

    num_airplanes = atoi(argv[1]);
    num_taxis = atoi(argv[2]);

    printf("You entered: %d airplanes per hour\n", num_airplanes);
    printf("You entered: %d taxis\n", num_taxis);

    //initialize queue
    queue = createQueue(BUFFER_SIZE);

    //declare arrays of threads and initialize semaphore(s)
    pthread_t air_threads[num_airplanes];
    pthread_t taxi_threads[num_taxis];
    sem_init(&empty_Count, 0, BUFFER_SIZE);
    sem_init(&full_Count, 0, 0);
    sem_init(&Mutex, 0, 1);

    //create arrays of integer pointers to ids for taxi / airplane threads
    int *taxi_ids[num_taxis];
    int *airplane_ids[num_airplanes];

    //create threads for airplanes
    for (i = 0; i < num_airplanes; i++)
    {
        int *ptr = malloc(sizeof(int));
        *ptr = i; //pointer to the id
        airplane_ids[i] = ptr;
        pthread_create(&air_threads[i], NULL, FnAirplane, (int *)airplane_ids[i]);
        printf("Creating airplane thread %i\n", i);
    }
    sleep(1); //allowing the queue to fill up
    //create threads for taxis
    for (i = 0; i < num_taxis; i++)
    {
        int *ptr = malloc(sizeof(int));
        *ptr = i;
        taxi_ids[i] = ptr;
        pthread_create(&taxi_threads[i], NULL, FnTaxi, (int *)taxi_ids[i]);
    }

    //join airplane threads
    for (i = 0; i < num_airplanes; i++)
    {
        pthread_join(air_threads[i], NULL);
    }
    //join taxi threads
    for (i = 0; i < num_taxis; i++)
    {
        pthread_join(taxi_threads[i], NULL);
    }

    sem_destroy(&empty_Count);
    sem_destroy(&full_Count);

    pthread_exit(NULL);
}
