#include <iostream>
#include <vector>
using namespace std;

template<typename Container, typename Index>
auto authAndAcces(Container& c, Index i) -> decltype(c[i]) 
{
	//�Լ��տ� auto�� ���� ������ ���谡 ����.
	//���� ��ȯ �������� auto�� int&�� �ȴ�. c[i]�� int&�� ��ȯ�ϴ� ���۷������̱� ������.
	return c[i]; //decltype�� ��ȯ ������ �Ű��������� �̿��ؼ� ��������.
}

template<typename Container, typename Index>
auto authAndAcces2(Container& c, Index i)
{
	//c[i]�� int&�� ��ȯ������ auto������ ���ؼ� int�� ����.
	return c[i]; //decltype�� ��ȯ ������ �Ű��������� �̿��ؼ� ��������.
}

template<typename Container, typename Index>
decltype(auto) authAndAcces3(Container& c, Index i)
{
	//auto�� �ش� ������ �����Ǿ�� ���� ���ϰ�
	//decltype�� ���� ���� ��Ģ���� ����Ǿ�� ���� ���Ѵ�.
	return c[i];
}

//C++14����
template<typename Container, typename Index>
decltype(auto) authAndAcces4(Container&& c, Index i)
{
	//auto�� �ش� ������ �����Ǿ�� ���� ���ϰ�
	//decltype�� ���� ���� ��Ģ���� ����Ǿ�� ���� ���Ѵ�.
	return std::forward<Container>(c)[i]; //&�� &�� &&�� &&�� �����ϰ� ���ش�.

	//std::forward<Container>(c)�̺κи� ������ �ؼ� & &&�Լ��� �����ؼ� ȣ�����ش�
}

//C++11����
template<typename Container, typename Index>
auto authAndAcces5(Container&& c, Index i) -> decltype(std::forward<Container>(c)[i])
{
	//auto�� �ش� ������ �����Ǿ�� ���� ���ϰ�
	//decltype�� ���� ���� ��Ģ���� ����Ǿ�� ���� ���Ѵ�.
	return std::forward<Container>(c)[i];
}


decltype(auto) f1()
{
	int x = 0;
	return x; //int����ȯ
}

decltype(auto) f2()
{
	int x = 0;
	return (x);//int&�� ��ȯ
}

int main()
{
	const int i = 0;
	decltype(i) a = 1;

	std::vector<int> v;
	v.push_back(10);

	authAndAcces(v, 0) = 1;

	//�̷��� ȣ���ϸ� int �������� ���� �����Ϸ��� ����� �Ǿ� ������ ����
	//authAndAcces2(v, 0) = 1; 

	authAndAcces3(v, 0) = 2;

	//�������� decltype ���� ���� ��Ģ ����

	int w;
	const int& cw = w;

	auto myWidget1 = cw; //myWidget1�� ������ int

	decltype(auto) myWidget2 = cw; //const int&

	//������ �����̳ʸ� �ѱ��. (�����̳� �ӽð�ü�� ������ �����̳ʴ�)

	authAndAcces4(vector<int>(), 0) = 10;

	authAndAcces5(v, 0) = 2;
	return 0;
}