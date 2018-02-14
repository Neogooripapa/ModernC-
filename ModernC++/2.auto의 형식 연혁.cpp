#include <iostream>
#include <vector>
using namespace std;

void someFunc(int, double) {}

template<typename T>
void f(T param) {}

template<typename T>
void t(std::initializer_list<T> param) {}

auto createInitList()
{
	//return { 1,2,3 }; //오류! 템플릿의 형식 연역을 따르기 때문에.
	return 0;
}

int main()
{
	//1.auto를 이용해서 변수를 선언할 때 auto는 템플릿의 T와 동일한 역할을 한다.
	auto x = 27;
	//template<typename T>
	//void func_for_x(T param);
	//func_for_x(27);

	const auto cx = x;
	//template<typename T>
	//void func_for_x(const T param);
	//func_for_x(x);

	const auto& rx = x;
	//template<typename T>
	//void func_for_x(const T& param);
	//func_for_x(x);

	//2.형식 지정자가 보편 참조인 경우.
	auto&& uref1 = x; //int&
	auto&& uref2 = cx;//const int&
	auto&& uref3 = 27; //int&&

	//3.auto 에대한 포인터 형식 연혁 붕괴
	const char name[] = "pcwmp";

	auto arr1 = name; //const char*
	auto& arr2 = name; //const char (&) [6]

	auto func1 = someFunc; //void (*) (int, double)
	auto& func2 = someFunc; //void (&) (int, double)

	//4.auto와 템플릿 T의 다른점(auto에 대한 특별한 형식 연역 규칙)

	int x1 = 27; //형식 int, 값이 27인 변수를 선언.
	int x2(27); //형식 int, 값이 27인 변수를 선언.
	int x3 = { 27 }; //std::initializer_list<int> 형식의 변수를 선언
	int x4{ 27 }; //std::initializer_list<int> 형식의 변수를 선언(N3922에서는 int)

	//auto로 선언된 변수의 초기를 중괄호로 감싸면 연역된 형식은 std::initializer_list이다.

	//auto x5 = { 1,2,3.0f }; //오류! std::initializer_list<T>의 T를 연역할 수 없음.

	auto xx = { 11, 23, 9 };
	
	//f({ 11, 23, 9 }); //오류! T에 대한 형식을 연역할 수 없음.
	t({ 11, 23, 9 });

	//5.람다 매개변수에 쓰인 auto, 함수 반환 형식을 auto는 템플릿 형식 연역이 적용된다.
	createInitList();

	std::vector<int> v;

	auto resetV = [&v](const auto& newValue) { v = newValue; };

	//resetV({1,2,3}); //오류 템플릿 형식 연역을 사용하므로.
	return 0;
}