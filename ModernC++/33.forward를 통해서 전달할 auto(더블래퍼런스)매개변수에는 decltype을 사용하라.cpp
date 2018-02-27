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

//������ �Ʒ��� ������
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
	//���� �Ϻ������� �ϰ� �ʹٸ�?
	auto f = [](auto&& x) 
	{
		//���ø��̾����� T�� �����߰����� ���⼱ ġĪ�� �� ����.
		return normalize(std::forward<decltype(x)>(x)); 
	};

	//�ᱹ �������������϶� �������� �����϶�� ���ʿ� ���� �ʰ� ������ ������ �����ص� �����ϹǷ� �ϰ� ����.
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

//������ ��� �������� ���͵� �ް��� ���޵�����.
void ff(Widget&& w)
{
	forward(w); //w��ü�� �ް�.
}

Widget&& forward(Widget& param)
{
	return static_cast<Widget&&>(param);
}

//���Ͱ��� ��Ŭ Ÿ���� ����Ѵٸ�
Widget&& && forward2(Widget& param)
{
	return static_cast<Widget&& &&>(param);
}

//��ó�� ����� �Ͼ�� ���� ���� ���������� �� �� �ִ�.
Widget&& forward3(Widget& param)
{
	return static_cast<Widget&&>(param);
}

void main()
{
	
}