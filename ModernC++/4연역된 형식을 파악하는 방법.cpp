#include <iostream>
#include <vector>
using namespace std;

template<typename T>
class TD;


class Widget {};

template<typename T>
void f(const T& param) 
{
	//std::type_info::name�� �ݵ�� �־��� ������ ���ø� �Լ��� �� ���� ó�� �ؼ��Ѵ�.
	//���� ������ �� const�� ���ŵȰ�.
	cout << "T =" << typeid(T).name() << std::endl; //class Widget const * �߸��� ������ ǥ��
	cout << "param =" << typeid(param).name() << std::endl; //class Widget const * �߸��� ������ ǥ��
};

std::vector<Widget> createVec()
{
	vector<Widget> t;
	t.push_back(Widget());
	return t;
}

int main()
{
	//1. IDE �����⸦ �̿��ؼ� ���콺�� �÷����� Ȯ�ΰ���
	const int a = 22;
	auto b = a; //auto�� ���콺�� �ø��� ǥ�ŵȴ�.

	//2. ���Ǻθ� �������� �ʾƼ� ������ �߻��Ͽ� Ȯ��
	const int theAnswer = 42;
	auto x = theAnswer;
	auto y = &theAnswer;

	//TD<decltype(x)> xType; //'xType'��(��) ���ǵ��� ���� class 'TD<int>'��(��) ����մϴ�.
	//TD<decltype(y)> yType; //'yType'��(��) ���ǵ��� ���� class 'TD<const int *>'��(��) ����մϴ�.

	//3. ������� ���
	std::cout << typeid(x).name() << std::endl;
	std::cout << typeid(y).name() << std::endl;

	const auto vw = createVec();

	if (!vw.empty())
	{
		f(&(vw[0])); //vw[0] -> vwm�� const�� ����Ǿ��� ������, const������ �Լ��� ȣ���Ѵ�. const Widget&�� ��ȯ
					 //T -> Widget * const
					 //param-> const Widghet * const&
	}

	//�̰Ŷ� ��������.
	int bb;
	const int& aa = bb;
	const auto& cc = &aa;

	//4.�ν�Ʈ�� ��Ȯ�� ���� ���

	/*
	#include <boost/type_index.hpp>

	template<typename T>
	void f(const T& param)
	{
		using boost::typeindex::type_id_with_cvr;

		cout << "T =" << type_id_with_cvr<T>().pretty_name() << endl;
		cout << "param =" << type_id_with_cvr<decltype(param)>().pretty_name() << endl;
	}
	*/

	//5.�ᱹ ��� �ؼ��� �߿�
	return 0;
}