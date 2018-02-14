#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
using namespace std;

class Widget
{
};

//T&&는 보편참조이다, &&가 되기도 하고 &가 되기도 한다.
template<typename T>
void f(T&& param) {}; //param 보편참조

Widget var1;
auto&& var2 = var1; //var2는 보편참조.

//형식 연역이 일어나지 않는 문맥에서 T&&는 오른값 참조이다.
void f(Widget&& val) //val는 오른값 참조.
{
}

//참조의 형태가 T&&의 형태이어야 보편참조이다. 이 경우 보편참조가 될 수 없다.
template<typename T>
void f1(std::vector<T>&& param) {}; //Widget&& 랑 같게 생각하면 된다.

template<typename T>
void f2(const T&& param) {}; //param은 오른값참조 (보편참조 아님) const가 붙어서 하나의 타입으로 박힌다고 생각하면?

//T&&라고 다 보편참조는 아니다.
template<class T, class Allocator = allocator<T>>
class myvector
{
public:
	void push_back(T&& x); //확실히 형태는 보편참조이지만..
	//void push_back(Widget&& x); //일허게 치환되버린다 결국 오른값 참조.
};

myvector<Widget> v; //push_back의 형식을 완전히 결정한다.

//반면 emplace_back은 형식 연역을 사용한다.
template<class T, class Allocator = allocator<T>>
class myvector2
{
public:
	template<class... Args>
	void emplace_back(Args&&... args); //args는 벡터형식의 매개변수 T와 독립적이다.
};

//람다의 보편참조
auto timeFuncInvo = [](auto&& func, auto&&... params) //func 보편참조. params임의의 형식, 임의의 개수의 0개이상의 보편참조들.
{
	//타이머 시작.
	std::forward<decltype(func)>(func)(std::forward<decltype(params)>(params)...);
	//경과 시간 기록.
};

int main()
{
	Widget v2;

	//f(v2); //오류! 타입이 다르기 때문에 넘길수 없다.
	f(Widget()); //성공!

	//Widget&& v3 = v2; //오류! 타입이 다름.
	Widget&& v4 = Widget(); //성공! 형식 연역없음 v4 오른값 참조

	//보편 참조가 오른값 참조를 나타내는지 왼값 참조를 나타내는지는 보편 참조의 초기치가 결정한다.
	//초기치가 오른값이면 보편참조는 오른값 참조에해당
	//초기치가 왼값이면 보편참조는 왼값 참조에해당

	Widget w;
	f(w); //param형식은 Widget&
	f(std::move(w)); //param형식은 Widget&&

	std::vector<int> v;

	//f1(v); //오류! 오른값참조에 왼값을 넘김.
	f1(std::move(v)); 

	return 0;
}