#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
using namespace std;

struct SomeDataStructure
{
};
class Widget
{
public:
	Widget() {};
	//rhs�� ������ �����̱⶧���� move�� �翬�ϴ�.
	Widget(Widget&& rhs)
		: name(std::move(rhs.name)), 
		p(std::move(rhs.p))
	{
	}

	//newName�� ���������̱� ������ forward�� �翬�ϴ�.
	template<typename T>
	void SetName(T&& newName)
	{
		name = std::forward<T>(newName);
	}

	//���������� move?! ���ڴ�.
	template<typename T>
	void SetName2(T&& newName)
	{
		name = std::move(newName);
	}
	
	//�ߺ� ���� ����. ���� �ʴ�.
	//�Ű������� �þ�ٸ� �ߺ� ���� ������ 2�� n���� �ɰ��̴�.
	void SetName3(const std::string& newName)
	{
		name = newName;
	}
	void SetName3(std::string&& newName) 
	{
		name = std::move(newName);
	}
private:
	std::string name;
	std::shared_ptr<SomeDataStructure> p;
};


int main()
{
	Widget w;

	string s = "what";

	w.SetName2(s); //���� s���� �˼� ����.

	//�ӽ� string�����Ǿ� setName�� �Ű������� ���̰�
	//���ӽ� string�� w�� �ڷ� ����� �̵��Ѵ�.
	//������, �̵�����������, �Ҹ���
	w.SetName3("sdfd"); //const char* Ÿ���̴�. ���ͷ���. �̰� stringŸ������ ����ȯ�� �Ͼ�鼭 �ӽ� ��ü�� ����°�.
	w.SetName("sdfd"); //�������� �������� T�� const char*�� �����ϱ� ������ �ӽ� ��ü�� �������� �ʴ´�.

	return 0;
}

//�Լ��� ����� ������ �����شٸ�
//�׸��� �װ��� ������ ������ ���������� ���� ��ü���
//�ش� ������ �����ִ� return���� move�� forward�� ����ϴ°��� �ٶ����ϴ�.

class Matrix
{
public:
	Matrix operator +=(const Matrix& rhs)
	{
		return rhs;
	}
};

Matrix operator+(Matrix&& lsh, const Matrix& rhs)
{
	lsh += rhs;
	return std::move(lsh); //��ȯ������ �̵��Ѵ�.
	//return lsh; ��ȯ ������ �����Ѵ�.
	//Matrix�� �̵������� �����Ѵٰ� �ص� move�� �ذ� ���� �ʴ´�. �ڵ����� ���� ������ ȣ���.
	//���� ���߿� Matrix�� �̵������� �߰��Ǹ� �̵��� �ȴ�.
};

class Fraction
{
};

template<typename T>
Fraction reduceAndCopy(T&& frac) //�� ���� ����� ��ȯ��.
{
	frac.reduce();
	return std::forward<T>(frac); //���������� ���� forward �������� ��ȯ������ �̵��ϰ�, �ް��� ��ȯ������ �����Ѵ�.
}

//��ȯ������ ����� ������ ���� �Ű������� std::move�� �����ϸ�
//���� ������ �̵� �������� ���Ѵٸ� Ȯ�� �ؼ��ϴ� ���
//�Լ��� ��ȯ�� ���� �������� �̷��� ����ȭ�� ������ �� �������̶�� ������ ����.
class Widget2
{
};


//��ȯ�� ����ȭ�� ����
//1.���� ��ü�� ������ �Լ��� ��ȯ ���İ� ��ġ
//2.�� ���� ��ü�� �ٷ� �Լ��� ��ȯ���� �Ǿ�� �Ѵ�.

//�������
Widget makeWidget()
{
	Widget w;
	//... w�� �����Ѵ�.
	return w; 
	//w�� �Լ��� ��ȯ���� ���� ������ �޸𸮾ȿ� �����Ͽ� ���縦 ���Ѵ�.(��ȯ�� ����ȭ)
	//���� ��ȯ ��ΰ� �����ؼ� ����ȭ�� �Ұ����Ѱ��
	//return std::move(w);�� �����Ϸ��� ġȯ�Ѵ�. �ᱹ ����ڰ� return std::move(w)�� ����ϴ°� ���� �ʴ�.
}

//�̵�����
Widget makeWidget2()
{
	Widget w;
	//... w�� �����Ѵ�.
	//���縦 �̵����� �ٲ����μ� ����ȭ�� �����ϴٰ� �����Ѵ�..

	//����ȭ�� �Ͼ�� �ʴ´�.
	return std::move(w); //�� �Լ��� �����ִ°���, ������ü w�� �ƴ϶�, move�� ��� w�� ���� �����̴�. (���� ��ü�� �ƴ϶� ��)
	                     //���� �����Ϸ� ����ȭ�� ���ʶ߸� ���̵ȴ�.
}

//�� ���� ����� �Լ� �Ű������� ���
Widget makeWidget(Widget w)
{
	return w;
}

Widget makeWidget2(Widget w)
{
	return std::move(w); //�����Ϸ��� �̷��� ����Ѵ�.
}

//�����ϸ� �Լ��� ������ü�� ������ ��ȯ�ϴ� ���
//�� ���� ��ü�� move�� �����Ѵٰ� �ؼ� �����Ϸ����� ������ ���� �ʴ´ٴ°��̴�.

//�մ��� ���� ���̻� ������� ���� ������ �ٸ� �Լ��� �Ѱ��ִ� ���