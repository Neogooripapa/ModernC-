#include <iostream>
#include <memory>
#include <vector>
#include <list>
#include <algorithm>
#include <string>
using namespace std;

int main()
{
	//constexpr이 적용된 객체는 실제로 const이며, 그 값은 실제로 컴파일 시점에서 알려진다.
	//컴파일 시점에서 알려지는 값들에는 특별한 권한이 있다.
	//그런 값들은 읽기 전용 메모리에 배치될 수 있다.
	//컴파일 시점에서 알려진 정수 값을 C++에서 정수 상수 표현식이 요구되는 문맥에 사용할 수 있다.

	int sz;				//비 constexpr 변수

	constexpr auto arraySize1 = sz; //오류! sz의 값이 컴파일 도중에 알려지지 않음.

	std::array<int, sz> data1;		//오류! 같은 문제

	constexpr auto arraySize2 = 10; //OK. 컴파일 시점 상수

	std::array<int, arraySize2> data2; //OK. arraySize2는 constexpr 객체.

	//모든 constexpr은 const이다.
	//모든 const객체가 constexpr인 것은 아니다.
	//어떤 변수 값을 반드시 컴파일 시점 상수를 요구하는 문맥에 사용하려면 constexpr을 사용하라.
	int sz;

	const auto arraySize = sz;

	std::array<int, arraySize> data; //오류! arraySize의 값은 컴파일 시점에서 알려지지 않음.

	//constexpr함수
	//컴파일 시점 상수를 요구하는 문맥에 constexpr 함수를 사용할 수 있다.
	//인수의 값이 컴파일 시점에 알려지지 않는다면 컴파일 거부

	//컴파일 시점에서 알려지지 않는 값이 있다면 constexpr함수는 보통의 함수처럼 동작한다.
	//이는 같은 연산을 수행하는 두번을 만들 필요가 없음을 뜻한다.

	constexpr int pow(int base, int exp) noexcept
	{
	}

	constexpr auto numConds = 5;

	atd::array<int, pow(3, numConds)> results;

	//C++11에서 constexpr 함수는 실행 가능 문장이 많아야 하나 이어야한다 (0, 1)
	//3항 연산자나 재귀함수로 대체해야한다 
	constexpr int pow(int base, int exp) noexcept
	{
		return (exp == 0 ? 1 : base * pow(base, exp - 1));
	}

	//C++14에서는 제약이 느슨하다
	constexpr int pow(int base, int exp) noexcept
	{
		auto result = 1;
		for (int i = 0; i < exp; ++i) result *= base;
		return result;
	}

	class Point
	{
	public:
		constexpr Point(double xVal = 0, double yVal = 0) noexcept
			: x(xVal), y(yVal)
		{}

		constexpr double xValue() const noexcept { return x; }
		constexpr double yValue() const noexcept { return y; }

		constexpr void setX(double newX) { x = newX; }
		constexpr void setY(double newY) { y = newY; }
	private:
		double x, y;
	};

	constexpr Point p1(9.4, 27.7); //OK constexpr 생성자가 컴파일 시점에 실행
	constexpr Point p2(9.4, 27.7); //OK constexpr 생성자가 컴파일 시점에 실행

	constexpr Point midpoint(const Point& p1, const Point& p2) noexcept
	{
		return{ (p1.xValues() + p2.xValues() / 2, (p1.yValues() + p2.yValues() / 2 } ; //constexpr멤버 함수들 호출
	}

	constexpr auto mid = midpoint(p1, p2); //constexpr함수의 결과를 이용해서 constexpr객체를 초기화한다.

	//C++11에서는 constexpr멤버 함수는 암묵적으로 const로 선언된다.
	//멤버함수가 void반환인데 C++11에서는 void는 리터럴 형식이 아니다.
	return 0;
}