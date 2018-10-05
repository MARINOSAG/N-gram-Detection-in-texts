#include "static_trie.h"
#include "static_unit_test.h"
#include "static_func.h"
#include "static_hashTable.h"
#include "static_heap.h"
#include "static_hashTableHeap.h"
#include "job_scheduler.h"
#include <string>
#include <sstream>
#include <fstream>
Bloomfilter  * mybloom ;

int main(int argc, char *argv[])
{
	char init_filename[BUFFER_SIZE];
	char  query_filename[BUFFER_SIZE];
	int unit_test = 0 ;//flag for unitest
	char unitest_results[BUFFER_SIZE];
//	mybloom = new Bloomfilter();

	if(argc <5 || argc >7)
	{
		cout<<"Invalid arguments "<<endl;
		return 1;
	}
	for(int i =1 ;i<argc;i++)
	{
		if(!strcmp(argv[i],"-i") )
		{
			strcpy (init_filename,argv[i+1]);
			i++;
		}
		else if(!strcmp(argv[i] , "-q") )
		{
			strcpy(query_filename , argv[i+1]);
			i++;
		}
		else if (!strcmp(argv[i],"-untest" ) )
		{
			unit_test = 1;
			strcpy(unitest_results,argv[i+1]);
			i++;
		}
		else 
		{
			cout<<"Unknown flag"<<endl;
			return 1;
		}

	}

	ifstream init_file(init_filename);		//open file to read
	ifstream work_file(query_filename);		//open file to read
	string line;
	char *ch;
	Index *trie = new Index();
	//hashTable hash(m,2,LOAD_FACTOR);
	hashTable * hash= new hashTable(m,2000,LOAD_FACTOR);
	trie->create(hash);
  	ofstream myfile;
  	
	std::getline(init_file, line);//ayto to exoume gia na diavazoume thn prwth grammh DYNAMIC / STATIC
	while (std::getline(init_file, line))	//read line from init_file
	{ 		
 		//convert line for string to char*
		char * input_line = new char[line.size() + 1];
		memset(input_line,0,line.size() + 1);
		copy(line.begin(), line.end(), (char*)input_line);
		input_line[line.size()] = '\0';

		trie->insert(input_line);

		delete []input_line;
	}
	if(unit_test)
		myfile.open ("output.txt");

	int * empty_query_table = new int[BUFFER_SIZE];
	int counter =0, top ,empty_query_counter=0;
	string query_result, top_result;
	//int* empty_query_table=NULL; 

	//this structure is a Heap to find top K ngrams
	BinaryMaxHeap *binaryHeap = NULL;
	hashTableHeap *hash_Heap = NULL;

	int number_of_queries = BUFFER_SIZE, query_counter = 0;
	//char **query_queue = NULL;
	ArgumentStruct* arg = new ArgumentStruct();
	arg->trie = trie;
	JobScheduler * job_scheduler = NULL;
	job_scheduler = new JobScheduler();
	job_scheduler->initialize_scheduler(4,10);
cerr<<"START WORK FILE\n\n";

	while (std::getline(work_file, line))	//read line from work_file
	{ 	
 		//convert line for string to char*
		char * input_line = new char[line.size() + BUFFER_SIZE];
		memset(input_line,'\0',line.size() + BUFFER_SIZE);
		copy(line.begin(), line.end(), input_line);

		if(is_empty_string(input_line) == -1)
		{
			ch = strtok(input_line, " ");
			
			if(strcmp(ch,"Q") == 0)
			{
				ch = strtok(NULL, "\n");
				Job *job = new Job(ch, query_counter);
				job_scheduler->submit_job(job);
				query_counter++;
			}
			else if(strcmp(ch,"F") == 0)
			{
				ch = strtok(NULL, " ");

				binaryHeap = new BinaryMaxHeap();
				hash_Heap = new hashTableHeap(m2,2,LOAD_FACTOR2);

				string *query_results_buffer = new string[query_counter];
				
				job_scheduler->queryarg.binarymaxheap = binaryHeap;
				job_scheduler->queryarg.hashheap = hash_Heap;


				
				arg->binarymaxheap = binaryHeap;
				arg->hash_Heap = hash_Heap;

				arg->query_results_buffer = query_results_buffer;
				job_scheduler->execute_all_jobs(arg);
				job_scheduler->wait_all_tasks_finish();
				
				for(int ii=0;ii<empty_query_counter;ii++)	query_results_buffer[empty_query_table[ii]] = "-1";

				for(int ii=0;ii<query_counter;ii++)	cout<<query_results_buffer[ii]<<endl;
				empty_query_counter = 0;
				job_scheduler->clearqueue();

				query_counter = 0;
				delete []query_results_buffer;
				top = atoi(ch);
				if(top > 0)
				{
					cout<<"Top: ";
					top_result = binaryHeap->top(top);
					cout<<top_result<<endl;
					//after printing top k for this batch, delete binaryHeap in order to create an empty-new one for the next batch
					delete binaryHeap;
					binaryHeap = NULL;
					delete hash_Heap;
					hash_Heap = NULL;
					}
					else cout<<"Please give a positive integer in TOP K"<<endl;
				
			}
		}
		else
		{
			if(input_line[0] == 'Q')	
			{
				empty_query_table[empty_query_counter] = query_counter;
				query_counter++;
				empty_query_counter++;
			}
			else if(input_line[0] == 'F')	
			{
				string *query_results_buffer = new string[query_counter];
				
				job_scheduler->queryarg.binarymaxheap = binaryHeap;
				job_scheduler->queryarg.hashheap = hash_Heap;


				
				arg->binarymaxheap =NULL ;// binaryHeap;
				arg->hash_Heap = NULL; //hash_Heap;

				arg->query_results_buffer = query_results_buffer;
				job_scheduler->execute_all_jobs(arg);
				job_scheduler->wait_all_tasks_finish();
				
				for(int ii=0;ii<empty_query_counter;ii++)	query_results_buffer[empty_query_table[ii]] = "-1";
				for(int ii=0;ii<query_counter;ii++)	cout<<query_results_buffer[ii]<<endl;
				empty_query_counter = 0;
				job_scheduler->clearqueue();

				query_counter = 0;
				delete []query_results_buffer;
				
				delete binaryHeap;
				binaryHeap = NULL;
				delete hash_Heap;
				hash_Heap = NULL;
			}
		}
		delete []input_line;
	}

	delete arg;
	delete job_scheduler;
	delete trie;
	delete hash;
	delete [] empty_query_table;



	return 0;
}
