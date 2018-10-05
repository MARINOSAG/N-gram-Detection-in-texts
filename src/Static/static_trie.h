#ifndef tr
#define tr

#define N 2
#define BUFFER_SIZE 2000
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "static_hashTable.h"
#include "static_heap.h"
#include "queryarg.h"
#include "mutex.h"
//#include "job_scheduler.h"

using namespace std;
//extern Bloomfilter  * mybloom ;

class hashTable;

class Trie_node{
private:	
	char* word;
	Trie_node *children;
	char is_final;
	int n;
	int current_children;
	int level;
	void double_table();

	/////////////////////////////////////////////////////////members compress
	int size_words_array;//max megethos buffer 
	int size_words_length;//max megethos buffer
	int current_len_words_array;//current posa exoume sto words array  
	int current_len_words_length;//current >> >> >>
	
	char* words_array;//oles oi lekseis poy eginan apo to compress
	short int * words_length;

public:
	Trie_node()
	{
		word = NULL;
		children = NULL;
		//tpt aplos constructor gia na arxikopoioyme thn mnhmh kenh 
	};
	Trie_node(char *word, int final, int level);
	~Trie_node();
	char* get_word(){return word;}
	int get_n(){return n;}
	int get_current_children(){return current_children;}
	char get_is_final(){return is_final;}
	void set_current_children(int c){current_children = c;}
	void set_is_final(char c){is_final = c;}
	Trie_node *get_children(){return children;}
	//int insert(char *ngram, Trie_node*& returned_node, hashTable*& hashIndex);
	int insert_to_level(char *word, int final, int level);
	void print_level();
	void print();
	//int search_sentence(int ,int ,char**, hashTable*&);
	int binary_search(int, int, char *);
	void rearrange_table(int);
	void update_is_final(Trie_node*);
	void compress();
	char * get_words_array(){return words_array;};
	short int * get_words_length(){return words_length;};
	int get_current_len_words_array(){return current_len_words_array;}
	int get_current_len_words_length(){return current_len_words_length;}
	
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
	void create(hashTable * hash){
		

		hashIndex = hash;
	}
	int insert(char *);
	string  query(char * sentence,queryArg * arg);//find all Ngrams in trie that exist in sentence
	int delete_ngram(char *);
	int  insert_to_trie(char *ngram, Trie_node*& returned_node);
	int search_sentence(int start,int end,char ** sentence_table);

};




#endif

