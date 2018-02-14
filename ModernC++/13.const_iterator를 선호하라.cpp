#include <iostream>
#include <memory>
#include <vector>
#include <list>
#include <algorithm>
#include <string>
using namespace std;

int main()
{
	//C++98 스타일
	//const_iterator를 사용하도록 코드를 고치는것은 간단한 일이지만 그렇지 않다.
	typedef std::vector<int>::iterator Iter;
	typedef std::vector<int>::iterator constIter;

	std::vector<int> values;

	//C++98에서는 비const 컨테이너로부터 const_iterator를 얻는 간단한 방법이 없기때문에 캐스팅.
	//const참조변수에 받아서 사용해도 되긴된다.
	constIter ci = std::find(static_cast<constIter>(values.begin()), static_cast<constIter>(values.end()) , 1983);

	//C++98에서는 삽입 위치를 iterator로만 받을 수 있다.
	values.insert(static_cast<Iter>(ci), 1998); //컴파일이 안될 수 도 있다.
												//const_iterator에서 iterator로 이식성있는 변환이 없기 때문에.

	//C++11 스타일
	std::vector<int> values2;

	auto it = std::find(values2.cbegin(), values2.cend(), 1983);
	values2.insert(it, 1998);

	

	return 0;
}

//일반성을 극대화한 코드는 특정 멤버 함수의 존재를 요구하는 대신 그 멤버 함수에 상응하는 비멤버 함수를 사용한다.
//이코드는 C++14에서 잘동작하지만 C++11에선 동작하지 않는데 표준에 없기 때문에.
template<typename C, typename V>
void findAndInsert(C& container, const V& targetVal, const V& insertVal)
{
	using std::cbegin;
	using std::cend;

	auto it = std::find(cbegin(container), cend(container), targetVal);

	container.insert(it, inserVal);
}

//C++11을 위한 비멤버함수 구현
template<class C>
auto cbegin(const C& container)->decltype(std::begin(container))
{
	//container -> const vector<int>&
	return std::begin(container); //const컨테이너에 대한 비멤버 cbegin함수를 호출하면
								  //const_iterator 형식의 반복자가 반환된다.
}