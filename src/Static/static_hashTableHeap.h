#ifndef hashHeap
#define hashHeap

#include <iostream>
#include <cmath>
#include "static_heap.h"
#include "static_bloomfilter.h"

using namespace std;

const float LOAD_FACTOR2 = 0.75;
static int table_size2 = 10;
const int m2 = 2;

class bucketHeap;
class HeapNode;

class hashTableHeap{
	
	private:
		bucketHeap **table;
		int number_of_buckets;
		int old_number_of_buckets;
		int bucket_size;
		int number_of_keys;
		int bucket_to_split;
		int round;
		float load_factor;
		int hashFunc(int,int,int);
		int hashFuncString(char *);
	public:
		hashTableHeap(int,int,float);
		~hashTableHeap();
		void newBucket();
		void insert(char*,HeapNode*&);
		void print();
		int idExist(char*);
		HeapNode* find(char* k);
		void delet(char* key);
		void change_heapPointer(char *k, HeapNode* node_to_change);

	
};

class bucketHeap{
	
	private:
		class bucketNode
		{
			public:
				char *key;
				HeapNode* data;
				bucketNode(){key = NULL;data = NULL;};
				void deleteBucket(){
					delete []key;key=NULL;data = NULL;
				};
				//~bucketNode();
				char* isFree(){return key;};
		};
		bucketNode *tnode;
//		bucket *next_bucket;
		int current_size;
		int size;
		void overflow();
		void bucketSort();
		
	public:
		bucketHeap(int);
		~bucketHeap();
		void insert(char*,HeapNode*);
		void split(int,int,int,bucketHeap* &);
		void print();
		void move(bucketNode &);
		int hashFunc(int,int,int);
		int hashFuncString(char *key);
		int idExist(char*);
		HeapNode* find(char* k);
		HeapNode* binary_search(int l, int r, char *word);
		void rearrange_bucket(int place_to_start);
		int delet(char* k);
		void update_data_value(char*k, HeapNode*l);
		void change_heapPointer(char *k, HeapNode* node_to_change);
		void binary_search_change(int l, int r, char *word, HeapNode* node_to_change);
};

#endif