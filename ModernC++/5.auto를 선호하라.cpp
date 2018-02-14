#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include <unordered_map>
using namespace std;

int main()
{
	//1.auto�� std::function���� ����

	//auto x2; //����! �ʱ�ġ�� �� �ʿ���
	
	//�����Ϸ��� �˴� ������ ������ �� �ִ�. r
	auto r = [](const auto& p1, const auto& p2) -> bool
	{
		return *p1 < *p2;
	};

	//ȣ�Ⱑ���� ��ü��� ����̵� ����ų�� �ִ�. (�� �Լ�ó�� ȣ�Ⱑ���Ѱ��� ����ų�� �ִ� ���ٰ�����)
	std::function<bool(const std::unique_ptr<int>&, const std::unique_ptr<int>&)>
		ref = [](const std::unique_ptr<int>& p1, const std::unique_ptr<int>& p2) { return *p1 < *p2; };


	//������ �Ȱ��̸� Ŭ��������� auto , ����� auto�� �¸���. ������̵� �׷���
	//auto�� �����, Ŭ������ ��� ������ Ŭ������ ���� �����̸�, �� Ŭ������ �䱸�Ǵ� ��ŭ�� �޸𸮸� ����Ѵ�.
	//�ݸ� std::function�� ���ø��� �ν��Ͻ��̸�, �� ũ��� �־��� ���� ���� �����Ǿ� �ִ�. �� ũ�Ⱑ Ŭ������ ��µ�
	//�����ϸ� ���޸𸮸� �Ҵ��� �� �� �ִ�. 
	//��������� std::function ��ü�� auto�� ����� ��ü���� �޸𸮸� �� ���� �Һ��Ѵ�.
	//���� �ζ���ȭ�� �����ϰ�, �����Լ� ȣ���� �����ϴ� ���� ���λ��� ������ auto���� ������. �׸��� �޸� ������ ������ �� �ִ�.


	//2.auto�� ���� ������ ���� �� �ִ�.
	std::vector<int> v;
	unsigned sz = v.size(); //32��Ʈ���� 32��Ʈ ũ��� ������ 64��Ʈ������ std::vector<int>::size_type�� 64��Ʈ�̹Ƿ� �߸� �� �ִ�.

	auto sz2 = v.size(); //�̷��� �ϸ� �ȴ�~!


	std::unordered_map<string, int> m; //���⼭ ���� ������ std::pair<const string,int> �̴�

	//�ӽú����� �����ؼ� std::pair<string,int> �� std::pair<const string,int>�� ���������Ѵ�.
	//p = �ӽú��� ���� p�� �ӽú����� �ּҸ������� �ִ�.
	for (const std::pair<string, int>& p : m) 
	{

	}

	const int a = 1;
	//int& ref = a; //�̰� �ȵǴ°Ͷ� ���� ����ġ
	const int& ref1 = a; //�̰ŵȰ�

	//���� ��Ȯ�� �������� �����Ǵ� auto�� ���� �ӽú��� ������ ������ �ִ�.
	//p = ����m���� �ѿ�Ҹ� ������ �ּҸ� ������ �ȴ�.
	for (const auto& p : m)
	{

	}

	return 0;
}