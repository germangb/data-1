// Example taken from: https://gcc.gnu.org/wiki/AutoFDO/Tutorial
// Andrea Brunato: andrea.brunato@arm.com

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <sys/types.h>
#include <string.h> // memcpy
#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <unistd.h>
#include <sys/syscall.h>
#define gettid() syscall(SYS_gettid)
#define ARRAY_LEN 30

typedef struct payload_t{
	int *array;
} payload_t;


void print_array(int* arr){
	assert(arr != NULL);
	for(int i=0; i<ARRAY_LEN; i++){
		printf("%d\n", arr[i]);
	}
	return;
}

//void bubble_sort (int *a, int n) {
void *bubble_sort(void *payload) {
	printf("Sorting from thread %ld\n", gettid());
	payload_t* p =  (payload_t*) payload;
	int* a = p->array;
	int i, t, s = 1;
	while (s) {
		s = 0;
		for (i = 1; i < ARRAY_LEN; i++) {
			if (a[i] < a[i - 1]) {
				t = a[i];
				a[i] = a[i - 1];
				a[i - 1] = t;
				s = 1;
			}
		}
	}
	return NULL;
}

int main(int argc, char* argv[]) {
	int array0[ARRAY_LEN], array1[ARRAY_LEN], i;

	for(i=0; i<ARRAY_LEN; ++i){
		array0[i] = rand();
		array1[i] = rand();
	}

	payload_t* p1 = malloc(sizeof(payload_t));
	payload_t* p2 = malloc(sizeof(payload_t));

	pthread_t thread1, thread2;

	// Init the payload
	p1->array = array0;
	p2->array = array1;

	printf("Main thread tid is: %ld\n", gettid());

	/* Create independent threads each of which will sort its own array */
	pthread_create(&thread1, NULL, bubble_sort, p1);
	pthread_create(&thread2, NULL, bubble_sort, p2);

	// Let's wait for the threads to finish
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	print_array(p1->array);
	print_array(p2->array);

	free(p1);
	free(p2);

	return 0;
}
