#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include <unordered_map>
#include <atomic>
using namespace std;

class Widget
{
public:
	Widget() {};
	Widget(int i, bool b) {};
	Widget(int i, double d) {};
	Widget(std::initializer_list<long double> il) {};

	operator float() const {};
private:
	int x{ 0 }; //����
	int y = 0;  //����
	//int z(0);   //����

};

class Widget2
{
public:
	Widget2(int i, bool b) {};
	Widget2(int i, double d) {};
	Widget2(std::initializer_list<bool> il) {};
};

class Widget3
{
public:
	Widget3() {};
	Widget3(std::initializer_list<int> il) {};
};

int main()
{
	//1.������ �ʱ�ȭ�� ��𼭳� ����� �� �ִ°��� �߰�ȣ �ʱ�ȭ(���� �ʱ�ȭ) ��.


	Widget w1;		//�Ϲ� ������ ȣ��
	Widget w2 = w1; //���� �����ڸ� ȣ��
	w1 = w2;		//operator =�� ȣ��

	std::vector<int> v{ 1, 3, 5 }; //�ʱ� ��������

	std::atomic<int> ai1{ 0 }; //����
	std::atomic<int> ai2(0);   //����
	std::atomic<int> ai3 = 0;  //����


	//2.������ ��ȯ�� �������ش�

	double x, y, z;
	y = z = x = 1.f;

	int sum1(x + y + z);  //������ ��ȯ�� �������� �ʴ´�.
	int sum2 = x + y + z; //������ ��ȯ�� �������� �ʴ´�.
	//int sum{ x + y + z }; //������ ��ȯ�� �����ϹǷ� ����

	Widget t(); //�Լ��� �����ϰ� �ȴ�.
	Widget w{}; //�߰�ȣ�� �̿��ϸ� �̷��� ������ ���� �ʴ´�.

	//3.�߰�ȣ �ʱ�ȭ�� ����
	Widget h1(10, true);
	Widget h2{ 10, true }; // initializer_list������ ȣ�� 10�� true�� long double�� ��ȯ��
	Widget h3(10, 5.0);
	Widget h4{ 10, 5.0 }; // initializer_list������ ȣ�� 10�� 5.0�� long double�� ��ȯ��

	Widget h5(h1); //���� ������ ȣ��
	Widget h6{ h1 }; //initializer_list ������ ȣ�� h1�� float���� ��ȯ�ǰ� float long double�� ��ȯ��.

	Widget h7(std::move(h1)); //�̵� ������ ȣ��.
	Widget h8{ std::move(h1) }; //initializer_list ������ ȣ�� h1�� float���� ��ȯ�ǰ� float long double�� ��ȯ��.

	//Widget2 wiget{ 10, 5.0 }; //����! ������ ��ȯ�� �ʿ���. bool�� ������ ���� �ڷ����̹Ƿ�

	Widget3 o1; //�⺻ ������ ȣ��
	Widget3 o2{}; //�⺻ ������ ȣ��
	Widget3 o3(); //�Լ� ������!

	Widget3 o4({}); //initializer_list �����ڸ� �� �ʱ�ġ ������� ȣ��
	Widget3 o5{{}}; //initializer_list �����ڸ� �� �ʱ�ġ ������� ȣ��

	std::vector<int> v1(10, 20); //�� std::initializer_list �����ڸ� ���
								 //��� ��Ұ��� 20�� ��� 10��
	std::vector<int> v2{ 10, 20 }; //std::initializer_list �����ڸ� ���
								   //10 20 ��� �߰�

	return 0;
}