#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <chrono>
#include <array>
#include <functional>
using namespace std;

vector<int> list{20,5};

void WhatIsTheLamDa()
{
	//���� ǥ����.
	//Ŭ������ ���ٿ� ���� ������� ���� ���� ��ü�̴�. ���������(ĸó ���)�� ����, Ŭ������ �������� �ڷ��� ���纻��
	//���� ���� �ְ� �� �ڷῡ ���� ������ ���� �� �� �ִ�. 3��° �μ��� ���޵Ǵ� ��ü�̴�.

	//Ŭ���� Ŭ������ Ŭ������ ����µ� ���� Ŭ������ ���Ѵ�
	//������ ���ٿ� ���� �����Ϸ��� ������ Ŭ���� Ŭ������ ����� ����.
	//���� ���� ������� �ش� Ŭ���� Ŭ������ ��� �Լ��� ���� ���� ������ ��ɵ��� �ȴ�.
	vector<int>::iterator iter = std::find_if(list.begin(), list.end(), [](int val) { return 0 < val && val < 10; });

	if (iter != list.end())
	{
		cout << "ã��";
	}
	else
	{
		cout << "�� ã��";
	}

	int x;

	auto c1 = [x](int y) { return x * y > 55; }; //c1�� ���ٿ� ���� ������� Ŭ������ ���纻.

	auto c2 = c1; //c2�� c1�� ���纻.
	auto c3 = c2; //c3�� c2�� ���纻.
}

/////////////////////////////////////////////////////////////////////////////////////////////
using FilterContainer = std::vector<std::function<bool(int)>>;

FilterContainer filters;


void CaptureMode()
{
	//�⺻ ������ ���� �ΰ�����.
	//������ ���� ���������, ���� ���� ������ ����̴�.

	//���� �������� ����ϴ� Ŭ����
	//�������� �Ǵ� ���ٰ� ���ǵ� �������� �� �� �ִ� �Ű������� ���� ������ ������ �ȴ�.
	//���ٿ� ���� ������ Ŭ������ ������ �� ���� ������ �Ű������� ������ ���, Ŭ���� ���� ������ ����� �Ҵ´�.
	filters.emplace_back([](int value) {return value % 5 == 0;} ); //5�� ��� ���� �Լ�.
}

int computeSomeValue()
{
	return 1;
}

int computeDivisor(int cal1, int cal2)
{
	return cal1 + cal2;
}

void addDivisorFilter()
{
	auto calc1 = computeSomeValue();
	auto calc2 = computeSomeValue();

	auto divisor = computeDivisor(calc1, calc2);

	//������ ���� �������� ����ϰ� �Ǹ�.
	//addDivisorFilter�� ��ȭ�Ǹ� divisor�� �������� �ʰԵǹǷ� ������ �Ұ� �ȴ�.
	filters.emplace_back([&](int value) {return value % divisor == 0; }); 

	//�̷��� ����� �������� �� ���� ǥ������ divisor�� ���� �����Ѵٴ� ���� ��Ȯ�� ��Ÿ���� ������ �ֱ��ϴ�.
	//�̴� [&]�� �Ͻ��ϴ� �� � ������ ����� ���� �ʰ� �ؾ��Ѵ� ��� �Ϲ����� ��� ���� ���� ��ü�� �̴�.
	filters.emplace_back([&divisor](int value) {return value % divisor == 0; });
}

//Ŭ������ ��� ���Ǹ�, Ŭ������ ������� ���� ���� �˰� �ִٸ�
//Ŭ���� ���� ������ �ش� ���ٰ� ������ ȯ�� ���� ���� ������ �Ű��������� ���� ��Ƴ��� ������ ����.
//�׷��ٸ� ������ ����� �Ҵ� �ϵ� ������ ���� ���̹Ƿ� �⺻ ���� ������ ��带 ���� �ʿ䵵 ���� �����?
template<typename C>
void workWithContainer(const C& container)
{
	auto calc1 = computeSomeValue();
	auto calc2 = computeSomeValue();

	auto divisor = computeDivisor(calc1, calc2);

	using ContElemT = typename C::value_type; //�����̿� ��� ������ ����.

	//all_of �� ������ ��� ��Ұ� �־��� ������ �����ϴ����� ���θ� �����ִ� �˰���
	if (std::all_of(std::begin(container), std::end(container),
		[&](const auto& value) //c++14 ������ �Ű������� auto��� ����.
		//[&](const ContElemT& value)
		{ return value % divisor == 0; }))
	{
		//�����̳��� ��� ���� divisor�� ������ ����
	}
	else
	{
		//��� �ϳ��� �ƴѰ��.
	}

	//�� ���ٰ� �ٸ� ���ƿ����� �����ҰŶ�� ������ ���α׷��Ӱ� �̸� �Լ� �����̳ʿ� �����ؼ� �ٸ������� ����� ���ɼ��� �ִ�.
	//������ ������ ���� divisor�� ������ �м��� ���� �Ѵٴ� ���� �˷��ִ� ��Ʈ�� ���� �����Ƿ�. ���α׷��ӷμ��� �׷� ������
	//�ν��ϱⰡ ���� �ʴ�.
	//��������� ����, ���ٰ� �����ϴ� ���� ����, �Ű������� ��������� �����ϴ°��� ����.

	//�ؼ� ����� �⺻�� ������ ��带 ����ϴ� ���̴�.
	//�׷��ٰ� �̰� �Ϻ������� �ʴ� �������� ��� ������ �ܺο��� delete�� ȣ��Ǽ� ����� ���ɼ��� �ִ�.
	//[=](const auto& value) { return value % divisor == 0; } 
}
///////////////////////////////////////////////////////////////////////////////////////////////
class Widget
{
public:
	void addFilter() const;
private:
	int divisor;
};

void Widget::addFilter() const
{
	//������ �� ���� ���ٰ� ������ ���� �ȿ��� ���̴�, static�� �ƴ� ��������, �Ű��������� ����ȴ�.
	//���⼭ divoisor�� Ŭ������ �� �ڷ� ����̹Ƿ� ������ �� �� ����.
	//�⺻�� �������� ����ϴ� �� ���������� �����Ϸ��� �� �ڵ带 ��ġ �̷��� ����Ѵ�.
	filters.emplace_back([=](int value) { return value % divisor == 0; });

	auto currentObjectPtr = this;
	filters.emplace_back([currentObjectPtr](int value) { return value % currentObjectPtr->divisor == 0; });

	//C++14���� �ڷ� ����� �������ϴ� �� ���� ����� �ִ�.
	//�Ϲ�ȭ�� ���� �������� ����ϴ°��̴�.
	filters.emplace_back([divisor = divisor](int value) { return value % divisor == 0; });
	//filters.emplace_back([divisor](int value) { return value % divisor == 0; }); //����! �������� ���� divisor�� ����(������ ��������)
	//filters.emplace_back([](int value) { return value % divisor == 0; }); //������ �ȵ�~
}

//��ó�� �����Ϳ� �����ϰ� ������ �Ʒ� ����ó�� ����Ʈ �����͸� �Ἥ �ı��ɶ� ������ ���� ���ٴ� ����� �Ұ� �ȴ�.
void doSomeWork()
{
	auto pw = std::make_unique<Widget>();

	pw->addFilter();

	//���⼭ Widget�� �ı��ȴ�. filters���� ����� ���� �����Ͱ� �����Ѵ�.
}
///////////////////////////////////////////////////////////////////////////////////////////////
//���� ���� �⺻ ������ ����� �� �ٸ� ������, �ش� Ŭ������ �ڱ� �ϰ����̰� Ŭ���� �ٱ����� �Ͼ��
//�ڷ��� ��ȭ�κ��� �ݸ��Ǿ� �ִٴ� ���ظ� �θ� �� �ִٴ� ���̴�.
//�ֳ��ϸ� ���� ����� ����Ⱓ�� ���� ��ü���� ������ �� �ֱ� �����̴�.
//���������� �̸����� �������� ���ǵ� ��ü�� Ŭ����, �Լ�, ���� �ȿ��� static���� ����� ��ü�� �׷� ��ü�� �ش��Ѵ�.
//�׷� ��ü�� ���پȿ��� ����� �� ������, ������ �� ���� ����.
void addDivisorFilter2()
{
	//�⺻ �� ������ ����� ǥ��� ��ġ ���� ���� ����  ��ü�� ��� ������ �ȴٴ� ������ �ش�.
	static auto calc1 = computeSomeValue();
	static auto calc2 = computeSomeValue();

	static auto divisor = computeDivisor(calc1, calc2);

	//�� ���ٴ� divisor�� ������ ������ �ѰͰ� ������,
	//�̴� �⺻�� ������ ��尡 ���ϴ� �ٿ� ���������� ����ȴ�.
	filters.emplace_back(
		[=](int value) 
		{return value % divisor == 0; }); //�ƹ��͵� ���������� ����, ���� ���� ������ ��Ī�Ѵ�.

	++divisor; //divisor�� �����Ѵ�.
}

void main()
{
}