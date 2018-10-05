#ifndef func
#define func

#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include "static_heap.h"
#include "static_hashTableHeap.h"

using namespace std;

int sentence_split(char *sentence, char **&words);
char* delete_duplicate(char* sentence);
int is_empty_string(char *sentence);
void print_batch_results(char **buff, int size);
void insert_query_results_to_heap(BinaryMaxHeap * binaryHeap,hashTableHeap * hash_Heap, string *query_results_buffer,int query_counter);
#endif