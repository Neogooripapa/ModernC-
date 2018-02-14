#include <iostream>
#include <memory>
#include <vector>

using namespace std;

//����ð��� ���̴� ��� pimple���뱸

class Gadget
{

};
///////////////////////////////////////
//#include "widget.h"
//����� cpp���� �ʿ��ϰ� �ȴ�.
class Widget
{
public:
	Widget();
	~Widget();//���� �صд�.

	//�Ҹ��ڸ� �����ϸ� �����Ϸ��� �̵������� �������� �����Ƿ� �����ؾ��Ѵ�.
	Widget(Widget&& rhs);
	Widget& operator=(Widget&& rhs);

	//unique_ptr���� �̵� ���� ������ �ִ� Ŭ������ ���ؼ��� �����Ϸ��� ���� ������� �ۼ������� �����Ƿ� �ۼ�
	//�ۼ��Ѵٰ� �ص� ���� ����. �츮�� ���ϴ°��� �����Ͱ� ����Ű�� ������ �����ϴ� ���� ����.
	Widget(const Widget& rhs);
	Widget& operator=(const Widget& rhs);
private:
	struct Impl;
	std::unique_ptr<Impl> pImpl; //�������� ��� ����Ʈ �����͸� ���.
};

//widget.cpp///////////////////////////////////
//#include "widget.h"
//#include "gadget.h"

struct Widget::Impl
{
	std::string name;
	std::vector<double> data;
	Gadget g1, g2, g3;
};

Widget::Widget()
	: pImpl(std::make_unique<Impl>())
	//: pImpl(new Impl)
{

}

Widget::Widget(const Widget& rhs)
: pImpl(nullptr)
{
	if (rhs.pImpl)
	{
		pImpl = std::make_unique<Impl>(*rhs.pImpl);
	}
}

Widget& Widget::operator=(const Widget& rhs)
{
	if (!rhs.pImpl)
		pImpl.reset();
	else if (!pImpl)
		pImpl = std::make_unique<Impl>(*rhs.pImpl);
	else
		*pImpl = *rhs.pImpl;

	return *this;
}

Widget::Widget(Widget&& rhs) = default;
Widget& Widget::operator=(Widget&& rhs) = default;

Widget::~Widget() = default;

/*
//w�� ���ǰ� �ʿ������Ƿ� ���Ǹ� �س��´�.
Widget::~Widget()
{
}
*/

/*
Widget::~Widget()
{
	delete pImpl;
}
*/
////////////////////////////////////////

//#include "widget.h"
int main()
{
	//�� ������ w�� �ı��µǴ� ������ ���ؼ� �����Ϸ��� �ۼ��ϴ� �ڵ忡�� �߻��Ѵ�.
	//w�� �Ҹ��ڰ� ȣ��Ǵµ� unique_ptr�� �̿��ϴ� Widget Ŭ������ �Ҹ��ڰ� ���� ����(�Ҹ��ڰ� ���� ������ ���� ������)
	//�� ��� �����Ϸ��� ��� �Ҹ��ڸ� �������ش�. �����Ϸ��� pImpl�� �Ҹ��ڸ� ȣ���ϴ� �ڵ带 �����Ѵ�.
	//pImpl �⺻�����ڸ� ����ϴ� unique_ptr�̰� �⺻�����ڴ� �������͸� delete�ϴ� �Լ��̴�.
	//�׷��� ��κ��� ǥ�� ���̺귯�� �����鿡�� �� ������ �Լ��� delete�� �����ϱ� ���� �������Ͱ� �ҿ����� ������ ����Ű������
	//static_assert�� �̿��ؼ� �����Ѵ�. �̶� ���� �ƴѰ��� �����Ǹ� ������ �߻��Ѵ�.
	//�̴� �����Ϸ��� �ۼ��ϴ� �ٸ� ��� Ư�� ����Լ� ó�� Widget�� �Ҹ��ڴ� �Ϲ������� inline�̱� �����̴�. (�ζ��ο��� �����ε� �ʿ������Ƿ�)
	//���� �Ҹ��ڸ� ���Ǹ� �صд�.
	Widget w; //����!

	//shared_ptr�� ����Ͽ� pimple�� ������ ��� ������ ������ ������ �� �̻� ������� �ʴ´�.
	//�׷� ��� Widget���� �Ҹ��ڸ� ������ �ʿ䰡 ����, ����� �Ҹ��ڰ� �������� �����Ϸ��� �Ҹ��ڸ� �ۼ��Ѵ�.
	//�׸��� �̵�������� �츮�� ���ϴ� ������� �۵��Ѵ�.
	//������ unique_ptr���� ������ ������ ����Ʈ ������ ������ �Ϻκ��̸�, �̴��п� �ӵ��� ������.
	//�̷��� �������� ����� ����� �����Ϸ��� �ۼ��� Ư�� ��� �Լ��� ���̴� �������� ����Ī ���ĵ��� ������ �����̾�� �Ѵٴ°��̴�.
	//(�������Ҷ� �Ҹ������ĵ� �������ؾ��ϴϱ� �̽����� �ʿ������µ�)

	//�ݸ� shared_ptr�� �������� ������ ����Ʈ������ ������ �Ϻΰ� �ƴϴ�. �̶����� �� ��������, �����Ϸ��� �ۼ��ϴ� ����Լ����� ���̴�
	//�������� ����Ī ���ĵ��� ������ �����̾�� �Ѵٴ� ������ �������.
	return 0;
}