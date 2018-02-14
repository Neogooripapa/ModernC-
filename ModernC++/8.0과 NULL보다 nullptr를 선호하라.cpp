#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include <unordered_map>
#include <atomic>
#include <thread>
#include <mutex>
using namespace std;

void f(int) {};
void f(bool) {};
void f(void*) {};

class Widget
{

};

int f1(std::shared_ptr<Widget> spw) { return 0; };
double f2(std::unique_ptr<Widget> upw) { return 0.1f; };
bool f3(Widget* pw) { return true; };


template<typename FuncType, typename MuxType, typename PtrType>
decltype(auto) LockAndCall(FuncType func, MuxType& mutex, PtrType ptr)
{
	using MuxGuard = std::lock_guard<std::mutex>;

	MuxGuard g(mutex);
	return func(ptr);
}
int main()
{
	f(0); //int함수 호출
	f(NULL); //int함수 호출
	f(nullptr); //void*함수 호출
	
	std::mutex f1m, f2m, f3m;

	using MuxGuard = std::lock_guard<std::mutex>;

	{
		MuxGuard g(f1m);
		auto result = f1(0);
	}

	{
		MuxGuard g(f2m);
		auto result = f2(NULL);
	}

	{
		MuxGuard g(f3m);
		auto result = f3(nullptr);
	}

	//auto result1 = LockAndCall(f1, f1m, 0); //오류! 0은 int로 연역 std::shared_ptr<Widget>와 다름.
	//auto result2 = LockAndCall(f2, f2m, NULL); //오류! 마찬가지
	auto result3 = LockAndCall(f1, f3m, nullptr); //std::nullptr_t로 암묵적 변환이 일어난다.
	auto result4 = LockAndCall(f2, f3m, nullptr); //std::nullptr_t는 암묵적으로 모든 포인터 형식으로 변환된다.
	auto result5 = LockAndCall(f3, f3m, nullptr); //Widget*로 해석됨.
	
	return 0;
}