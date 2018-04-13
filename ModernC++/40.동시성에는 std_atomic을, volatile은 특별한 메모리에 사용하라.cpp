#include <iostream>
#include <memory>
#include <vector>
#include <list>
#include <functional>
#include <unordered_map>
#include <atomic>
#include <thread>
#include <mutex>
using namespace std;

//std::atomic<int>, std::atomic<bool>, std::atomic<Widget*>�� �ٸ� ��������� �ݵ�� ���������� �ν��ϴ� ������� �����Ѵ�.
//atomic��ü�� ���������� �����ǰ���, �� ��ü�� ���� ������ ��ġ ���ؽ��� ��ȣ�Ǵ� �Ӱ� ���� �ȿ��� ����Ǵ� ��ó�� �����Ѵ�.

void foo()
{
	std::atomic<int> ai(0);

	ai = 10; //���������� 10�� �����Ѵ�.

	//���������� ���� �д´�.
	cout << ai; //�������� �κ��� ai�� �дºκи�!

	//������ ������.
	++ai; 
	--ai;

	//�ϴ� ��������� ��ü�� �����ǰ� ����, �� ��ü�� ���� ��� ��� �Լ���, ������ raw������� �����ϴ� ��� �Լ���
	//�ٸ� �����忡�� �ݵ�� ���������� ���δ�.


	//�ݸ� ����ƿ�� ����ϴ� �ڵ�� ���� ������ ���ƿ��� ���� �ƹ��͵� �������� �ʴ´�.
	volatile int vi(0);

	vi = 10; 

	cout << vi; 

	++vi;
	--vi;
}

//�ڵ� ���ġ
//�������� ������ ��� �����Ϸ��� ���ġ�� �� �� �ִ�.
//x = y; a = b;
//a = b; x = y;

int computeImportantValue() {};

void foo2()
{
	//�̹��忡�� ���ġ�� �Ͼ�� ���Ѵ�.
	//������ ������� ����ϸ� ���ġ ������ �����.
	//����� ���� ���� ������ ����� ���Ŀ� ���� ������� �ʴ´�.

	std::atomic<bool> valAvaliable(false);

	auto imptValue = computeImportantValue();

	valAvaliable = true;

	//����ƿ�� ��� �����Ϸ��� ���ġ�� �� �ִ�.
	volatile bool valAvaliable(false);

	auto imptValue = computeImportantValue();

	valAvaliable = true;
}

//����ƿ�� ����
void foo3()
{
	int x = 0;
	auto y = x;
	y = x;
	x = 10;
	x = 20;

	//�����Ϸ��� ������ ���� ����ȭ�� ������ �� �ִ�.
	//auto y = x;
	//x = 20;

	//����ƿ�� �� �޸� ���꿡 ���� �� � ����ȭ�� �������� ����� ���ø� �Ѵ�.
	

	//������� ���� ������ �������� �ʴ´�.
	//a ���� aa�� ���� �Ϸ��°��� �������̷��� �����Ϸ��� �а� ����ϴ� �۾��� �ϳ��� ������ �ڵ�� �����ؾ��Ѵ�.
	//������ �׷� ������ ������ ��ü�� �ϵ����� �������� �ʴ´�.
	std::atomic<int> a;
	//auto aa = a; //����!
	//aa = a;		//����!

	//�̰��� ������ �Ǳ������� �ι��忡 �ϳ��� ������ �������� ����Ǹ���� ����� �� ����.
	std::atomic<int> aa(a.load()); //�д´�.
	aa.store(a.load()); //�ٽ� �д´�.

	a = 10;
	a = 20;

	//�������� = a.load();
	//std::atomic<int> aa(��������);
	//y.store(��������); �� ����ȭ �� ���� �ִ�.

	volatile std::atomic<int> val; //�������̸�, ����ȭ�� ���� ���ŵ� �� ����.
}

void main()
{
}