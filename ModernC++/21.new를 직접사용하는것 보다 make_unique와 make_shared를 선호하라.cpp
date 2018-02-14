#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
using namespace std;

//C++11������ make_unique�� �������� �����Ƿ� ������ ���� ���鼭 ���
template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&&... params)
{
	return std::unique_ptr<T>(new T(std::forward<Ts>(params)...)); //�ζ���ȭ �ȴ�.
}

//make_unique�� make_shared�� ������ ������ ������ �μ����� �޾Ƽ� �����ڷ� �Ϻ������ؼ� ��ü�� �������� �����Ѵ�.
class Widget
{

};

void processWidget(std::shared_ptr<Widget> spw, int priorlity)
{

}

int computePriority()
{
	return 1;
}

class ReallyBigType {};

int main()
{
	//1.Widget�� �ι����� ���ŷο��� ���� �� ����.
	auto upw1(std::make_unique<Widget>());
	std::unique_ptr<Widget>upw2(new Widget);

	auto spw1(std::make_shared<Widget>());
	std::shared_ptr<Widget>spw2(new Widget);

	//2.���� ������
	processWidget(std::shared_ptr<Widget>(new Widget), computePriority()); //�ڿ� ������ ������ ����.

	//������ �߻��ϴ� ������ �����Ϸ��� �ҽ��ڵ带 �����ڵ�� �����ϴ� ��İ� ���� �ִ�.
	//coputePriority�Լ��� �ٸ� ȣ�� ���̿��� ȣ��ǵ��� �򰡵� �� �ִ�.
	//1.new Widget�� �����Ѵ�.
	//2.computePriority�Լ��� �����Ѵ�.
	//3.shared_ptr �����ڸ� ȣ���Ѵ�.
	//new Widget�����Ŀ� computePriority�Լ��� ���ܸ� �����ٸ� Widget�� �������Ͱ� ����Ű�� �ִ� �����Ҵ� ��ü�� �ȴ�.
	//shared_ptr�� ȣ���Ͽ� �������� ���߱� ������.

	//����
	processWidget(std::make_shared<Widget>(), computePriority()); //�ڿ� ������ ������ ����.

	//3.make_shared�� ����ϸ� �����Ϸ��� �� �� ������ �ڷᱸ���� ����ϴ� �� �۰� ���� �ڵ带 �����Ѵ�.
	std::shared_ptr<Widget> spw(new Widget); //�ι��� �޸� �Ҵ� (��ü + ������)

	auto spw19 = std::make_shared<Widget>(); //make_shared�� (��ü + ������)�� �ѹ��� ���� �� �ִ� ũ���� �޸� ������ �ѹ��� �Ҵ��Ѵ�.
											 //�޸� �Ҵ��� �ѹ��� ȣ���ϹǷ� ���α׷��� ���� ũ�Ⱑ �پ���. �ڴ��� �ӵ��� ��������.
											 //���� ��Ͽ� ���� ������ ���� ������ ������ ������ �ʿ䰡 �������� ���α׷��� �޸� ��뷮�� �ش�.

	//4.����� �Ұ����� ���.
	auto widgetDeleter = [](Widget* widget)
	{
		delete widget;
	};

	//make�Լ����� Ŀ���� �����ڸ� ������ �� ����
	std::unique_ptr<Widget, decltype(widgetDeleter)> upw20(new Widget, widgetDeleter);
	std::shared_ptr<Widget> spw20(new Widget, widgetDeleter);

	//�Ѵ� �� 20 ���� 10���Ҵ��Ѵ� �̴� make�Լ��� ����������
	//�Ϻ����� �Ҵ� ��ȣ�� ������� ���Ѵ�.
	//���������� ����Ī��ü�� �߰�ȣ �ʱ�ġ�� �����Ϸ��� ���� new�� �����ؾ��Ѵ�.
	//Widget w{{}}; or Widget w({})
	//vector<int> v1{10,20} 10, 20���ҷ� ����.

	auto upv = std::make_unique<std::vector<int>>(10, 20);
	auto spv = std::make_unique<std::vector<int>>(10, 20);

	//�߰�ȣ �ʱ�ȭ ��ȸ���
	auto initList = { 10,20 }; //std::initializer_list��ü ����
	auto spv2 = std::make_shared<std::vector<int>>(initList);

	//make_unique�� ��쿡�� �� �ΰ��� ��Ȳ�� ������ �Ǵ� ��Ȳ�� ����
	
	//Ŭ���� ���� operator new�� operator delete�� �ִ� ������ ��ü��
	//make�Լ��� �����ϴ� ���� ��ü�� �ٶ��� ���� �ʴ�.
	//std::allocate_shared�� �䱸�ϴ� �޸������� ũ��� �������� �Ҵ�Ǵ�
	//��ü�� ũ�Ⱑ �ƴ϶�, �� ũ�⿡ �������� ���Ѱ��̱� �����̴�.
	//��ü�� �������� ������ �޸� ������ ���δ�.

	//��ü�� �����ϰ� �ִ� �޸� ������ �������� �ı��Ǳ� �������� �����ɼ� ����.
	//�ᱹ shared_ptr�� weak_ptr�� �����Ͽ��� �������� ��ü�� �����Ǳ� ������ 
	//shared_ptr�� weak_ptr�� �ı� ������ ������ ��ٸ�, ��ü �ı� ������ ��ü�� �����ϴ� 
	//�޸𸮰� �����Ǵ� ���� ���̿� �ð� ������ ���� �� �ִ�.
	
	auto pBigObj = std::make_shared<ReallyBigType>(); //����ū ��ü�� ����.
	//1.ū ��ü�� ����Ű�� shared_ptr�� weak_ptr���� �����ؼ� ����Ѵ�.
	//2.���⼭ ��ü�� ����Ű�� shared_ptr�� �ı��ǳ� weak_ptr���� ������ ���� �ִ�.

	//3.�̺κп��� ū��ü�� �����ϴ� �޸𸮴� ������ �Ҵ�� �����̴�.
	//4.���⼭ ��ü�� ����Ű�� ������ weak_ptr�� �ı��ȴ�. ���� ���� ��ϰ� ��ü�� �����ϴ� �޸𸮰� �����ȴ�.

	std::shared_ptr<ReallyBigType> pBigObj2(new ReallyBigType); //new�� �̿��ؼ� ����.
	//1.ū ��ü�� ����Ű�� shared_ptr�� weak_ptr���� �����ؼ� ����Ѵ�.
	//2.���⼭ ��ü�� ����Ű�� shared_ptr�� �ı��ǳ� weak_ptr���� ������ ���� �ִ�. ��ü�� �޸𸮴� �����ȴ�.
	
	//3.�̺κп��� ���� ����� ���� �޸𸮸� �Ҵ�� �����̴�.
	//4.���⼭ ��ü�� ����Ű�� ������ weak_ptr�� �ı��ȴ�; ���� ���� ����� �����ϴ� �޸𸮰� �����ȴ�.

	//���� new�� ��������ϸ鼭�� ������ ������ ������ �ذ��ؾ��Ѵ�.
	//��å�� new�� ����� �ٸ����� �������� �ʴ� ���忡�� ����Ʈ �������� �����ڿ� ��� �Ѱ��ִ� ���̴�.

	//������ ���忡 �ΰ� �����Ѵ�.
	std::shared_ptr<Widget> s(new Widget, widgetDeleter);
	processWidget(s, computePriority()); //��Ȯ ������ ������ �ƴ�.
	//shared_ptr�� ���������� �������� Ȯ���ϱ� ������
	//s�� �����ڰ� ���ܸ� �����ٰ��ص� (�̸��׸� ������ �����Ҵ� ���з�) new Widget���� ������� �����Ϳ�����
	//�����ڰ� Ȯ���� ȣ��ǹǷ� �޸� ������ �߻����� �ʴ´�.

	//�ް��� �Ѱ��ֹǷ� ���������� �ٲ��� ����Ȯ�� ����.
	processWidget(std::move(s), computePriority());

	return 0;
}