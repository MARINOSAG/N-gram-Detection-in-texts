#ifndef tr
#define tr

#define N 2
#define BUFFER_SIZE 2000
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "hashTable.h"
#include "heap.h"
#include "hashTableHeap.h"
#include "queryarg.h"
#include "mutex.h"
using namespace std;

extern Bloomfilter  * mybloom ;

class hashTable;
class hashTableHeap;

class Trie_node{
private:	
	char* word;
	Trie_node *children;
	char is_final;
	int n;
	int current_children;
	int level;
	int A_version ,D_version;
	void double_table();

public:
	Trie_node()
	{
		word = NULL;
		children = NULL;
		//tpt aplos constructor gia na arxikopoioyme thn mnhmh kenh 
	};
	Trie_node(char *word, int final, int level,int a_version,int d_version);
	~Trie_node();
	char* get_word(){return word;}
	int get_n(){return n;}
	int get_current_children(){return current_children;}
	char get_is_final(){return is_final;}
	void set_current_children(int c){current_children = c;}
	void set_is_final(char c){is_final = c;}
	Trie_node *get_children(){return children;}
	//int insert(char *ngram, Trie_node*& returned_node, hashTable*& hashIndex);
	int insert_to_level(char *word, int final, int level,int A_version );
	void print_level();
	void print();
	//int search_sentence(int ,int ,char**, hashTable*&);
	int binary_search(int, int, char *);
	void rearrange_table(int);
	void update_is_final(Trie_node*);
	int get_A_version(){return A_version;};
	int get_D_version(){return D_version ;};
	void set_A_version(int x ){A_version = x;};
	void set_D_version(int x){D_version = x;};
};

class Index{
	hashTable *hashIndex;		//this is a linear hash table index to the first level of the Trie

public:
	Index(){
		hashIndex = NULL;
	}
	~Index(){
		hashIndex = NULL;
	}
	void create(hashTable& hash){
		char *str = new char[100];
		memset(str,'\0',100);
		sprintf(str,"%s","b");
		delete []str;

		hashIndex = &hash;
	}
	int insert(char *word,int A_version );
	string  query(char * sentence,queryArg * arg,int current_version );//find all Ngrams in trie that exist in sentence
	int delete_ngram(char *);
	int  insert_to_trie(char *ngram, Trie_node*& returned_node,int A_version);
	int search_sentence(int start,int end,char ** sentence_table,Trie_node*& temp ,int current_version);
	int delete_ngram2(char *ngram,int d_version);

};


#endif

