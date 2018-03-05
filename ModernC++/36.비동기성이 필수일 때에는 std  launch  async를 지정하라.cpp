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

void f()
{
}

void foo()
{
	//std::async ȣ���� �Լ��� � �õ���ħ�� ���� �����Ѵٴ� �Ϲ����� �ǹ̸� ������.	
	//std::launch::async �õ���ħ�� �����ϸ� �Լ�f�� �ݵ�� �񵿱�������, �ٸ� �����忡�� ����ȴ�.
	//std::launch::deferred �õ���ħ�� �����ϸ� �Լ�f�� std::async�� ������ �̷���ü�� ���� get�̳� wait��
	//ȣ��� ������ ����� �� �ִ�. �ٽ� ���� f�� �׷��� ȣ���� �Ͼ�� ���� �����ȴ�.
	//get�̳� wait�� ȣ��Ǹ� f�� ���������� ����ȴ�. get�̳� wait�� ȣ����� ������ f�� ������� �ʴ´�.

	//��ȣ���� �ǹ̴� ����.
	auto fut1 = std::async(f);
	auto fut2 = std::async(std::launch::async | std::launch::deferred, f);
}

using namespace std::literals;

void ff()
{
	std::this_thread::sleep_for(1s); //1�� ����.
}

void foo2()
{
	//�⺻ �õ� ��ħ�� �����ٸ� �������� ����ð��� �ִ� wait��� �������� ������ ��ģ��.
	//������ ������ ���� waif_for�� wait_until�� ȣ���ϸ� std::future_status_deferred��� ���� ��ȯ�ȱ� ������.
	//�̹��� ������ �������� ������ ó�� ���̴� ������ �����δ� ���� ������ �� �� �ִ�.
	auto fut = std::async(ff);

	//������ �����Ǿ�����.
	if (fut.wait_for(0s) == std::_Future_status::deferred)
	{
		//fut�� wait�� get�� �����ؼ� f�� ���������� ȣ���Ѵ�.
	}
	else
	{
		//f�� ������ ���� ������ ������ ����.
		//f�� ���� �ȴٸ�, fut.wait_for�� �׻� std::future_status_deferred �� �����ش�. �װ��� std::future_status::ready��
		//���� ���� �����Ƿ� ���ѷ����� �ȴ�.
		while (fut.wait_for(100ms) != std::future_status::ready)
		{

		}

		//fut�� �غ�Ǿ���.
	}

	//� ������ ���� �⺻ �õ���ħ�� �Բ� std::async�� ����ϴ� ���� ���� ������ ������ ������ �����ϴ�.
	//1.������ get�̳� wait�� ȣ���ϴ� ������� �ݵ�� ���������� ����Ǿ�� �ϴ°��� �ƴϴ�.
	//2.���� �������� � �������� thread_local�������� �а� �������� �߿����� �ʴ�.
	//3.std::async�� ������ �̷���ü�� ���� get�̳� wait�� �ݵ�� ȣ��ȴٴ� ������ �ְų�, ������ ���� ������� �ʾƵ� ������.
	//4.������ ������ ������ ���� �ִٴ� ���� waif_for�̳� wait_untild�� ����ϴ� �ڵ忡 �ݿ��Ǿ� �ִ�.
}

template<typename F, typename... Ts>
inline auto reallyAsync(F&& f, Ts&&... params)
{
	return std::async(std::launch::async, std::forward<F>(f), std::forward<Ts>(params)...);
}

void foo3()
{
	//�� ���ǿ� �������� ������ �־��� ������ �������� �񵿱������� �����ϵ��� ������ �ʿ䰡 �ִ�.
	auto fut = std::async(std::launch::async, f);

	//std::launch::async ���� ���ø�
	auto fut2 = reallyAsync(f);
}

void main()
{
}