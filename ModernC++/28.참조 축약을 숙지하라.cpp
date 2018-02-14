#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <chrono>
using namespace std;

//�������� �Ű��������� �� �μ��� �ް�����, ������������ ��ȣȭ �Ǿ� �ִ�.
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

//2.�⺻������ std::forward�� �۵��ϴ� ���� �� ���� ��� �����̴�.
template<typename T>
T&& forward(typename remove_reference<T>::type& param)
{
	return static_cast<T&&>(param); //T& && �ް� -> T&
	                                //T&& && ������ -> T&&
}

template<typename T>
void someFunc(T&& fParam)
{
}

template<typename T>
void f(T&& fParam)
{
	//param��ü�� �ް�. T�� �����Ϸ��°� forward�� ����.
	someFunc(forward<T>(fParam));
}

int main()
{
	//1.������ ���� ������ �����̴�.
	int x;
	//auto& && rx = x; //����! ������ ���� ������ ������ �� ����.

	Widget w;

	func(w); //T�� Widget&�� �����ȴ�.
	         //void func(Widget& && param)�� ���� ����̵ȴ�.
	         //���������� ������ �ҹ������� �����Ϸ��� �������� �ʴ´�.
	         //param�� �ް����� �ʱ�ȭ �ǹǷ� param�� ������ �ް� ������ �ȴ�.
	         //void func(Widget& param) �������°� �ȴ�.
			
	//��� �̷�����? ���� ���� ����̴�. ������ ���� ������ ����������, �����Ϸ��� ������ ���� ������ �����ϴ� ���� ���ȴ�.

	//���� ��� ��Ģ
	//�� ���� �� �ϳ��� �ް� �����̸� ����� �ް� ����.
	//�Ѵ� ������ �����̸� ����� ������ �����̴�.

	func(widgetFactory()); //T�� Widget���� �����ǰ� &&�� �ٴ´�.

	return 0;
}