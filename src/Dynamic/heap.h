#ifndef heap
#define heap

using namespace std;

#define BUFFER_SIZE 2000
#include <string.h>
#include <stdio.h>
#include <iostream>
#include "hashTableHeap.h"


void heapify();
class BinaryMaxHeap;
class hashTableHeap;
class HeapNode{
public:
	int flag;
	int value;
	char *caller;
	HeapNode *parent;
	HeapNode *left;
	HeapNode *right;
	
	HeapNode(char *c)
	{
		flag  = 0;
		caller = new char[strlen(c)+1];
		strcpy(caller, c);
		parent = NULL;
		left = NULL;
		right = NULL;
	}
	~HeapNode()
	{
		delete []caller;
	}

	void add_one_value(BinaryMaxHeap * temp_heap, hashTableHeap *);
	void print(){cout<<caller;}
	char* get_caller(){return caller;}
};

class BinaryMaxHeap{
private:
	double sum;
	HeapNode *root;		//pointer to the root of the tree
	HeapNode *last;		//pointer to the last node of the tree

	void del(HeapNode*);
	HeapNode* find(HeapNode*,char*);
	HeapNode* find(char*);
	void top(HeapNode*, HeapNode*&, int*);
	void print(HeapNode*);
	void flag_init(HeapNode*);
public:
	BinaryMaxHeap();
	~BinaryMaxHeap();
	HeapNode* insert(char*, hashTableHeap *);
	string top(int);
	void print();
	void clear();
	void move_up(HeapNode*, hashTableHeap *);

};

#endif