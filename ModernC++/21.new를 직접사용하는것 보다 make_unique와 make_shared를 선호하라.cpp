#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
using namespace std;

//C++11에서는 make_unique를 지원하지 않으므로 다음과 같이 만들서 사용
template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&&... params)
{
	return std::unique_ptr<T>(new T(std::forward<Ts>(params)...)); //인라인화 된다.
}

//make_unique와 make_shared는 임의의 개수와 형식의 인수들을 받아서 생성자로 완벽전달해서 객체를 동적으로 생성한다.
class Widget
{

};

void processWidget(std::shared_ptr<Widget> spw, int priorlity)
{

}

int computePriority()
{
	return 1;
}

class ReallyBigType {};

int main()
{
	//1.Widget을 두번쓰는 번거로움을 줄일 수 있음.
	auto upw1(std::make_unique<Widget>());
	std::unique_ptr<Widget>upw2(new Widget);

	auto spw1(std::make_shared<Widget>());
	std::shared_ptr<Widget>spw2(new Widget);

	//2.예외 안정성
	processWidget(std::shared_ptr<Widget>(new Widget), computePriority()); //자원 누수의 위험이 있음.

	//누수가 발생하는 이유는 컴파일러가 소스코드를 목적코드로 번역하는 방식과 관련 있다.
	//coputePriority함수는 다른 호출 사이에서 호출되도록 평가될 수 있다.
	//1.new Widget을 실행한다.
	//2.computePriority함수를 실행한다.
	//3.shared_ptr 생성자를 호출한다.
	//new Widget실행후에 computePriority함수가 예외를 던진다면 Widget은 생포인터가 가리키고 있는 동적할당 객체가 된다.
	//shared_ptr을 호출하여 관리되지 못했기 때문에.

	//따라서
	processWidget(std::make_shared<Widget>(), computePriority()); //자원 누수의 위험이 없음.

	//3.make_shared를 사용하면 컴파일러가 좀 더 간결한 자료구조를 사용하는 더 작고 빠른 코드를 산출한다.
	std::shared_ptr<Widget> spw(new Widget); //두번에 메모리 할당 (객체 + 제어블록)

	auto spw19 = std::make_shared<Widget>(); //make_shared가 (객체 + 제어블록)를 한번에 담을 수 있는 크기의 메모리 조각을 한번에 할당한다.
											 //메모리 할당을 한번만 호출하므로 프로그램의 정적 크기가 줄어든다. 코더의 속도도 빨라진다.
											 //제어 블록에 일정 정도의 내부 관리용 정보를 포함할 필요가 없어져서 프로그램의 메모리 사용량도 준다.

	//4.사용이 불가능한 경우.
	auto widgetDeleter = [](Widget* widget)
	{
		delete widget;
	};

	//make함수들은 커스텀 삭제자를 지정할 수 없다
	std::unique_ptr<Widget, decltype(widgetDeleter)> upw20(new Widget, widgetDeleter);
	std::shared_ptr<Widget> spw20(new Widget, widgetDeleter);

	//둘다 값 20 으로 10개할당한다 이는 make함수가 내부적으로
	//완벽전달 할대 괄호를 사용함을 뜻한다.
	//불행한일은 피지칭객체를 중괄호 초기치로 생성하려면 직접 new로 생성해야한다.
	//Widget w{{}}; or Widget w({})
	//vector<int> v1{10,20} 10, 20원소로 갖음.

	auto upv = std::make_unique<std::vector<int>>(10, 20);
	auto spv = std::make_unique<std::vector<int>>(10, 20);

	//중괄호 초기화 우회방법
	auto initList = { 10,20 }; //std::initializer_list객체 생성
	auto spv2 = std::make_shared<std::vector<int>>(initList);

	//make_unique의 경우에는 위 두가지 상황이 문제가 되는 상황의 전부
	
	//클래스 고유 operator new와 operator delete가 있는 형식의 객체를
	//make함수로 생성하는 것은 대체로 바람직 하지 않다.
	//std::allocate_shared가 요구하는 메모리조각의 크기는 동적으로 할당되는
	//객체의 크기가 아니라, 그 크기에 제어블록을 더한것이기 때문이다.
	//객체와 제어블록은 동일한 메모리 조각에 놓인다.

	//객체가 차지하고 있던 메모리 조각은 제어블록이 파괴되기 전까지는 해제될수 없다.
	//결국 shared_ptr과 weak_ptr이 제어블록에서 해제된후 객체가 해제되기 때문에 
	//shared_ptr와 weak_ptr의 파괴 사이의 간격이 길다면, 객체 파괴 시점과 객체가 점유하던 
	//메모리가 해제되는 시점 사이에 시간 지연이 생길 수 있다.
	
	auto pBigObj = std::make_shared<ReallyBigType>(); //아주큰 객체를 생성.
	//1.큰 객체를 가리키는 shared_ptr과 weak_ptr들을 생성해서 사용한다.
	//2.여기서 객체를 가리키는 shared_ptr이 파괴되나 weak_ptr들은 여전히 남아 있다.

	//3.이부분에서 큰객체가 차지하던 메모리는 여전히 할당된 상태이다.
	//4.여기서 객체를 가리키던 마지막 weak_ptr가 파괴된다. 이제 제어 블록과 객체가 차지하던 메모리가 해제된다.

	std::shared_ptr<ReallyBigType> pBigObj2(new ReallyBigType); //new를 이용해서 생성.
	//1.큰 객체를 가리키는 shared_ptr과 weak_ptr들을 생성해서 사용한다.
	//2.여기서 객체를 가리키는 shared_ptr이 파괴되나 weak_ptr들은 여전히 남아 있다. 객체의 메모리는 해제된다.
	
	//3.이부분에서 제어 블록을 위한 메모리만 할당된 상태이다.
	//4.여기서 객체를 가리키는 마지막 weak_ptr가 파괴된다; 이제 제어 블록이 차지하던 메모리가 해제된다.

	//따라서 new를 직접사용하면서도 예외의 안정성 문제를 해결해야한다.
	//방책은 new의 결과를 다른일은 전혀하지 않는 문장에서 스마트 포인터의 생성자에 즉시 넘겨주는 것이다.

	//개별적 문장에 두고 실행한다.
	std::shared_ptr<Widget> s(new Widget, widgetDeleter);
	processWidget(s, computePriority()); //정확 하지만 최적은 아님.
	//shared_ptr이 생포인터의 소유권을 확보하기 때문에
	//s의 생성자가 예외를 던진다고해도 (이를테면 제어블록 동적할당 실패로) new Widget으로 만들어진 포인터에대해
	//삭제자가 확실히 호출되므로 메모리 누수가 발생하지 않는다.

	//왼값을 넘겨주므로 오른값으로 바꺼서 성능확보 가능.
	processWidget(std::move(s), computePriority());

	return 0;
}