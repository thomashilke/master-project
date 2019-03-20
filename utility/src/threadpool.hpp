#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <pthread.h>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <queue>

template<typename Ret>
class Job
{
  Job(const Job&);
  Job& operator=(const Job&);
public:
  Job(){}
  virtual ~Job(){}
  
  virtual Ret Do() = 0;
};

template<typename Ret>
class Job0Free: public Job<Ret>
{
  Ret (*m_fnc)();
public:
  Job0Free(Ret (*fnc)()): m_fnc(fnc) {}
  virtual ~Job0Free(){}
  virtual Ret Do();
};

template<typename Ret>
Ret Job0Free<Ret>::Do(){return m_fnc();}

template<>
void Job0Free<void>::Do(){m_fnc();}


template<typename C, typename Ret>
class Job0Mem: public Job<Ret>
{
  Ret (C::*m_fnc)();
  C& m_instance;
public:
  Job0Mem(C& inst, Ret (C::*fnc)()) : m_instance(inst), m_fnc(fnc) {}
  virtual ~Job0Mem(){}
  virtual Ret Do(){return (m_instance.*m_fnc)();}
};

template<typename Ret, typename Arg1>
class Job1Free: public Job<Ret>
{
  Ret (*m_fnc)(Arg1);
  Arg1 m_a1;
public:
  Job1Free(Ret (*fnc)(Arg1), Arg1 a1): m_fnc(fnc), m_a1(a1) {}
  virtual ~Job1Free(){}
  virtual Ret Do(){return m_fnc(m_a1);}
};

template<typename C, typename Ret, typename Arg1>
class Job1Mem: public Job<Ret>
{
  Ret (C::*m_fnc)(Arg1);
  C& m_instance;

  Arg1 m_a1;
public:
  Job1Mem(C& inst, Ret (C::*fnc)(Arg1), Arg1 a1) : m_instance(inst), m_fnc(fnc), m_a1(a1) {}
  virtual ~Job1Mem(){}
  virtual Ret Do(){return (m_instance.*m_fnc)(m_a1);}
};


template<typename Ret, typename Arg1, typename Arg2>
class Job2Free: public Job<Ret>
{
  Ret (*m_fnc)(Arg1,Arg2);
  Arg1 m_a1;
  Arg2 m_a2;
public:
  Job2Free(Ret (*fnc)(Arg1, Arg2), Arg1 a1, Arg2 a2): m_fnc(fnc), m_a1(a1), m_a2(a2) {}
  virtual ~Job2Free(){}
  virtual Ret Do(){return m_fnc(m_a1, m_a2);}
};

template<typename C, typename Ret, typename Arg1, typename Arg2>
class Job2Mem: public Job<Ret>
{
  Ret (C::*m_fnc)(Arg1, Arg2);
  C& m_instance;

  Arg1 m_a1;
  Arg2 m_a2;
public:
  Job2Mem(C& inst, Ret (C::*fnc)(Arg1,Arg2), Arg1 a1, Arg2 a2) : m_instance(inst),
							m_fnc(fnc),
							m_a1(a1),
							m_a2(a2)
  {}
  virtual ~Job2Mem(){}
  virtual Ret Do(){return (m_instance.*m_fnc)(m_a1, m_a2);}
};


template<typename Ret, typename Arg1, typename Arg2, typename Arg3>
class Job3Free: public Job<Ret>
{
  Ret (*m_fnc)(Arg1,Arg2,Arg3);
  Arg1 m_a1;
  Arg2 m_a2;
  Arg3 m_a3;
public:
  Job3Free(Ret (*fnc)(Arg1, Arg2, Arg3), Arg1 a1, Arg2 a2, Arg3 a3): m_fnc(fnc), m_a1(a1), m_a2(a2), m_a3(a3) {}
  virtual ~Job3Free(){}
  virtual Ret Do(){return m_fnc(m_a1, m_a2, m_a3);}
};

template<typename C, typename Ret, typename Arg1, typename Arg2, typename Arg3>
class Job3Mem: public Job<Ret>
{
  Ret (C::*m_fnc)(Arg1, Arg2, Arg3);
  C& m_instance;

  Arg1 m_a1;
  Arg2 m_a2;
  Arg3 m_a3;
public:
  Job3Mem(C& inst, Ret (C::*fnc)(Arg1,Arg2,Arg3), Arg1 a1, Arg2 a2, Arg3 a3) : m_instance(inst),
									       m_fnc(fnc),
									       m_a1(a1),
									       m_a2(a2),
									       m_a3(a3)
  {}
  virtual ~Job3Mem(){}
  virtual Ret Do(){return (m_instance.*m_fnc)(m_a1, m_a2, m_a3);}
};

template<typename Ret>
Job<Ret>* MakeJob(Ret (*m_func)())
{ return new Job0Free<Ret>(m_func); }

template<typename Ret, typename C>
Job<Ret>* MakeJob(C& inst, Ret (C::*m_func)())
{ return new Job0Mem<C, Ret>(inst, m_func); }

template<typename Ret, typename Arg1>
Job<Ret>* MakeJob(Ret (*m_func)(Arg1), Arg1 a1)
{ return new Job1Free<Ret, Arg1>(m_func, a1); }

template<typename Ret, typename C, typename Arg1>
Job<Ret>* MakeJob(C& inst, Ret (C::*m_func)(Arg1), Arg1 a1)
{ return new Job1Mem<C, Ret, Arg1>(inst, m_func, a1); }

template<typename Ret, typename Arg1, typename Arg2>
Job<Ret>* MakeJob(Ret (*m_func)(Arg1, Arg2), Arg1 a1, Arg2 a2)
{ return new Job2Free<Ret, Arg1, Arg2>(m_func, a1, a2); }

template<typename Ret, typename C, typename Arg1, typename Arg2>
Job<Ret>* MakeJob(C& inst, Ret (C::*m_func)(Arg1, Arg2), Arg1 a1, Arg2 a2)
{ return new Job2Mem<C, Ret, Arg1, Arg2>(inst, m_func, a1, a2); }

template<typename Ret, typename Arg1, typename Arg2, typename Arg3>
Job<Ret>* MakeJob(Ret (*m_func)(Arg1, Arg2, Arg3), Arg1 a1, Arg2 a2, Arg3 a3)
{ return new Job3Free<Ret, Arg1, Arg2, Arg3>(m_func, a1, a2, a3); }

template<typename Ret, typename C, typename Arg1, typename Arg2, typename Arg3>
Job<Ret>* MakeJob(C& inst, Ret (C::*m_func)(Arg1, Arg2), Arg1 a1, Arg2 a2, Arg3 a3)
{ return new Job3Mem<C, Ret, Arg1, Arg2, Arg3>(inst, m_func, a1, a2, a3); }

template<typename R>
class ResultAccountant
{
public:
  virtual ~ResultAccountant(){}
  virtual void Process(const R&) = 0;
};

template<typename Ret>
class ThreadPool
{
  pthread_cond_t cv_empty_queue;
  pthread_cond_t cv_filled_queue;
  
  pthread_mutex_t m_mutex_q;
  pthread_mutex_t m_mutex_result;

  ResultAccountant<Ret>* m_resultaccountant;

  bool m_shutdownrequested;
  std::queue<Job<Ret>*> m_jobqueue;
  
  unsigned int m_threadsnum;
  std::vector<pthread_t> m_threads;

  pthread_mutex_t m_mutex_running;
  unsigned int m_runningThreads;

  static void* bootstrap(void* inst)
  {
    ThreadPool* tp(reinterpret_cast<ThreadPool*>(inst));

    tp->threadkernel();

    pthread_exit(NULL);
  }
  
  void threadkernel()
  {
    do{
      pthread_mutex_lock(&m_mutex_q);
      while(m_jobqueue.size() == 0 && !m_shutdownrequested)
	{
	  pthread_cond_wait(&cv_filled_queue, &m_mutex_q);
	}
      if(!m_shutdownrequested)
	{
	  Job<Ret>* j(m_jobqueue.front());
	  m_jobqueue.pop();
	
	  pthread_mutex_unlock(&m_mutex_q);

	  //Do the job:
	  pthread_mutex_lock(&m_mutex_running);
	  ++m_runningThreads;
	  pthread_mutex_unlock(&m_mutex_running);
	  Ret r(j->Do());
	  if(m_resultaccountant)
	    {
	      pthread_mutex_lock(&m_mutex_result);
	      m_resultaccountant->Process(r);
	      pthread_mutex_unlock(&m_mutex_result);
	    }
	  pthread_mutex_lock(&m_mutex_running);
	  --m_runningThreads;
	  pthread_mutex_unlock(&m_mutex_running);

	  delete j;
	  j = NULL;

	  pthread_mutex_lock(&m_mutex_q);
	  pthread_cond_signal(&cv_empty_queue);
	  pthread_mutex_unlock(&m_mutex_q);
	}
    else
      pthread_mutex_unlock(&m_mutex_q);
      
    }while(!m_shutdownrequested);
  }
  
public:
  ThreadPool(unsigned int n = 0): m_resultaccountant(NULL),
				  m_shutdownrequested(false),
				  m_threadsnum(n),
				  m_threads(n),
				  m_runningThreads(0)
				  
  {
    if(n == 0)
      {
	m_threadsnum = sysconf(_SC_NPROCESSORS_ONLN);
	m_threads.resize(m_threadsnum, NULL);
      }
    
    pthread_cond_init(&cv_empty_queue, NULL);
    pthread_cond_init(&cv_filled_queue, NULL);
    pthread_mutex_init(&m_mutex_q, NULL);
    pthread_mutex_init(&m_mutex_result, NULL);
    pthread_mutex_init(&m_mutex_running, NULL);

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
    
    for(unsigned int j(0);j<m_threadsnum;++j)
      pthread_create(&m_threads[j], &attr,
		     bootstrap, reinterpret_cast<void*>(this));    
  }
  void setResultAccountant(ResultAccountant<Ret>* ra){m_resultaccountant = ra;}
  ~ThreadPool()
  {
    pthread_mutex_lock(&m_mutex_q);
    while(m_jobqueue.size())
      {
	pthread_cond_wait(&cv_empty_queue, &m_mutex_q);
      }
    pthread_mutex_unlock(&m_mutex_q);


    pthread_mutex_lock(&m_mutex_q);
    m_shutdownrequested = true;
    pthread_cond_broadcast(&cv_filled_queue);
    pthread_mutex_unlock(&m_mutex_q);
    
    void* result;
    for(unsigned int j(0);j<m_threadsnum;++j)
      pthread_join(m_threads[j], &result);
  }
  
  void enqueue(Job<Ret>* j)
  {
    pthread_mutex_lock(&m_mutex_q);
    m_jobqueue.push(j);
    pthread_cond_signal(&cv_filled_queue);
    pthread_mutex_unlock(&m_mutex_q);
  }
  void waitForEmptyQueue()
  {
    pthread_mutex_lock(&m_mutex_q);
    while(m_jobqueue.size())
      {
	pthread_cond_wait(&cv_empty_queue, &m_mutex_q);
      }
    pthread_mutex_unlock(&m_mutex_q);
  }

  void waitForJobsDone()
  {
    pthread_mutex_lock(&m_mutex_q);
    while(m_jobqueue.size() || (m_runningThreads > 0))
      {
	pthread_cond_wait(&cv_empty_queue, &m_mutex_q);
      }
    pthread_mutex_unlock(&m_mutex_q);
  }
};

#endif
