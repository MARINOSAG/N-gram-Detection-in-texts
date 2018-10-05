#include "static_func.h"

int sentence_split(char *sentence, char **&words)
{
	char *saveptr;
	//char * temp_sentence = new char [strlen(sentence)+1];
	//memset(temp_sentence,0,strlen(sentence)+1);
	//sprintf(temp_sentence,"%s",sentence);

	int counter = 50000;
	char *sentence_copy = new char[strlen(sentence)+1];
	//memset(sentence_copy,'\0',strlen(sentence)+1);
	sprintf(sentence_copy,"%s",sentence);
	/*char *word = strtok_r(temp_sentence," ",&saveptr);
	while(word != NULL)
	{
		counter++;
		word = strtok_r(NULL," ",&saveptr);
	}*/
	char* word;
	words = new char*[counter];
	word = strtok_r(sentence_copy," ",&saveptr);
	counter = 0;
	while(word != NULL)
	{
		words[counter] = new char[strlen(word)+1];
		//memset(words[counter],'\0',strlen(word)+1);
		sprintf(words[counter],"%s",word);
		counter++;
		word = strtok_r(NULL," ",&saveptr);
	}
	delete []sentence_copy;
	//delete [] temp_sentence;
	return counter;
}

//this function delete duplicate substring of a string
//example: if sentence = "a|a|b|a|b|b" then return sentence is "a|b"
char* delete_duplicate(char* sentence)
{
	int size = strlen(sentence)+1;
	char* buffer = new char[size];
	
	char *ch;
	memset(buffer,'\0',size);

	ch = strtok(sentence, "|");
	while(ch != NULL)
	{
		char *temp = new char[strlen(ch)+2];
		memset(temp,'\0',strlen(ch)+2);

		memcpy(temp,ch,strlen(ch));
		strcat(temp ,"|\0");
		if(strstr(buffer,temp) == NULL)
		{
			strcat(buffer,temp);
		}

		ch = strtok(NULL,"|");
			delete []temp;

	}


	buffer[strlen(buffer)-1] = '\0';
	return buffer;
}

//a string that contains only spaces and \n
int is_empty_string(char *sentence)
{
	int i;
	for(i=1;i<strlen(sentence)-1;i++)
		if(sentence[i] != ' ' || sentence[i] != '\t')
			return -1;
	return 1;
}

void print_batch_results(char **buff, int size)
{
	for(int i=0;i<size;i++)
		cout<<buff[i]<<endl;
}

void insert_query_results_to_heap(BinaryMaxHeap * binaryHeap,hashTableHeap * hash_Heap, string *query_results_buffer,int query_counter)
{
	char *buff, *word;
	int counter = 0;

	for(int i=0;i<query_counter;i++)
	{
		buff = (char*) query_results_buffer[i].c_str();
		char * temp_sentence = new char [strlen(buff)+1];
		memset(temp_sentence,0,strlen(buff)+1);
		sprintf(temp_sentence,"%s",buff); //h temp sentence einai ena oloklhro result
//cout<<"buf::"<<buff<<endl;
		if(strcmp(temp_sentence,"-1") == 0)	continue;
		word = strtok(temp_sentence,"|");
		while(word != NULL)
		{
			char * yolo = (char*)malloc(strlen(buff));
			memset(yolo,0,strlen(buff));
			sprintf(yolo,"%s",word);
			yolo[strlen(yolo)] ='\0';
			//strcat(word,"|");
//cerr<<"word:"<<yolo<<" "<<counter<<endl;
			
			HeapNode * heap_node =  hash_Heap->find(yolo);
			

			if (heap_node == NULL)//an den yparxei sto hashtable
			{
				heap_node = binaryHeap->insert(yolo, hash_Heap);
				hash_Heap -> insert(yolo,heap_node);
			}
			else
			{
				heap_node->add_one_value(binaryHeap, hash_Heap);
			}

			counter++;
			word = strtok(NULL,"|");
			free(yolo);
		}
		delete []temp_sentence;
		buff = NULL;
	}
	//cout<<"................................................................."<<endl;
}
