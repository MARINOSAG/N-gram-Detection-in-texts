#include "func.h"



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