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

//���� ������ �� �����尡 �ϳ� �Ǵ� �� �̻��� ��ٸ��� �����带 ���� �� �ֵ��� �ϴ� �����̴�.
//notify_one();
//-�ش� ���� ������ ��ٸ��� �ִ� ������ �� �� ���� �����带 �ƿ��.
//notify_all();
//-�ش� ���� ������ ��ٸ��� �ִ� ��� �����带 �����.

std::condition_variable cv; //����� ���� ���� ����
std::mutex m;				//cv�� �Բ� ����� ���ؽ�

void thread1()
{
	//�Ӱ� ������ ����.
	{
		std::unique_lock<std::mutex> lk(m); //���ؽ��� ��ٴ�.

		cv.wait(lk); //������ ��ٸ���.

	}//�Ӱ� ������ �ݴ´�. lk�� �Ҹ��ڰ� m�� ���� �Ѵ�.

	//���ڵ忡 ������
	//1.���� ���� ���ٹ�Ŀ� ���ؽ��� �ʿ��ϴٴ°��� ���迡 ���� ������ ������ �Ͻ��Ѵ�.
	//2.���� ���� ������ wait�� �����ϱ� ���� ���� ������ ���� ������ �����ϸ� ���� ������ ���߰� �ȴ�.
	//  ���������� wait�� �����ϱ� ���� ���� ������ ������ �����ϸ� ���������� �� ������ ������ ������ ������ ��ٸ��� �ȴ�.
	//  (wait�� ���� �Ŵ� �Ű� notify_one�� ���� Ǫ�°Ŷ�� �����ϸ�ȴ�. ���� Ǯ�� ���� �ɾ���� �ǹ̰� �����Ƿ�)
	//3.wait ȣ�⹮�� ��¥ ����� ������� �ʴ´�.
	//  ���� ������ ��ٸ��� �ڵ尡 ���� ������ �������� �ʾҴµ��� ��� �� �ִٴ� ���� ���� �Ͼ�� ���̴�. �̰��� ��¥ ����̶�� �Ѵ�.
	//  ��¥ ����� ����� ó���Ϸ��� ��ٸ��� ������ ���� �߻��ߴ��� üũ�ؾ��Ѵ�.
}

void foo()
{
	auto f1 = std::async(thread1);

	//����� �����Ѵ�
	cv.notify_one(); //���� �������� �˸���.
}


//��¥ ��� üũ��� 1
std::atomic<bool> flag(false);

void thread2()
{
	//�̹���� �÷��׸� üũ�ϸ鼭 �������� ��� �Դ´�.
	while (!flag)
	{

	}
}

void foo2()
{
	auto f1 = std::async(thread2);

	//����� �����Ѵ�
	flag = true;
	cv.notify_one(); //���� �������� �˸���.
}

//��¥ ��� üũ��� 2
bool flag_(false);

void thread3()
{
	{
		std::unique_lock<std::mutex> lk(m); //���ؽ��� ��ٴ�.

		cv.wait(lk, [] {return flag_;  }); //��¥ ��� �����ϱ����� ���ٸ� ����Ѵ�.
	}
}

void foo3()
{
	auto f1 = std::async(thread3);

	//����� �����Ѵ�
	{
		std::lock_guard<std::mutex> g(m);
		flag_ = true;
	}
	
	cv.notify_one(); //���� �������� �˸���.
}

//��¥ ��� üũ��� 3
//�� ���迡�� ���ý��� �ʿ����� ������, ���� ������ wait�� ����ϱ� ���� ���� ������ �ڽ��� std::promise�� �����ص� �۵��Ѵ�.
//��¥ ��� ����. wait ȣ�� �� �������� ���� �ǹǷ� ��ٸ��� ���� �ý��� �ڿ��� ���� �Ҹ����� �ʴ´�.
//�� ������ ������ �ܹ߼��̰�, �������¸� ����� ������ �޸� �����Ҵ��� �䱸�Ѵ�.

std::promise<void> p; //������ �ڷᰡ ���� �ܼ��� ���� �̱� ������ void�� ���

void thread4()
{
	p.get_future().wait(); //p�� �ش��ϴ� �̷� ��ü�� ��ٸ���.
}

void foo4()
{
	auto f1 = std::async(thread4);

	//����� �����Ѵ�
	p.set_value(); //������������ �����Ѵ�.
}

//�ܹ߼��̶� �����Ѵ�.
std::promise<void> p_;

void rect() {};  //���� ������ �ش��ϴ� �Լ�.

class ThreadRAII
{
public:
	enum class DtorAction { join, detach };

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

void detect() //���� ������ �ش��ϴ� �Լ�.
{
	ThreadRAII tr(std::thread ([]
	{
		p_.get_future().wait(); //�̷� ��ü�� ������ ������ t�� ����
		rect();
	}), ThreadRAII::DtorAction::join);

	//ThreadRAII�� ����� ������ �շ� �Ұ������� ����� ���ؼ�.

	//���⼭ t�� ������ ������.
	//(���࿩�⼭ ���ܰ� �߻��ϸ� set_valueȣ���� �Ͼ�� �����Ƿ� ���� ������� ��� ���� ���°� �ȴ�!)
	//���� tr�� �Ҹ��ڰ� ���� �Ϸ���� �����Ƿ� ���Լ��� ���߰Եȴ�.

	p_.set_value(); //t�� ������ Ǭ��.

	//�߰� �۾��� ����

}

//������ �������� ���� ������ �����Ѵ�.
int threadToRun = 10;

void detact2()
{
	auto sf = p.get_future().share(); //sf�� ������ std::shared_future<void>

	std::vector<std::thread> vt;

	for (int i = 0; i < threadToRun; ++i)
	{
		vt.emplace_back([sf] { sf.wait(); rect(); });
	}

	//���� ���⼭ ���ܰ� �߻��ϸ� �շ������� ��������� �ı��Ǿ ���α׷��� ����ȴ�.

	p_.set_value();

	//��� ������ ���� �շ� �Ұ������� �����.
	for (auto& t : vt)
	{
		t.join();
	}

}

void main()
{
}
