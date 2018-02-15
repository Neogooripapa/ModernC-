#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <chrono>
using namespace std;


//2.기본적으로 std::forward가 작동하는 것은 이 참조 축약 덕분이다.
template<typename T>
T&& forward(remove_reference_t<T>& param)
{
	return static_cast<T&&>(param); //T& && 왼값 -> T&
	                                //T&& && 오른값 -> T&&
}

//예를 들면 Widget이 왼값으로 전달된 경우.
/*
Widget& && forward(typename remove_reference<Widget&>::type& param)
{
	return static_cast<Widget& &&>(param);
}

//참조 축약이 적용되어 요러코롱 변한다.
Widget& forward(Widget& param)
{
	return static_cast<Widget&>param);
}
*/
//예를 들어 Wiget이 오른값으로 전달된 경우.
/*
Widget&& && forward(typename remove_reference<Widget>::type& param)
{
	return static_cast<Widget&& &&>(param);
}

Widget&& forward(Widget& param)
{
	return static_cast<Widget&&>(param);
}
*/

template<typename T>
void someFunc(T&& fParam)
{
}

template<typename T>
void f(T&& fParam)
{
	//param자체는 왼값. T를 복원하려는게 forward의 목적.
	someFunc(forward<T>(fParam));
}

//1.참조에 대한 참조는 위법이다.

//보편참조 매개변수에는 그 인수가 왼값인지, 오른값인지가 부호화 되어 있다.
template<typename T>
void func(T&& param)
{
}

class Widget
{
};

Widget widgetFactory()
{
	return Widget();
}


int main()
{
	
	int x;
	//auto& && rx = x; //오류! 참조에 대한 참조는 선언할 수 없음.

	Widget w;

	func(widgetFactory()); //T는 Widget으로 연역되고 &&가 붙는다.

	func(w); //T는 Widget&로 연역된다.
	         //void func(Widget& && param)과 같은 모습이된다.
	         //참조에대한 참조는 불법이지만 컴파일러는 불평하지 않는다.
	         //param은 왼값으로 초기화 되므로 param의 형식은 왼값 참조가 된다.
	         //void func(Widget& param) 최종형태가 된다.
			
	//어떻게 이런일이? 답은 참조 축약이다. 참조에 대한 참조가 위법이지만, 컴파일러가 참조에 대한 참조를 산출하는 것이 허용된다.

	//참조 축약 규칙
	//두 참조 중 하나라도 왼값 참조이면 결과는 왼값 참조.
	//둘다 오른값 참조이면 결과는 오른값 참조이다.

	auto&& w1 = w; //auto의 형식은 Widget& && => Widget&
	
	auto&& w2 = widgetFactory(); //auto의 형식은 Widget && => Widget&& (오른값이면 auto는 비참조 형식 Widget이된다!)

	//따라서 보편 참조는 사실상 오른쪽 참조이다.
	//1.형식 연역에서 왼값과 오른값이 구분된다. T형식의 왼값은 형식 T&로 연역되고, T 형식의 오른값은 형식T로 연역된다.
	//2.참조 축약이 적용된다.

	//3.나머지 참조 축약이 적용되는경우
	//typedef가 지정 또는 평가되는 도중에 참조에 대한 참조가 발생하면 참조축약이 끼어들어 참조에 대한 참조를 제거한다.

	template<typename T>
	class Widget2
	{
	public:
		typedef T&& RvalueRefToT;
	};
	//그리고 Widget2을 왼값 참조 형식으로 인스턴트화 한다고 하자.
	//Widget2<int&> w;

	//typedef int& && RvalueRefToT => typedef int& RvalueRefToT

	//참조 축약이 일어나는 마지막 문맥은 decltype 사용이다.
	//컴파일러가 decltype에 관여하는 형식을 분석하는 도중에 참조에 대한 참조가 발생하면
	//참조 축약이 등장해서 그것을 제거한다.

	return 0;
}