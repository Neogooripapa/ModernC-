#include <iostream>
#include <memory>
#include <vector>
#include <list>
#include <functional>
#include <unordered_map>
#include <atomic>
#include <thread>
#include <mutex>
using namespace std;

//std::atomic<int>, std::atomic<bool>, std::atomic<Widget*>은 다른 스레드들이 반드시 원자적으로 인식하는 연산들을 제공한다.
//atomic객체가 성공적으로 생성되고나면, 그 객체에 대한 연산은 마치 뮤텍스로 보호되는 임계 영역 안에서 수행되는 것처럼 동작한다.

void foo()
{
	std::atomic<int> ai(0);

	ai = 10; //원자적으로 10을 설정한다.

	//원자적으로 값을 읽는다.
	cout << ai; //원자적인 부분은 ai를 읽는부분만!

	//원자적 증감소.
	++ai; 
	--ai;

	//일단 아토믹으로 객체가 생성되고 나면, 그 객체에 대한 모든 멤버 함수는, 심지어 raw연산들을 수행하는 멤버 함수도
	//다른 스레드에게 반드시 원자적으로 보인다.


	//반면 볼라틸을 사용하는 코드는 다중 스레드 문맥에서 거의 아무것도 보장하지 않는다.
	volatile int vi(0);

	vi = 10; 

	cout << vi; 

	++vi;
	--vi;
}

//코드 재배치
//독립적인 변수인 경우 컴파일러가 재비치를 할 수 있다.
//x = y; a = b;
//a = b; x = y;

int computeImportantValue() {};

void foo2()
{
	//이문장에서 재배치가 일어나면 망한다.
	//하지만 아토믹을 사용하면 재배치 제약이 생긴다.
	//아토믹 전에 나온 문장은 아토믹 이후에 절대 실행되지 않는다.

	std::atomic<bool> valAvaliable(false);

	auto imptValue = computeImportantValue();

	valAvaliable = true;

	//볼라틸의 경우 컴파일러가 재배치할 수 있다.
	volatile bool valAvaliable(false);

	auto imptValue = computeImportantValue();

	valAvaliable = true;
}

//볼리틸의 장점
void foo3()
{
	int x = 0;
	auto y = x;
	y = x;
	x = 10;
	x = 20;

	//컴파일러는 다음과 같은 최적화를 수행할 수 있다.
	//auto y = x;
	//x = 20;

	//볼라틸은 이 메모리 연산에 대해 그 어떤 최적화도 수행하지 말라는 지시를 한다.
	

	//아토믹은 복사 연산을 지원하지 않는다.
	//a 에서 aa로 복사 하려는것이 원자적이려면 컴파일러는 읽고 기록하는 작업을 하나의 원자적 코드로 산출해야한다.
	//하지만 그런 원자적 연산을 대체로 하드웨어에서 지원하지 않는다.
	std::atomic<int> a;
	//auto aa = a; //오류!
	//aa = a;		//오류!

	//이것이 컴파일 되긴하지만 두문장에 하나의 원자적 연산으로 실행되리라고 기대할 수 없다.
	std::atomic<int> aa(a.load()); //읽는다.
	aa.store(a.load()); //다시 읽는다.

	a = 10;
	a = 20;

	//레지스터 = a.load();
	//std::atomic<int> aa(레지스터);
	//y.store(레지스터); 로 최적화 할 수도 있다.

	volatile std::atomic<int> val; //원자적이며, 최적화에 의해 제거될 수 없다.
}

void main()
{
}