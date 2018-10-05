#include "unit_test.h"
#include "heap.h"

int Unit_test:: check_insert(char * input,Index * & trie, BinaryMaxHeap*& binaryHeap)
{
	
	char *str = new char[BUFFER_SIZE];
	memset(str,'\0',BUFFER_SIZE);
	sprintf(str,"%s",input);
	char *str_to_check = new char[BUFFER_SIZE];
	memset(str_to_check,'\0',BUFFER_SIZE);
	sprintf(str_to_check,"%s",input);

	string l = trie->query(input,binaryHeap);
	char * input_line = new char[l.size() + 1];
	memset(input_line,'\0',l.size() + 1);
	memset(input_line,0,l.size() + 1);
	copy(l.begin(), l.end(), (char*)input_line);
	input_line[l.size()] = '\0';

	strcat(input_line,"|");
	strcat(str_to_check,"|");
	//if( strstr (input_line,str_to_check ) !=NULL ) cout<<"input already exists in Trie\n";
	//se periptwsh pou yparxei hdh kai den tha alla3ei kati h insert

	trie->insert(input);

	string line = trie->query(input,binaryHeap);
	char * input_line2 = new char[line.size() + 1];
	memset(input_line2,'\0',line.size() + 1);
	memset(input_line2,0,line.size() + 1);
	copy(line.begin(), line.end(), (char*)input_line2);
	input_line2[line.size()] = '\0';

	strcat(input_line2,"|");
	//cout<<"Check Inserting :: "<<input_line2<<endl;
	assert( (strstr (input_line2,str_to_check ) !=NULL ) && ":input wasn't inserted successfully string :: ");		//after inserting input to trie, if input does not exist in result return error

	delete []str;
	delete []str_to_check;
	delete []input_line;
	delete []input_line2;
	return 1;
}

int Unit_test::check_delete(char * input, Index *& trie, BinaryMaxHeap*& binaryHeap)
{
	char *str = new char[BUFFER_SIZE];
	memset(str,'\0',BUFFER_SIZE);
	sprintf(str,"%s",input);
	
	trie->insert(input);
	//cout<<"check_delete   input after insert : "<<input <<endl;

	trie->query(input,binaryHeap);
	//cout<<"check_delete   input after query : "<<input <<endl;
	trie-> delete_ngram(input);

	

	string line = trie->query(input,binaryHeap);
	char * input_line2 = new char[line.size() + 1];
	memset(input_line2,'\0',line.size() + 1);
	memset(input_line2,0,line.size() + 1);
	copy(line.begin(), line.end(), (char*)input_line2);
	input_line2[line.size()] = '\0';

	strcat(input_line2,"|");
	strcat(str,"|");

	assert( (strstr (input_line2,str ) ==NULL ) && ":input wasn't deleted successfully");		//after inserting input to trie, if input does not exist in result return error

	delete [] str;
	delete[] input_line2;

}
//epistrefei 1 an ginei swsta h search alliws skaei sto assert
int Unit_test::check_search(char *input,Index * & trie, BinaryMaxHeap*& binaryHeap)
{
	char *str = new char[strlen(input)+1];
	sprintf(str,"%s",input);

	trie->insert(input);
	string line = trie->query(input,binaryHeap);

	char * input_line = new char[line.size() + 1];
	memset(input_line,0,line.size() + 1);

	copy(line.begin(), line.end(), (char*)input_line);
	input_line[line.size()] = '\0';

	strcat(input_line,"|");
	strcat(str,"|");

	assert( (strstr (input_line,str ) !=NULL ));

	delete []str;
	delete []input_line;
	return 1;
}

