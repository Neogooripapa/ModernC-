#include <iostream>
#include <array>
using namespace std;

//&& Rvalue ���� : ���, �ӽú���
//"�������� ��� �޸� ��ġ�� ����Ű�µ�, & �����ڸ� ���� �� ��ġ�� ������ �� �ִ�. �ٸ����� �ּҸ� ���� �� �ִ�"
//"X& �� ������ ���� ��� �θ����� �սô�"
//"���� �̸��� �ִٸ� ������, ���ٸ� �������̴�."

template<typename T>
void f1(T& param)
{
}

template<typename T>
void f2(const T& param)
{
}

template<typename T>
void f3(T* param)
{
}

template<typename T>
void f4(T&& param)
{
}

template<typename T>
void f5(T param)
{
}

//�迭�� ���� ������ �����ϴ� �ɷ��� �̿��ϸ� �迭�� ��� ���ҵ��� ������ �����ϴ� ���ø��� ���� �� �ִ�.
//������ ������ ����� �����ش�.
//�迭 �Ű������� �̸��� ������ �������� �ʿ��Ѱ��� ���� ���� ���̱� ������.
//constexpr�Լ��� ȣ������ �������߿� �����ϰ� �����.
template<typename T, std::size_t N>
constexpr std::size_t arraySize(T(&)[N] ) noexcept
{
	return N;
}

void someFunc(int, double)
{
}

int main()
{
	//template<typename T> TŸ�� ����
	//void f(ParamType param) ParamType����
	//f(expr)

	int a = 1;
	int b = 1;

	int d = a + b; //���� �� �ӽ� ���� �ı�.
	int&&c = a + b; //���絵 �ı��� �Ͼ�� ����.

	//1.���������̰� ���������� �ƴ�.
	int x = 27;
	const int cx = x;
	const int& rx = x;

	f1(x);  //T�� int      , param�� int&
	f1(cx); //T�� const int, param�� const int&
	f1(rx); //T�� const int, param�� const int& //rx�� �������� ���õ�.

	f2(x);  //T�� int, param�� const int&
	f2(cx); //T�� int, param�� const int& //T�� const�� �������� �ָ�.
	f2(rx); //T�� int, param�� const int& //T�� const�� �������� �ָ�. //rx�� �������� ���õ�.

	int x1 = 27;
	const int* px = &x1;

	f3(&x); //T�� int      , param�� int*
	f3(px); //T�� const int, param�� const int*

	//2.���������� ���
	//expr�� �ް��̸�, T�� ParamType �Ѵ� �ް� ������ �����ȴ�.

	f4(x);   //x��  �ް�   T�� int&, param�� int&			 
	f4(cx);  //cx�� �ް�   T�� const int&, param�� const int& 
	f4(rx);  //rx�� �ް�   T�� const int&, param�� const int&
	f4(27);  //27�� ������ T�� int, param�� int&& //���� ��Ģ

	//3.ParamType�� �����͵� �ƴϰ� ������ �ƴ�
	//expr�� ������ �����̸�, �����κ��� ���õȴ�.
	//expr�� �������� ��������, ���� expr�� const�̸� const���� �����Ѵ�. ���� volatile�̸� ���� ����
	//param�� cx��rx�� �������� ���纻�̹Ƿ� �翬�� ����̴�.
	//cx��rx�� �����ɼ� ���ٴ� ���� param�� �������� ���οʹ� ���� �����ϴ�.

	f5(x);  //T�� int, param�� int
	f5(cx); //T�� int, param�� int
	f5(rx); //T�� int, param�� int

	const char* const ptr = "Fun C++";

	f5(ptr); //T�� const char*, param�� const char* //ptr�� ����Ű�� ���� const���� �����ȴ�.

	int pp = 1;
	const int* ptr1 = &pp; //����Ű�� ���� const
	int* const ptr2 = &pp; //������ ��ü�� const

	//4.�迭�μ�
	const char name[] = "pcw";    //name�� ������ const char[3]
	const char* ptrToName = name; //�迭�� �����ͷ� �ر��Ѵ�.

	f5(name); //name�� �迭������ const char* �� �����Ѵ�.
	f1(name); //T�� const char [3]���� �����ǰ�, param�� const char (&)[3]���� �����ȴ�.

	int size = arraySize(name);

	int keyVals[] = { 1,3,5 };

	int mappedVals[arraySize(keyVals)]; //�������߿� ����������.
	std::array<int, arraySize(keyVals)> mappedVals2;

	//5.�Լ��μ�
	//�Լ������ͷ� �ر��Ѵ�.
	f5(someFunc); //param �� void (*) (int, double)
	f1(someFunc); //param �� void (&) (int, double)
	return 0;
}
