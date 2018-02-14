#include <iostream>
#include <memory>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
using namespace std;

class Polynomial
{
public:
	using RootsType = std::vector<double>;

	RootsType roots() const
	{
		std::lock_guard<std::mutex> g(m); //���ͺ������� m�� const������ ���صȴ�. const�Լ� ���̹Ƿ�.

		if (!rootAreValid)
		{
			//�ٵ��� ���, �����Ѵ�
			rootAreValid = true;
		}

		return rootVals; //���ý��� Ǭ��
	}

private:
	mutable std::mutex m; //std::mutex�� �����ϰų� �̵��� �� ���� ������ Polynomial Ŭ������ ���� & �̵� �ɷµ� �������.
	mutable bool rootAreValid{ false };
	mutable RootsType rootVals{};
};

class Point
{
public:
	double distanceFromOrigin() const noexcept
	{
		++callCount;

		return std::hypot(x, y);
	}
private:
	//std::atomic�� �����ϰų� �̵��� �� ���� ������ Point Ŭ������ ���� & �̵� �ɷµ� �������.
	mutable std::atomic<unsigned> callCount{ 0 };
	double x, y;
};

//�� ���� const����Լ��� �ΰ��̻��� �����忡�� ����ϸ� ������ �Ǳ⶧���� const����Լ��� �����忡 �����ϰ� ����°��� ����.
//std::atomic ������ ���ý��� ���� ���ɿ� ������ ������, �ϳ��� ���� �Ǵ� �޸� ��Ҹ� �ٷ궧���� �����ϴ�.

int main()
{
	return 0;
}