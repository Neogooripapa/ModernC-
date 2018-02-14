#include <iostream>
#include <memory>
#include <vector>
#include <list>
#include <functional>
#include <unordered_map>
#include <atomic>
#include <thread>
#include <mutex>
using namespace std;

typedef void(*FP)(int, const std::string&); //FP가 이름

using FP = void(*) (int, const std::string&); //별칭 성언

template<typename T>
using MyAllocList = std::list<T, std::vector<T>>;

//typedef 에 직접적으로 template을 쓸 수 없다.
template<typename T>
struct MyAllocList2
{
	typedef std::list<T, std::vector<T>> type;
};


class Wine {};
template<>
class MyAllocList2<Wine>
{
	enum class WineType { White, Red, Rose };
	WineType type;
};

template<typename T>
class Widget
{
	//MyAllocList2<T>::type은 템플릿 형식 매개변수 (T)에 의존적인 형식을 지칭한다.
	//MyAllocList2<T>::type은 의존적인 형식이다.
	//의존적인 형식의 이름 앞에 반드시 typename을 붙여야 한다.
	
	//MyAllocList2<T>::type을 만난 컴파일러는 그것이 형식의 이름임을 확신하지 못한다.
	//왜냐하면 MyAllocList2의 어떤 특수화 에서 MyAllocList2<T>::type이 형식 이외의 것을 지칭할 가능성이 남아 있기 때문이다.
	//class MyAllocList2<Wine> 를보면 알수 있듯이.
	typename MyAllocList2<T>::type list;
	//MyAllocList2<Wine>::type list2; //형식이 아니라 자료멤버를 지칭한다. 오류!
									  //따라서 MyAllocList2<T>::type이 형식을 지칭하는지는 전적으로 T가 무엇인지에 의존한다.
									  //이때문에 형식임을 받아들이게 하기 위해서는 typename을 붙여야한다.
};



template<typename T>
class Widget2
{
	//typename ::type 이 필요없다
	//MyAllocList2<T>::type처럼 MyAllocList<T>도 템플릿 매개변수 T에의존하는것 같지만
	//컴파일러는 Widget2 템플릿을 처리하는 과정에서 MyAllocList<T>가 쓰인 부분에 도달했을때,
	//컴파일러는 MyAllocList<T>가 형식의 이름임을 이미 알고 있다.
	//MyAllocList<T>가 형식 템플릿이므로, 비 의존적이다. 따라서 typename은 필요가 없고 붙이면 안된다.
	MyAllocList<T> list; 
};

int main()
{
	//1.별칭 템플릿 (typedef는 템플릿화 할 수 없지만 별칭 선언은 템플릿화가 가능)
	MyAllocList<int> lw;

	MyAllocList2<int>::type lw2;
	return 0;
}