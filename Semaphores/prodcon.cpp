// header files
#include <stdio.h>
#include <stdlib.h> // needed for rand()
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h> // for threads
#include <semaphore.h> // for semaphores
#include "buffer.h" // queue being manipulated
#include <iostream>
#include <time.h> // for rand() seeding

using namespace std;

// global variables
queueType<int> buffer;
pthread_mutex_t mutex;
sem_t full;
sem_t empty;
int itemsProduced = 0;
int itemsConsumed = 0;
int prodID, conID = 0;

// thread prototypes
void *producer(void *param);

void *consumer(void *param);

// main
int main(int agrc, char *argv[])
{
 // initialize variables
 srand(time(0)); // seed time for rand()
 pthread_t *prod, *con;

 // change argument values to integers
 int numProd = atoi(argv[2]);
 int numCon = atoi(argv[3]);
 int sleepTime = atoi(argv[1]);

 // set up semaphores and locks
 prod = new pthread_t[numProd];
 con = new pthread_t[numCon];
 pthread_mutex_init(&mutex, NULL);
 sem_init(&full, 0, 0);
 sem_init(&empty, 0, 4);

 // create the producer threads
 for(int i=0; i<numProd; i++)
  pthread_create(&prod[i], NULL, producer, NULL);

 // create the consumer threads
 for(int i=0; i<numCon; i++)
  pthread_create(&con[i], NULL, consumer, NULL);

 // halt main process for given time while threads run
 sleep(sleepTime);

 // report items consumed/produced
 cout << endl << "Items Produced: " << itemsProduced;
 cout << endl << "Items Consumed: " << itemsConsumed;
 cout << endl << endl;

 // end main process along with any threads still running
 return 1;
} // main end

// thread implementations
void *producer(void *param)
{
 int id = prodID+1;
 prodID++;

 while(true)
 {
  // sleep for a random period of time
  sleep(rand()%5);

  // generate a random number to insert
  int item = rand();

  sem_wait(&empty);
  pthread_mutex_lock(&mutex);

  // enter critical zone
  if(buffer.insert_item(item))
   cout << "Producer fail: buffer is full" << endl;
  // insert item
  else
  {
   cout << "producer " << id << " produced " << item << endl;
   itemsProduced++;
  }
  // exit critical zone and release lock
  pthread_mutex_unlock(&mutex);
  sem_post(&full);
 }
 // end thread
 return 0;
}

void *consumer(void *param)
{
 int id = conID+1;
 conID++;

 while(true)
 {
  // sleep for a random period of time
  sleep(rand()%5);

  int item;

  sem_wait(&full);
  pthread_mutex_lock(&mutex);

  // enter critical zone and retrieve item from buffer
  if(buffer.remove_item(item))
   cout << "Consumer fail: no items in buffer" << endl;
  else
  {
   cout << "\t consumer " << id << " consumed " << item << endl;
   itemsConsumed++;
  }
  // exit critical zone and release lock
  pthread_mutex_unlock(&mutex);
  sem_post(&empty);
 }

 // end thread
 return 0;
}
