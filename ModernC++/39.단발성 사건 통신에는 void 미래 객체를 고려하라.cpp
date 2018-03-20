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

//조건 변수는 한 스레드가 하나 또는 그 이상의 기다리는 스레드를 깨울 수 있도록 하는 변수이다.
//notify_one();
//-해당 조건 변수를 기다리고 있는 스레드 중 한 개의 스레드를 꺠운다.
//notify_all();
//-해당 조건 변수를 기다리고 있는 모든 스레드를 깨운다.

std::condition_variable cv; //사건을 위한 조건 변수
std::mutex m;				//cv와 함께 사용할 뮤텍스

void thread1()
{
	//임계 영역을 연다.
	{
		std::unique_lock<std::mutex> lk(m); //뮤텍스를 잠근다.

		cv.wait(lk); //통지를 기다린다.

	}//임계 영역을 닫는다. lk의 소멸자가 m을 해제 한다.

	//이코드에 문제점
	//1.조건 변수 접근방식에 뮤텍스가 필요하다는것은 설계에 뭔가 문제가 있음을 암시한다.
	//2.만일 반응 과제가 wait를 실행하기 전에 검출 과제가 조건 변수를 통지하면 반응 과제가 멈추게 된다.
	//  반응과제가 wait를 실행하기 전에 검출 과제가 통지를 실행하면 반응과제는 그 통지를 놓지고 영원히 통지를 기다리게 된다.
	//  (wait가 락을 거는 거고 notify_one이 락을 푸는거라고 생각하면된다. 락을 풀고 락을 걸어봤자 의미가 없으므로)
	//3.wait 호출문은 가짜 기상을 고려하지 않는다.
	//  조건 변수를 기다리는 코드가 조건 변수가 통지되지 않았는데도 깨어날 수 있다는 것은 흔히 일어나는 일이다. 이것을 가짜 기상이라고 한다.
	//  가짜 기상을 제대로 처리하려면 기다리던 조건이 정말 발생했는지 체크해야한다.
}

void foo()
{
	auto f1 = std::async(thread1);

	//사건을 검출한다
	cv.notify_one(); //반응 과제에게 알린다.
}


//가짜 기상 체크방법 1
std::atomic<bool> flag(false);

void thread2()
{
	//이방법은 플래그를 체크하면서 점유율을 깍아 먹는다.
	while (!flag)
	{

	}
}

void foo2()
{
	auto f1 = std::async(thread2);

	//사건을 검출한다
	flag = true;
	cv.notify_one(); //반응 과제에게 알린다.
}

//가짜 기상 체크방법 2
bool flag_(false);

void thread3()
{
	{
		std::unique_lock<std::mutex> lk(m); //뮤텍스를 잠근다.

		cv.wait(lk, [] {return flag_;  }); //가짜 기상 방지하기위해 람다를 사용한다.
	}
}

void foo3()
{
	auto f1 = std::async(thread3);

	//사건을 검출한다
	{
		std::lock_guard<std::mutex> g(m);
		flag_ = true;
	}
	
	cv.notify_one(); //반응 과제에게 알린다.
}

//가짜 기상 체크방법 3
//이 설계에는 뮤택스가 필요하지 않으며, 반응 과제가 wait로 대기하기 전에 검출 과제가 자신의 std::promise를 설정해도 작동한다.
//가짜 기상도 없다. wait 호출 후 진정으로 차단 되므로 기다리는 동안 시스템 자원을 전혀 소모하지 않는다.
//다 좋은것 같지만 단발성이고, 공유상태를 만들기 때문에 메모리 동적할당을 요구한다.

std::promise<void> p; //전달할 자료가 없고 단순히 통지 이기 때문에 void를 사용

void thread4()
{
	p.get_future().wait(); //p에 해당하는 미래 객체를 기다린다.
}

void foo4()
{
	auto f1 = std::async(thread4);

	//사건을 검출한다
	p.set_value(); //반응과제에게 통지한다.
}

//단발성이라도 쓸만한다.
std::promise<void> p_;

void rect() {};  //반응 과제에 해당하는 함수.

class ThreadRAII
{
public:
	enum class DtorAction { join, detach };

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

void detect() //검출 과제에 해당하는 함수.
{
	ThreadRAII tr(std::thread ([]
	{
		p_.get_future().wait(); //미래 객체가 설정될 때까지 t를 유보
		rect();
	}), ThreadRAII::DtorAction::join);

	//ThreadRAII를 사용한 이유는 합류 불가능으로 만들기 위해서.

	//여기서 t는 유보된 상태임.
	//(만약여기서 예외가 발생하면 set_value호출이 일어나지 않으므로 람다 스레드는 계속 유보 상태가 된다!)
	//따라서 tr의 소멸자가 결코 완료되지 않으므로 이함수는 멈추게된다.

	p_.set_value(); //t의 유보를 푼다.

	//추가 작업을 수행

}

//이제는 여러개의 반응 과제에 통지한다.
int threadToRun = 10;

void detact2()
{
	auto sf = p.get_future().share(); //sf의 형식은 std::shared_future<void>

	std::vector<std::thread> vt;

	for (int i = 0; i < threadToRun; ++i)
	{
		vt.emplace_back([sf] { sf.wait(); rect(); });
	}

	//만일 여기서 예외가 발생하면 합류가능한 스레드들이 파괴되어서 프로그램이 종료된다.

	p_.set_value();

	//모든 스레드 들을 합류 불가능으로 만든다.
	for (auto& t : vt)
	{
		t.join();
	}

}

void main()
{
}
