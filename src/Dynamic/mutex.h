#ifndef MUT
#define MUT

class Mutex
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


#endif