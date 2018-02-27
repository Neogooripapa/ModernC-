#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <chrono>
#include <array>
#include <functional>
using namespace std;

//초기화 갈무리로는 다음과 같은 것들을 지정할 수 있다.
//1.람다로부터 생성되는 클로저 클래스에 속한 자료 멤버의 이름
//2.그 자료 멤버를 초기화 하는 표현식

class Widget
{
public:
	bool isValidated() const;
	bool isProcessed() const;
	bool isArchived() const;
private:
};

void foo()
{
	auto pw = std::make_unique<Widget>();

	auto func1 = [&pw] { return pw->isValidated() && pw->isArchived(); };
	//=의 좌변은 클로저 클래스 안의 자료 멤버의 이름, 우변은 그것을 초기화하는 표현식이다.
	//좌변의 범위는 해당 클로저 클래스의 범위이고,
	//우변의 범위는 람다가 정의되는 지점의 범위와 동일하다.
	auto func2 = [pcw = std::move(pw)] { return pcw->isValidated() && pcw->isArchived(); };
	
	//직접 초기화도 가능.
	auto func3 = [pcw = std::make_unique<Widget>()]{ return pcw->isValidated() && pcw->isArchived(); };
}

//C++11버전
//람다 표현식은 그냥 컴파일러가 하나의 클래스를 자동으로 작성해서 그 클래스의 객체를 생성하게 만든는 수단일 뿐이므로
class IsValAndArch
{
public:
	using DataType = std::unique_ptr<Widget>;

	explicit IsValAndArch(DataType&& ptr)
		: pw(std::move(ptr)) 
	{
	}

	bool operator()() const
	{
		return pw->isValidated() && pw->isArchived();
	}
private:
	DataType pw;
};

auto func = IsValAndArch(std::make_unique<Widget>()); //make_unique는 14에만 있으므로 21항목에서 가져다 쓰면됨.


void foo2()
{
	//C++14
	std::vector<double> data{ 1.2f, 1.3f };

	auto func = [data = std::move(data)]{};

	//C++11

	//람다 표현식 처럼 std::bind는 함수 객체를 산출한다. 바인드 객체
	//std::bind의 첫 인수는 호출가능한 객체이고, 나머지 인수들은 그 객체에 전달할 값들을 나타낸다.
	//바인드 객체는 std::bind에 전달된 모든 인수의 복사본들을 포함한다.
	//각 왼값 인수에 대해, 바인드 객체에는 그에 해당하는 복사 생성된 객체가 있다.
	//각 오른값에 대해서는 이동 생성된 객체가 있다.
	//이 예에서 인수는 오른값이다, 따라서 data는 바인드 객체안으로 이동된다.
	//즉 오른값을 바인드 객체안으로 이동함으로써 오른값이 이동이 불가능하다는 C++11 클로저의 한계를 우회한다.

	//기본적으로, 람다로부터 만들어진 클로저 클래스의 operator()멤버 함수는 const이다. 이 때문에 람다 본문 안에서
	//클로저의 모든 자료 멤버는 const가 된다.
	//따라서 여기서도 const를 넣어서 data를 선언.
	auto func2 = std::bind(
		[](const std::vector<double>& data) {}
		, std::move(data)
	);

	
	//변이 가능한 람다를 사용한다면 mutable을 사용
	//뮤터블 람다의 초기화 갈무리를 흉내 내는 방법.
	auto func3 = std::bind(
		[](std::vector<double>& data) mutable {}
		, std::move(data)
	);

	//바인드 객체의 수명이 클로저의 수명과 같으므로, 바인드 객체 안의 객체들을 마치 클로저 안에 있는 것처럼 취급이 가능하다.
	//(바인드는 모든 객체를 저장하므로 첫번째 인자 함수도 마찬가지 나중에 동시에 해제하므로 수명이 같다)

	auto func4 = [pcw = std::make_unique<Widget>()]
	{ 
		return pcw->isValidated() && pcw->isArchived(); 
	};

	auto func5 = std::bind(
		[](const std::unique_ptr<Widget>& pw)
		{
			return pw->isValidated() && pw->isArchived();
		}
		, std::make_unique<Widget>()
	);
}

void main()
{
}