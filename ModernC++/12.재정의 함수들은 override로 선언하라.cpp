#include <iostream>
#include <memory>
#include <vector>
#include <list>
using namespace std;

//������ ����
//1.��� Ŭ���� �Լ��� �ݵ�� ���� �Լ����� �Ѵ�.
//2.�̸��� �����ؾ��Ѵ�(�����Լ��� ����)
//3.�Ű����� ���ĵ��� �ݵ�� �����ؾ��Ѵ�.
//4.const���� �ݵ�� �����ؾ� �Ѵ�.
//5.��ȯ ���İ� ���� ���� �ݵ�� ȣȯ�Ǿ�� �Ѵ�.
//6.��� �Լ����� ���� ��������� �ݵ�� ���� �ؾ� �Ѵ�.

//�����Լ��� �ƴ� ��� �Լ����� ���� �����縦 ������ �� �ִ�.
class Widget
{
public:
	void doWork() & { cout << "�ް�"; }; //*this�� �ް��� ������ ����ȴ�.
	void doWork() && { cout << "������"; };  //*this�� �������� ������ ����ȴ�.
};

Widget&& makeWidget()
{
	return Widget();
}

//������ �Ǵ°��
class Base
{
public:
	virtual void mf1() const;
	virtual void mf2(int x);
	virtual void mf3() &;
	void mf4() const;
};

class Dervied : public Base
{
public:
	virtual void mf1();					//const ����.
	virtual void mf2(unsigned int x);   //unsigned��
	virtual void mf3() &&;				//&&������
	void mf4() const;					//����Լ��� virtual�� ����.
};

//���� C++11�� �����Ǹ� ��������� ǥ���ϴ� ����� �����Ѵ�.
//��� �Ͼ���� �ʼ�
class Dervied2 : public Base
{
public:
//	virtual void mf1() override ;				//const ����.
//	virtual void mf2(unsigned int x) override;  //unsigned��
//	virtual void mf3() && override;				//&&������
//	virtual void mf4() const override;			//����Լ��� virtual�� ����.
};

//���� �Լ��� final�� �����ϸ� �Ļ� Ŭ�������� �� �Լ��� �������� �� ���� �ȴ�.
//Ŭ���� ��ü�� �ɸ� �� Ŭ������ ��� �Լ��� ���� �� ���� �ȴ�.

class Warning
{
public:
	void override(); //���ٶ��̵� Ű����� �Լ� ������ ���� ���� ������ ����� �ǹ̸� ������.
					 //override�� final�� Ű���尡 �ƴ϶� �ĺ����̴�.
};

//���� �����縦 �ٿ��� �ϴ� ��Ȳ
class A
{
public:
	using DataType = std::vector<double>;

	DataType& data() { return values; };

private:
	DataType values;
};

A&& makeA()
{
	return A();
}

class B
{
public:
	using DataType = std::vector<double>;

	DataType& data() & { return values; };
	DataType&& data() && { return std::move(values); };

private:
	DataType values;
};

B&& makeB()
{
	return B();
}

int main()
{
	makeWidget().doWork();

	Widget w;
	w.doWork();

	//

	A a;

	auto vals1 = a.data(); //������ 100���ִٸ� 100�� ���簡 �Ͼ��. ���⼭ auto�� �����̹Ƿ�&���õ�, & �����ڰ� �Ҹ���.

	auto vals2 = makeA().data(); //������ 100���ִٸ� 100�� ���簡 �Ͼ��.

	B b;

	auto vals3 = b.data(); //������ 100���ִٸ� 100�� ���簡 �Ͼ��. ���⼭ auto�� �����̹Ƿ�&���õ� , & �����ڰ� �Ҹ���.

	auto vals4 = makeB().data(); //�̵� ������. &&�����ڰ� �Ҹ���.
	
	//move�� &&�� ��ȯ���ϰ� &&�����ڰ� ȣ��Ǵ°��� �ٽ��̴�.
	//&&�����ڿ����� �����͸� �Ѱ��ְ� �ش� ������ nullptr�� ����� ������ ������ �ӽð�ü �Ҹ��ڿ� �����Ͱ˻��� ������ �ؾ��Ѵ�.

	return 0;
}