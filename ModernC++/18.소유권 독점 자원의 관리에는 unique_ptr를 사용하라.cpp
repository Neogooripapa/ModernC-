#include <iostream>
#include <memory>
#include <vector>
using namespace std;

class Investment 
{
public:
	virtual ~Investment() {};
};

class Stock : public Investment
{
public:
	virtual ~Stock() {};
};

class Bond : public Investment
{
public:
	virtual ~Bond() {};
};

class RealEstate : public Investment
{
public:
	virtual ~RealEstate() {};
};


//Ŀ���� ���� �Լ��� ����� �� �ִ�.
auto delInvmt = [](Investment* pInvestment)
{
	std::cout << "���� �α�" << endl;
	delete pInvestment;
};

template<typename... Ts>
//std::unique_ptr <Investment, decltype(delInvmt)> makeInvestment(Ts&&... params)
auto makeInvestment(Ts&&... params)
{
	std::unique_ptr<Investment, decltype(delInvmt)> pInv(nullptr, delInvmt);

	//new�� ���� �������͸� �����ϴ� ������ ������ ���� �����Ƿ� reset�� ���
	//�������Ϳ��� ����Ʈ ������ �Ϲ�����ȯ�� �����Ǹ� ������ ����⶧���� ���Ƴ�����.
	if (1)
	{
		pInv.reset(new Stock(std::forward<Ts>(params)...));
	}
	else if (1)
	{
		pInv.reset(new Bond(std::forward<Ts>(params)...));
	}
	else if (1)
	{
		pInv.reset(new RealEstate(std::forward<Ts>(params)...));
	}

	return pInv;
}

//�Ϲ������� �� �����Ϳ� ����Ʈ������ ũ�Ⱑ ���ٰ� �����Ѵ�.
//�׷��� �Լ������͸� �����ڷ� �����Ѱ�쿡�� std::unique_ptrũ�Ⱑ 1���忡�� 2����� �����Ѵ�.
//���¾��� �Լ� ��ü(ĸó�� ���� ����)�� ��쿡�� ũ�� ��ȭ�� ����. 
//���ٴ� ��ü�̱⶧���� �̸�(Ÿ��)�� �˸� ����Ʈ�����Ϳ��� ���� ȣ���ϹǷ� ����ų �ʿ䰡 ����.
//ĸó�� �ִ°�쿡�� ĸó�� ��ü���� ���� �˾ƾ� ��ü�� ����� �����Ƿ� ũ�Ⱑ Ŀ���� �ȴ�.

//�迭�� std::unique_ptr<T[]>�� �׳� �˰� ����.

std::vector<unique_ptr<Investment, decltype(delInvmt)>> g_vec;

void foo()
{
	unique_ptr <Investment, decltype(delInvmt)> ptr = makeInvestment();

	g_vec.push_back(std::move(ptr)); //���⼭ ����ä�� �����̳ʰ� �����Ǵ� ������ �����Ͱ� �����ȴ�.

	//g_vec.push_back(makeInvestment());
	cout << "�Լ�ȣ�� ��" << endl;
}

//����ũ�� ������ ��ȯ (���� ȿ�����̴�)
std::shared_ptr<Investment> sp = makeInvestment();

int main()
{
	//�ı����� �ʴ°��
	//1.���ܰ� �������� ���Լ� �ٱ����α��� ���ĵǰų�
	//2.noexcept ���� ���ݵǸ�
	//3.std::abort�� � �����Լ� std::_Exit, std::exit, std::quick_exit�� ȣ��Ǹ�
	foo();

	cout << "�ı�" << endl;

	return 0;
}