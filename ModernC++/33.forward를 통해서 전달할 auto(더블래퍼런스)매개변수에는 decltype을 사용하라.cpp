#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <chrono>
#include <array>
#include <functional>
using namespace std;

void normalize(int&& x)
{
}

auto f = [](auto x) {return normalize(x); };

//구현은 아래와 같은것
class
{
public:
	template<typename T>
	auto operator()(T x) const
	{
		return normalize(x);
	}
};

void foo()
{
	//만약 완벽전달을 하고 싶다면?
	auto f = [](auto&& x) 
	{
		//템플릿이었으면 T를 지정했겠지만 여기선 치칭할 수 없다.
		return normalize(std::forward<decltype(x)>(x)); 
	};

	//결국 오른값참조에일때 비참조를 전달하라는 관례와 맞지 않게 오른값 참조를 전달해도 동일하므로 믿고 쓰자.
	auto f1 = [](auto&&... xs)
	{
		return normalize(std::forward<decltype(xs)>(xs)...);
	};
}

class Widget
{
};


template<typename T>
T&& forward_my(remove_reference_t<T>& param)
{
	return static_cast<T&&>(param);
}

//보통의 경우 오른값이 들어와도 왼값이 전달되지만.
void ff(Widget&& w)
{
	forward(w); //w자체는 왼값.
}

Widget&& forward(Widget& param)
{
	return static_cast<Widget&&>(param);
}

//위와같이 디클 타입을 사용한다면
Widget&& && forward2(Widget& param)
{
	return static_cast<Widget&& &&>(param);
}

//이처럼 축약이 일어나고 보통 경우와 동일해짐을 알 수 있다.
Widget&& forward3(Widget& param)
{
	return static_cast<Widget&&>(param);
}

void main()
{
	
}