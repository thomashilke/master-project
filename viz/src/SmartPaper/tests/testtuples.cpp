#include <utils/tuple.hpp>

int main()
{
  Tuple<bool, float, double ,int> t1(true, 34.f, 12., 8);
  Tuple<bool> t2(true);
  Tuple<bool,float> t3(true, 54.f);

  std::cout << val<2>(t1) << std::endl;
  
  t1 = MakeTuple(false, 34.6f,12., 3);

  Pair<std::string,
       Pair<float,
	    Pair<int, void> > >
    p1(Make_pair(std::string("hello"), Make_pair(3.14f , Make_pair(2))));

  std::cout << val<1>(p1) << std::endl;
  std::cout << val<2>(p1) << std::endl;
  std::cout << val<3>(p1) << std::endl;
  
  std::cout << "ApplyOn Tuple: " << t1 <<  std::endl;

  return 0;
}
