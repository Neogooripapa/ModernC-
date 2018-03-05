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

void main()
{
	//std::async 호출은 함수를 어떤 시동방침에 따라 실행한다는 일반적인 의미를 가진다.	
	//std::launch::async 시동방침을 지정하면 함수f는 반드시 비동기적으로, 다른 스레드에서 실행된다.
	//std::launch::deferred 시동방침을 지정하면 함수f는 std::async가 돌려준 미래객체에 대해 get이나 wait가
	//호출될 때에만 실행될 수 있다. 다시 말해 f는 그러한 호출이 일어날때 까지 지연된다.
	//get이나 wait가 호출되면 f는 동기적으로 실행된다. get이나 wait가 호출되지 않으면 f는 실행되지 않는다.


}