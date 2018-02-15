#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <chrono>
using namespace std;


//2.�⺻������ std::forward�� �۵��ϴ� ���� �� ���� ��� �����̴�.
template<typename T>
T&& forward(remove_reference_t<T>& param)
{
	return static_cast<T&&>(param); //T& && �ް� -> T&
	                                //T&& && ������ -> T&&
}

//���� ��� Widget�� �ް����� ���޵� ���.
/*
Widget& && forward(typename remove_reference<Widget&>::type& param)
{
	return static_cast<Widget& &&>(param);
}

//���� ����� ����Ǿ� �䷯�ڷ� ���Ѵ�.
Widget& forward(Widget& param)
{
	return static_cast<Widget&>param);
}
*/
//���� ��� Wiget�� ���������� ���޵� ���.
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
	//param��ü�� �ް�. T�� �����Ϸ��°� forward�� ����.
	someFunc(forward<T>(fParam));
}

//1.������ ���� ������ �����̴�.

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


int main()
{
	
	int x;
	//auto& && rx = x; //����! ������ ���� ������ ������ �� ����.

	Widget w;

	func(widgetFactory()); //T�� Widget���� �����ǰ� &&�� �ٴ´�.

	func(w); //T�� Widget&�� �����ȴ�.
	         //void func(Widget& && param)�� ���� ����̵ȴ�.
	         //���������� ������ �ҹ������� �����Ϸ��� �������� �ʴ´�.
	         //param�� �ް����� �ʱ�ȭ �ǹǷ� param�� ������ �ް� ������ �ȴ�.
	         //void func(Widget& param) �������°� �ȴ�.
			
	//��� �̷�����? ���� ���� ����̴�. ������ ���� ������ ����������, �����Ϸ��� ������ ���� ������ �����ϴ� ���� ���ȴ�.

	//���� ��� ��Ģ
	//�� ���� �� �ϳ��� �ް� �����̸� ����� �ް� ����.
	//�Ѵ� ������ �����̸� ����� ������ �����̴�.

	auto&& w1 = w; //auto�� ������ Widget& && => Widget&
	
	auto&& w2 = widgetFactory(); //auto�� ������ Widget && => Widget&& (�������̸� auto�� ������ ���� Widget�̵ȴ�!)

	//���� ���� ������ ��ǻ� ������ �����̴�.
	//1.���� �������� �ް��� �������� ���еȴ�. T������ �ް��� ���� T&�� �����ǰ�, T ������ �������� ����T�� �����ȴ�.
	//2.���� ����� ����ȴ�.

	//3.������ ���� ����� ����Ǵ°��
	//typedef�� ���� �Ǵ� �򰡵Ǵ� ���߿� ������ ���� ������ �߻��ϸ� ��������� ������ ������ ���� ������ �����Ѵ�.

	template<typename T>
	class Widget2
	{
	public:
		typedef T&& RvalueRefToT;
	};
	//�׸��� Widget2�� �ް� ���� �������� �ν���Ʈȭ �Ѵٰ� ����.
	//Widget2<int&> w;

	//typedef int& && RvalueRefToT => typedef int& RvalueRefToT

	//���� ����� �Ͼ�� ������ ������ decltype ����̴�.
	//�����Ϸ��� decltype�� �����ϴ� ������ �м��ϴ� ���߿� ������ ���� ������ �߻��ϸ�
	//���� ����� �����ؼ� �װ��� �����Ѵ�.

	return 0;
}