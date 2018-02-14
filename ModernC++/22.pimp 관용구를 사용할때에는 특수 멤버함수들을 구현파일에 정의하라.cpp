#include <iostream>
#include <memory>
#include <vector>

using namespace std;

//빌드시간을 줄이는 기법 pimple관용구

class Gadget
{

};
///////////////////////////////////////
//#include "widget.h"
//헤더가 cpp에만 필요하게 된다.
class Widget
{
public:
	Widget();
	~Widget();//선언만 해둔다.

	//소멸자를 정의하면 컴파일러가 이동연산을 정의하지 않으므로 정의해야한다.
	Widget(Widget&& rhs);
	Widget& operator=(Widget&& rhs);

	//unique_ptr같은 이동 전용 형식이 있는 클래스에 대해서는 컴파일러가 복사 연산들을 작성해주지 않으므로 작성
	//작성한다고 해도 얕은 복사. 우리가 원하는것은 포인터가 가르키는 대상까지 복사하는 깊은 복사.
	Widget(const Widget& rhs);
	Widget& operator=(const Widget& rhs);
private:
	struct Impl;
	std::unique_ptr<Impl> pImpl; //생포인터 대신 스마트 포인터를 사용.
};

//widget.cpp///////////////////////////////////
//#include "widget.h"
//#include "gadget.h"

struct Widget::Impl
{
	std::string name;
	std::vector<double> data;
	Gadget g1, g2, g3;
};

Widget::Widget()
	: pImpl(std::make_unique<Impl>())
	//: pImpl(new Impl)
{

}

Widget::Widget(const Widget& rhs)
: pImpl(nullptr)
{
	if (rhs.pImpl)
	{
		pImpl = std::make_unique<Impl>(*rhs.pImpl);
	}
}

Widget& Widget::operator=(const Widget& rhs)
{
	if (!rhs.pImpl)
		pImpl.reset();
	else if (!pImpl)
		pImpl = std::make_unique<Impl>(*rhs.pImpl);
	else
		*pImpl = *rhs.pImpl;

	return *this;
}

Widget::Widget(Widget&& rhs) = default;
Widget& Widget::operator=(Widget&& rhs) = default;

Widget::~Widget() = default;

/*
//w가 정의가 필요해지므로 정의만 해놓는다.
Widget::~Widget()
{
}
*/

/*
Widget::~Widget()
{
	delete pImpl;
}
*/
////////////////////////////////////////

//#include "widget.h"
int main()
{
	//이 오류는 w가 파괴는되는 지점에 대해서 컴파일러가 작성하는 코드에서 발생한다.
	//w의 소멸자가 호출되는데 unique_ptr을 이용하는 Widget 클래스는 소멸자가 따로 없다(소멸자가 따로 할일이 없기 때문에)
	//이 경우 컴파일러가 대신 소멸자를 생성해준다. 컴파일러는 pImpl의 소멸자를 호출하는 코드를 삽입한다.
	//pImpl 기본삭제자를 사용하는 unique_ptr이고 기본삭제자는 생포인터를 delete하는 함수이다.
	//그런데 대부분의 표준 라이브러리 구현들에서 그 삭제자 함수를 delete를 적용하기 전에 생포인터가 불완전한 형식을 가리키는지를
	//static_assert를 이용해서 점검한다. 이때 참이 아닌것이 판정되면 오류를 발생한다.
	//이는 컴파일러가 작성하는 다른 모든 특수 멤버함수 처럼 Widget의 소멸자는 암묵적으로 inline이기 때문이다. (인라인에는 구현부도 필요해지므로)
	//따라서 소멸자를 정의만 해둔다.
	Widget w; //오류!

	//shared_ptr을 사용하여 pimple을 구현할 경우 위에서 조언한 내용이 더 이상 적용되지 않는다.
	//그런 경우 Widget에서 소멸자를 선언할 필요가 없고, 사용자 소멸자가 없음으로 컴파일러가 소멸자를 작성한다.
	//그리고 이동연산들은 우리가 원하는 방식으로 작동한다.
	//이유는 unique_ptr에서 삭제자 형식은 스마트 포인터 형식의 일부분이며, 이덕분에 속도가 빠르다.
	//이런한 유연성이 빚어내는 결과는 컴파일러가 작성한 특수 멤버 함수가 쓰이는 시점에서 피지칭 형식들이 완전한 형식이어야 한다는것이다.
	//(컴파일할때 소멸자형식도 컴파일해야하니깐 이시점에 필요해지는듯)

	//반면 shared_ptr은 삭제자의 형식의 스마트포인터 형식의 일부가 아니다. 이때문에 더 느리지만, 컴파일러가 작성하는 멤버함수들이 쓰이는
	//시점에서 피지칭 형식들이 완전한 형식이어야 한다는 제약이 사라진다.
	return 0;
}