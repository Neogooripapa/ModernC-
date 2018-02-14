#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
using namespace std;

class Widget
{
};

int main()
{
	//weak_ptr�� shared_ptr�� �����ϴ� �����ʹ�.
	auto spw = make_shared<Widget>(); //�������� 1

	weak_ptr<Widget> wpw(spw); //�������� 1

	spw = nullptr; //���� ���� 0�̵ǰ� Wiget�� �ı� �ȴ�.
				   //����wpw�� ����� ���� �����̴�.

	//wpw�� ��ü�� ����Ű�� �ʴ��� �˻�.
	if (wpw.expired())
	{
		//����Ī ��ü�� �����ϴ� ����� �ڵ带 ���ø� �� ������,
		//weak_ptr���� ������ ������ �����Ƿ� ������ �ʴ´�.

		//����� �� �����, weak_ptr�� ���Ῡ�θ� �����ϰ� ���� ������� �ʾ�����
		//����Ī ��ü�� ���� ������ �����ִ� ������ �ϳ��� ������ �������� �����ϴ� ���̴�.

		shared_ptr<Widget> spw1 = wpw.lock(); //wpw�� �����̸� spw1�� ��
		//auto spw2 = wpw.lock(); //���� ����

		shared_ptr<Widget> spw3(wpw); //wpw �����̸� std::bad_weak_ptr ���ܰ� �߻��Ѵ�.
	}

	return 0;
}

//weak_ptr�� ������ ���� 1 (ĳ��) (������ ���)
//�� �Լ��� ����� ũ�ٰ� ��������. ���� ������ ĳ�� �� ĳ�ÿ��� �����ϴ� �Լ��� �ʿ�������.
unique_ptr<const Widget> loadWidget(int widgetId)
{
	return unique_ptr<const Widget>();
}


//���Լ� �ٱ����� ��ü�� �Ҿ��ٸ� ���⼭ ���� �ٰ��̰�
//��ü�� ���� �ʾҴٸ� ĳ�̵� ��ü�� �ٰ��̴�. ���⼭ ��ü�� ��� �ִ��� �Ǵ��ϴ� weak_ptr�� �ٽ��̴�.
shared_ptr<const Widget> fastLoadWidget(int widgetId)
{
	static std::unordered_map<int, weak_ptr<const Widget>> cache;

	auto objPtr = cache[widgetId].lock();

	if (!objPtr)
	{
		objPtr = loadWidget(widgetId);
		cache[widgetId] = objPtr;
	}

	return objPtr;
}

//weak_ptr�� ������ ���� 2 (��ȯ�� ����)
//��Ȳ B��ü�� A�� C��ü�� ���ÿ� �������� �����ϴ� ��Ȳ A ---> B <--- C
//�̻��¿��� B���� �ٽ� A�� ����Ű�� �����Ͱ� �ʿ��ϰ� �Ǿ��ٰ� �ϸ� ���������� ������? 
//A ---> B <--- C
//  <---
//1.�������� : A�� �ı��� ����� B�� �𸣹Ƿ� ������ ������ �߻��� �� ����.
//2.shared_ptr : ��ȯ������ ���ؼ� �Ѵ� �ı����� ���Ѵ�( A�� �ı��Ǳ� ���ؼ� B���ı��Ǿ� ��. �ݴ�� B�� �ı��Ǳ� ���ؼ� A�� �ı��Ǿ�� ��)
//3.weak_ptr : B�� A�� �ı� ����� �� �� �ְ� ���� �������� ������ ���� �����Ƿ� �ذ�ȴ�.

//������ Ʈ���������� tip
//�ڽĳ��� �θ��� �����ϹǷ� �θ� �ı��Ǹ� �ڽĵ� �ı��Ǿ�� �ϱ� ������ unique_ptr�� ����.
//�ڽĿ��� �θ�ε��� ����ũ�� �������͸� ����ص� ����. �ڽ��� ������ �θ𺸴� ��� ���� ������. �� �ڽĳ�尡 �θ� �������� ���� ����.

//weak_ptr�� ��ü�� ������ ������ �������� ������, ���� ����Ī ��ü�� ���� Ƚ���� ������ ��ġ�� �ʴ´�.
//�����Ͽ��� �ι�° ���� Ƚ���� ������ �װ��� �ٷ� weak_ptr�� �����ϴ� ���� Ƚ���̴�.