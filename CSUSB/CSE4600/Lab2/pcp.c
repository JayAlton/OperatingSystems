// Implementation of the producers-consumers problem
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // For sleep

// Define buffer and condition variables
#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
int count = 0; // How many things are in the buffer
pthread_mutex_t mutex; // A mutex lock
pthread_cond_t not_full; // A condition variable for a not-full buffer
pthread_cond_t not_empty; // A condition variable for a not-empty buffer

// Define the number of items to produce/consume
// and the number of producer-consumer parts
int NUM_THREAD_PAIRS = 3;
int NUM_ITEMS = 10;

// Define the time it takes to produce and consume items
int PRODUCE_TIME = 2;
int CONSUME_TIME = 1;

// Thread function for the producer
void* producer (void* arg) {
	// Get the arg that was passed in, as that is the thread ID
	int id = *(int*)arg;

	// Produce items to store in the buffer
	for (int i = 0; i < NUM_ITEMS; i++) {
		// Store the numbers from 1 to NUM_ITEMS to the buffer
		int item = i + 1;

		// Acquire the lock
		pthread_mutex_unlock(&mutex);

		// If the buffer is full, wait for it to empty out.
		while (count == BUFFER_SIZE) {
			printf("Buffer is full. Producer %d is now waiting...\n", id);
			pthread_cond_wait(&not_full, &mutex);
		}

		// Add an item to the buffer
		buffer[count++] = item;
		printf("Producer %d produced an item: %d\n", id, item);

		// If the buffer was empty, signal to the consumers that the buffer
		// now has items to be taken out.
		pthread_cond_signal(&not_empty);

		// Release the lock
		pthread_mutex_unlock(&mutex);

		// Sleep to simulate production time
		sleep(PRODUCE_TIME);
	}

	return NULL;
}

// Thread function for the consumer
void* consumer(void* arg) {
	// Get the arg that was passed in, as that is the thread ID.
	int id = *(int*)arg;

	// Consumer items from the buffer
	for (int i = 0; i < NUM_ITEMS; i++) {

		// Acquire the lock
		pthread_mutex_lock(&mutex);

		// If the buffer is empty, wait for it to fill up
		while (count == 0) {
			printf("Buffer is empty. Consumer %d is now waiting...\n", id);
			pthread_cond_wait(&not_empty,  &mutex);
		}

		// Remove an item from the buffer
		int item = buffer[--count];
		printf("Consumer %d consumed an item: %d\n", id, item);

		// If the buffer was full, signal to the producers that there is
		// space in the buffer for new items.
		pthread_cond_signal(&not_full);

		// Sleep to simulate consumption time
		sleep(CONSUME_TIME);
	}
	return NULL;
}

// Main function
int main() {

	// Initialize thread handles for producers and consumers
	pthread_t producers[NUM_THREAD_PAIRS], consumers[NUM_THREAD_PAIRS];

	// Initialize mutex and condition variables
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&not_full, NULL);
	pthread_cond_init(&not_empty, NULL);

	// Create an array for producer and consumer IDs
	// There are the same number of producers as consumers
	// so it's OK if they share the same array
	int thread_ids[NUM_THREAD_PAIRS];

	for (int i = 0; i < NUM_THREAD_PAIRS; i++) {
		thread_ids[i] = i;
	}

	// Create producer and consumer threads
	// For each thread created, pass in thread_ids[i] as the ID.
	for (int i = 0; i < NUM_THREAD_PAIRS; i++) {
		int id = i;
		pthread_create(&producers[i], NULL, producer, &thread_ids[i]);
		pthread_create(&consumers[i], NULL, consumer, &thread_ids[i]);
	}

	// Join threads
	for (int i = 0; i < NUM_THREAD_PAIRS; i++) {
		pthread_join(producers[i], NULL);
		pthread_join(consumers[i], NULL);
	}

	// Cleanup
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&not_full);
	pthread_cond_destroy(&not_empty);

	return 0;
}

	
