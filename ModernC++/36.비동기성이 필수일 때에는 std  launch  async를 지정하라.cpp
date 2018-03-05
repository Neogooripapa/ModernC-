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

void f()
{
}

void foo()
{
	//std::async 호출은 함수를 어떤 시동방침에 따라 실행한다는 일반적인 의미를 가진다.	
	//std::launch::async 시동방침을 지정하면 함수f는 반드시 비동기적으로, 다른 스레드에서 실행된다.
	//std::launch::deferred 시동방침을 지정하면 함수f는 std::async가 돌려준 미래객체에 대해 get이나 wait가
	//호출될 때에만 실행될 수 있다. 다시 말해 f는 그러한 호출이 일어날때 까지 지연된다.
	//get이나 wait가 호출되면 f는 동기적으로 실행된다. get이나 wait가 호출되지 않으면 f는 실행되지 않는다.

	//두호출의 의미는 같다.
	auto fut1 = std::async(f);
	auto fut2 = std::async(std::launch::async | std::launch::deferred, f);
}

using namespace std::literals;

void ff()
{
	std::this_thread::sleep_for(1s); //1초 수면.
}

void foo2()
{
	//기본 시동 방침의 스케줄링 유연성이 만료시간이 있는 wait기반 루프에도 영향을 미친다.
	//지연된 과제에 대해 waif_for나 wait_until을 호출하면 std::future_status_deferred라는 값이 반환된기 때문에.
	//이문제 때문에 언제가는 끝날것 처럼 보이는 루프가 실제로는 무한 루프가 될 수 있다.
	auto fut = std::async(ff);

	//과제가 지연되었으면.
	if (fut.wait_for(0s) == std::_Future_status::deferred)
	{
		//fut에 wait나 get을 적용해서 f를 동기적으로 호출한다.
	}
	else
	{
		//f의 실행이 끝날 때까지 루프를 돈다.
		//f가 지연 된다면, fut.wait_for는 항상 std::future_status_deferred 를 돌려준다. 그값은 std::future_status::ready와
		//절대 같지 않으므로 무한루프가 된다.
		while (fut.wait_for(100ms) != std::future_status::ready)
		{

		}

		//fut가 준비되었다.
	}

	//어떤 과제에 대해 기본 시동방침과 함께 std::async를 사용하는 것은 다음 조건이 성립할 때에만 적합하다.
	//1.과제가 get이나 wait를 호출하는 스레드와 반드시 동시적으로 실행되어야 하는것은 아니다.
	//2.여러 스레드중 어떤 스레드의 thread_local변수들을 읽고 쓰는지가 중요하지 않다.
	//3.std::async에 돌려준 미래객체에 대해 get이나 wait가 반드시 호출된다는 보장이 있거나, 과제가 전혀 실행되지 않아도 괜찮다.
	//4.과제가 지연된 상태일 수도 있다는 점이 waif_for이나 wait_untild을 사용하는 코드에 반영되어 있다.
}

template<typename F, typename... Ts>
inline auto reallyAsync(F&& f, Ts&&... params)
{
	return std::async(std::launch::async, std::forward<F>(f), std::forward<Ts>(params)...);
}

void foo3()
{
	//위 조건에 성립하지 않으면 주어진 과제를 진정으로 비동기적으로 실행하도록 강제할 필요가 있다.
	auto fut = std::async(std::launch::async, f);

	//std::launch::async 버전 템플릿
	auto fut2 = reallyAsync(f);
}

void main()
{
}