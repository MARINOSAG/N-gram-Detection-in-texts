#ifndef ARG 
#define ARG
#include "heap.h"
#include "mutex.h"
//struct gia na grafei to kathe thread sto binaryMaxheap 
struct queryArg
{
	BinaryMaxHeap* binarymaxheap;
	hashTableHeap* hashheap;
	Mutex heap_mutex;			//mutex
	CondVar* write_heap_cond; //conition variable for writers
	int writers;

};

#endif