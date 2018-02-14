#include <iostream>
#include <memory>
#include <vector>
#include <list>
#include <algorithm>
#include <string>
using namespace std;


int f(int x) throw(); //���ܸ� �������� ���� C++98��
					  //���� ���� ���ݵǸ� ȣ�� ������ f�� ȣ���� ������ ������ ������ Ǯ����, ���������� ��� �������Ѵ�.
int f(int x) noexcept; //���ܸ� �������� ���� C++11��
					   //���α׷� ������ ����Ǳ� ���� ȣ�� ������ Ǯ�� ���� �ְ� Ǯ���� ���� ���� �ִ�.
					   //noexcept �Լ����� �����Ϸ� ����ȭ��� ���ܰ� �Լ� �ٱ����� ���ĵ� �� �ִٰ� �ص� ������� ������ Ǯ�� ������
					   //���·� ������ �ʿ䰡 ����.
					   //���ܰ� noexcept �Լ��� ����ٰ� �ص� noexcept �Լ� �ȿ� ��ü���� �ݵ�� ������ �ݴ� ������ �ı��ؾ� �ϴ°͵� �ƴ�.


int f(int x) throw(); //����ȭ ������ ����
int f(int x);		  //����ȭ ������ ����
int f(int x) noexcept; //����ȭ ������ ���� ũ��.

//���Ǻ� noexcept
//����� ����� ���� Ŭ������ �����Ѵٴ°��̴�.
//�ؿ����� Ŭ������ move������ && move���Կ����� �� �����ϰ� �ִ�.
template<class T>
void swap(T& a, T& b) noexcept (std::is_nothrow_move_constructible<T>::value
									&& std::is_nothrow_move_assignable<T>::value)
								//T�κ��� Ÿ���� �����ؼ� Ű����� move������ && move���Կ����� ���� �˻��Ѵ�.
{

}

template<class T, size_t N>
void swap(T(&a)[N], T(&b)[N]) noexcept(noexcept(swap(*a, *b))) //a,b�� ������ Ÿ���̱⶧���� *�� �����ؾ� Ÿ�� �߷��� �ȴ�.
								//���� ���ǵ� swap���κ��� noexcept(move������ && move���Կ����ڰ� noexcept���� �˻��Ѵ�)
{
	//���Լ��� ���� noexcept() ���꿡 ���ؼ� noexcept�Լ��� �ɼ��� �ְ� �ƴ� ���� �ִ�.
}

template<class T1, class T2>
struct pair
{
public:
	void swap(pair& p) noexcept(noexcept(swap(first, p.first)) && noexcept(swap(second, p.second)) )
	{

	}
};

int main()
{
	//std::vector���� push_back�� �̵������� ���ܸ� �������� ������ Ȯ���� ��쿡 ���縦 �̵����� ��ü�Ѵ�.
	//�׷��� ���� ��� �����Ѵ�. (�̵��߿� ���ܰ� �߻��ϸ� �̵��ع��� ��ü���� �������� ������ ���� ���Ͱ����� ��������)
	//�����ϸ� �̵��ϵ� �ʿ��ϸ� �����Ѵ�.
	//�����ڿ� noexcept�� ������ �����Ͽ� �����Ѵ�. 
	//std::vector::push_back -> std::move_if_noexcept -> �̵� �������� noexcept���ο� ���� ������ ���Ǻ� ĳ������ �����Ѵ�.



	//������ �߸����� �Լ� => �����δ� ���ܸ� ������ ������ ���ܸ� ������ �Լ����� ȣ���� ���� �̴�.
	//�ٸ� �Լ��� ���ܸ� ������ ���� �߸� �ռ��� �� ���ܸ� �׳� ��� ��Ų��.
	//��ó�� �׳� ����ϴ� ���ܰ� ������ �� �����Ƿ�
	//���� �߸��� �Լ��� noexcept�� �� �� ����. ���� ��κ��� �Լ��� noexcept�� �ƴϴ�.

	//�⺻������ ��� �޸� ���� �Լ���, ��� �Ҹ��ڰ��� �Ϲ����� noexcept�̴�.
	//noexcept(flase)���ϸ� �Ҹ��ڵ� noexcept�� ���� �� �ִ�.
	
	//noecept�Լ����� ���ܸ� ������ ���α׷��� ����ȴ�.

	return 0;
}