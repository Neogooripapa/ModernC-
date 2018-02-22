#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <chrono>
#include <array>
#include <functional>
using namespace std;

vector<int> list{20,5};

void WhatIsTheLamDa()
{
	//람다 표현식.
	//클로저는 람다에 의해 만들어진 실행 시점 객체이다. 갈무리모드(캡처 모드)에 따라, 클로저가 갈무리된 자료의 복사본을
	//가질 수도 있고 그 자료에 대한 참조를 가질 수 도 있다. 3번째 인수로 전달되는 객체이다.

	//클로저 클래스는 클로저를 만드는데 쓰인 클래스를 말한다
	//각각의 람다에 대해 컴파일러는 고유한 클로저 클래스를 만들어 낸다.
	//람다 안의 문장들은 해당 클로저 클래스의 멤버 함수들 안의 실행 가능한 명령들이 된다.
	vector<int>::iterator iter = std::find_if(list.begin(), list.end(), [](int val) { return 0 < val && val < 10; });

	if (iter != list.end())
	{
		cout << "찾음";
	}
	else
	{
		cout << "못 찾음";
	}

	int x;

	auto c1 = [x](int y) { return x * y > 55; }; //c1은 람다에 의해 만들어진 클로저의 복사본.

	auto c2 = c1; //c2는 c1의 복사본.
	auto c3 = c2; //c3는 c2의 복사본.
}

/////////////////////////////////////////////////////////////////////////////////////////////
using FilterContainer = std::vector<std::function<bool(int)>>;

FilterContainer filters;


void CaptureMode()
{
	//기본 갈무리 모드는 두가지다.
	//참조에 의한 갈무리모드, 값에 의한 갈무리 모드이다.

	//참조 갈무리를 사용하는 클로저
	//지역변수 또는 람다가 정의된 범위에서 볼 수 있는 매개변수에 대한 참조를 가지게 된다.
	//람다에 의해 생성된 클로저의 수명이 그 지역 변수나 매개변수의 수명보다 길면, 클로저 안의 참조는 대상을 잃는다.
	filters.emplace_back([](int value) {return value % 5 == 0;} ); //5의 배수 선별 함수.
}

int computeSomeValue()
{
	return 1;
}

int computeDivisor(int cal1, int cal2)
{
	return cal1 + cal2;
}

void addDivisorFilter()
{
	auto calc1 = computeSomeValue();
	auto calc2 = computeSomeValue();

	auto divisor = computeDivisor(calc1, calc2);

	//제수를 실행 시점에서 계산하게 되면.
	//addDivisorFilter가 반화되면 divisor는 존재하지 않게되므로 참조를 잃게 된다.
	filters.emplace_back([&](int value) {return value % divisor == 0; }); 

	//이러한 명시적 갈무리는 이 람다 표현식이 divisor의 수명에 의존한다는 점이 명확히 나타나는 장점이 있긴하다.
	//이는 [&]가 암시하는 그 어떤 참조도 대상을 잃지 않게 해야한다 라는 일반적인 경고 보다 좀더 구체적 이다.
	filters.emplace_back([&divisor](int value) {return value % divisor == 0; });
}

//클로저가 즉시 사용되며, 클로저가 복사되지 않을 것을 알고 있다면
//클로저 가진 참조가 해당 람다가 생성된 환경 안의 지역 변수나 매개변수보다 오래 살아남을 위험은 없다.
//그렇다면 참조가 대상을 잃는 일도 생기지 않을 것이므로 기본 참조 갈무리 모드를 피할 필요도 없지 않을까나?
template<typename C>
void workWithContainer(const C& container)
{
	auto calc1 = computeSomeValue();
	auto calc2 = computeSomeValue();

	auto divisor = computeDivisor(calc1, calc2);

	using ContElemT = typename C::value_type; //컨테이에 담긴 데이터 형식.

	//all_of 한 구간의 모든 요소가 주어진 조건을 만족하는지의 여부를 돌려주는 알고리즘
	if (std::all_of(std::begin(container), std::end(container),
		[&](const auto& value) //c++14 람다의 매개변수에 auto사용 가능.
		//[&](const ContElemT& value)
		{ return value % divisor == 0; }))
	{
		//컨테이너의 모든 값이 divisor의 배수라면 여기
	}
	else
	{
		//적어도 하나는 아닌경우.
	}

	//위 람다가 다른 문맥에서도 유용할거라고 생각한 프로그래머가 이를 함수 컨테이너에 복사해서 다른곳에서 사용할 가능성이 있다.
	//람다의 갈무리 절에 divisor의 수명을 분석해 봐야 한다는 점을 알려주는 힌트가 전혀 없으므로. 프로그래머로서는 그런 문제를
	//인식하기가 쉽지 않다.
	//장기적으로 볼때, 람다가 의존하는 지역 변수, 매개변수를 명시적으로 나열하는것이 좋다.

	//해소 방법은 기본값 갈무리 모드를 사용하는 것이다.
	//그렇다고 이게 완벽하지는 않다 포인터의 경우 포인터 외부에서 delete가 호출되서 사라질 가능성이 있다.
	//[=](const auto& value) { return value % divisor == 0; } 
}
///////////////////////////////////////////////////////////////////////////////////////////////
class Widget
{
public:
	void addFilter() const;
private:
	int divisor;
};

void Widget::addFilter() const
{
	//갈무리 는 오직 람다가 생성된 범위 안에서 보이는, static이 아닌 지역변수, 매개변수에만 적용된다.
	//여기서 divoisor는 클래스의 한 자료 멤버이므로 갈무리 될 수 없다.
	//기본값 갈무리를 사용하는 이 버전에서는 컴파일러는 이 코드를 마치 이렇게 취급한다.
	filters.emplace_back([=](int value) { return value % divisor == 0; });

	auto currentObjectPtr = this;
	filters.emplace_back([currentObjectPtr](int value) { return value % currentObjectPtr->divisor == 0; });

	//C++14에는 자료 멤버를 갈무리하는 더 나은 방법이 있다.
	//일반화된 람다 갈무리를 사용하는것이다.
	filters.emplace_back([divisor = divisor](int value) { return value % divisor == 0; });
	//filters.emplace_back([divisor](int value) { return value % divisor == 0; }); //오류! 갈무리할 지역 divisor가 없음(참조도 마찬가지)
	//filters.emplace_back([](int value) { return value % divisor == 0; }); //컴파일 안됨~
}

//위처럼 포인터에 의존하고 있으니 아래 예제처럼 스마트 포인터를 써서 파괴될때 위에서 넣은 람다는 대상을 잃게 된다.
void doSomeWork()
{
	auto pw = std::make_unique<Widget>();

	pw->addFilter();

	//여기서 Widget이 파괴된다. filters에는 대상을 잃은 포인터가 존재한다.
}
///////////////////////////////////////////////////////////////////////////////////////////////
//값에 의한 기본 갈무리 모드의 또 다른 단점은, 해당 클로저가 자기 완결적이고 클로저 바깥에서 일어나는
//자료의 변화로부터 격리되어 있다는 오해를 부를 수 있다는 점이다.
//왜냐하면 정적 저장소 수명기간을 가진 객체에도 의존할 수 있기 때문이다.
//전역범위나 이름공간 범위에서 정의된 객체와 클래스, 함수, 파일 안에서 static으로 선언된 객체가 그런 객체에 해당한다.
//그런 객체는 람다안에서 사용할 수 있지만, 갈무리 할 수는 없다.
void addDivisorFilter2()
{
	//기본 값 갈무리 모드의 표기는 마치 위와 같은 설명에  객체도 모두 갈무리 된다는 느낌을 준다.
	static auto calc1 = computeSomeValue();
	static auto calc2 = computeSomeValue();

	static auto divisor = computeDivisor(calc1, calc2);

	//이 람다는 divisor를 참조로 갈무리 한것과 같으며,
	//이는 기본값 갈무리 모드가 뜻하는 바와 직접적으로 모순된다.
	filters.emplace_back(
		[=](int value) 
		{return value % divisor == 0; }); //아무것도 갈무리하지 않음, 위의 정적 변수를 지칭한다.

	++divisor; //divisor를 수정한다.
}

void main()
{
}