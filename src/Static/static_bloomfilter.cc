#include "static_bloomfilter.h"

void  Bloomfilter::init_vector()
{
  memset(bit_vector,'0',m);
}


double error_rate(int n , int m ,int k )
{
  int x = -1* k * (n+0.5)/(m-1); //ektheths tou e 
  double y = pow(2.7 ,x ); // ypsonoume to e ston ektheth 
  y = 1-y ; //1-e 
  double error  = pow(y,k);//(1-e^x)^k
  return error;

}
void define_mk(int n ,int* m ,int* k )
{
    *m = INIT_M;
    *k =ceil( ((float)(*m)/(float)n)*0.69);


    double error = error_rate(n,*m,*k);
  
  while(error  >0.1)
  {
    
      *m = (*m)*2;
      *k =ceil( ((float)(*m)/(float)n)*0.693147);//k = (m/n)ln(2)
      error = error_rate(n,*m,*k);
  } 

  if(*k <= 1)(*k)++;


  //fprintf(stderr,"error rate = %f   , m = %d  , k =  %d \n\n\n\n\n",error, *m , *k );
}

 //synarthsh poy pairnei ena string kai me katallhles diadikasies mas epistrefei 
//enan integer ton opoio meta ton pername apo allh hashfunction gia na doyme se poia thesh einai hashtable
int elfHash(char* toHash)
{
	int pos, hashValue = 0;
	for(pos = 0; pos < strlen(toHash); pos++) // use all elements
  	{
		hashValue = (hashValue << 4) + toHash[pos]; // shift/mix
		int hiBits = hashValue & 0xF0000000;  // get high nybble
		if(hiBits != 0) hashValue ^= hiBits >> 24;  //xor high nybble with second nybble
		hashValue &= ~hiBits; // clear high nybble
  	}
  	return hashValue;
};
//synarthsh poy pairnei ena string kai epistrefei enan integer pou mas dixnei se poia thesh einai sto bitvector
int Bloomfilter:: bloomhash(char* mystring )
{
	int index = elfHash(mystring);

	return index%m ;//epistrefw to mod gia aplh hashfunction
};

//opou key einai o pinakas apo xarakthres , len einai to plithos twn byte sthn periptwsh mas strlen(key) ,seed einai to seed
uint32_t  murmur3_32(const uint8_t* key, size_t len, uint32_t seed) {
  uint32_t h = seed;
  if (len > 3) {
    const uint32_t* key_x4 = (const uint32_t*) key;
    size_t i = len >> 2;
    do {
      uint32_t k = *key_x4++;
      k *= 0xcc9e2d51;
      k = (k << 15) | (k >> 17);
      k *= 0x1b873593;
      h ^= k;
      h = (h << 13) | (h >> 19);
      h = (h * 5) + 0xe6546b64;
    } while (--i);
    key = (const uint8_t*) key_x4;
  }
  if (len & 3) {
    size_t i = len & 3;
    uint32_t k = 0;
    key = &key[i - 1];
    do {
      k <<= 8;
      k |= *key--;
    } while (--i);
    k *= 0xcc9e2d51;
    k = (k << 15) | (k >> 17);
    k *= 0x1b873593;
    h ^= k;
  }
  h ^= len;
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;
  return h;
}


//synarthsh pou xrhsimopoiei to murmur hash gia diaforetiko k
//h synarthsh epistrefei ena array me megethos k pou antistoixei tis theseis pou brisketai ston bitvector 
int * hashCassandra(char* mystring,int m ,int k, int*result)
{
	uint32_t hash1 = murmur3_32((uint8_t*)mystring,strlen(mystring),0);
	uint32_t hash2 = murmur3_32((uint8_t*)mystring,strlen(mystring),hash1);
	for(int i = 0;i<k;i++)
	{
		result[i]= (int) ((hash1 + i * hash2)%m);
	}
	return result;

}

Bloomfilter:: Bloomfilter()
{
  int n = INIT_N;//problepomenes eisagwges
  define_mk(n,&m , &k );
  //exit(1);
 	//m = M;
	//k = K;
  //cout<<"m::"<<m<<endl;
	bit_vector = new char[m];//dhmiourgia bit_vector kai arxikopoihsh toy 
	///for(int i = 0; i < m ;i ++)bit_vector[i] = '0';
  memset(bit_vector,'0',m);

}
Bloomfilter::~Bloomfilter()
{
	delete [] bit_vector;
};

//synarthsh epistrefei 0 an brethei estw kai ena 0 stis theseis pou einai h leksh me vasei tis hashfunction mas //dhladh epistrefei h 0 an den yparxei hdh
//epistrefei 1 an h leksh yparxei
int Bloomfilter::already_exists(char* mystring,int * result)//result einai o pinakas pou mas deixnei tis theseis ston bit_vector me vash tis hashfunction
{
  
	//int index = bloomhash(mystring);

	//int * result = hashCassandra(char* mystring,int m ,int k)

	int return_flag =1;
  for(int i =0;i< k ;i++)
  {
    return_flag = bit_vector[result[i]];
    if(return_flag == '0' )return 0;
  }
  //printf("for index : %d ,bit_vector[index] = %d\n\n",index,bit_vector[index]);
  //delete [] result;
  return 1;
};
void Bloomfilter:: print_bit_vector()
{
		for(int i = 0;i<m ;i++)	cout<<bit_vector[i]<<" ";
		cout<<"\n";

}

//synarthsh pou kanei insert thn leksh dhladh vazei stis antistoixes theseis tou bit_vector 1
int Bloomfilter:: insert_to_filter(int * result)
{
	
	for(int i = 0;i<k ;i++)	bit_vector[result[i]] ='1';

};
