#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <chrono>
#include <array>
#include <functional>
#include <thread>
#include <future>
using namespace std;

//합류 가능 std::thread는 바탕 실행 스레드중 이거나
//실행 중 상태로 전이할 수 있는 스레드에 대응된다.
//차단된 상태, 기다리는 상태, 실행이 완료된 스레드도 합류가능으로 간주한다.

//합류 불가능 std::thread는 말그대로 합류할 수 없는 스레드
//함수없이 기본생성된 스레드.
//이동복사된 스레드
//join에 의해 합류된 std::thread
//detach 에 의해 탈착된 std::thread : 객체와 그 대응되는 바탕스레드 사이의 연결을 끊는다.

//합류 가능성이 중요한 이유는
//만일 합류 가능한 스레드의 소멸자가 호출되면 프로그램 실행이 종료된다.

//기본적으로 과제기반 설계를 사용하는 것이 좋지만
//네이티브 핸들이 필요하기 때문에 std::thread를 사용

constexpr auto telMillion = 10'000'000; //C++14 쉽게 읽을수 있게 해준다.

//스마트 포인터처럼 소멸자를 호출해서 합류불가능으로 만든다.
class ThreadRAII
{
public:
	enum class DtorAction {join, detach};

	ThreadRAII(std::thread&& t, DtorAction a)
		: action(a), t(std::move(t)) //초기화 목록은 선언된 순서에 따른다.
									 //일반적으로 한 멤버의 초기화가 다른 멤버에 의존할 수 있다.
									 //std::thread는 초기화되자 마자 해당 함수를 실행 할 수 있으므로,
									 //클래스에서 항상 제일 마지막에 선언하는 것이 좋다.
	{
	}

	ThreadRAII(ThreadRAII&&) = default;
	ThreadRAII& operator=(ThreadRAII&&) = default;

	~ThreadRAII()
	{
		//합류 불가능한 스레드에 join이나 detach를 호출하면 미정의 행동이 나오므로 필요하다.
		if (t.joinable())
		{
			if (action == DtorAction::join)
			{
				t.join();
			}
			else
			{
				t.detach();
			}
		}
	}

	std::thread& get() { return t; }

private:
	DtorAction action;
	std::thread t;
};
bool conditionsAreSatisfied()
{
	return true;
}

void performComputation(std::vector<int>& val)
{

}

bool doWork(std::function<bool(int)> filter, int maxVal = telMillion)
{
	std::vector<int> goodVals;

	std::thread t([&filter, maxVal, &goodVals]
	{
		for (auto i = 0; i <= maxVal; ++i)
		{
			if (filter(i))
				goodVals.push_back(i);
		}
	});

	auto nh = t.native_handle();

	//여기서 t의 네이티브 핸들을 이용해서 t의 우선 순위를 설정한다.

	//조건들이 만족되었다면 t의 완료를 기다린다.
	if (conditionsAreSatisfied())
	{
		t.join();
		performComputation(goodVals); //계산을 수행.
		return true; //계산 수행 ok
	}

	return false; //계산 수행 no
	//만약 t가 join이 안되거나, detach가 안되거나, 예외가 발생해서 여기왔을때 프로그램은 종료된다.
}

//개선
bool doWork2(std::function<bool(int)> filter, int maxVal = telMillion)
{
	std::vector<int> goodVals;

	ThreadRAII t(std::thread([&filter, maxVal, &goodVals]
	{
		for (auto i = 0; i <= maxVal; ++i)
		{
			if (filter(i))
				goodVals.push_back(i);
		}
	}
	), ThreadRAII::DtorAction::join
	);

	auto nh = t.get().native_handle();

	//여기서 t의 네이티브 핸들을 이용해서 t의 우선 순위를 설정한다.

	//조건들이 만족되었다면 t의 완료를 기다린다.
	if (conditionsAreSatisfied())
	{
		t.get().join();
		performComputation(goodVals); //계산을 수행.
		return true; //계산 수행 ok
	}

	return false; //계산 수행 no
				  //만약 t가 join이 안되거나, detach가 안되거나, 예외가 발생해서 여기왔을때 프로그램은 종료된다.
}

void main()
{
}