#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <chrono>
#include <array>
#include <functional>
using namespace std;

//�ʱ�ȭ �������δ� ������ ���� �͵��� ������ �� �ִ�.
//1.���ٷκ��� �����Ǵ� Ŭ���� Ŭ������ ���� �ڷ� ����� �̸�
//2.�� �ڷ� ����� �ʱ�ȭ �ϴ� ǥ����

class Widget
{
public:
	bool isValidated() const;
	bool isProcessed() const;
	bool isArchived() const;
private:
};

void foo()
{
	auto pw = std::make_unique<Widget>();

	auto func1 = [&pw] { return pw->isValidated() && pw->isArchived(); };
	//=�� �º��� Ŭ���� Ŭ���� ���� �ڷ� ����� �̸�, �캯�� �װ��� �ʱ�ȭ�ϴ� ǥ�����̴�.
	//�º��� ������ �ش� Ŭ���� Ŭ������ �����̰�,
	//�캯�� ������ ���ٰ� ���ǵǴ� ������ ������ �����ϴ�.
	auto func2 = [pcw = std::move(pw)] { return pcw->isValidated() && pcw->isArchived(); };
	
	//���� �ʱ�ȭ�� ����.
	auto func3 = [pcw = std::make_unique<Widget>()]{ return pcw->isValidated() && pcw->isArchived(); };
}

//C++11����
//���� ǥ������ �׳� �����Ϸ��� �ϳ��� Ŭ������ �ڵ����� �ۼ��ؼ� �� Ŭ������ ��ü�� �����ϰ� ����� ������ ���̹Ƿ�
class IsValAndArch
{
public:
	using DataType = std::unique_ptr<Widget>;

	explicit IsValAndArch(DataType&& ptr)
		: pw(std::move(ptr)) 
	{
	}

	bool operator()() const
	{
		return pw->isValidated() && pw->isArchived();
	}
private:
	DataType pw;
};

auto func = IsValAndArch(std::make_unique<Widget>()); //make_unique�� 14���� �����Ƿ� 21�׸񿡼� ������ �����.


void foo2()
{
	//C++14
	std::vector<double> data{ 1.2f, 1.3f };

	auto func = [data = std::move(data)]{};

	//C++11

	//���� ǥ���� ó�� std::bind�� �Լ� ��ü�� �����Ѵ�. ���ε� ��ü
	//std::bind�� ù �μ��� ȣ�Ⱑ���� ��ü�̰�, ������ �μ����� �� ��ü�� ������ ������ ��Ÿ����.
	//���ε� ��ü�� std::bind�� ���޵� ��� �μ��� ���纻���� �����Ѵ�.
	//�� �ް� �μ��� ����, ���ε� ��ü���� �׿� �ش��ϴ� ���� ������ ��ü�� �ִ�.
	//�� �������� ���ؼ��� �̵� ������ ��ü�� �ִ�.
	//�� ������ �μ��� �������̴�, ���� data�� ���ε� ��ü������ �̵��ȴ�.
	//�� �������� ���ε� ��ü������ �̵������ν� �������� �̵��� �Ұ����ϴٴ� C++11 Ŭ������ �Ѱ踦 ��ȸ�Ѵ�.

	//�⺻������, ���ٷκ��� ������� Ŭ���� Ŭ������ operator()��� �Լ��� const�̴�. �� ������ ���� ���� �ȿ���
	//Ŭ������ ��� �ڷ� ����� const�� �ȴ�.
	//���� ���⼭�� const�� �־ data�� ����.
	auto func2 = std::bind(
		[](const std::vector<double>& data) {}
		, std::move(data)
	);

	
	//���� ������ ���ٸ� ����Ѵٸ� mutable�� ���
	//���ͺ� ������ �ʱ�ȭ �������� �䳻 ���� ���.
	auto func3 = std::bind(
		[](std::vector<double>& data) mutable {}
		, std::move(data)
	);

	//���ε� ��ü�� ������ Ŭ������ ����� �����Ƿ�, ���ε� ��ü ���� ��ü���� ��ġ Ŭ���� �ȿ� �ִ� ��ó�� ����� �����ϴ�.
	//(���ε�� ��� ��ü�� �����ϹǷ� ù��° ���� �Լ��� �������� ���߿� ���ÿ� �����ϹǷ� ������ ����)

	auto func4 = [pcw = std::make_unique<Widget>()]
	{ 
		return pcw->isValidated() && pcw->isArchived(); 
	};

	auto func5 = std::bind(
		[](const std::unique_ptr<Widget>& pw)
		{
			return pw->isValidated() && pw->isArchived();
		}
		, std::make_unique<Widget>()
	);
}

void main()
{
}