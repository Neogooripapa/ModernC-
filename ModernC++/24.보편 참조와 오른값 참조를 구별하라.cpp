#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
using namespace std;

class Widget
{
};

//T&&�� ���������̴�, &&�� �Ǳ⵵ �ϰ� &�� �Ǳ⵵ �Ѵ�.
template<typename T>
void f(T&& param) {}; //param ��������

Widget var1;
auto&& var2 = var1; //var2�� ��������.

//���� ������ �Ͼ�� �ʴ� ���ƿ��� T&&�� ������ �����̴�.
void f(Widget&& val) //val�� ������ ����.
{
}

//������ ���°� T&&�� �����̾�� ���������̴�. �� ��� ���������� �� �� ����.
template<typename T>
void f1(std::vector<T>&& param) {}; //Widget&& �� ���� �����ϸ� �ȴ�.

template<typename T>
void f2(const T&& param) {}; //param�� ���������� (�������� �ƴ�) const�� �پ �ϳ��� Ÿ������ �����ٰ� �����ϸ�?

//T&&��� �� ���������� �ƴϴ�.
template<class T, class Allocator = allocator<T>>
class myvector
{
public:
	void push_back(T&& x); //Ȯ���� ���´� ��������������..
	//void push_back(Widget&& x); //����� ġȯ�ǹ����� �ᱹ ������ ����.
};

myvector<Widget> v; //push_back�� ������ ������ �����Ѵ�.

//�ݸ� emplace_back�� ���� ������ ����Ѵ�.
template<class T, class Allocator = allocator<T>>
class myvector2
{
public:
	template<class... Args>
	void emplace_back(Args&&... args); //args�� ���������� �Ű����� T�� �������̴�.
};

//������ ��������
auto timeFuncInvo = [](auto&& func, auto&&... params) //func ��������. params������ ����, ������ ������ 0���̻��� ����������.
{
	//Ÿ�̸� ����.
	std::forward<decltype(func)>(func)(std::forward<decltype(params)>(params)...);
	//��� �ð� ���.
};

int main()
{
	Widget v2;

	//f(v2); //����! Ÿ���� �ٸ��� ������ �ѱ�� ����.
	f(Widget()); //����!

	//Widget&& v3 = v2; //����! Ÿ���� �ٸ�.
	Widget&& v4 = Widget(); //����! ���� �������� v4 ������ ����

	//���� ������ ������ ������ ��Ÿ������ �ް� ������ ��Ÿ�������� ���� ������ �ʱ�ġ�� �����Ѵ�.
	//�ʱ�ġ�� �������̸� ���������� ������ �������ش�
	//�ʱ�ġ�� �ް��̸� ���������� �ް� �������ش�

	Widget w;
	f(w); //param������ Widget&
	f(std::move(w)); //param������ Widget&&

	std::vector<int> v;

	//f1(v); //����! ������������ �ް��� �ѱ�.
	f1(std::move(v)); 

	return 0;
}