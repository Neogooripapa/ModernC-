#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <chrono>
using namespace std;

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

//2.기본적으로 std::forward가 작동하는 것은 이 참조 축약 덕분이다.
template<typename T>
T&& forward(typename remove_reference<T>::type& param)
{
	return static_cast<T&&>(param); //T& && 왼값 -> T&
	                                //T&& && 오른값 -> T&&
}

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

int main()
{
	//1.참조에 대한 참조는 위법이다.
	int x;
	//auto& && rx = x; //오류! 참조에 대한 참조는 선언할 수 없음.

	Widget w;

	func(w); //T는 Widget&로 연역된다.
	         //void func(Widget& && param)과 같은 모습이된다.
	         //참조에대한 참조는 불법이지만 컴파일러는 불평하지 않는다.
	         //param은 왼값으로 초기화 되므로 param의 형식은 왼값 참조가 된다.
	         //void func(Widget& param) 최종형태가 된다.
			
	//어떻게 이런일이? 답은 참조 축약이다. 참조에 대한 참조가 위법이지만, 컴파일러가 참조에 대한 참조를 산출하는 것이 허용된다.

	//참조 축약 규칙
	//두 참조 중 하나라도 왼값 참조이면 결과는 왼값 참조.
	//둘다 오른값 참조이면 결과는 오른값 참조이다.

	func(widgetFactory()); //T는 Widget으로 연역되고 &&가 붙는다.

	return 0;
}