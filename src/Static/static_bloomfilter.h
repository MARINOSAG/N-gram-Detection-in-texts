#ifndef BLOOM
#define BLOOM

#define M 500000
#define K 2

#define INIT_M 10
#define INIT_N 500000

#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>


using namespace std;

uint32_t  murmur3_32(const uint8_t* key, size_t len, uint32_t seed);
int * hashCassandra(char* mystring,int m ,int k, int* result);

class Bloomfilter
{

	int m ;
	int k ;
	char * bit_vector;
public:
	Bloomfilter();
	~Bloomfilter();
	int already_exists(char*,int *);
	int insert_to_filter(int*);
	int bloomhash(char*);
	void print_bit_vector();
	void init_vector();

};

#endif