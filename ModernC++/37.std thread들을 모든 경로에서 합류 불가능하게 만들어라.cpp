#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <chrono>
#include <array>
#include <functional>
#include <thread>
#include <future>
using namespace std;

//�շ� ���� std::thread�� ���� ���� �������� �̰ų�
//���� �� ���·� ������ �� �ִ� �����忡 �����ȴ�.
//���ܵ� ����, ��ٸ��� ����, ������ �Ϸ�� �����嵵 �շ��������� �����Ѵ�.

//�շ� �Ұ��� std::thread�� ���״�� �շ��� �� ���� ������
//�Լ����� �⺻������ ������.
//�̵������ ������
//join�� ���� �շ��� std::thread
//detach �� ���� Ż���� std::thread : ��ü�� �� �����Ǵ� ���������� ������ ������ ���´�.

//�շ� ���ɼ��� �߿��� ������
//���� �շ� ������ �������� �Ҹ��ڰ� ȣ��Ǹ� ���α׷� ������ ����ȴ�.

//�⺻������ ������� ���踦 ����ϴ� ���� ������
//����Ƽ�� �ڵ��� �ʿ��ϱ� ������ std::thread�� ���

constexpr auto telMillion = 10'000'000; //C++14 ���� ������ �ְ� ���ش�.

//����Ʈ ������ó�� �Ҹ��ڸ� ȣ���ؼ� �շ��Ұ������� �����.
class ThreadRAII
{
public:
	enum class DtorAction {join, detach};

	ThreadRAII(std::thread&& t, DtorAction a)
		: action(a), t(std::move(t)) //�ʱ�ȭ ����� ����� ������ ������.
									 //�Ϲ������� �� ����� �ʱ�ȭ�� �ٸ� ����� ������ �� �ִ�.
									 //std::thread�� �ʱ�ȭ���� ���� �ش� �Լ��� ���� �� �� �����Ƿ�,
									 //Ŭ�������� �׻� ���� �������� �����ϴ� ���� ����.
	{
	}

	ThreadRAII(ThreadRAII&&) = default;
	ThreadRAII& operator=(ThreadRAII&&) = default;

	~ThreadRAII()
	{
		//�շ� �Ұ����� �����忡 join�̳� detach�� ȣ���ϸ� ������ �ൿ�� �����Ƿ� �ʿ��ϴ�.
		if (t.joinable())
		{
			if (action == DtorAction::join)
			{
				t.join();
			}
			else
			{
				t.detach();
			}
		}
	}

	std::thread& get() { return t; }

private:
	DtorAction action;
	std::thread t;
};
bool conditionsAreSatisfied()
{
	return true;
}

void performComputation(std::vector<int>& val)
{

}

bool doWork(std::function<bool(int)> filter, int maxVal = telMillion)
{
	std::vector<int> goodVals;

	std::thread t([&filter, maxVal, &goodVals]
	{
		for (auto i = 0; i <= maxVal; ++i)
		{
			if (filter(i))
				goodVals.push_back(i);
		}
	});

	auto nh = t.native_handle();

	//���⼭ t�� ����Ƽ�� �ڵ��� �̿��ؼ� t�� �켱 ������ �����Ѵ�.

	//���ǵ��� �����Ǿ��ٸ� t�� �ϷḦ ��ٸ���.
	if (conditionsAreSatisfied())
	{
		t.join();
		performComputation(goodVals); //����� ����.
		return true; //��� ���� ok
	}

	return false; //��� ���� no
	//���� t�� join�� �ȵǰų�, detach�� �ȵǰų�, ���ܰ� �߻��ؼ� ��������� ���α׷��� ����ȴ�.
}

//����
bool doWork2(std::function<bool(int)> filter, int maxVal = telMillion)
{
	std::vector<int> goodVals;

	ThreadRAII t(std::thread([&filter, maxVal, &goodVals]
	{
		for (auto i = 0; i <= maxVal; ++i)
		{
			if (filter(i))
				goodVals.push_back(i);
		}
	}
	), ThreadRAII::DtorAction::join
	);

	auto nh = t.get().native_handle();

	//���⼭ t�� ����Ƽ�� �ڵ��� �̿��ؼ� t�� �켱 ������ �����Ѵ�.

	//���ǵ��� �����Ǿ��ٸ� t�� �ϷḦ ��ٸ���.
	if (conditionsAreSatisfied())
	{
		t.get().join();
		performComputation(goodVals); //����� ����.
		return true; //��� ���� ok
	}

	return false; //��� ���� no
				  //���� t�� join�� �ȵǰų�, detach�� �ȵǰų�, ���ܰ� �߻��ؼ� ��������� ���α׷��� ����ȴ�.
}

void main()
{
}