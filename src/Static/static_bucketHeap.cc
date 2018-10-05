#include "static_hashTableHeap.h"


bucketHeap::bucketHeap(int num)
{
	current_size = 0;
	size = num;
	tnode = new bucketNode[num];
}

bucketHeap::~bucketHeap()
{
	for(int i=0;i<size;i++)	
	{
		if(tnode[i].key != NULL) 
		{
			//delete tnode[i].data;
			tnode[i].deleteBucket();
		}
	}
	delete []tnode;
}

void bucketHeap::insert(char* k,HeapNode* l)
{
	int i=0,c=0;
	while( (tnode[i].isFree() != NULL) && (i<size) )		
	{
		if(i<size-1) i++;
		else c++;
		if(c==1)
		{
			i++;
			break;
		}
	}
	//periptwsh opou uparxei xwros sto bucket gia insert neou
	if(i < size)
	{
		tnode[i].key = new char[strlen(k)+1];
		strcpy(tnode[i].key,k);
		tnode[i].data = l;	
		//print();cout<<endl<<endl;	
	}
	//periprwsh uperxeilishs kai diplasiasmou tou bucket
	else	
	{
		//print();cout<<endl<<endl;

		overflow();
		tnode[size/2].key = new char[strlen(k)+1];
		memset(tnode[size/2].key,'\0',strlen(k)+1);
		sprintf(tnode[size/2].key,"%s",k);
		tnode[size/2].data = l;
	}
	bucketSort();
	current_size++;
}

void bucketHeap::update_data_value(char*k, HeapNode*l)
{
	//cout<<"UPDATE::::: "<<k<<endl;
	for(int i=0;i<current_size;i++)
	{
		if(strcmp(tnode[i].key,k) == 0)
		{
			//tnode[i].data->add_one_value();
		}
	}
}

void bucketHeap::overflow()
{
	bucketNode *temp = new bucketNode[2*size];
	memcpy(temp, tnode, size*sizeof(bucketNode));
	//tnode->deleteBucket();
	/*for(int i=0;i<size;i++)
	{
		temp[i].key = tnode[i].key;
		temp[i].data = tnode[i].data;
	}*/
	delete []tnode;
	tnode = temp;
	size = size * 2;
	//delete []temp;
}

void bucketHeap::split(int round,int number_of_buckets,int bs,bucketHeap* &p)
{
	int i,h1,h2,size_to_reduce=0;
	
	for(i=0;i<size;i++)
	{
		if(tnode[i].isFree() != NULL)
		{
			h1 = hashFunc(round,hashFuncString(tnode[i].key),m2);
			h2 = hashFunc(round+1,hashFuncString(tnode[i].key),m2);
			//cout<<h1<<" "<<h2<<" "<<tnode[i].key<<round<<number_of_buckets<<endl;
			//if(h2 != bs)
			/*if((h1 > bs) && (h1 != number_of_buckets-1))	
			{
				p->move(tnode[i]);
				current_size--;
				tnode[i].key = NULL;
				tnode[i].data = NULL;
				rearrange_bucket(i);

				//tnode[i].~bucketNode();
			}*/
			if(h2 != bs)
			{
				p->move(tnode[i]);
				current_size--;
				tnode[i].key = NULL;
				tnode[i].data = NULL;
				rearrange_bucket(i);
				i--;
			}
		}
	}
}

void bucketHeap::print()
{
	int i;
	for(i=0;i<size;i++)
	{
		if(tnode[i].key != NULL)
		{
			cout<<tnode[i].key<<endl;
			//tnode[i].data->print();
		}
		else cout<<"empty:"<<i<<"---";
	}
}

int bucketHeap::hashFunc(int i,int k,int m)
{
	int result=1;
	if(i!=0)
	for(int j=1;j<=i;j++)	result = result*2;
	return k%(result*m);
}

int bucketHeap::hashFuncString(char *key)
{
	int count=0,i;
	for(i=0;i<strlen(key);i++) count += key[i];
	return count;
}

void bucketHeap::move(bucketNode &copy_from)
{
	//cout<<"MOVING:::::"<<copy_from.key<<endl;
	int i=0,c=0;
	while( (tnode[i].isFree() != NULL) && (i<size) )		
	{
		if(i<size-1) i++;
		else c++;
		if(c==1)
		{
			i++;
			break;
		}
	}
	if(i < size)
	{
		//tnode[i].key = new char[strlen(copy_from.key)+1];
		//strcpy(tnode[i].key,copy_from.key);
		tnode[i].key = copy_from.key;
		tnode[i].data = copy_from.data;
	}
	else	
	{
		//print();cout<<endl<<endl;
		overflow();
		//print();cout<<endl<<endl;

		//tnode[size/2].key = new char[strlen(copy_from.key)+1];
		//memset(tnode[size/2].key,'\0',strlen(copy_from.key)+1);
		//sprintf(tnode[size/2].key,"%s",copy_from.key);		
		tnode[size/2].key = copy_from.key;
		tnode[size/2].data = copy_from.data;
	}
	bucketSort();
	current_size++;
}

int bucketHeap::idExist(char* k)
{
	for(int i=0;i<size;i++)		
		if(tnode[i].key != NULL)
			if(strcmp(tnode[i].key,k) == 0)	return 1;
	return 0;
}

void bucketHeap::bucketSort()
{
	bucketNode *temp = new bucketNode();
	for(int i=current_size;i>0;i--)
	{
		if(tnode[i].key != NULL)
		{
			if(strcmp(tnode[i].key,tnode[i-1].key) < 0)
			{
				//swaping the nodes
				temp->key = tnode[i].key;
				temp->data = tnode[i].data;
				tnode[i].key = tnode[i-1].key;
				tnode[i].data = tnode[i-1].data;
				tnode[i-1].key = temp->key;
				tnode[i-1].data = temp->data;
				//memcpy(temp,&tnode[i],sizeof(bucketNode));
				//memcpy(&tnode[i],&tnode[i-1],sizeof(bucketNode));
				//memcpy(&tnode[i-1],temp,sizeof(bucketNode));
			}
			else break;
		}
	}
	delete temp;
}

int bucketHeap::delet(char* k)
{
	int i;
	for(i=0;i<current_size;i++)
	{
		if(strcmp(tnode[i].key,k) == 0)	break;
	}
	current_size--;
	delete tnode[i].data;
	tnode[i].deleteBucket();
	rearrange_bucket(i);
}


HeapNode* bucketHeap::find(char* k)
{
	return binary_search(0, current_size-1, k);
}

HeapNode* bucketHeap::binary_search(int l, int r, char *word)
{

	if (r >= l)
   	{
        int mid = l + (r - l)/2;
 
        // If the element is present at the middle itself
        if(strcmp(tnode[mid].key,word) == 0) return tnode[mid].data;
 
        // If element is smaller than mid, then it can only be present
        // in left subarray
        if (strcmp(tnode[mid].key,word) > 0) return binary_search(l, mid-1, word);
 
        // Else the element can only be present in right subarray
        return binary_search(mid+1, r, word);
   }
 
   // We reach here when element is not present in array
   return NULL;

}

void bucketHeap::rearrange_bucket(int place_to_start)
{
	int i;
	bucketNode *temp_node;
	for(i=place_to_start;i<current_size;i++)
	{
		//memcpy(&tnode[i],& tnode[i+1],sizeof(bucketNode));
		if(tnode[i+1].key != NULL)
		{
			tnode[i].key = tnode[i+1].key;
			tnode[i].data = tnode[i+1].data;
			tnode[i+1].key = NULL;
			tnode[i+1].data = NULL;
		}
	}
}

void bucketHeap::change_heapPointer(char *k, HeapNode* node_to_change)
{
	binary_search_change(0, current_size-1, k, node_to_change);	
}

void bucketHeap::binary_search_change(int l, int r, char *word, HeapNode* node_to_change)
{

	if (r >= l)
   	{
        int mid = l + (r - l)/2;
 
        // If the element is present at the middle itself
        if(strcmp(tnode[mid].key,word) == 0) {tnode[mid].data = node_to_change;return;}
 
        // If element is smaller than mid, then it can only be present
        // in left subarray
        if (strcmp(tnode[mid].key,word) > 0) return binary_search_change(l, mid-1, word, node_to_change);
 
        // Else the element can only be present in right subarray
        return binary_search_change(mid+1, r, word, node_to_change);
   }

   return ;
}