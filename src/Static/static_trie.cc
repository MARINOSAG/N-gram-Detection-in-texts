#include "static_trie.h"
#include "static_func.h"
#include "static_bloomfilter.h"

Trie_node::Trie_node(char *word, int final, int level)
{
	int i;
	n = N;
	this->level = level;
	this->current_children = 0;
	this->word = new char[strlen(word)+1];
	memset(this->word,'\0',strlen(word)+1);
	sprintf(this->word,"%s",word);

   // this->children = new Trie_node[n];//desmeboume apo thn arxi xwro gia ta paidia 
	this->children = (Trie_node *)malloc(n*sizeof(Trie_node));
	if(final)is_final = '1';
	else is_final ='0';

	words_array = NULL;
	words_length = NULL;
	size_words_array = BUFFER_SIZE;
	size_words_length = BUFFER_SIZE;
	current_len_words_array = 1;//current megethos buffer gia to key 
	current_len_words_length = 1;
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

	if(words_length != NULL)	
	{
		free(words_length);
		words_length = NULL;
	}
	if(words_array != NULL)		
	{
		free(words_array);
		words_array = NULL;
	}
}

int Index::insert(char *word){
	int val;
	Trie_node *p;
	val = insert_to_trie(word,p);//anathetoume sto p ton deikth tou kombou pou vrisketai sto prwto epipedo 
	hashIndex->insert(p->get_word(),p);//kai meta ton kanoume insert sto hashtable

	return val;
}

int  Index::insert_to_trie(char *ngram, Trie_node*& returned_node)
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
	if(temp == NULL)	temp = new Trie_node(ngram_table[0],final,0);
	//else update the value FINAL of existing Trie Node
	else if(final == 1) temp->set_is_final('1');
	returned_node = temp;
	temp_node = temp;



	for(int i=1;i<size_of_ngram;i++)
	{	
		//insert to Hash Table if level = 0
	
		//if word is the last word set final = 1
		if(i == size_of_ngram-1) index = temp_node->insert_to_level(ngram_table[i],1,i);
		//else final = 0
		else index = temp_node->insert_to_level(ngram_table[i],0,i);

		temp_node = &temp_node->get_children()[index];
	
		//for level = 0 keep the TrieNode* to insert it to Hash Table
		//if(i == 0)	returned_node = &temp_node->get_children()[index];

	}

	for(int i=0;i<size_of_ngram;i++)
		delete []ngram_table[i];
	delete []ngram_table;

	return 1;
}

int Trie_node::insert_to_level(char *word, int final, int level)
{
	int index;

	index = binary_search(0, current_children-1, word);

	//double the size of table
	if( (current_children == n ) && (index == -1) )		double_table();

	if( index == -1)
	{
		//Trie_node *temp = new Trie_node(); 
		index = current_children;
		//current_children++;

		//new (& children[current_children]) Trie_node(word, final, level); //dhmiourgia kainourgiou Trie_node 
		/*for(int i=current_children-1;i>0;i--)
		{
			if(strcmp(children[i].word,children[i-1].word) < 0)
			{
				//swaping the nodes
				memcpy(temp,&children[i],sizeof(Trie_node));
				memcpy(&children[i],&children[i-1],sizeof(Trie_node));
				memcpy(&children[i-1],temp,sizeof(Trie_node));
				index = i-1;


			}
			else break;
		}*/

		int array_index ; 
		int flag = 0 ; 
		for(int  i = 0 ; i< current_children;i ++)
		{
			if(strcmp(children[i].word,word) > 0)
			{
				memmove(&children[i+1],&children[i],(current_children -i  )*sizeof(Trie_node));
				new (& children[i]) Trie_node(word, final, level); //dhmiourgia kainourgiou Trie_node 
				index = i ;
				flag =1;
				break;
			}
		}
		if(flag ==0)
		{
			new (& children[current_children]) Trie_node(word, final, level); //dhmiourgia kainourgio
		} 
		current_children ++;

		/*temp->word = NULL;
		temp->children = NULL;
		temp->words_array = NULL;
		temp->words_length = NULL;
		delete temp;*/
	}
	//if the word we want to insert exists in a level and this word is the last of the ngram we want to insert
	//set this level final
	else if(final == 1)	children[index].is_final = '1';
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
	children= temp;*/

	n = 2*n ;
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
int Index::search_sentence(int start,int end,char ** sentence_table)
{
	Trie_node * temp_node;
	int found;
	char *node_words;
	short int *words_length, words_length_sum=0;

	temp_node = hashIndex->find(sentence_table[start]);

	//case where i word is not found in level i of the trie--->then return 0
	if(temp_node == NULL) return 0;
	//case where i word is found in level i of the trie and word is final--->return 1
	else if ((temp_node->get_is_final() == '1') && (start == end) )
	{
		return 1;
	}
	found = 1;
	

	for (int i = start+1 ;i<=end;i++)
	{
		//if word is the first one then search in Hash Table
		/*if(i == start)
		{
			temp_node = hashIndex->find(sentence_table[i]);

			//case where i word is not found in level i of the trie--->then return 0
			if(temp_node == NULL) return 0;
			//case where i word is found in level i of the trie and word is final--->return 1
			else if ((temp_node->get_is_final() == '1') && (i == end) )
			{
				return 1;
			}
			found = 1;
		}*/
		//else search in compressed node or in Trie levels
		//else 
		//{
			//if temp_node has been compressed then search next words to this node
			if(temp_node->get_current_len_words_length() > 1)
			{
				node_words = temp_node->get_words_array();
				words_length = temp_node->get_words_length();
				if(words_length[0] > 0)	words_length_sum = words_length[0];
				else words_length_sum = -words_length[0];
				for (int j = 1 ;j<temp_node->get_current_len_words_length();j++)
				{
					if(words_length[j] > 0)	
					{
						words_length_sum += words_length[j];
						char *word_to_check = new char[words_length[j]+1];
						//int word_index = 0;
						//sximatizoume mia mia tis lekseis apo to sunoliko string pou periexei oles tis lekseis mazi
						/*for(int k=words_length_sum - words_length[j]; k<words_length_sum; k++)
						{
							word_to_check[word_index] = node_words[k];
							word_index++;
						}*/
						memcpy(word_to_check,node_words,words_length[j]);
						node_words += words_length[j];

						word_to_check[words_length[j]] = '\0';
						//if a word of sentence_table is found in compressed node, then search for the next word of sentence_table in compressed node
						if(strcmp(word_to_check,sentence_table[i]) == 0)
						{
							delete [] word_to_check;
							if (i == end)	return 2;
							i++;
							continue;
						}
						delete [] word_to_check;

					}
					else
					{
						words_length_sum -= words_length[j];
						char *word_to_check = new char[-words_length[j]+1];
						int word_index = 0;
						/*for(int k=words_length_sum + words_length[j]; k<words_length_sum; k++)
						{
							word_to_check[word_index] = node_words[k];
							word_index++;
						}*/
						memcpy(word_to_check,node_words,-words_length[j]);
						node_words -= words_length[j];

						word_to_check[-words_length[j]] = '\0';
						//if a word of sentence_table is found in compressed node, then search for the next word of sentence_table in compressed node
						if(strcmp(word_to_check,sentence_table[i]) == 0)
						{
							//sentence found--->return 1
							delete [] word_to_check;

							if (i == end)	return 1;
							i++;
							continue;

						}
						delete [] word_to_check;

					}
					break;	

				}
			}
			//if sentence_table[i] hasn't been found in compressed node, then try to find it in Trie levels
			found = temp_node->binary_search(0,temp_node->get_current_children()-1,sentence_table[i]); //searching for the sentence_table[i] word
			//case where i word is not found in level i of the trie--->then return 0
			if(found == -1) return 0;
			//case where i word is found in level i of the trie and word is final--->return 1
			else if ((temp_node->get_children()[found].get_is_final() == '1') && (i == end) )
			{
				//cerr<<"lele"<<endl;
				return 1;
			}

			temp_node = &temp_node->get_children()[found];
		//}
	}
	

	//case where all words have been found in trie but last word was not final
	//in this case we dont want to break the loop in Index::query because we will probably find a final word
	//to the next iteration of the loop--->return 2
	if(found != -1) return 2;
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

string  Index:: query(char * sentence,queryArg * arg)//find all Ngrams in trie that exist in sentence
{

	char ** sentence_table; 
	int start=0, end , current_size=0; 
	int buff_size = BUFFER_SIZE;
	char * result = (char*)malloc(buff_size*sizeof(char));//new char [buff_size];
	memset(result ,0,buff_size);

	int number_of_words = sentence_split(sentence,sentence_table);	
	start = 0;
	
	Bloomfilter  * mybloom = new Bloomfilter();
	mybloom->init_vector();
	int * bit_vector_result = new int[K];
	while(start < number_of_words)//here we find all the subsentences 
	{
		end=start;
		while(end < number_of_words)
		{
			//cout<<start<<"   "<<end<<endl;
			int found = search_sentence(start,end, sentence_table);
			if(found == 1)
			{

				char *temp = new char [buff_size];
				memset(temp,0,buff_size);
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

					if(arg->hashheap !=NULL)	write_to_binary_heap(arg,temp);

					mybloom->insert_to_filter(bit_vector_result);
				}
				delete []temp;
			}
			else if(found != 2) break;
			end++;
		}
	start++;
	}
	delete []bit_vector_result;
	//ayto afairei kai thn teleytaia '|  '
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
	/*int i;
	Trie_node *temp_node;
	for(i=place_to_start;i<current_children-1;i++)
	{
		memcpy(&children[i],& children[i+1],sizeof(Trie_node));
	}*/
	memmove(&children[place_to_start],&children[place_to_start+1] , (current_children - place_to_start)*sizeof(Trie_node));

}

void Trie_node::update_is_final(Trie_node* l)
{
	if(is_final == '0')	is_final = l->get_is_final();
}

void Trie_node :: compress()
{
	Trie_node * temp = this;

	words_array = (char*)malloc(sizeof(char)*size_words_array);
	strcpy(words_array,word);

	words_length = (short int *)malloc(sizeof(short int )* size_words_length );
	if(is_final == '1')		words_length[0] = -1*(strlen(word));
	else  words_length[0] = strlen(word) ;

//cout<<words_array<<endl;
//cout<<words_length[0]<<endl;

	for(int i = 0 ;i<current_children;i++)	
		temp->children[i].compress();
	

	if(current_children == 1)//an exei mono ena paidi 
	{
		char * child_words_array = temp->children[0].get_words_array();
		short int * child_words_length = temp->children[0].get_words_length();
		int child_current_len_words_length = temp->children[0].get_current_len_words_length();
		int child_current_len_words_array = temp->children[0].get_current_len_words_array();
		//elegxoi diplasiasmou  pinakwn
		if( (current_len_words_array + child_current_len_words_array) > size_words_array)
		{
			size_words_array = 2* size_words_array;
			words_array = (char*)realloc(words_array,size_words_array);

		}
		if( ( current_len_words_length + child_current_len_words_length) > size_words_length )
		{
			size_words_length = 2* size_words_length;
			words_length = (short int *)realloc(words_length, size_words_length);

		}

		strcat(words_array ,child_words_array);
		current_len_words_length += child_current_len_words_length;
		//cout<<"words_array:"<<words_array<<endl;
		//cout<<"words_len:"<<words_length[0]<<endl;
		//cout<<"copying words_length\n\n";
		for(int i = 0 ;i<current_len_words_length-1; i++)
		{
			words_length[i+1] = child_words_length[i];
			//cout<<"words_len:"<<words_length[i+1]<<endl;
		}
		//cout<<"copying words_enddddd\n\n";

		Trie_node* child = temp->children;
		temp->set_current_children(temp->children[0].get_current_children());//exei tosa paidia oso to paidi toy twra
		//printf("found parent with one child \n\n");

		//temp->children = child[0].children;
		if(temp->children[0].get_current_children() == 0 )//an to paidi den exei paidia
		{
			//cout<<"\t\t\tto paidi DENNNN exei paidia\n\n";
			temp->children = NULL;
			child[0].~Trie_node();//katastrefw to paidi
			free(child);
			//delete [] child;
			child = NULL;

		}
		else
		//temp_children[0]  = NULL;
		{
		
			//cout<<"\t\t\tto paidi exei paidia \n\n";
			temp->children = child[0].children;
			child->children = NULL;
			child[0].~Trie_node();
			free(child);
			//delete [] child;
			child = NULL;
		}
		//free(child[0]);
		//free(temp_children);


		//temp_children.~Trie_node();
		//temp->children[0].~Trie_node();
	}
}



