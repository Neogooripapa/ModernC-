#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <chrono>
using namespace std;

std::multiset<std::string> names;

void logAndAdd(const std::string& name)
{
	auto now = std::chrono::system_clock::now();

	//�α׸� ����Ѵ�.

	names.emplace(name);
}

//���� ����
template<typename T>
void logAndAdd2(T&& name)
{
	auto now = std::chrono::system_clock::now();

	//�α׸� ����Ѵ�.

	names.emplace(std::forward<T>(name));
}

//���ڿ� ������ �̸��� ���� ���� ���ϴ� Ŭ���̾�Ʈ�� �ִ�.
//�׷� Ŭ���̾�Ʈ�� ���ϴ� �̸��� ��� ���θ� ������ �ִ� ���� �����ϰ�
std::string nameFromIdx(int idx)
{
	return "";
}

void logAndAdd2(int idx)
{
	auto now = std::chrono::system_clock::now();

	//�α׸� ����Ѵ�.

	names.emplace(nameFromIdx(idx));
}

//������ �޴� �����Լ� ��� �׿� ���� ���� �ϴ� �����ڸ� ���� Ŭ������ �����ϸ� ���? ������ �� �ɰ�������.
//explicit ����� ����ȯ�� �����ϰ�
class Person
{
public:
	template<typename T>
	explicit Person(T&& n)
		: name(std::forward<T>(n))
	{
	}

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

//�Ļ�Ŭ���������� �� ��������
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

int main()
{
	std::string petName("Darla");

	logAndAdd(petName); //�ް� �ѱ� emplace(���� �Ͼ)

	logAndAdd(std::string("Persephone")); //������ �ѱ� emplace(���� �Ͼ)

	logAndAdd("Patty Dog"); //���ͷ��� �ѱ� �ӽ� string�� ����� name�� �ӽ� string�� ���δ�. emplace(���� �Ͼ)

	//���� ���� ȣ��

	logAndAdd2(petName); //�ް� �ѱ� emplace(���� �Ͼ)

	logAndAdd2(std::string("Persephone")); //������ �ѱ� emplace(�������� �̵��Ѵ�)

	logAndAdd2("Patty Dog"); //�ӽ� string��ü�� �����ϴ� ���, multiset�ȿ� string�� �����Ѵ�.

	//���ι��� �α� �Լ� ȣ��
	logAndAdd2(22); //�ߺ������ ������� ���� ���ؼ��� ���� �����Ѵ�.

	short nameIdx = 22;
	//logAndAdd2(nameIdx); //����! T�� short&�� �����Ѵ�. ���� ���� ������ ȣ����� �ʴ´�.
	                     //emplace�� ���޵� ���� string�ε� short�� ��� �����Ƿ� �����Ѵ�.

	//���� ���������� �ߺ����縦 �����ϴ°��� ���� ���� �����̵ȴ�.

	//Ŭ���� �ߺ����� �Լ� ȣ��
	Person p("nancy");
	//auto cloneOfP(p); //������ ����! �Ϻ������� �����ڸ� ȣ���Ѵ�.
	                  //explicit Person(Person& n)���� �߷� �Ͽ� ���յǰ� �ȴ�.

	const Person cp("nancy");
	auto cloneOfCp(cp); //���ø��� �Ȱ��ٸ� �����Լ� ȣ���� �켱�̴� ���� �����ڰ� ȣ��ȴ�~!

	return 0;
}