#include "trie.h"
#include "func.h"
#include "bloomfilter.h"
//static int A_version_counter = 0 ; 
Trie_node::Trie_node(char *word, int final, int level,int a_version,int D_version)
{

	int i;
	n = N;
	this->A_version = a_version;
	this->D_version = -1;//D_version ;
	this->level = level;
	this->current_children = 0;
	this->word = new char[strlen(word)+1];
	memset(this->word,'\0',strlen(word)+1);
	sprintf(this->word,"%s",word);
   // this->children = new Trie_node[n];//desmeboume apo thn arxi xwro gia ta paidia 
	this->children = (Trie_node *)malloc(n*sizeof(Trie_node));
	if(final)is_final = '1';
	else is_final ='0';

	//cerr<<"A_v = "<<this->A_version <<" D_v= "<<this->D_version<<"  final = "<<final<<endl;
}

Trie_node::~Trie_node()
{
	
	if(word != NULL) delete []word;
	word = NULL;
	
	if (children != NULL)
	{
		for(int i = 0 ;i<current_children;i++)
		{
			//delete children[i];
			children[i].~Trie_node();
		}
		free(children);
	}
	children = NULL;
}

int Index::insert(char *word,int A_version ){
	int val;
	Trie_node *p;
	val = insert_to_trie(word,p, A_version );//anathetoume sto p ton deikth tou kombou pou vrisketai sto prwto epipedo 
	hashIndex->insert(p->get_word(),p);//kai meta ton kanoume insert sto hashtable

	return val;
}

int  Index::insert_to_trie(char *ngram, Trie_node*& returned_node,int A_version )
{
	int index, size_of_ngram;
	char **ngram_table;
	Trie_node *temp_node;
	//split ngram in words
	size_of_ngram = sentence_split(ngram,ngram_table);


		int final;
		//if ngram has only 1 word set final = 1
		if(0 == size_of_ngram-1) final = 1;//an einai mono mia le3h tote to anathetoume se final
		else final = 0;

		//search for this key in Hash Table
		Trie_node *temp = hashIndex->find(ngram_table[0]);
		//if key does not exist create a new Trie Node
		if(temp == NULL)
		{
			if(final == 1)	temp = new Trie_node(ngram_table[0],final,0,A_version,-1);
			else temp = new Trie_node(ngram_table[0],final,0,-1,-1);//an einai parapanw apo mia leksh
		}
		//else update the value FINAL of existing Trie Node
		//an yparxei sto hashtable  kai einai mono mia leksh tote kanoume apla update
		else if(final == 1 && temp->get_A_version() == -1)
		{
			temp->set_is_final('1');
			temp->set_A_version(A_version);
			//cerr<<"mphke eeeeeeeeeeeeeeeeeeeeeeeee\n\n\n\n\n";
			//exit(1);
		}
		else //an yparxei sto hashtable alla den einai mono mia leksh 
		{
			//temp->set_A_version(-1); //an den einai h final leksh tou ngram A_version = -1

		}
		returned_node = temp;
		temp_node = temp;
		


	for(int i=1;i<size_of_ngram;i++)
	{	
		
		
				//if word is the last word set final = 1
		if(i == size_of_ngram-1) index = temp_node->insert_to_level(ngram_table[i],1,i,A_version);
		//else final = 0
		//an den einai h teleytaia leksh tou ngram
		else index = temp_node->insert_to_level(ngram_table[i],0,i,-1); //an den einai h final leksh tou ngram A_version = -1

		temp_node = &temp_node->get_children()[index];
	
	//for level = 0 keep the TrieNode* to insert it to Hash Table
	//if(i == 0)	returned_node = &temp_node->get_children()[index];

	}

	for(int i=0;i<size_of_ngram;i++)
		delete []ngram_table[i];
	delete []ngram_table;

	return 1;
}

int Trie_node::insert_to_level(char *word, int final, int level,int A_version )
{
	int index;

	index = binary_search(0, current_children-1, word);

	//double the size of table
	if( (current_children == n ) && (index == -1) )		double_table();

	if( index == -1)//an h leksh den yparxei sto level
	{
		index = current_children;
		
		int array_index ;
		int flag = 0 ; 
		for(int  i = 0 ; i< current_children;i ++)
		{
			if(strcmp(children[i].word,word) > 0)
			{
				memmove(&children[i+1],&children[i],(current_children -i  )*sizeof(Trie_node));
				new (& children[i]) Trie_node(word, final, level,A_version,-1); //dhmiourgia kainourgiou Trie_node 
				index = i ;
				flag =1;
				break;
			}

		}
		if(flag ==0)
		{
			new (& children[current_children]) Trie_node(word, final, level ,A_version,-1); //dhmiourgia kainourgio
		} 
		current_children++;
		/*temp->word = NULL;
		temp->children = NULL;
		delete temp;*/
	}
	//if the word we want to insert exists in a level and this word is the last of the ngram we want to insert
	//set this level final
	else if(final == 1 && children[index].A_version == -1)//an h leksh yparxei sto level kai einai kai final tou ngram
	{
		children[index].is_final = '1';
		children[index].A_version = A_version;
		children[index].D_version =-1; 
	}
	return index;	
}

int Trie_node::binary_search(int l, int r, char *word)
{

	if (r >= l)
   	{
        int mid = l + (r - l)/2;
 
        // If the element is present at the middle itself
        if(strcmp(children[mid].word,word) == 0) return mid;
 
        // If element is smaller than mid, then it can only be present
        // in left subarray
        if (strcmp(children[mid].word,word) > 0) return binary_search(l, mid-1, word);
 
        // Else the element can only be present in right subarray
        return binary_search(mid+1, r, word);
   }
 
   // We reach here when element is not present in array
   return -1;

}

void Trie_node::double_table()
{
	/*Trie_node * temp =(Trie_node*) malloc(2*n*sizeof(Trie_node) );
	memcpy(temp, children,n*sizeof(Trie_node));
	free(children);
	children= temp;
	n = 2*n ;*/
	n = 2*n;
	children = (Trie_node * )realloc(children,n * sizeof(Trie_node));

}

void Trie_node::print_level()
{
	for(int i=0;i<current_children;i++) cout<<children[i].word<<"   ";
	cout<<endl;
}

void Trie_node::print()
{
	
	if(is_final == '1') 
	{
		cout<<"is_final = "<<is_final;
		printf("  %*s-%s,FINAL\n",2*(level),"",word);
	}
	else 
	{
		cout<<"is_final = "<<is_final;
		printf("  %*s-%s\n",2*(level),"",word);
	}
	for(int i=0;i<current_children;i++)children[i].print();
}
//returns 0 = "not found" and 1 "found"
int Index :: search_sentence(int start,int end,char ** sentence_table,Trie_node* & temp_pointer,int query_version)
{
	Trie_node * temp_node;
	int found;
	//int x = temp_node -> current_children;
	
	if(temp_pointer !=NULL)//se periptwsh pou se proigoumenh fash to ngram pou brhke den htan final h teleytaia leksh 
	{
		
		
		temp_node = temp_pointer;
		//for(int i =start ;i<)
		//cerr<<"binary\n";
		found = temp_node->binary_search(0,temp_node->get_current_children()-1,sentence_table[end]); //searching for the sentence_table[i] word
		//case where i word is not found in level i of the trie--->then return 0
		temp_pointer = NULL;

		Trie_node * children =  temp_node->get_children();

		if(found == -1) return 0;

		//case where i word is found in level i of the trie and word is final--->return 1
		else if ((children[found].get_is_final() == '1')  )
		{
			if(children[found].get_D_version() ==-1)//an den exei diagrafei
			{
				if(children[found].get_A_version()<= query_version)
				{
					
					return 1;
				}
			}
			else
			{
				if(children[found].get_A_version()<= query_version && children[found].get_D_version() >query_version  )
				{
					return 1;
				}
			}		
		}
		//an bretheikei h leksh sentence_word[i] alla den htan final 
		//tote vazoume ton temp na deixnei sta paidia ths

		temp_node = &children[found];
		temp_pointer= temp_node;
		if(found != -1) //se periptwsh pou h teleytaia den einai final 
		{
			temp_pointer = temp_node;
			return 2;
		}
		temp_pointer = NULL;
		return 0;
	}

//cerr<<"BBBBBBBBBBBBBBBBBBBBBBBBBBBBB\n";
temp_node = hashIndex->find(sentence_table[start]);

			//case where i word is not found in level i of the trie--->then return 0
			if(temp_node == NULL) return 0;
			//case where i word is found in level i of the trie and word is final--->return 1
			else if ((temp_node->get_is_final() == '1') && (start == end) )
			{
				if(temp_node->get_D_version() ==-1)
				{
					if(temp_node->get_A_version()<= query_version)
						return 1;
				}
				else
				{
					if(temp_node->get_A_version()<= query_version && temp_node->get_D_version() >query_version  )
					{
						return 1;
					}
				}
			}
			found = 1;

	for (int i = start+1 ;i<=end;i++)
	{
	
			found = temp_node->binary_search(0,temp_node->get_current_children()-1,sentence_table[i]); //searching for the sentence_table[i] word
			Trie_node * children = temp_node->get_children();
			//case where i word is not found in level i of the trie--->then return 0
			if(found == -1) return 0;
			//case where i word is found in level i of the trie and word is final--->return 1
			else if ((children[found].get_is_final() == '1') && (i == end) )
			{
				if(children[found].get_D_version() ==-1)
				{
					if(children[found].get_A_version()<= query_version)
					{
						return 1;
					}
				}
				else
				{
					if(children[found].get_A_version()<= query_version && children[found].get_D_version() >query_version  )
					{
						return 1;
					}
				}
				
			}
			//an bretheikei h leksh sentence_word[i] alla den htan final 
			//tote vazoume ton temp na deixnei sta paidia ths
			temp_node = &temp_node->get_children()[found];
	}
	//case where all words have been found in trie but last word was not final
	//in this case we dont want to break the loop in Index::query because we will probably find a final word
	//to the next iteration of the loop--->return 2
	if(found != -1)
	{
		temp_pointer = temp_node;
		return 2;
	}
	temp_pointer = NULL;
	return 0;
} 
void write_to_binary_heap(queryArg * arg,char * temp)//function that inserts a word to binary maxheap
{

	arg->heap_mutex.lock();

		while(arg->writers>0)
		{
			arg->write_heap_cond->wait();
		}

			arg->writers ++;
	arg->heap_mutex.unlock();


	//afou paroume ton condition variable pame kai prosthetoume sto temp



	HeapNode * heap_node =  arg->hashheap->find(temp);
	if (heap_node == NULL)//an den yparxei sto hashtable
	{
		heap_node = arg->binarymaxheap->insert(temp,  arg->hashheap);
		arg->hashheap -> insert(temp,heap_node);
	}
	else
	{
		heap_node->add_one_value(arg->binarymaxheap,  arg->hashheap);
	}
	arg->heap_mutex.lock();
		arg->writers --;
		arg->write_heap_cond->signal();
	arg->heap_mutex.unlock();


}


string  Index:: query(char * sentence,queryArg * arg,int query_version )//find all Ngrams in trie that exist in sentence
{
	char ** sentence_table; 
	int start=0, end , current_size=0; 
	int buff_size = BUFFER_SIZE;
	char * result = (char*)malloc(buff_size*sizeof(char));//new char [buff_size];
	memset(result ,0,buff_size);

	int number_of_words = sentence_split(sentence,sentence_table);	
	start = 0;
	
	//Bloomfilter  * mybloom = new Bloomfilter();
	//mybloom->init_vector();
	Trie_node * temp_pointer=NULL;

	Bloomfilter  * mybloom = new Bloomfilter();
	mybloom->init_vector();
	int * bit_vector_result = new int[K];

	while(start < number_of_words)//here we find all the subsentences 
	{
		end=start;
		temp_pointer=NULL;
		while(end < number_of_words)
		{			
			int found = search_sentence(start,end, sentence_table,temp_pointer,query_version);
			if(found == 1)//an bretheike kai einai final
			{
				char *temp = new char [buff_size];
				memset(temp,'\0',buff_size);
				for(int i = start ;i<end ; i++)
				{
						strcat(temp,sentence_table[i]);
						strcat(temp, " ");
						//if(i != end) strcat(temp, " ");
						//else strcat (temp,"|");
				}
				strcat(temp,sentence_table[end]);
				strcat (temp,"|");
				hashCassandra(temp,M,K,bit_vector_result);
				if(! mybloom->already_exists(temp,bit_vector_result))//an den yparxei 
				{
					int size=0;
					for (int i = start;i<=end;i++)
					{  	
						size +=strlen(sentence_table[i]) +1;
					}
					current_size += size+1;
					if(current_size > buff_size)//periptwsh gemismatos pinaka result 
					{
						buff_size*=2;
						result = (char*) realloc(result ,buff_size*sizeof(char));
					}
					
					strcat(result,temp);
					temp[strlen(temp)-1] = '\0';

					if(arg->hashheap!= NULL ) write_to_binary_heap(arg,temp);

					mybloom->insert_to_filter(bit_vector_result);
				}

				//delete []bit_vector_result;

				delete []temp;
			}
			else if(found != 2) break;
			end++;
		}
	start++;
	}
	//ayto afairei kai thn teleytaia '|  '
	delete []bit_vector_result;

	result[strlen(result)-1] ='\0';
	string temp_string;	
	if(strlen(result) == 0)
	{	
		string output_string("-1");
		temp_string = output_string;

	}
	else
	{
		string output_string(result); 
		temp_string = output_string;
	}
	for(int i=0;i<number_of_words;i++)	delete []sentence_table[i];
	delete []sentence_table;
	free(result);
	delete mybloom;
	return temp_string;	

}

//if the ngram doesnt exist we return 0
int Index::delete_ngram2(char *ngram,int d_version)
{
	char ** sentence_table; 
	int number_of_words = sentence_split(ngram,sentence_table);
	int i,*index; 

	Trie_node *temp_node,*last;
	Trie_node **trie_level_pointer = new Trie_node*[number_of_words];	//in this table we will save pointers to the levels of the trie
	index = new int[number_of_words];
	for(i=0;i<number_of_words;i++)	trie_level_pointer[i] = NULL;

	//temp_node = root;
	//here we search for the sentence in our trie  and if the sentence exitsts, we save the pointers of the Trienodes we want to delete and the index of the children array of the pointer 
	int found;
		//if word is the first one then search in Hash Table
		
	temp_node = hashIndex->find(sentence_table[0]);
	last =temp_node;
	if(temp_node == NULL)
	{
		delete []index;
		delete []trie_level_pointer;
		for(int i=0;i<number_of_words;i++)	delete []sentence_table[i];
		delete []sentence_table;
		return 0;
	}
	else  trie_level_pointer[0] = temp_node;
		

	for (int i = 1 ;i<number_of_words;i++)
	{
		//int found;
		//if word is the first one then search in Hash Table
		
		found = temp_node->binary_search(0,temp_node->get_current_children()-1,sentence_table[i]); //searching for the sentence_table[i] word
		
		if(found == -1)
		{
			delete []index;
			delete []trie_level_pointer;
			for(int i=0;i<number_of_words;i++)	delete []sentence_table[i];
			delete []sentence_table;
			return 0;
		}
		else  
		{
			trie_level_pointer[i] = &temp_node->get_children()[found];
			index[i] = found;
		}
		last = temp_node;
		temp_node = &temp_node->get_children()[found];
	}
//	cerr<<d_version<<"\n\n\n";
	temp_node->set_D_version(d_version);

	//trie_level_pointer[i-1]->set_D_version(d_version);
	//last ->set_D_version(d_version);
	delete []index;
	//delete []trie_level_pointer;
	for(int i=0;i<number_of_words;i++)	delete []sentence_table[i];
	delete []sentence_table;
return 1;


}

int Index::delete_ngram(char *ngram)
{
	char ** sentence_table; 
	int number_of_words = sentence_split(ngram,sentence_table);
	int i,*index; 

	Trie_node *temp_node;
	Trie_node **trie_level_pointer = new Trie_node*[number_of_words];	//in this table we will save pointers to the levels of the trie
	index = new int[number_of_words];
	for(i=0;i<number_of_words;i++)	trie_level_pointer[i] = NULL;

	//temp_node = root;
	//here we search for the sentence in our trie  and if the sentence exitsts, we save the pointers of the Trienodes we want to delete and the index of the children array of the pointer 
	for (int i = 0 ;i<number_of_words;i++)
	{
		int found;
		//if word is the first one then search in Hash Table
		if(i == 0)
		{
			temp_node = hashIndex->find(sentence_table[i]);

			if(temp_node == NULL)
			{
				delete []index;
				delete []trie_level_pointer;
				for(int i=0;i<number_of_words;i++)	delete []sentence_table[i];
				delete []sentence_table;
				return 0;
			}
			else  trie_level_pointer[i] = temp_node;
		}
		else 
		{
			found = temp_node->binary_search(0,temp_node->get_current_children()-1,sentence_table[i]); //searching for the sentence_table[i] word
		
			if(found == -1)
			{
				delete []index;
				delete []trie_level_pointer;
				for(int i=0;i<number_of_words;i++)	delete []sentence_table[i];
				delete []sentence_table;
				return 0;
			}
			else  
			{
				trie_level_pointer[i] = &temp_node->get_children()[found];
				index[i] = found;
			}
			temp_node = &temp_node->get_children()[found];
		}
	}
	//here since the sentence that we want to delete exists, we go and erase it 
	for (int i = number_of_words-1 ;i >= 0; i--)
	{   //an den exei paidia kai den einai final || an den exei paidia ,einai h teleytaia le3h kai einai final  -->tote pame kai thn diagrafoume
		if( ((trie_level_pointer[i]->get_current_children() == 0) && (trie_level_pointer[i]->get_is_final() == '0') ) || ((trie_level_pointer[i]->get_current_children() == 0) && (i == number_of_words-1)	&& (trie_level_pointer[i]->get_is_final() == '1')) )	//this node does not have any children
		{
			//trie_level_pointer[i]->~Trie_node();
			if(i != 0) //efoson o kombos den einai o root 
			{
				trie_level_pointer[i]->~Trie_node();
				//kanoume NULL ton deikkth toy patera gia thn thesh 'i' kai anadiamorfwnoyme ton pinaka twn deiktwn sta paidia toy 
				trie_level_pointer[i-1]->rearrange_table(index[i]);
				trie_level_pointer[i-1]->set_current_children(trie_level_pointer[i-1]->get_current_children()-1);//meiwnoume ta paidia toy patera kata 1

			}
			else //an o kombos einai o root kanw ta idia kai diagrafw thn leksi apo to Hash Table
			{
				//root->rearrange_table(0);
				//root->set_current_children(root->get_current_children()-1);
				hashIndex->delet(sentence_table[0]);
			}
		}
		//an o kombos pros diagrafi exei paidia kai einai final tote apla ton kanoume non final
		else if(i == number_of_words-1)	trie_level_pointer[i]->set_is_final('0');
	}

	delete []index;
	delete []trie_level_pointer;
	for(int i=0;i<number_of_words;i++)	delete []sentence_table[i];
	delete []sentence_table;

	return 1;

}

//when we delete a node from children_table a NULL pointer rests in the "middle" of the table
//whis funcion "sends" this NULL pointer to the end of the table and moves all the other elements by 1 place left
void Trie_node::rearrange_table(int place_to_start)
{
	//int i;
	//Trie_node *temp_node;
	/*for(i=place_to_start;i<current_children-1;i++)
	{
		memcpy(&children[i],& children[i+1],sizeof(Trie_node));
	}*/
	memmove(&children[place_to_start],&children[place_to_start+1] , (current_children - place_to_start)*sizeof(Trie_node));
}

void Trie_node::update_is_final(Trie_node* l)
{
	if(is_final == '0')	is_final = l->get_is_final();
}



