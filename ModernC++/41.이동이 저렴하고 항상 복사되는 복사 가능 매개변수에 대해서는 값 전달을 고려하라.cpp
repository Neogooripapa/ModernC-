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

class Widget
{
public:
	void addName(const std::string& newName)
	{
		names.push_back(newName); //���� 1ȸ
	}

	void addName(const std::string&& newName)
	{
		names.push_back(std::move(newName)); //�̵� 1ȸ.
	}
private:
	std::vector<string> names;
};

class Widget2
{
public:
	template<typename T>
	void addName(T&& newName) 
	{
		names.push_back(std::forward<T>(newName)); //�ް� ����1ȸ, ������ �̵� 1ȸ.
	}
private:
	std::vector<string> names;
};

class Widget3
{
public:
	//������ ����Ȱ��� ������ �̵��ϱ⶧���� �ٸ� ���� ������ ����.
	//���������� �ȵǴ°�쵵 ����������.(30��)
	//���ø��� �ƴϱ⶧���� �Լ��� �ϳ��� ���̴�.
	
	//�� ������ ����� ũ�ٰ� ������ �װ��� C++98���
	//C++98������ ȣ���ڰ� ������ �Ѱ��ִ� ��������� ���� �����ȴ�.
	//C++11������ newName�� �ް��� ������ ���� �����ǰ�, �������� ������ �̵� ������ ���� �����ȴ�.

	void addName(std::string newName)//�ް� ���� ����1ȸ, ������ �̵� ����1ȸ
	{
		names.push_back(std::move(newName));//������ �̵� 1ȸ
	}
private:
	std::vector<string> names;
};

//���� ���� �Ű������� ���ؼ��� �� ������ ����ؾ��Ѵ�.
//���� �� �� ���� �Ű������� �ݵ�� �̵� ���������� ���̴�. (�������� �ȵȴٴ¶�)
//�̵� ���� ���Ŀ����� ���� �����ڰ� ��Ȱ��ȭ �Ǿ� �ֱ� ������ ������ ������ �޴� �����ϳ��� ������ �ȴ�.

class Widget4
{
public:
	void setPtr(std::unique_ptr<std::string>&& ptr)
	{
		p = std::move(ptr);
	}
private:
	std::unique_ptr<std::string> p; //����ũ ptr�� �̵����� ����.
};

void foo()
{
	Widget4 w;

	//������ ������ ����. �Ѻ���� �̵�1ȸ.
	w.setPtr(std::make_unique<std::string>("Modern C++"));
}


class Widget5
{
public:
	void setPtr(std::unique_ptr<std::string> ptr)
	{
		p = std::move(ptr);
	}
private:
	std::unique_ptr<std::string> p; //����ũ ptr�� �̵����� ����.
};

void foo2()
{
	Widget5 w;

	//�Ű������� ������ �޴´ٸ�, �̵����� 1ȸ, �̵����� 1ȸ. ��2ȸ.
	w.setPtr(std::make_unique<std::string>("Modern C++"));
}

void main()
{

}