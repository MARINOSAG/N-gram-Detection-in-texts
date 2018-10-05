#ifndef test
#define test
#include "trie.h"
#include <assert.h>

class Unit_test
{
	
public:
	
	int check_insert(char * input,Index * & trie, BinaryMaxHeap*& binaryHeap);
	int check_delete(char * input, Index *& trie, BinaryMaxHeap*& binaryHeap);
	int check_search(char * input,Index * & trie, BinaryMaxHeap*& binaryHeap);


};

#endif

