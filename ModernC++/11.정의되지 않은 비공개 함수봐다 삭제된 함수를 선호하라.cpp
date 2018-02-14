#include <iostream>
#include <memory>
#include <vector>
#include <list>
using namespace std;

//유저가 사용못하게 함, 프렌드 클래스에서 호출하려고하면 정의가 없으므로 링크 실패.
class foo
{
private:
	foo(const foo&);			//not defined
	foo& operator=(const foo&); //not defined
};

//C++11 삭제된 함수
//멤버 변수나, friend함수에서 객체를 복사하려 하면 컴파일에 실패한다.
//링크 시점을 컴파일 시점으로 가져오는 장점
class foo2
{
public: //퍼블릭으로 하는것이 관례 private으로 인한 오류를 보지 않기 위해서.
	foo2(const foo2&) = delete;
	foo2& operator=(const foo2&) = delete;
};

//삭제된 함수는 그어떤 함수도 삭제 할 수 있다.
bool isLucky(int number) { return true; }

bool isLucky(char) = delete;
bool isLucky(bool) = delete;
bool isLucky(double) = delete; //double과 float을 모두 배재.

//삭제된 함수는 템플릿 인스턴스화를 방지한다.
template<typename T>
void processPointer(T* ptr)
{
}

template<>
void processPointer<void>(void*) = delete;
template<>
void processPointer<char>(char*) = delete;

template<>
void processPointer<const void>(const void*) = delete;
template<>
void processPointer<const char>(const char*) = delete;

//템플릿의 일부 인스턴스화를 방지하려는 목적으로 private선언을 적용할 수 없다.
class Widget
{
public:
	template<typename T>
	void processPointer(T* ptr) {};

private:
	template<>
	void processPointer<void>(void* ptr) {}; //컴파일 오류! 위에 주체가되는 녀석이랑 접근성이다를 수가 없다.
										  //템플릿 특수화는 반드시 클래스 범위가 아니라 이름 공간 범위에서 작성해야한다.
};

class Widget2
{
public:
	template<typename T>
	void processPointer(T* ptr) {};
};

template<>
void Widget2::processPointer<void>(void*) = delete; //퍼블릭이지만 삭제되었음.


int main()
{
	//if (isLucky('a'))
	{

	}

	//if (isLucky(true))
	{

	}

	//if (isLucky(3.5))
	{

	}

	return 0;
}