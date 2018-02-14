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

typedef void(*FP)(int, const std::string&); //FP�� �̸�

using FP = void(*) (int, const std::string&); //��Ī ����

template<typename T>
using MyAllocList = std::list<T, std::vector<T>>;

//typedef �� ���������� template�� �� �� ����.
template<typename T>
struct MyAllocList2
{
	typedef std::list<T, std::vector<T>> type;
};


class Wine {};
template<>
class MyAllocList2<Wine>
{
	enum class WineType { White, Red, Rose };
	WineType type;
};

template<typename T>
class Widget
{
	//MyAllocList2<T>::type�� ���ø� ���� �Ű����� (T)�� �������� ������ ��Ī�Ѵ�.
	//MyAllocList2<T>::type�� �������� �����̴�.
	//�������� ������ �̸� �տ� �ݵ�� typename�� �ٿ��� �Ѵ�.
	
	//MyAllocList2<T>::type�� ���� �����Ϸ��� �װ��� ������ �̸����� Ȯ������ ���Ѵ�.
	//�ֳ��ϸ� MyAllocList2�� � Ư��ȭ ���� MyAllocList2<T>::type�� ���� �̿��� ���� ��Ī�� ���ɼ��� ���� �ֱ� �����̴�.
	//class MyAllocList2<Wine> ������ �˼� �ֵ���.
	typename MyAllocList2<T>::type list;
	//MyAllocList2<Wine>::type list2; //������ �ƴ϶� �ڷ����� ��Ī�Ѵ�. ����!
									  //���� MyAllocList2<T>::type�� ������ ��Ī�ϴ����� �������� T�� ���������� �����Ѵ�.
									  //�̶����� �������� �޾Ƶ��̰� �ϱ� ���ؼ��� typename�� �ٿ����Ѵ�.
};



template<typename T>
class Widget2
{
	//typename ::type �� �ʿ����
	//MyAllocList2<T>::typeó�� MyAllocList<T>�� ���ø� �Ű����� T�������ϴ°� ������
	//�����Ϸ��� Widget2 ���ø��� ó���ϴ� �������� MyAllocList<T>�� ���� �κп� ����������,
	//�����Ϸ��� MyAllocList<T>�� ������ �̸����� �̹� �˰� �ִ�.
	//MyAllocList<T>�� ���� ���ø��̹Ƿ�, �� �������̴�. ���� typename�� �ʿ䰡 ���� ���̸� �ȵȴ�.
	MyAllocList<T> list; 
};

int main()
{
	//1.��Ī ���ø� (typedef�� ���ø�ȭ �� �� ������ ��Ī ������ ���ø�ȭ�� ����)
	MyAllocList<int> lw;

	MyAllocList2<int>::type lw2;
	return 0;
}