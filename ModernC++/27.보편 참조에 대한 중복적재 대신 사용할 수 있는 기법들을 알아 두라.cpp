#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <chrono>
using namespace std;

//���� ������ �ߺ����� �Լ��� ����Ҷ� ������ ����°��� ���ϴ� ����� �����Ѵ�.
//(�����ϰ� �������� �Լ��� �Ҹ��ٰų�~ 26�׸� ����)

//1.����ǥ ����� ����Ѵ�.
std::multiset<std::string> names;

//���Լ��� int�� ȣ���� ��� �ȿ��� string�� ���ϹǷ� ������ �߻��Ѵ�.
template<typename T>
void logAndAdd(T&& name)
{
	auto now = std::chrono::system_clock::now();
	//�α׸� ����Ѵ�.
	names.emplace(std::forward<T>(name));
}


//���� �Լ��ȿ��� �����ϴ� ������ �߰��غ���. (true_type, false_type�� ����ǥ�� �ȴ�)
template<typename T>
void logAndAddImpl(T&& name, std::false_type)
{
	auto now = std::chrono::system_clock::now();
	//�α׸� ����Ѵ�.
	names.emplace(std::forward<T>(name));
}

string nameFromIdx(int idx) 
{
	return "TEST"; 
}

void logAndAddImpl(int idx, std::true_type)
{
	names.emplace(nameFromIdx(idx));
}

template<typename T>
void logAndAdd2(T&& name)
{
	//int�ް��� ��� ���� T�� int&�� �����ϴµ� �̴� ���������� �ƴϴ�. ���� �̰� ���� �Դ´�.
	//logAndAddImpl(std::forward<T>(name), std::is_integral<T>()); 
	 
	logAndAddImpl(std::forward<T>(name), std::is_integral<std::remove_reference_t<T>::type>());
}

//2.���� ������ �޴� ���ø��� �����Ѵ�. 
//�����ڿ��� ����ǥ ����� ����ص� �����Ϸ��� �����ϴ� �Լ��� ���ؼ� ����ǥ ����� ������� ���� ������ �ִ�.
//std::enable_if�� �̿��ϸ� �����Ϸ��� ��ġ Ư�� ���ø��� �������� �ʴ� ��ó�� �ൿ�ϰ� ���� �� �ִ�. �̸� ��Ȱ���� ���ø��̶�� �θ���.
//std::enable_if�� ����ϴ� ���ø��� ���� �� std::enable_if�� ������ ������ ������ ������ Ȱ��ȭ �ȴ�.
//std::is_same �������� ������ ����
//std::decay ��� ���� �������, const, volatile�� ����

class Person
{
public:
	//�� ����� Person�� �ٸ� � Person���� �����ϴ� ���, �װ��� �ް��̵�, �������̵�
	//const�̵� ��const�̵�, volatile�̵� ��volatile�̵� ���������� �޴� �����ڴ� ���� ȣ����� �ʴ´�.
	//(Person�� �̵� �����ڰ� ȣ��Ǳ⸦ ����ߴµ� ������ ����Ǽ� ���⼭ ���°���)
	template<typename T,
		//typename = typename std::enable_if<!std::is_same<Person, typename std::decay<T>::type>::value>::type> (��Ŵ� �Ļ� Ŭ�������� Ŀ���� �ȵ�)
		//typename = typename std::enable_if<!std::is_base_of<Person, typename std::decay<T>::type>::value>::type> //�Ļ� Ŭ�������� Ŀ����.
		//typename = std::enable_if_t<!std::is_base_of<Person, std::decay_t<T>::type>::value>> //C++14��Ī ����.
		typename = std::enable_if_t<!std::is_base_of<Person, std::decay_t<T>::type>::value //�����μ��� ������ �μ��� ���� �߰�.
		&& !std::is_integral<std::remove_reference<T>>::value
		>
	>
	explicit Person(T&& n)
		: name(std::forward<T>(n))
	{
	}

	//���� ������ �̳༮�� �Ҹ���¡.
	explicit Person(int idx)
		: name(nameFromIdx(idx))
	{
	}

	//�����Ϸ��� �ۼ���.
	//Person(const Person& rhs);
	//Person(Person&& rhs);
private:
	std::string name;
};

class SpecialPerson : public Person
{
public:
	SpecialPerson(const SpecialPerson& rhs)
		:Person(rhs) //�Ϻ� ���� �����ڸ� ȣ��.
	{
	}
	SpecialPerson(SpecialPerson&& rhs)
		:Person(std::move(rhs)) //�Ϻ� ���� �����ڸ� ȣ��.
	{
	}
};

//3.�Ϻ� ������ ���� (���� �޽����� �����ϴ�)
//�Ϻ� ���޿� ������ ���� ��Ŀ����� �����Ϸ��� const char16_t ���� �迭�� �ƹ� ������� �������� �Ű������� ���´�.
//�����ڴ� �� �Ű������� Person�� string�ڷ� ����� �����ڿ� �����ϸ�, �׶��� ȣ���ڰ� ������ �� (const char16_t�迭)��
//�ʿ��� ��(string �����ڰ� �޾Ƶ��� �� �ִ� ����������) ������ ����ġ�� �߰ߵȴ�.
//�׷��� ���� �޽����� �̳� ���� ��ȣ�ϴ�. ���� ������ ���޵Ǵ� Ƚ���� ���� ���� ���� �� ��Ȳ�� �޽����� ���´�.
//Person p(u"Kor");//"Kor"�� const char16_t ������ ���ڵ�� �̷����.


class Person1
{
public:
	template<typename T,
		typename = std::enable_if_t<!std::is_base_of<Person1, std::decay_t<T>::type>::value
		&& !std::is_integral<std::remove_reference<T>>::value
		>
	>
		explicit Person1(T&& n)
		: name(std::forward<T>(n))
	{
		//�̰�� ���� ���� �Ű� ������ std::string�� ���� �ʱ�ġ�� ���� ���� �˰� �����Ƿ�,
		//�׷� �ʱ�ġ�� ����ϴ°��� �������� �����ϴ� ����� �ִ�.
		static_assert(std::is_constructible<std::string, T>::value,
			"Parameter n can't be used to construct a std::string"
			);
		//��Ÿ ���Ե�, �� ������ static_assert�� �������� ������ ������, �Ű������� �����ϴ� �ڵ�� �׺��� �տ� �ִ�.
		//���� �����Ž��� ���� ������ ���� ���Ʈ �޽����� ���´� -_-;
	}
private:
	std::string name;
};

int main()
{
	return 0;
}