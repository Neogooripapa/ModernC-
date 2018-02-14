#include <iostream>
#include <memory>
#include <vector>
#include <list>
#include <algorithm>
#include <string>
using namespace std;

int main()
{
	//constexpr�� ����� ��ü�� ������ const�̸�, �� ���� ������ ������ �������� �˷�����.
	//������ �������� �˷����� ���鿡�� Ư���� ������ �ִ�.
	//�׷� ������ �б� ���� �޸𸮿� ��ġ�� �� �ִ�.
	//������ �������� �˷��� ���� ���� C++���� ���� ��� ǥ������ �䱸�Ǵ� ���ƿ� ����� �� �ִ�.

	int sz;				//�� constexpr ����

	constexpr auto arraySize1 = sz; //����! sz�� ���� ������ ���߿� �˷����� ����.

	std::array<int, sz> data1;		//����! ���� ����

	constexpr auto arraySize2 = 10; //OK. ������ ���� ���

	std::array<int, arraySize2> data2; //OK. arraySize2�� constexpr ��ü.

	//��� constexpr�� const�̴�.
	//��� const��ü�� constexpr�� ���� �ƴϴ�.
	//� ���� ���� �ݵ�� ������ ���� ����� �䱸�ϴ� ���ƿ� ����Ϸ��� constexpr�� ����϶�.
	int sz;

	const auto arraySize = sz;

	std::array<int, arraySize> data; //����! arraySize�� ���� ������ �������� �˷����� ����.

	//constexpr�Լ�
	//������ ���� ����� �䱸�ϴ� ���ƿ� constexpr �Լ��� ����� �� �ִ�.
	//�μ��� ���� ������ ������ �˷����� �ʴ´ٸ� ������ �ź�

	//������ �������� �˷����� �ʴ� ���� �ִٸ� constexpr�Լ��� ������ �Լ�ó�� �����Ѵ�.
	//�̴� ���� ������ �����ϴ� �ι��� ���� �ʿ䰡 ������ ���Ѵ�.

	constexpr int pow(int base, int exp) noexcept
	{
	}

	constexpr auto numConds = 5;

	atd::array<int, pow(3, numConds)> results;

	//C++11���� constexpr �Լ��� ���� ���� ������ ���ƾ� �ϳ� �̾���Ѵ� (0, 1)
	//3�� �����ڳ� ����Լ��� ��ü�ؾ��Ѵ� 
	constexpr int pow(int base, int exp) noexcept
	{
		return (exp == 0 ? 1 : base * pow(base, exp - 1));
	}

	//C++14������ ������ �����ϴ�
	constexpr int pow(int base, int exp) noexcept
	{
		auto result = 1;
		for (int i = 0; i < exp; ++i) result *= base;
		return result;
	}

	class Point
	{
	public:
		constexpr Point(double xVal = 0, double yVal = 0) noexcept
			: x(xVal), y(yVal)
		{}

		constexpr double xValue() const noexcept { return x; }
		constexpr double yValue() const noexcept { return y; }

		constexpr void setX(double newX) { x = newX; }
		constexpr void setY(double newY) { y = newY; }
	private:
		double x, y;
	};

	constexpr Point p1(9.4, 27.7); //OK constexpr �����ڰ� ������ ������ ����
	constexpr Point p2(9.4, 27.7); //OK constexpr �����ڰ� ������ ������ ����

	constexpr Point midpoint(const Point& p1, const Point& p2) noexcept
	{
		return{ (p1.xValues() + p2.xValues() / 2, (p1.yValues() + p2.yValues() / 2 } ; //constexpr��� �Լ��� ȣ��
	}

	constexpr auto mid = midpoint(p1, p2); //constexpr�Լ��� ����� �̿��ؼ� constexpr��ü�� �ʱ�ȭ�Ѵ�.

	//C++11������ constexpr��� �Լ��� �Ϲ������� const�� ����ȴ�.
	//����Լ��� void��ȯ�ε� C++11������ void�� ���ͷ� ������ �ƴϴ�.
	return 0;
}