#include <iostream>
#include <memory>
#include <vector>
#include <list>
using namespace std;

//������ �����ϰ� ��, ������ Ŭ�������� ȣ���Ϸ����ϸ� ���ǰ� �����Ƿ� ��ũ ����.
class foo
{
private:
	foo(const foo&);			//not defined
	foo& operator=(const foo&); //not defined
};

//C++11 ������ �Լ�
//��� ������, friend�Լ����� ��ü�� �����Ϸ� �ϸ� �����Ͽ� �����Ѵ�.
//��ũ ������ ������ �������� �������� ����
class foo2
{
public: //�ۺ����� �ϴ°��� ���� private���� ���� ������ ���� �ʱ� ���ؼ�.
	foo2(const foo2&) = delete;
	foo2& operator=(const foo2&) = delete;
};

//������ �Լ��� �׾ �Լ��� ���� �� �� �ִ�.
bool isLucky(int number) { return true; }

bool isLucky(char) = delete;
bool isLucky(bool) = delete;
bool isLucky(double) = delete; //double�� float�� ��� ����.

//������ �Լ��� ���ø� �ν��Ͻ�ȭ�� �����Ѵ�.
template<typename T>
void processPointer(T* ptr)
{
}

template<>
void processPointer<void>(void*) = delete;
template<>
void processPointer<char>(char*) = delete;

template<>
void processPointer<const void>(const void*) = delete;
template<>
void processPointer<const char>(const char*) = delete;

//���ø��� �Ϻ� �ν��Ͻ�ȭ�� �����Ϸ��� �������� private������ ������ �� ����.
class Widget
{
public:
	template<typename T>
	void processPointer(T* ptr) {};

private:
	template<>
	void processPointer<void>(void* ptr) {}; //������ ����! ���� ��ü���Ǵ� �༮�̶� ���ټ��̴ٸ� ���� ����.
										  //���ø� Ư��ȭ�� �ݵ�� Ŭ���� ������ �ƴ϶� �̸� ���� �������� �ۼ��ؾ��Ѵ�.
};

class Widget2
{
public:
	template<typename T>
	void processPointer(T* ptr) {};
};

template<>
void Widget2::processPointer<void>(void*) = delete; //�ۺ������� �����Ǿ���.


int main()
{
	//if (isLucky('a'))
	{

	}

	//if (isLucky(true))
	{

	}

	//if (isLucky(3.5))
	{

	}

	return 0;
}