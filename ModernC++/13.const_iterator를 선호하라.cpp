#include <iostream>
#include <memory>
#include <vector>
#include <list>
#include <algorithm>
#include <string>
using namespace std;

int main()
{
	//C++98 ��Ÿ��
	//const_iterator�� ����ϵ��� �ڵ带 ��ġ�°��� ������ �������� �׷��� �ʴ�.
	typedef std::vector<int>::iterator Iter;
	typedef std::vector<int>::iterator constIter;

	std::vector<int> values;

	//C++98������ ��const �����̳ʷκ��� const_iterator�� ��� ������ ����� ���⶧���� ĳ����.
	//const���������� �޾Ƽ� ����ص� �Ǳ�ȴ�.
	constIter ci = std::find(static_cast<constIter>(values.begin()), static_cast<constIter>(values.end()) , 1983);

	//C++98������ ���� ��ġ�� iterator�θ� ���� �� �ִ�.
	values.insert(static_cast<Iter>(ci), 1998); //�������� �ȵ� �� �� �ִ�.
												//const_iterator���� iterator�� �̽ļ��ִ� ��ȯ�� ���� ������.

	//C++11 ��Ÿ��
	std::vector<int> values2;

	auto it = std::find(values2.cbegin(), values2.cend(), 1983);
	values2.insert(it, 1998);

	

	return 0;
}

//�Ϲݼ��� �ش�ȭ�� �ڵ�� Ư�� ��� �Լ��� ���縦 �䱸�ϴ� ��� �� ��� �Լ��� �����ϴ� ���� �Լ��� ����Ѵ�.
//���ڵ�� C++14���� �ߵ��������� C++11���� �������� �ʴµ� ǥ�ؿ� ���� ������.
template<typename C, typename V>
void findAndInsert(C& container, const V& targetVal, const V& insertVal)
{
	using std::cbegin;
	using std::cend;

	auto it = std::find(cbegin(container), cend(container), targetVal);

	container.insert(it, inserVal);
}

//C++11�� ���� �����Լ� ����
template<class C>
auto cbegin(const C& container)->decltype(std::begin(container))
{
	//container -> const vector<int>&
	return std::begin(container); //const�����̳ʿ� ���� ���� cbegin�Լ��� ȣ���ϸ�
								  //const_iterator ������ �ݺ��ڰ� ��ȯ�ȴ�.
}