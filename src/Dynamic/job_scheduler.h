#ifndef SCHEDULER
#define SCHEDULER

#include <pthread.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#include "hashTableHeap.h"
#include "trie.h"
#include "queryarg.h"
#include "mutex.h"
//epanaorismos twn mutex kai condition variable gia na douleuoun swsta sth C++
/*class Mutex
{
    friend class CondVar;
    pthread_mutex_t  m_mutex;
 
  public:
    Mutex() { pthread_mutex_init(&m_mutex, NULL); }
    virtual ~Mutex() { 
	    pthread_mutex_unlock(&m_mutex); 
	    pthread_mutex_destroy(&m_mutex); 
	}
 
    int lock() { return  pthread_mutex_lock(&m_mutex); }
    int trylock() { return  pthread_mutex_trylock(&m_mutex); }
    int unlock() { return  pthread_mutex_unlock(&m_mutex); }   
};

class CondVar
{
    pthread_cond_t  m_cond;
    Mutex&          m_lock;
 
  public:
    CondVar(Mutex& mutex) : m_lock(mutex) { pthread_cond_init(&m_cond, NULL); }
    virtual ~CondVar() { pthread_cond_destroy(&m_cond); }
 
    int wait() { return  pthread_cond_wait(&m_cond, &(m_lock.m_mutex)); }
    int signal() { return pthread_cond_signal(&m_cond); } 
    int broadcast() { return pthread_cond_broadcast(&m_cond); } 
 
  private:
    CondVar();
};
*/
class Job{ //klash gia ena job-query dhladh kratame to keimeno kai to query number Jobid

private:
	char* query = NULL;
	int jobId;
	int version;
public:
	Job(char *q, int id,int current_version)
	{

		query = new char[strlen(q)+1];
		memset(query,'\0',strlen(q)+1);
		sprintf(query,"%s",q);
		//query = q;
		this->version= current_version;
		jobId = id;
	}
	~Job()
	{
		if(query != NULL)
		{
			delete []query;
			query = NULL;
		}
	}
	static void *job_to_execute(void *argument);//synarthsh twn thread
	char *getQuery() {return query;}
	int getId() {return jobId;}
};

class Queue{

private:
	int max_size;
	int current_size;
	int size;
	int next_job_to_add;
	int next_job_to_pop;
	Job *jobs;

public:
	Queue(int);
	~Queue();
	Job* pop();
	void add(Job *j);
	void clear();
	void clear2();
	int get_size() {return current_size;}
	void set_max_size(int s) {max_size = s;}
	void reduce_size() {current_size--;}
	void add_next_job() {next_job_to_pop++;}

	char* getJob(int i){return jobs[i].getQuery();}
	int getJobId(int i){return jobs[i].getId();}
	Job* getJobs() {return jobs;}
	void setJobs(Job *j) {jobs = j;}
};

class JobScheduler{

private:
	int execution_threads; 	// number of execution threads
	pthread_t* tids; 		// execution threads
	Mutex mutex;			//mutex
	CondVar* read_cond; //conition variable for readers
	int readers;
	int max_number_of_jobs; //einai sthn ousia to megethos tou pinaka ths ouras gia na thn kanoume realloc
	int current_job_num;    //to jobnumber dhladh job1 anaferetai sto query1 

public:
	Queue* queue; 				// a queue that holds submitted jobs / tasks
	queryArg queryarg;

	JobScheduler();
	~JobScheduler();
	void initialize_scheduler(int execution_threads, int job_num);
	void submit_job(Job* j);
	void execute_all_jobs(void *argument);
	void wait_all_tasks_finish(); //waits all submitted tasks to finish
	int clear_scheduler() {queue->clear();}
	int get_readers() {return readers;}
	void add_readers() {readers++;}
	void reduce_readers() {readers--;}
	int get_queue_size() {return queue->get_size();}

	void cond_wait() {read_cond->wait();}
	void cond_signal() {read_cond->signal();}
	void cond_broadcast() {read_cond->broadcast();}
	void mtx_lock() {mutex.lock();}
	void mtx_unlock() {mutex.unlock();}
	void clearqueue();
};

struct ArgumentStruct{
	Index *trie;
	BinaryMaxHeap * binarymaxheap;
	hashTableHeap * hash_Heap;
	string *query_results_buffer;
	JobScheduler* job_scheduler;
};



#endif