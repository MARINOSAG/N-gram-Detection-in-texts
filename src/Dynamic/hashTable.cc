#include "hashTable.h"
#include "bloomfilter.h"

hashTable::hashTable(int s,int bs,float ld)
{
	int i;
	bucket_size = bs;
	number_of_keys = 0;
	bucket_to_split = 0;
	round = 0;
	load_factor = ld;
	number_of_buckets = s;
	old_number_of_buckets = s;
	table = new bucket*[table_size];
	for(i=0;i<number_of_buckets;i++)
	{ 
		table[i] = new bucket(bucket_size);
		//cout<<"table node created"<<endl;
	}
}

hashTable::~hashTable()
{
	int i;
	for(i=0;i<number_of_buckets;i++)
	{
		delete table[i];
		//cout<<"table node destroyed"<<endl;
	}
	delete []table;
}

void hashTable::newBucket()
{
	if(number_of_buckets < table_size)
	{
		table[number_of_buckets] = new bucket(bucket_size);
		number_of_buckets++;
	}
	else
	{
		int i;
		table_size = table_size*2;
		bucket **temp_table;
		temp_table = new bucket*[table_size];
		//for(i=0;i<number_of_buckets;i++)		temp_table[i] = table[i];
		memcpy(temp_table,table,number_of_buckets*sizeof( bucketHeap*));

		delete []table;
		table = temp_table;
		table[number_of_buckets] = new bucket(bucket_size);
		number_of_buckets++;
	}
}

int hashTable::hashFunc(int i,int k,int m)
{
	int result=1;
	if(i!=0)
	for(int j=1;j<=i;j++)	result = result*2;
	return k%(result*m);
}

int hashTable::hashFuncString(char *key)
{
	int count=0,i;
	for(i=0;i<strlen(key);i++) count += key[i];
	return count;
}


void hashTable::insert(char* key, Trie_node*&l)
{
	int h1,h2,h;
	float ld;
	h1 = hashFunc(round,hashFuncString(key),m);
	h2 = hashFunc(round+1,hashFuncString(key),m);
	//cout<<h1<<" "<<h2<<endl;
	number_of_keys++;
	if(h1>=bucket_to_split)	h=h1;
	else					h=h2;
	ld = (float) number_of_keys/(number_of_buckets*bucket_size);
	if(idExist(key) == 0)	table[h]->insert(key,l);
	else					table[h]->update_data_value(key,l);//an yparxei kai den einai final 

	if(ld>load_factor)//an 3eperasoume to load factor 
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

void hashTable::delet(char* key)//an prepei na ginei delete apo to prwto epipedo kaleitai kai ayto gia na to diagrapsoume apo to hashtable
{
	int n;
	if(hashFunc(round,hashFuncString(key),m) >= bucket_to_split)	table[hashFunc(round,hashFuncString(key),m)]->delet(key);
	else 	table[hashFunc(round+1,hashFuncString(key),m)]->delet(key);
}


void hashTable::print()
{
	int i;
	for(i=0;i<number_of_buckets;i++)	
	{
		table[i]->print();cout<<endl;
	}
}

int hashTable::idExist(char* k)//briskei me vash to katallhlo hashfunction thn thesh ston pinaka tou key 
{
	int n;
	if(hashFunc(round,hashFuncString(k),m) >= bucket_to_split)	return table[hashFunc(round,hashFuncString(k),m)]->idExist(k);
	else 	return table[hashFunc(round+1,hashFuncString(k),m)]->idExist(k);
}

Trie_node* hashTable::find(char* k)//apla gyrnaei kai to deikth sto prwto epipedo
{
	int n;
	if(hashFunc(round,hashFuncString(k),m) >= bucket_to_split)	return table[hashFunc(round,hashFuncString(k),m)]->find(k);
	else 	return table[hashFunc(round+1,hashFuncString(k),m)]->find(k);
}

