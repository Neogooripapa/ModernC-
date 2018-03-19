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

//호출자(미래객체) <--- 피호출자의 결과(공유 상태) <--- 피호출자(보통은 std::promise)

//미래 객체 소멸자의 행동을 그 미래 객체와 연관된 공유 상태가 결정한다.
//1.std::async를 통해서 시동된 비지연 과제에 대한 공유 상태를 참조하는 마지막 미래 객체의 소멸자는
//  과제가 완료될 때까지 차단된다.
//2.다른 미래 객체의 소멸자는 그냥 해당 미래 객체를 파괴한다.

//미래 객체의 소멸자는 바탕스레드를 그 무엇과도 합류(join)시키지 않으며, 그 무엇 으로부터도 탈착하지 않으며(detach)
//그 무엇도 실행하지 않는다. 참조하는 공유 상태안의 참조 횟수를 감소한다.

//정상 행동에 대한 예외는 다음 조건들을 모두 만족하는 미래 객체에 대해서만 일어난다.
//1.미래 객체가 std::async 호출에 의해 생성된 공유 상태를 참조한다.
//2.과제의 시동 방침이 std::launch::async이다. (실행시점 시스템이 선택한 경우와, std::async 호출시 명시적으로 지정한 경우 모두)
//3.미래 객체가 공유상태를 참조하는 마지막 미래 객체이다.
//이 모든 조건이 성립할 때에만 비동기적으로 실행되는 과제가 완료될 때까지 소멸자의 실행이 차단되는 것이다.
//이는 std::async로 생성한 과제를 실행하는 스레드에 대해 암무적인 join을 호출하는 것에 해당한다.

//미래 객체에 대한 API는 주어진 미래 객체가 std::async 호출에 의해 생긴 공유 상태를 참조하는지를 판단할 수단을 제공하지 않는다.
//따라서 임의의 미래 객체에 대해 그 소멸자가 비동기적으로 실행되는 과제의 완료를 기다리느라 차단될 것인지를 알아내는 것은 불가능하다.


//이 컨테이너는 소멸자에서 차단 될 수도 있다.
//컨테이너에 담긴 하나 이상의 미래 객체들이 std::async를 통해 시동된 비지연 과자에
//대한 공유 상태를 참조할 수 도 있기 때문이다.
std::vector<std::future<void>> futs;

//Widget 객체의 소멸자가 차단될 수 도 있다.
class Widget
{
public:

private:
	std::shared_future<double> fut;
};

//여러 원인으로도 공유 상태가 생성될 수 있다.
//그중 하나가 std::packaged_task의 사용이다.
//std::packaged_task 객체는 주어진 함수(또는 호출가능 객체)를 비동기적으로 실행할 수 있도록 포장한다.
//포장된 함수의 실행 결과는 공유 상태에 저장된다.

int calcValue() { return 0; }

void foo()
{
	std::packaged_task<int()> pt(calcValue); //비동기적 실행을 위해 calcValue를 포장한다.

	//이 경우에는 미래 객체가 std::async 호출로 만들어진 공유 상태를 참조하지 않음이 명확하므로, 해당 소멸자가 정상동작 한다.
	auto fut = pt.get_future(); //pt에 대한 미래 객체를 얻는다.

	//성공적으로 생성한 std::packaged_task 객체는 임의의 스레드에서 사용할 수 있다.
	std::thread t(std::move(pt)); //복사가 안되므로 이동

	//여기서 일어날 3가지 해동
	//1.t에 아무 일도 일어나지 않는다. t는 합류 가능 스레드이므로 프로그램이 종료된다.
	//2.t에대해 join을 수행한다. join이 된다면 소멸자에서 차단될 이유가 없다.
	//3.t에대해 detach를 수행한다. detach를 수행한다면 fut 소멸자에서 그것을 수행할 필요가 없다.

	//다른 말로 하자면 std::packaged_task에 의해 만들어진 공유 상태를 참조하는 미래 객체가 있다면
	//소멸자의 특별한 행동을 고려한 코드를 작성할 필요가 없다.
}

void main()
{
	
}