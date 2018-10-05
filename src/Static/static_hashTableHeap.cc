#include "static_hashTableHeap.h"


hashTableHeap::hashTableHeap(int s,int bs,float ld)
{
	int i;
	bucket_size = bs;
	number_of_keys = 0;
	bucket_to_split = 0;
	round = 0;
	load_factor = ld;
	number_of_buckets = s;
	old_number_of_buckets = s;
	table = new bucketHeap*[table_size2];
	for(i=0;i<number_of_buckets;i++)
	{ 
		table[i] = new bucketHeap(bucket_size);
		//cout<<"table node created"<<endl;
	}
}

hashTableHeap::~hashTableHeap()
{
	int i;
	for(i=0;i<number_of_buckets;i++)
	{
		delete table[i];
		//cout<<"table node destroyed"<<endl;
	}
	delete []table;
}

void hashTableHeap::newBucket()
{
	if(number_of_buckets < table_size2)
	{
		table[number_of_buckets] = new bucketHeap(bucket_size);
		number_of_buckets++;
	}
	else
	{
		int i;
		table_size2 = table_size2*2;
		bucketHeap **temp_table;
		temp_table = new bucketHeap*[table_size2];
		//for(i=0;i<number_of_buckets;i++)		temp_table[i] = table[i];
		memcpy(temp_table,table,number_of_buckets*sizeof( bucketHeap*));

		delete []table;
		table = temp_table;
		table[number_of_buckets] = new bucketHeap(bucket_size);
		number_of_buckets++;
	}
}

int hashTableHeap::hashFunc(int i,int k,int m)
{
	int result=1;
	if(i!=0)
	for(int j=1;j<=i;j++)	result = result*2;
	return k%(result*m);
}

int hashTableHeap::hashFuncString(char *key)
{
	int count=0,i;
	for(i=0;i<strlen(key);i++) count += key[i];
	return count;
}

void hashTableHeap::insert(char* key, HeapNode*&l)
{
	int h1,h2,h;
	float ld;
	h1 = hashFunc(round,hashFuncString(key),m2);
	h2 = hashFunc(round+1,hashFuncString(key),m2);
	//cout<<h1<<" "<<h2<<endl;
	number_of_keys++;
	if(h1>=bucket_to_split)	h=h1;
	else					h=h2;
	ld = (float) number_of_keys/(number_of_buckets*bucket_size);
	if(idExist(key) == 0)	table[h]->insert(key,l);
	//else					table[h]->update_data_value(key,l);

	if(ld>load_factor)
	{
		newBucket();
		table[bucket_to_split]->split(round,number_of_buckets,bucket_to_split,table[number_of_buckets-1]);
		bucket_to_split++;
		if(number_of_buckets == 2*old_number_of_buckets)
		{
			bucket_to_split = 0;
			round++;
			old_number_of_buckets = number_of_buckets;
		}
	}
}

void hashTableHeap::delet(char* key)
{
	int n;
	if(hashFunc(round,hashFuncString(key),m2) >= bucket_to_split)	table[hashFunc(round,hashFuncString(key),m2)]->delet(key);
	else 	table[hashFunc(round+1,hashFuncString(key),m2)]->delet(key);
}


void hashTableHeap::print()
{
	int i;
	for(i=0;i<number_of_buckets;i++)	
	{
		table[i]->print();cout<<endl;
	}
}

int hashTableHeap::idExist(char* k)
{
	int n;
	if(hashFunc(round,hashFuncString(k),m2) >= bucket_to_split)	return table[hashFunc(round,hashFuncString(k),m2)]->idExist(k);
	else 	return table[hashFunc(round+1,hashFuncString(k),m2)]->idExist(k);
}

HeapNode* hashTableHeap::find(char* k)
{
	int n;
	if(hashFunc(round,hashFuncString(k),m2) >= bucket_to_split)	return table[hashFunc(round,hashFuncString(k),m2)]->find(k);
	else 	return table[hashFunc(round+1,hashFuncString(k),m2)]->find(k);
}

void hashTableHeap::change_heapPointer(char *k, HeapNode* node_to_change)
{
	int n;
	if(hashFunc(round,hashFuncString(k),m2) >= bucket_to_split)	table[hashFunc(round,hashFuncString(k),m2)]->change_heapPointer(k,node_to_change);
	else 	table[hashFunc(round+1,hashFuncString(k),m2)]->change_heapPointer(k,node_to_change);
}


