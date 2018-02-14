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
		std::lock_guard<std::mutex> g(m); //뮤터블이지만 m은 const변수로 간준된다. const함수 안이므로.

		if (!rootAreValid)
		{
			//근드을 계산, 저장한다
			rootAreValid = true;
		}

		return rootVals; //뮤택스를 푼다
	}

private:
	mutable std::mutex m; //std::mutex를 복사하거나 이동할 수 없기 때문에 Polynomial 클래스의 복사 & 이동 능력도 사라진다.
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
	//std::atomic를 복사하거나 이동할 수 없기 때문에 Point 클래스의 복사 & 이동 능력도 사라진다.
	mutable std::atomic<unsigned> callCount{ 0 };
	double x, y;
};

//위 같은 const멤버함수를 두개이상의 스레드에서 사용하면 문제가 되기때문에 const멤버함수는 스레드에 안전하게 만드는것이 좋다.
//std::atomic 변수는 뮤택스에 비해 성능에 이점은 있지만, 하나의 변수 또는 메모리 장소를 다룰때에만 적합하다.

int main()
{
	return 0;
}