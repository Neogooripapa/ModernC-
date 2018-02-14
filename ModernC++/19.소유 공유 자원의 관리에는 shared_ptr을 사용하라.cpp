#include <iostream>
#include <memory>
#include <vector>
using namespace std;

class Widget
{
public:
	Widget() {};

	~Widget()
	{
		cout << "����";
	};
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////
class Widget2;
class Widget3;

std::vector<std::shared_ptr<Widget2>> processedWidgets;
std::vector<std::shared_ptr<Widget3>> processedWidgets2;

class Widget2
{
public:
	void process()
	{
		//����Լ� �ٱ����� this�� ����Ű�� �ٸ� shared_ptr�� �ִٸ� ���⼭ �������� ���� ��������Ƿ�
		//������ ������ �Ͼ��.
		processedWidgets.emplace_back(this); //������ ó���� ���� ��Ͽ� �߰� (�߸��� �����)
	};
};

//shared_ptr�� �����ϴ� Ŭ������ �ۼ� �Ҷ�, �� Ŭ������ this�����ͷ� ���� shared_ptr�� �����ϰ� �����ϴ� ���.
//enable_shared_from_this�� ���� ��ü�� ����Ű�� shared_ptr�� �����ϵ� �������� �������� �ʴ� ��� �Լ��� �ϳ� �����Ѵ�.
//shared_from_this�� �װ��̴�. this�� ����Ŵ.
class Widget3 : public std::enable_shared_from_this<Widget3>
{
private:
	//�����ڵ�..
public:
	void process()
	{
		//shared_from_this�� ���� ��ü������ �������� ��ȸ�ϰ� �� �������� ��Ī�ϴ� shared_ptr�� �����Ѵ�.
		//���� ��ü�� �������� �ִٰ� �����ϰ� �����Ѵ�. �׷� shared_ptr�� �������� �ʴ´ٸ� �Լ��� �ൿ�� ���ǵ��� �ʴ´�.

		//shared_ptr�� ��ȿ�� ��ü�� ����Ű�⵵ ���� Ŭ���̾�Ʈ�� shared_from_this�� ȣ���ϴ� ���� �����ϱ� ���ؼ�
		//�����ڸ� private���� �����Ѵ�.

		//�׸��� Ŭ���̾�Ʈ�� ��ü�� ������ �� �ֵ��� std::shared_ptr�� �����ִ� ���͸� �Լ��� �����Ѵ�.
		processedWidgets2.emplace_back(shared_from_this());
	};

	template<typename... Ts>
	static std::shared_ptr<Widget3> create(Ts&&... params)
	{
		return std::shared_ptr<Widget3>(params);
	};
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	//1.shared_ptr�� ũ��� ���������� �ι��̴�. (������+����Ƚ��)
	//2.���� Ƚ���� ���� �޸𸮸� �ݵ�� �������� �Ҵ��ؾ� �Ѵ� (��ü�� ����Ƚ���� �𸥴�)
	//3.���� Ƚ���� ������ ���Ұ� �ݵ�� �������� �����̾�� �Ѵ�. (������)


	//���� Ƚ���� �������� �ʴ� ���. (�̵�����)
	//���� shared_ptr�� �̵��ؼ� �� shared_ptr�� �����ϸ� ���� shared_ptr�� null�̵ȴ�.


	//������ ����.
	auto loggingDel = [](Widget* pw)
	{
		cout << "������ ȣ��" << endl;
		delete pw;
	};


	std::unique_ptr<Widget, decltype(loggingDel)> upw(new Widget, loggingDel); //������ ������ ������ ������ �Ϻ���.
	std::shared_ptr<Widget> spw(new Widget, loggingDel); //������ ������ ������ ������ �Ϻΰ� �ƴ�.

	//�̴� �����̳ʿ� ������ �����ϴ�, �Լ� ���ڷ� �ѱ涧��~
	auto customDelete1 = [](Widget* pw) {};
	auto customDelete2 = [](Widget* pw) {};

	std::shared_ptr<Widget> pw1(new Widget, customDelete1);
	std::shared_ptr<Widget> pw2(new Widget, customDelete2);

	//����ũ ptr�� ���� ��ü�� ���ٰ� ���Ƿ� �ؿ� ������ �ȵ�.
	std::vector<std::shared_ptr<Widget>> vpw{ pw1, pw2 };

	//unique_ptr���� �ٸ� ��������, Ŀ���� �����ڸ� �����ص� shared_ptr ��ü�� ũ�Ⱑ ������ �ʴ´�.
	//�����ڿ� �����ϰ� shared_ptr ��ü�� ũ��� �׻� ������ �ΰ� �з��̴�.
	//�߰� �޸𸮴� ���� �����Ǳ� ������. ������(����Ƚ�� + ������ ���纻) ���⼭ ������ ��.
	
	//�������ͷ� ���� ���� shared_ptr�� �����ϸ� ����Ī ��ä�� ���� ���� ���� ���� ����� ��������Ƿ�
	//������ �ൿ�� �ȴ�.
	auto pw = new Widget;
	
	std::shared_ptr<Widget> spw1(pw, loggingDel); //*pw�� ���� ���� ����� ������.
	std::shared_ptr<Widget> spw2(pw, loggingDel); //*pw�� ���� �ι�° ���� ����� ������.
	//���� �����ڿ� �������͸� �Ѱ��ִ� ���� ���ؾ� �Ѵ�. (make_shared)�� ���
	std::shared_ptr<Widget> spw3(new Widget, loggingDel); //���� new�� ��� ��.
	std::shared_ptr<Widget> spw4(spw2); //spw3�� spw4�� ������ �������� ����Ѵ�.


	//�������Ϳ� this�����ͷ� �������� ����.(��� ����)

	//shared_ptr => unique_ptr�� �Ұ����ϹǷ� ����带 ������ ���.
	//shared_ptr�δ� �迭�� ������ �� ����. ���� ��ü�� ����Ű�� �����͸� �����ϰ� ����Ǿ� ����.
	
	return 0;
}