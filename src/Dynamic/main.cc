#include "trie.h"
#include "unit_test.h"
#include "func.h"
#include "hashTable.h"
#include "heap.h"
#include "hashTableHeap.h"
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
	mybloom = new Bloomfilter();

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
	hashTable hash(m,2,LOAD_FACTOR);
	trie->create(hash);
  	ofstream myfile;
  		
  	int current_version =0,A_version=0,D_version=0,is_previous_Q=0;//to is_previous_Q einai metablhth pou einai 0 h 1 an h proigoumenh entolh htan Q

	//std::getline(init_file, line);//ayto to exoume gia na diavazoume thn prwth grammh DYNAMIC / STATIC

	while (std::getline(init_file, line))	//read line from init_file
	{ 		
 		//convert line for string to char*
		char * input_line = new char[line.size() + 1];
		//memset(input_line,0,line.size() + 1);
		copy(line.begin(), line.end(), (char*)input_line);
		input_line[line.size()] = '\0';

		trie->insert(input_line,A_version);

		delete []input_line;
	}
	// if(unit_test)
	// 	myfile.open ("output.txt");

	int * empty_query_table = new int[BUFFER_SIZE];
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int counter =0, top,  empty_query_counter=0;
	string query_result, top_result;
	//this structure is a Heap to find top K ngrams
	BinaryMaxHeap *binaryHeap = NULL;
	hashTableHeap *hash_Heap = NULL;

	int query_counter = 0;
	JobScheduler * job_scheduler = NULL;
	job_scheduler = new JobScheduler();
	job_scheduler->initialize_scheduler(10,10);
	ArgumentStruct* arg = new ArgumentStruct();

//return 1;
cerr<<"STARTING WORK FILE \n\n\n";
	int at_least_one_query=0; //flag poy mas deixnei an symbei ena toylaxiston query

	while (std::getline(work_file, line))	//read line from work_file
	{ 	
 		//convert line for string to char*
		char * input_line = new char[line.size()+1]; //+ BUFFER_SIZE];
		//memset(input_line,'\0',line.size() + BUFFER_SIZE);
		copy(line.begin(), line.end(), input_line);
		input_line[line.size()] = '\0';

		if(is_empty_string(input_line) == -1)
		{
			ch = strtok(input_line, " ");
			
			//if(strcmp(ch,"Q") == 0)
			if(*ch == 'Q')
			{
				at_least_one_query =1;
				is_previous_Q=1;
				ch = strtok(NULL, "\n");

				Job *job = new Job(ch, query_counter,current_version);
				job_scheduler->submit_job(job);
				query_counter++;
			}
			//else if(strcmp(ch,"A") == 0)
			else if(*ch == 'A')
			{
				if(is_previous_Q ==1)//an to proigoumeno erwthma htan query 
				{
					current_version++;
					A_version = current_version;
					is_previous_Q=0;
				} 

				ch = strtok(NULL, "\n");
				trie->insert(ch,current_version);
			}
			//else if(strcmp(ch,"D") == 0)
			else if (*ch =='D')
			{
				if(is_previous_Q ==1)//an to proigoumeno erwthma htan query 
				{
					current_version++;
					D_version = current_version;
					is_previous_Q=0;
				} 
				ch = strtok(NULL, "\n");

				trie->delete_ngram2(ch,current_version);
			}
			//else if(strcmp(ch,"F") == 0)
			else if (*ch == 'F' )
			{
				is_previous_Q=0;
				ch = strtok(NULL, " ");

				if(at_least_one_query ==0)continue;

				if(binaryHeap == NULL )
				{
					binaryHeap = new BinaryMaxHeap();
				
				}
				if(hash_Heap ==NULL) hash_Heap = new hashTableHeap(m2,2,LOAD_FACTOR2);
				string *query_results_buffer = new string[query_counter];
				
				arg->trie = trie;

				job_scheduler->queryarg.binarymaxheap = binaryHeap;

				job_scheduler->queryarg.hashheap = hash_Heap;


				
				arg->binarymaxheap = binaryHeap;
				arg->hash_Heap = hash_Heap;

				arg->query_results_buffer = query_results_buffer;
				job_scheduler->execute_all_jobs(arg);
				job_scheduler->wait_all_tasks_finish();
				
				for(int ii=0;ii<empty_query_counter;ii++)	query_results_buffer[empty_query_table[ii]] = "-1";
				empty_query_counter = 0;

				for(int ii=0;ii<query_counter;ii++)	cout<<query_results_buffer[ii]<<endl;

				query_counter = 0;
				job_scheduler->clearqueue();

				
				delete []query_results_buffer;
				at_least_one_query =0;

				if(ch == NULL) 
				{
					delete binaryHeap;
					binaryHeap = NULL;
					delete hash_Heap;
					hash_Heap = NULL;
					delete []input_line;
					continue;
				}
				//case where read an F and a number
				if(ch != NULL)	top = atoi(ch);
				{
					if(top > 0)
					{
						if(binaryHeap != NULL)
						{
											//cerr<<"yoleeeeeeee\n\n\n";

							//binaryHeap->print();
							cout<<"Top: ";
							top_result = binaryHeap->top(top);
							cout<<top_result<<endl;

							//hash_Heap->print();

							//after printing top k for this batch, delete binaryHeap in order to create an empty-new one for the next batch
							delete binaryHeap;
							binaryHeap = NULL;
							delete hash_Heap;
							hash_Heap = NULL;
											//cerr<<"yoleeeeeeee\n\n\n";

							//cout<<"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAMMMMMMMMMMMMMMMMMMMMMMMMMMMMMAAAAAAAAAAAAAAAAAAAAAANNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN\n\n\n";
						}
					}
					else cout<<"Please give a positive integer in TOP K"<<endl;
				}
			}
			delete []input_line;
		}
		else
		{
			is_previous_Q=0;
			if(input_line[0] == 'Q')//kratame tis theseis twn kenwn query dhladh aytwn poy exoyn apotelesma "-1"	
			{
				at_least_one_query=1;
				empty_query_table[empty_query_counter] = query_counter;
				query_counter++;
				empty_query_counter++;

				is_previous_Q =1;
				
				// if(unit_test)//ektypwnoume to apotelesma kai sto output.txt se periptwsh pou theloume na kanoyme unittest
				// {
				// 	myfile<<"-1"<<endl;
				// }
			}
			else if(input_line[0] == 'F')	//keno Top apla ektyponontai ta querys
			{
				is_previous_Q =0;
				//cerr<<"F to gamimeno\n\n";
				if(at_least_one_query ==0)continue;

				string *query_results_buffer = new string[query_counter];
				
				arg->trie = trie;
				job_scheduler->queryarg.binarymaxheap = binaryHeap;
				job_scheduler->queryarg.hashheap = hash_Heap;


				
				arg->binarymaxheap = NULL;//binaryHeap;
				arg->hash_Heap = NULL ;//hash_Heap;
				arg->query_results_buffer = query_results_buffer;				
				job_scheduler->execute_all_jobs(arg);
				job_scheduler->wait_all_tasks_finish();

				
				for(int ii=0;ii<empty_query_counter;ii++)	query_results_buffer[empty_query_table[ii]] = "-1";

				for(int ii=0;ii<query_counter;ii++)	cout<<query_results_buffer[ii]<<endl;
				empty_query_counter = 0;

				query_counter = 0;
				job_scheduler->clearqueue();
				delete []query_results_buffer;
				at_least_one_query=0;
			}
			else if (input_line[0] == 'D')
			{
				if(is_previous_Q ==1)//an to proigoumeno erwthma htan query 
				{
					current_version++;
					D_version = current_version;
					is_previous_Q=0;
				} 

			}
			else if (input_line[0] == 'A')
			{
				if(is_previous_Q ==1)//an to proigoumeno erwthma htan query 
				{
					current_version++;
					A_version = current_version;
					is_previous_Q=0;
				} 
				
			}
			delete []input_line;
		}

	}

	delete arg;
	delete[]empty_query_table;
	delete mybloom ;
	delete trie;
delete job_scheduler;

	return 0;
}
