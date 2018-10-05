#include "job_scheduler.h"
#include <string>

using namespace std;

JobScheduler::JobScheduler()
{
	read_cond = new CondVar(mutex);
	queryarg.write_heap_cond = new CondVar(queryarg.heap_mutex);
	readers = 0;
	queryarg.writers = 0;
	//number_of_jobs = initial_size;
	//queue = new Queue[initial_size];
}

JobScheduler::~JobScheduler()
{
	//queue->clear();
	delete queue;
	delete []tids;
	delete read_cond;
	delete queryarg.write_heap_cond;
}
//synarthsh pou arxikopoiei to plithos ton thread (execution_threads,plithos antikeimenwn-query sthn oura)
void JobScheduler::initialize_scheduler(int execution_threads, int initial_job_number) 
{
	this->execution_threads = execution_threads;
	tids = new pthread_t[execution_threads];
	queue = new Queue(initial_job_number);
	readers = 0;
	max_number_of_jobs = initial_job_number;
	current_job_num = 0;
}

void JobScheduler::clearqueue()
{
	queue->clear();
}

void Queue::clear()
{
	for(int i=0;i<size;i++)
		jobs[i].~Job();
	//free(jobs);

	next_job_to_pop = 0;
	next_job_to_add = 0;
	//max_size = current_size;
	current_size = 0;
	size = 0;
}

void JobScheduler::execute_all_jobs(void *argument)
{
	//cerr<<"execution\n\n";
	int err;
	struct ArgumentStruct* arg = (ArgumentStruct*) argument;
	arg->job_scheduler = this;
	//cout<<pthread_self()<<endl;
	for(int i=0;i<execution_threads;i++)            //create Workers
	{
		//cout<<"CREATED A THREAD HERE\n";
        if ( err =  pthread_create(tids+i, NULL, Job::job_to_execute, arg) ) {
            cerr<<" pthread_create error"<<endl;
            exit (1) ;
        }
	}
	//cerr<<"Thread creatoin done\n\n";
}

void JobScheduler::wait_all_tasks_finish()
{

	int err, status, counter = 0;
	for (int i = 0; i < execution_threads; i++)     //wait for Workers to join
    {
    	//printf("joined reader before for ");
        //cerr<<i<<"    "<<pthread_self()<<endl;
        //cerr<<"waiting to join with "<<tids[counter]<<endl;
        if ( (err = pthread_join (*(tids+i) ,NULL))){ //( void **) & status )) ) { /* Wait for thread */
         
            cerr<<" pthread_join error"<<endl;
            exit (1) ;
        }
        //counter++;
        //printf("joined reader ");
        //cout<<i<<"    "<<pthread_self()<<endl;
    }
}


void JobScheduler::submit_job(Job* j) //kanei push sthn oura to job(query) j
{
	/*cout<<"Job submitted\n";
	cout<<max_number_of_jobs<<endl;
	cout<<current_job_num<<endl;*/
	if(max_number_of_jobs == current_job_num)
	{
		max_number_of_jobs = 2*max_number_of_jobs;
		//Queue *temp = new Queue(max_number_of_jobs);
		/*Job *temp =  (Job *)malloc(max_number_of_jobs*sizeof(Job));
		memcpy(temp, queue->getJobs(), (max_number_of_jobs/2)*(sizeof(Job)));
		queue->clear2();
		queue->setJobs(temp);
*/
		queue->setJobs((Job*)realloc(queue->getJobs() ,max_number_of_jobs*sizeof(Job) ));
		//queue = (Queue*) realloc(queue, max_number_of_jobs*sizeof(Queue));
		queue->set_max_size(max_number_of_jobs);
	}
	current_job_num++;
	queue->add(j);
}


Queue::Queue(int s)
{
	next_job_to_pop = 0;
	next_job_to_add = 0;
	max_size = s;
	current_size = 0;
	size = 0;
	jobs = (Job *)malloc(max_size*sizeof(Job));
}

Queue::~Queue()
{
	for(int i=0;i<size;i++)	
		jobs[i].~Job();
	free (jobs);
}

Job* Queue::pop()
{
	return &jobs[next_job_to_pop];
	//next_job_to_pop++;
	//size--;
}

void Queue::add(Job *job)
{
	jobs[next_job_to_add] = *job;
	next_job_to_add++;
	current_size++;
	size++;
}
/*
void Queue::clear()
{
	for(int i=0;i<max_size;i++)
		jobs[i].~Job();
	free(jobs);
}
*/
void Queue::clear2()
{
	free(jobs);
}

void *Job::job_to_execute(void *argument)//synarthsh twn thread
{
	struct ArgumentStruct* arg = (ArgumentStruct*) argument;
	JobScheduler* job_scheduler = arg->job_scheduler;
	string query_result;
     
    while (1) {
    
	    job_scheduler->mtx_lock();//locking mutex
	    
		    /*if(job_scheduler->get_queue_size() == 0)      //all jobs are done
		    {
		        job_scheduler->cond_broadcast();
		        job_scheduler->mtx_unlock(); 
		        //cout<<"thread "<<pthread_self()<<" is exiting"<<endl;
		        pthread_exit(NULL);
		    }*/
		    //an o arithmos twn reader einai >0 tote perimenoume sto condvar
		    while(job_scheduler->get_readers() || job_scheduler->get_queue_size() <= 0){
		        
		        if(job_scheduler->get_queue_size() == 0)      //all jobs are done
		    	{
		        	job_scheduler->cond_broadcast();
		        	job_scheduler->mtx_unlock();
		        	//cout<<"thread "<<pthread_self()<<" is exiting"<<endl;
		        	pthread_exit(NULL);
		    	}
		       job_scheduler->cond_wait();
		    }

		    //critical section 
		    job_scheduler->add_readers();//ayksanw ton arithmo twn readers kata 1
	    job_scheduler->mtx_unlock();


	    Job* job = job_scheduler->queue->pop();//pairnoume to prwto job ths ouras
	    job_scheduler->mtx_lock();
		    job_scheduler->reduce_readers();//meiwnoume ton arithmo twn reader
		    job_scheduler->queue->add_next_job();	//ayksanoume ton index gia to pop kata 1 dhladh sto epomeno pop tha ginei aytos apo thn epomenh thesh
		    job_scheduler->queue->reduce_size(); //meiwnoume kata 1 to size ths ouras

		    job_scheduler->cond_signal();  //3ypname ena thread  pou perimenei sto condvarible
	    job_scheduler->mtx_unlock();//unlocking mutex

	queryArg *yolo_Arg = &job_scheduler->queryarg;
	


	query_result = arg->trie->query(job->query,yolo_Arg,job->version);//normal query
	arg->query_results_buffer[job->jobId] = query_result;

    //cout<<"Thread "<< pthread_self()<<"executes: \""<<job->getQuery()<<" "<<job->getId()<<"\""<<endl;


//cout<<"QUERY RESULTTTTTTTTTTTTTT::::"<<query_result<<endl;
	}

}