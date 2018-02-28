#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <chrono>
#include <array>
#include <functional>
using namespace std;

using Time = std::chrono::steady_clock::time_point; //시간상의 한지점

enum class Sound {Beep, Siren, Whistle};

using Duration = std::chrono::steady_clock::duration; //시간의 길이

using Volume = std::chrono::steady_clock::duration;

void setAlram(Time t, Sound s, Duration d)
{
}

void foo()
{
	auto setSoundL = 
		[](Sound s)
	{
		using namespace std::chrono;        

		//한시간 후부터 지정된 경보음 30초 발생.
		setAlram(steady_clock::now() + hours(1), s, seconds(30));
	};

	//C++14에서는 표준 접미사 들을 이용해서 더욱 간결하게 작성 할 수 있다.
	auto setSoundL2 =
		[](Sound s)
	{
		using namespace std::chrono;
		using namespace std::literals;
		//한시간 후부터 지정된 경보음 30초 발생.
		setAlram(steady_clock::now() + 1h, s, 30s);
	};
}

//위와 같은 일을 하는 함수를std::bind를 이용해서 작성하면.
void foo1()
{
	using namespace std::chrono;
	using namespace std::literals;

	using namespace std::placeholders; //_1을 사용하는데 필요함.

	//인수의 형식을 알수없다 따라서 setAlram의 선언을 봐야한다.
	//람다의 경우 steady_clock::now()는 람다 함수가 호출되는 시점에서 평가된다.
	//바인드는 바인드를 한 시간부터 1시간이 되버리므로 이상해진다.
	auto setSoundB = std::bind(setAlram, steady_clock::now() + 1h, _1, 30s);

	//위 문제를 바로 잡으려면 그 표현식을 setAlram 호출 때까지 지연하라 std::bind에게 알려주어야 한다.
	//그러러면 첫 std::bind안에 두개의 함수 호출을 내포시켜야 한다.
	auto setSoundB2 = std::bind(setAlram, 
								std::bind(std::plus<>(), std::bind(steady_clock::now), 1h) //함수를 바인드하면 함수 자체가 불리는 시점까지 지연된다.
								, _1, 30s);

	//C++11
	auto setSoundB3 = std::bind(setAlram,
		std::bind(std::plus<steady_clock::time_point>(), std::bind(steady_clock::now), 1h)
		, _1, 30s);
}

//setAlarm을 중복적재하면 bind에 새로운 문제가 발생한다.
void setAlram(Time t, Sound s, Duration d, Volume v)
{
}

void foo2()
{
	using namespace std::chrono;
	using namespace std::literals;

	using namespace std::placeholders;

	auto setSoundL =
		[](Sound s)
	{
		using namespace std::chrono;

		setAlram(steady_clock::now() + 1h, s, 30s);
	};//람다는 잘 작동된다.

	//C++11
	//auto setSoundB3 = std::bind(setAlram,
	//	std::bind(std::plus<steady_clock::time_point>(), std::bind(steady_clock::now), 1h)
	//	, _1, 30s); //어떤 setAlram인지를 모르므로 컴파일되지 않는다.(컴파일러가 알고 있는것은 이름뿐이며 이름만으로는 중이성 해소가 안된다)

	using SetAlarm3ParamType = void(*)(Time t, Sound s, Duration d);

	auto setSoundB = std::bind(static_cast<SetAlarm3ParamType>(setAlram),
		std::bind(std::plus<>(), std::bind(steady_clock::now), 1h)
		, _1, 30s); //이러면 호출은 된다.

	//하지만 위처럼 하면 람다와 차이점이 하나 더 생긴다.
	setSoundL(Sound::Siren); //인라인될 가능성이 크다.

	//여기는 함수 포인터를 호출하여 호출이 일어난다. 함수 포인터를 이용한 호출은 인라인 가능성이 적다.
	setSoundB(Sound::Siren); 
}

//C++14 람다의 장점
void foo3()
{
	int lowVal = 10;
	int highVal = 20;

	auto betweenL = 
		[lowVal, highVal](const auto& val)
	{
		return lowVal <= val && val <= highVal; 
	};

	//bind로하면
	using namespace std::placeholders;

	auto betweenB =
		std::bind(std::logical_and<>(),
			std::bind(std::less_equal<>(), lowVal, _1),
			std::bind(std::less_equal<>(), _1, highVal)
		);

	//C++11에서는 명시적으로 형식들을 지정해야하므로 다음과 같은 코드가 나온다.
	auto betweenB2 =
		std::bind(std::logical_and<bool>(),
			std::bind(std::less_equal<int>(), lowVal, _1),
			std::bind(std::less_equal<int>(), _1, highVal)
		);

	//물론 C++11에서 람다가 auto를 받지 못하므로 형식을 지정해야 한다.
	auto betweenL2 =
		[lowVal, highVal](const int& val)
	{
		return lowVal <= val && val <= highVal;
	};
}

//자리표
enum class CompLevel { Low, Normal, High };

class Widget
{
};

Widget compress(const Widget& w, CompLevel lev)
{
	return Widget();
}

void foo4()
{
	Widget w;
	using namespace std::placeholders;
	
	//w는 값으로 저장된다. 하지만 호출 구문만으로는 추론이 힘들다. 작동방식을 안다면 몰라도.
	auto compressRateB = std::bind(compress, w, _1);
	
	//참조로 전달하는 방법도 있다.
	auto compressRateB2 = std::bind(compress, std::ref(w), _1);

	//람다는 그에 비해 명확하다.
	auto compressRateL = 
		[w](CompLevel lev)
	{
		return compress(w, lev); //w는 값으로 갈무리되고, lev는 값으로 전달된다.
	};

	compressRateL(CompLevel::High); //인수는 값으로 전달된다

	compressRateB(CompLevel::High); //바인드 객체에 전달되는 모든 인수는 참조로 전달된다.
								    //이는 그런 객체의 함수 호출 연산자가 완벽 전달을 사용하기 때문이다

	//C++14에서 std::bind를 사용하는 것이 합당한 경우가 없다.
}

//C++11에서 bind사용이 정당화 되는 경우
//1. 이동 갈무리 : C++11 람다는 이동 갈무리를 지원하지 않는다. 그러나 람다와 bind의 조합으로 이동갈무리를 흉내낼수 있다.(항목32)
//2. 다형적 함수 객체 : 바인드 객체에 대한 함수 호출 연산자는 완벽 전달을 사용하기 때문에 그 어떤 형식의 인수들도 받을 수 있다.
//   (단 항목30에서 설명한 완벽 전달의 제약 안에서) 이는 객체를 템플릿화된 함수 호출 연산자와 묶으려 할때 유용하다.



void main()
{
}