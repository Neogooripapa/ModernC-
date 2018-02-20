#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <chrono>
#include <array>
using namespace std;

//�Ϻ������� �ܼ��� ��ü���� �����ϴ� �͸��� �ƴ϶�.
//�� ��ü���� �ֿ� Ư¡, ����, �ް� ������ ����, const �� volatile ���α����� �����ϴ°��� ���Ѵ�.

void f(const std::vector<int>& v)
{
}

//�����ϴ� �Լ��� �ʿ������� ���׸� �Լ��̴�.
template<typename T>
void fwd(T&& param)
{
	f(std::forward<T>(param));
}

//ǥ�� �����̳��� emplace�� �Լ����� �̷����� ���·� �����Ǿ� �ִ�.
template<typename... Ts>
void fwd2(Ts&&... params)
{
	f(std::forward<Ts>(params)...);
}

//f(ǥ����);   ��ȣ���� �ϴ� �ϰ�
//fwd(ǥ����); ��ȣ���� �ϴ� ���� �ٸ��ٸ�, fwdǥ������ f�� �Ϻ��ϰ� �������� ���Ѱ��̴�.



void foo()
{
	f({ 1,2,3 }); //������ ��ȭ���ϹǷ� OK

	//fwd({ 1,2,3 }); //����! fwd�� �Ű������� std::initializer_list�� �ɼ� ���� ���·� ����Ǿ� �ִ�.

	//���� �� ������ �ϳ��� �������� ������ �Ϻ������� �����Ѵ�.
	//1.fwd�� �Ű������� �� �ϳ� �̻� ���� �����Ϸ��� ������ �������� ���Ѵ�.
	//2.fwd�� �Ű������� �� �ϳ� �̻� ���� �����Ϸ��� ������ �߸����� �Ѵ�.
	//  �ϳ��� ���������δ� fwd�� �ν��Ͻ��� ������ �� �� ���°��̰�.
	//  �� �ϳ��� �����ϵȴٰ� �ص� fwd�� ������ ���ĵ��� �̿��ؼ� ȣ���ϴ� f�� fwd�� ���޵� �μ����
	//  f�� ����ȣ������ ���� �ٸ��� �ൿ�ϴ°��̴�. (f�� �ߺ�����Ȱ�� ����f��ȣ���Ҵ�� �ٸ� �ߺ����� �Լ��� �Ҹ��°��)

	//��̷ӰԵ� auto������ �߰�ȣ �ʱ�ġ�� �ʱ�ȭ�ص� �� ������ �� �����ȴ�.
	auto il = { 1,2,3 };

	fwd(il); //ok il�� f�� �Ϻ��ϰ� ���޵ȴ�.
}

//���� �� ���� static const �� constexpr �ڷ� ���
class Widget
{
public:
	//static const, static constexpr �ڷ� ����� ���� �ϸ�ȴ�.
	static constexpr std::size_t MinVals = 28; //const ���ĸ� �����ؼ� ����� �޸𸮸� ���� �Ҵ����� �ʴ´�.
											   //MinVals�� ��ġ�� ������ 28�̶�� ���� ��ġ�Ѵ�.
};

//constexpr std::size_t Widget::MinVals;

std::vector<int> widgetData;

void f1(std::size_t val)
{
}

template<typename T>
void fwd1(T&& param)
{
	f1(std::forward<T>(param));
}

void foo2()
{
	widgetData.reserve(Widget::MinVals);
	//���� ��ڵ尡 MinValsd�� �ּҸ� ���Ѵٸ� ���ڵ�� �����ϵǱ� ������ MinVals�� ���ǰ� ��� ��ũ�� �����Ѵ�.

	f1(Widget::MinVals);
	//�Ϻ������� ���� �����ν� �����ϰԵȴ�. (���� �ȳ��� �����Ϸ��� ������ �̽ļ��� ����)
	fwd1(Widget::MinVals); //����! ��ũ ����. ���������Լ�(������ ������ó�� ����Ѵ�)
}

//�ߺ� ����� �Լ� �̸��� ���ø� �̸�
void f3( int (*pf) (int) )
//void f( int pf (int) ) ���� ����
{
}

int processVal(int value)
{
	return 0;
}

int processVal(int value, int priority)
{
	return 0;
}

template<typename T>
T workOnVal(T param)
{
	return param;
}

template<typename T>
void fwd3(T&& param)
{
	f3(std::forward<T>(param));
}

void foo3()
{
	f3(processVal); //OK �����Ϸ��� f�� ������ ���� ������ processVal�� ã�Ƽ� ȣ���Ѵ�.

	//fwd3(processVal); //����! fwd3���� ȣ�⿡ �ʿ��� ���Ŀ� ���� ������ ���� ����.
	//fwd3(workOnVal);  //����! workOnVal�� � �ν��Ͻ�����?

	//��ȸ ���
	using ProcessFuncType = int(*) (int);
	ProcessFuncType processValPtr = processVal; //�ʿ��� ������ ����Ѵ�.

	fwd3(processValPtr);
	fwd3(static_cast<ProcessFuncType>(workOnVal));
}

//��Ʈ�ʵ�
struct IPv4Header
{
	std::uint32_t version : 4, IHL : 4, DSCP : 6, ECN : 2, totalLength : 16;
};

IPv4Header h;

void f4(std::size_t sz)
{
}

template<typename T>
void fwd4(T&& param)
{
	f4(std::forward<T>(param));
}

void main()
{
	f4(h.totalLength); //OK

	//������ fwd4�� �Ű������� ���� �̰�, h.totalLength�� �� const��Ʈ�ʵ��� ���̴�.
	//C++ ǥ���� ���� ���տ� ���� "�� const ������ ����� ��Ʈ�ʵ忡 ������ �ʾƾ� �Ѵ�"��� ���.
	//������ ��Ʈ���� ����Ű�� �����͸� �����ϴ� ����� ������, ���� ������ ������ ��Ʈ�鿡 ���� ����� ����.
	//fwd4(h.totalLength); //����!

	//��ȸ ���
	auto lengh = static_cast<std::uint16_t>(h.totalLength);
	fwd4(lengh); //���纻�� �����Ѵ�.

	//const�� ���� ������ �����ϴ� ����� ����.
	//const���� �Ű������� ���, ǥ�ؿ� ������ �� ������ ������ � ���� ����(�̸��׸� int)�� ��ü��
	//����� ��Ʈ�ʵ� ���� ���纻�� ������ �Ѵ�.
	//�� const������ ��Ʈ�ʵ� ��ü�� ���̴� ���� �ƴ϶�, ��Ʈ�ʵ尪�� ����� ���� ��ü�� ���δ�.
}