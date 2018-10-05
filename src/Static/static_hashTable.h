#ifndef head
#define head
#include <iostream>
#include <cmath>
#include "static_trie.h"

using namespace std;

const float LOAD_FACTOR = 0.75;
static int table_size = 10;
const int m = 2;

class bucket;
class Trie_node;

class hashTable{
	
	private:
		bucket **table;
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
		hashTable(int,int,float);
		~hashTable();
		void newBucket();
		void insert(char*,Trie_node*&);
		void print();
		int idExist(char*);
		Trie_node* find(char* k);
		void delet(char* key);
		int get_numofkeys(){return number_of_keys;};
		void compress();
};

class bucket{
	
	private:
		class bucketNode
		{
			public:
				char *key;
				Trie_node* data;
				bucketNode(){key = NULL;data = NULL;};
				void deleteBucket(){
					delete []key;key=NULL;data = NULL;
				};
				//~bucketNode();
				char* isFree(){return key;};
		};
		bucketNode *tnode;
//		bucket *next_bucket;
		int current_size; //arithmos kathlhmenwn bucketNode  
		int size;//megethos tou pinaka
		void overflow();//diplasiazei ton pinaka tnode
		void bucketSort();//sortarei me swap ktlp ton pikana tnode
		 
	public:
		bucket(int);
		~bucket();
		void insert(char*,Trie_node*);
		void split(int,int,int,bucket* &);//split se ena bucket 
		void print();
		void move(bucketNode &);
		int hashFunc(int,int,int);
		int hashFuncString(char *key);
		int idExist(char*); //an ena key yparxei sto hashtable
		Trie_node* find(char* k);
		Trie_node* binary_search(int l, int r, char *word);
		void rearrange_bucket(int place_to_start);
		int delet(char* k);
		void update_data_value(char*k, Trie_node*l);//einai mono gia na kanei kombous pou yparxoun hdh sto prwto epipedo final
		void compress();

};

#endif