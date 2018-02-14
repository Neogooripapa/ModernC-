#include <iostream>
#include <vector>
using namespace std;

template<typename Container, typename Index>
auto authAndAcces(Container& c, Index i) -> decltype(c[i]) 
{
	//함수앞에 auto는 형식 연역과 관계가 없다.
	//후행 반환 형식으로 auto는 int&가 된다. c[i]가 int&를 반환하는 오퍼레이터이기 때문에.
	return c[i]; //decltype로 반환 형식을 매개변수들을 이용해서 지정가능.
}

template<typename Container, typename Index>
auto authAndAcces2(Container& c, Index i)
{
	//c[i]은 int&를 반환하지만 auto연역에 의해서 int로 변경.
	return c[i]; //decltype로 반환 형식을 매개변수들을 이용해서 지정가능.
}

template<typename Container, typename Index>
decltype(auto) authAndAcces3(Container& c, Index i)
{
	//auto는 해당 형식이 연혁되어야 함을 뜻하고
	//decltype은 형식 연역 규칙들이 적용되어야 함을 뜻한다.
	return c[i];
}

//C++14버전
template<typename Container, typename Index>
decltype(auto) authAndAcces4(Container&& c, Index i)
{
	//auto는 해당 형식이 연혁되어야 함을 뜻하고
	//decltype은 형식 연역 규칙들이 적용되어야 함을 뜻한다.
	return std::forward<Container>(c)[i]; //&는 &로 &&는 &&로 리턴하게 해준다.

	//std::forward<Container>(c)이부분만 포워드 해서 & &&함수를 구분해서 호출해준다
}

//C++11버전
template<typename Container, typename Index>
auto authAndAcces5(Container&& c, Index i) -> decltype(std::forward<Container>(c)[i])
{
	//auto는 해당 형식이 연혁되어야 함을 뜻하고
	//decltype은 형식 연역 규칙들이 적용되어야 함을 뜻한다.
	return std::forward<Container>(c)[i];
}


decltype(auto) f1()
{
	int x = 0;
	return x; //int를반환
}

decltype(auto) f2()
{
	int x = 0;
	return (x);//int&를 반환
}

int main()
{
	const int i = 0;
	decltype(i) a = 1;

	std::vector<int> v;
	v.push_back(10);

	authAndAcces(v, 0) = 1;

	//이렇게 호출하면 int 오른값에 값을 대입하려는 모양이 되어 컴파일 에러
	//authAndAcces2(v, 0) = 1; 

	authAndAcces3(v, 0) = 2;

	//변수선언에 decltype 형식 연역 규칙 적용

	int w;
	const int& cw = w;

	auto myWidget1 = cw; //myWidget1의 형식은 int

	decltype(auto) myWidget2 = cw; //const int&

	//오른값 컨테이너를 넘긴다. (컨테이너 임시객체가 오른값 컨테이너다)

	authAndAcces4(vector<int>(), 0) = 10;

	authAndAcces5(v, 0) = 2;
	return 0;
}