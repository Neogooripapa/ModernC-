#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include <unordered_map>
using namespace std;

std::vector<bool> features(const int& w)
{
	vector<bool> t;
	return t;
}

int main()
{
	int w;

	//std::vector<bool>::referance�� ��� �� �ϳ��� bool���� �Ϲ��� ��ȯ�̴�. (bool&�� �ƴ϶� bool���� ��ȯ)
	bool priority = features(w)[5];

	//std::vector<bool>�� operator[]�� �����ִ� ���� �����̳��� �� ��ҿ� ���� ������ �ƴϴ�.
	//std::vector<bool>::referance ������ ��ü�̴�.
	//std::vector<bool>�� �ڽ��� bool���� bool�� 1��Ʈ�� ����� ���·� ǥ���ϵ��� ��õǾ� �ִ�.
	//�̷���������� std::vector<bool>�� operator[]�� ���������� ������ �� ����.
	//std::vector<T>�� operator[]�� T&�� �����ֵ��� �Ǿ� ������ C++���� ��Ʈ�� ���� ������ �����Ǿ� �ִ�.
	//bool&�� ���� ������ �� ���ٴ� ���� ������ bool&ó�� �۵��ϴ� ��ü�� �����ִ� ��ȸå�� ����� ���̴�!

	
	//priority2�� std::vector<bool>::referance ��ü�� �� ���纻�̸�, �ش� ���带 ����Ű�� �����Ϳ� 5����Ʈ�� �������� �����Ѵ�. 
	//std::vector<bool>::referance�� �ӽú��� �̹Ƿ� �����࿡�� �ı�
	
	//std::vector<bool> ��ü temp�� ���� -> temp operator[] -> std::vector<bool>::referance ��ü (�����,����ȿ��� ��Ʈ��)
	//auto priority2�� ���� & �������� ������ �ȴ�.
	//���� priority2�� ����� ���� �����Ͱ� �ȴ�.
	auto priority2 = features(w)[5]; //������ �ൿ!

	//auto someVer = �븮��;  "������ �ʴ� �븮�� Ŭ���� ������ ǥ������ ���ؾ� �Ѵ�"

	//�̶� �ع��� auto�� �����°��� �ƴϴ�.
	auto priority3 = static_cast<bool>(features(w)[5]);

	return 0;
}