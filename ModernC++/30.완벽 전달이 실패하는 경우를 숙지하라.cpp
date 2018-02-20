#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <chrono>
#include <array>
using namespace std;

//완벽전달은 단순히 객체들을 전달하는 것만이 아니라.
//그 객체들의 주요 특징, 형식, 왼값 오른값 여부, const 나 volatile 여부까지도 전달하는것을 말한다.

void f(const std::vector<int>& v)
{
}

//전달하는 함수는 필연적으로 제네릭 함수이다.
template<typename T>
void fwd(T&& param)
{
	f(std::forward<T>(param));
}

//표준 컨테이너의 emplace류 함수들은 이런식의 형태로 구현되어 있다.
template<typename... Ts>
void fwd2(Ts&&... params)
{
	f(std::forward<Ts>(params)...);
}

//f(표현식);   이호출이 하는 일과
//fwd(표현식); 이호출이 하는 일이 다르다면, fwd표현식을 f에 완벽하게 전달하지 못한것이다.



void foo()
{
	f({ 1,2,3 }); //암적적 변화를하므로 OK

	//fwd({ 1,2,3 }); //오류! fwd의 매개변수가 std::initializer_list가 될수 없는 형태로 선언되어 있다.

	//다음 두 조건중 하나라도 만족하지 않으면 완벽전달은 실패한다.
	//1.fwd의 매개변수들 중 하나 이상에 대해 컴파일러가 형식을 연역하지 못한다.
	//2.fwd의 매개변수들 중 하나 이상에 대해 컴파일러가 형식을 잘못연역 한다.
	//  하나는 그형식으로는 fwd의 인스턴스를 컴파일 할 수 없는것이고.
	//  또 하나는 컴파일된다고 해도 fwd의 연역된 형식들을 이용해서 호출하는 f가 fwd에 전달된 인수들로
	//  f를 직접호출했을 때와 다르게 행동하는것이다. (f가 중복적재된경우 직접f를호출할대와 다른 중복적재 함수가 불리는경우)

	//흥미롭게도 auto변수는 중괄호 초기치로 초기화해도 그 형식이 잘 연역된다.
	auto il = { 1,2,3 };

	fwd(il); //ok il이 f로 완벽하게 전달된다.
}

//선언만 된 정수 static const 및 constexpr 자료 멤버
class Widget
{
public:
	//static const, static constexpr 자료 멤버는 선언만 하면된다.
	static constexpr std::size_t MinVals = 28; //const 전파를 적용해서 멤버의 메모리를 따로 할당하지 않는다.
											   //MinVals가 배치된 모든곳에 28이라는 값을 배치한다.
};

//constexpr std::size_t Widget::MinVals;

std::vector<int> widgetData;

void f1(std::size_t val)
{
}

template<typename T>
void fwd1(T&& param)
{
	f1(std::forward<T>(param));
}

void foo2()
{
	widgetData.reserve(Widget::MinVals);
	//만일 어떤코드가 MinValsd의 주소를 취한다면 이코드는 컴파일되긴 하지만 MinVals의 정의가 없어서 링크에 실패한다.

	f1(Widget::MinVals);
	//완벽전달이 관여 함으로써 실패하게된당. (에러 안나는 컴파일러도 있지만 이식성이 없음)
	fwd1(Widget::MinVals); //오류! 링크 에러. 보편참조함수(참조는 포인터처럼 취급한다)
}

//중복 적재된 함수 이름과 템플릿 이름
void f3( int (*pf) (int) )
//void f( int pf (int) ) 위와 동일
{
}

int processVal(int value)
{
	return 0;
}

int processVal(int value, int priority)
{
	return 0;
}

template<typename T>
T workOnVal(T param)
{
	return param;
}

template<typename T>
void fwd3(T&& param)
{
	f3(std::forward<T>(param));
}

void foo3()
{
	f3(processVal); //OK 컴파일러가 f의 선언을 보고 적절한 processVal를 찾아서 호출한다.

	//fwd3(processVal); //오류! fwd3에는 호출에 필요한 형식에 관한 정보가 전혀 없다.
	//fwd3(workOnVal);  //오류! workOnVal의 어떤 인스턴스인지?

	//우회 방법
	using ProcessFuncType = int(*) (int);
	ProcessFuncType processValPtr = processVal; //필요한 서명을 명시한다.

	fwd3(processValPtr);
	fwd3(static_cast<ProcessFuncType>(workOnVal));
}

//비트필드
struct IPv4Header
{
	std::uint32_t version : 4, IHL : 4, DSCP : 6, ECN : 2, totalLength : 16;
};

IPv4Header h;

void f4(std::size_t sz)
{
}

template<typename T>
void fwd4(T&& param)
{
	f4(std::forward<T>(param));
}

void main()
{
	f4(h.totalLength); //OK

	//문제는 fwd4의 매개변수가 참조 이고, h.totalLength는 비 const비트필드라는 점이다.
	//C++ 표준은 둘의 조합에 대해 "비 const 참조는 절대로 비트필드에 묶이지 않아야 한다"라고 명시.
	//임의의 비트들을 가리키는 포인터를 생성하는 방법이 없으며, 따라서 참조를 임의의 비트들에 묶는 방법도 없다.
	//fwd4(h.totalLength); //오류!

	//위회 방법
	auto lengh = static_cast<std::uint16_t>(h.totalLength);
	fwd4(lengh); //복사본을 전달한다.

	//const에 대한 참조로 전달하는 방법도 가능.
	//const참조 매개변수의 경우, 표준에 따르면 그 참조는 실제로 어떤 정수 형식(이를테면 int)의 객체에
	//저장된 비트필드 값의 복사본에 묶여야 한다.
	//즉 const참조는 비트필드 자체에 묶이는 것이 아니라, 비트필드값이 복사된 보통 객체에 묶인다.
}