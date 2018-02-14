#include <iostream>
#include <vector>
using namespace std;

void someFunc(int, double) {}

template<typename T>
void f(T param) {}

template<typename T>
void t(std::initializer_list<T> param) {}

auto createInitList()
{
	//return { 1,2,3 }; //����! ���ø��� ���� ������ ������ ������.
	return 0;
}

int main()
{
	//1.auto�� �̿��ؼ� ������ ������ �� auto�� ���ø��� T�� ������ ������ �Ѵ�.
	auto x = 27;
	//template<typename T>
	//void func_for_x(T param);
	//func_for_x(27);

	const auto cx = x;
	//template<typename T>
	//void func_for_x(const T param);
	//func_for_x(x);

	const auto& rx = x;
	//template<typename T>
	//void func_for_x(const T& param);
	//func_for_x(x);

	//2.���� �����ڰ� ���� ������ ���.
	auto&& uref1 = x; //int&
	auto&& uref2 = cx;//const int&
	auto&& uref3 = 27; //int&&

	//3.auto ������ ������ ���� ���� �ر�
	const char name[] = "pcwmp";

	auto arr1 = name; //const char*
	auto& arr2 = name; //const char (&) [6]

	auto func1 = someFunc; //void (*) (int, double)
	auto& func2 = someFunc; //void (&) (int, double)

	//4.auto�� ���ø� T�� �ٸ���(auto�� ���� Ư���� ���� ���� ��Ģ)

	int x1 = 27; //���� int, ���� 27�� ������ ����.
	int x2(27); //���� int, ���� 27�� ������ ����.
	int x3 = { 27 }; //std::initializer_list<int> ������ ������ ����
	int x4{ 27 }; //std::initializer_list<int> ������ ������ ����(N3922������ int)

	//auto�� ����� ������ �ʱ⸦ �߰�ȣ�� ���θ� ������ ������ std::initializer_list�̴�.

	//auto x5 = { 1,2,3.0f }; //����! std::initializer_list<T>�� T�� ������ �� ����.

	auto xx = { 11, 23, 9 };
	
	//f({ 11, 23, 9 }); //����! T�� ���� ������ ������ �� ����.
	t({ 11, 23, 9 });

	//5.���� �Ű������� ���� auto, �Լ� ��ȯ ������ auto�� ���ø� ���� ������ ����ȴ�.
	createInitList();

	std::vector<int> v;

	auto resetV = [&v](const auto& newValue) { v = newValue; };

	//resetV({1,2,3}); //���� ���ø� ���� ������ ����ϹǷ�.
	return 0;
}