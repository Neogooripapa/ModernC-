#include <iostream>
#include <array>
using namespace std;

//&& Rvalue 참조 : 상수, 임시변수
//"좌측값은 어떠한 메모리 위치를 가리키는데, & 연산자를 통해 그 위치를 참조할 수 있다. 다른말로 주소를 취할 수 있다"
//"X& 를 좌측값 참조 라고 부르도록 합시다"
//"만일 이름이 있다면 좌측값, 없다면 우측값이다."

template<typename T>
void f1(T& param)
{
}

template<typename T>
void f2(const T& param)
{
}

template<typename T>
void f3(T* param)
{
}

template<typename T>
void f4(T&& param)
{
}

template<typename T>
void f5(T param)
{
}

//배열에 대한 참조를 선언하는 능력을 이용하면 배열에 담긴 원소들의 개수를 연역하는 템플릿을 만들 수 있다.
//컴파일 시점에 상수를 돌려준다.
//배열 매개변수에 이름을 붙이지 않은것은 필요한것은 원소 개수 뿐이기 때문에.
//constexpr함수의 호출결과를 컴파일중에 가능하게 만든다.
template<typename T, std::size_t N>
constexpr std::size_t arraySize(T(&)[N] ) noexcept
{
	return N;
}

void someFunc(int, double)
{
}

int main()
{
	//template<typename T> T타입 따로
	//void f(ParamType param) ParamType따로
	//f(expr)

	int a = 1;
	int b = 1;

	int d = a + b; //복사 후 임시 변수 파괴.
	int&&c = a + b; //복사도 파괴도 일어나지 않음.

	//1.참조형식이고 보편참조는 아님.
	int x = 27;
	const int cx = x;
	const int& rx = x;

	f1(x);  //T는 int      , param은 int&
	f1(cx); //T는 const int, param은 const int&
	f1(rx); //T는 const int, param은 const int& //rx의 참조성은 무시됨.

	f2(x);  //T는 int, param은 const int&
	f2(cx); //T는 int, param은 const int& //T에 const가 없어짐에 주목.
	f2(rx); //T는 int, param은 const int& //T에 const가 없어짐에 주목. //rx의 참조성은 무시됨.

	int x1 = 27;
	const int* px = &x1;

	f3(&x); //T는 int      , param은 int*
	f3(px); //T는 const int, param은 const int*

	//2.보편참조의 경우
	//expr이 왼값이면, T와 ParamType 둘다 왼값 참조로 연역된다.

	f4(x);   //x는  왼값   T는 int&, param은 int&			 
	f4(cx);  //cx는 왼값   T는 const int&, param은 const int& 
	f4(rx);  //rx는 왼값   T는 const int&, param은 const int&
	f4(27);  //27은 오른값 T는 int, param은 int&& //정상 규칙

	//3.ParamType이 포인터도 아니고 참조도 아님
	//expr의 형식이 참조이면, 참조부분은 무시된다.
	//expr의 참조성을 무시한후, 만일 expr이 const이면 const역시 무시한다. 만일 volatile이면 역시 무시
	//param은 cx나rx의 독립적인 복사본이므로 당연한 결과이다.
	//cx와rx가 수정될수 없다는 점은 param의 수정가능 여부와는 완전 무관하다.

	f5(x);  //T는 int, param은 int
	f5(cx); //T는 int, param은 int
	f5(rx); //T는 int, param은 int

	const char* const ptr = "Fun C++";

	f5(ptr); //T는 const char*, param은 const char* //ptr이 가리키는 것의 const성의 보존된다.

	int pp = 1;
	const int* ptr1 = &pp; //가리키는 값이 const
	int* const ptr2 = &pp; //포인터 자체가 const

	//4.배열인수
	const char name[] = "pcw";    //name의 형식은 const char[3]
	const char* ptrToName = name; //배열이 포인터로 붕괴한다.

	f5(name); //name은 배열이지만 const char* 로 연역한다.
	f1(name); //T는 const char [3]으로 연역되고, param은 const char (&)[3]으로 연역된다.

	int size = arraySize(name);

	int keyVals[] = { 1,3,5 };

	int mappedVals[arraySize(keyVals)]; //컴파일중에 가능해진다.
	std::array<int, arraySize(keyVals)> mappedVals2;

	//5.함수인수
	//함수포인터로 붕괴한다.
	f5(someFunc); //param 은 void (*) (int, double)
	f1(someFunc); //param 은 void (&) (int, double)
	return 0;
}
