#include <iostream>
#include <memory>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
using namespace std;

//C++11 에서는 이동생성자, 이동 배정 연산자가 특수 멤버 함수들로 추가되었다.
//필요할 때에만 작성된다.
//비정적 멤버들에 대해 "멤버별 이동"을 수행한다.
//이동연산은 멤버별 이동 요청에 가깝다.C++98 유물 클래스들 처럼 이동 연산에대한 지원을 제공하지 않으면 
//그 형식이 제공하는 복사 연산을 통해서 이동된다.
//정리하면 이동연산을 지원하지 않으면 복사연산을 수행한다.

//이동생성자와 이동 배정 연산자는 독립적이지 않기 때문에 하나를 작성하면 다른 하나를 컴파일러가 작성하지 않는다.
//복사 연산(생성또는 배정)을 하나라도 명시적으로 선언한 클래스에 대해서는 이동 연산들이 작성되지 않는다.

//이동연산(생성또는 배정)을 하나라도 명시적으로 선언하면 컴파일러는 복사 연산들을 비활성화한다.

//3의법칙
//소멸자가 있으면 복사 생성자 및 배정, 이동생성자 및 배정을 정의해야한다.
//소멸자가 있다는것은 메모리관리가 컴파일러가 만드는 함수에의해서 관리 될리가 없음므로
//따라서 C++11에서는 소멸자가 있으면 이동 생성 및 배정을 컴파일러가 작성하지 않는다.

//다음규칙에 의거하여 컴파일러는 이동연산 함수를 작성한다
//클래스에 그 어떤 복사 연산도 선언되어 있지 않다
//클래스에 그 어떤 이동 연산도 선언되어 있지 않다
//클래스에 소멸자가 선언되어 있지 않다.

class Widget
{
public:
	Widget(Widget&& rhs) {};

	Widget& operator=(Widget&& rhs) {};
};

class Base
{
public:
	//소멸자를 직접 선언하면 이동 연산들의 자동 작성이 금지되지만 디폴트를 사용하면 지원하게 만들 수 있다.
	virtual ~Base() = default; //만약 자식 클래스가 있다면 소멸자가 버추얼이므로 자식에 소멸자 정의도 필요해 지므로 의미가 있다.

	Base(Base&&) = default; //이동지원
	Base& operator=(Base&&) = default;

	Base(const Base&) = default; //복사 지원
	Base& operator=(const Base&) = default;
};

//멤버함수 템플릿이 존재하면 특수멤버 함수의 자동 작성은 유효하다.
class Widget2
{
public:
	template<typename T>
	Widget2(const T& rhs);			//어떤 것으로도 Widget2을 생성

	template<typename T>
	Widget2& operator=(const T& rhs); //어떤 것으로도 Widget2을 배정
};

int main()
{	
	

	return 0;
}