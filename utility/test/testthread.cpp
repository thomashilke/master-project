#include "utility/threadpool.hpp"
#include "utility/tuple.hpp"

typedef Tuple<int,double> Result;

class Work
{
  double m_lambda;
public:
  Work(double lambda): m_lambda(lambda)
  {}
  Result Do(int i)
  {
    sleep(1);
    return MakeTuple(i, m_lambda);
  }
};

class PrintSomeMessage: public ResultAccountant<Result >
{
public:
  void Process(const Result& x)
  { std::cout << "Job ID = " << val<1>(x)
	      << ", Lambda = " << val<2>(x) << std::endl;}
};

int main()
{
  Work w(3.1415);
  PrintSomeMessage psm;
  
  ThreadPool<Result> tp(2);

  tp.setResultAccountant(&psm);

  for(int k(0);k<10;++k)
    tp.enqueue(MakeJob(w,&Work::Do,k));
  
  tp.waitForJobsDone();

  std::cout << "fini!" << std::endl;
  return 0;
}
