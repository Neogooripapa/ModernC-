#include <iostream>
#include <memory>
#include <vector>
using namespace std;

class Widget
{
public:
	Widget() {};

	~Widget()
	{
		cout << "삭제";
	};
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////
class Widget2;
class Widget3;

std::vector<std::shared_ptr<Widget2>> processedWidgets;
std::vector<std::shared_ptr<Widget3>> processedWidgets2;

class Widget2
{
public:
	void process()
	{
		//멤버함수 바깥에서 this를 가리키는 다른 shared_ptr이 있다면 여기서 제어블록이 새로 만들어지므로
		//미정의 동작이 일어난다.
		processedWidgets.emplace_back(this); //위젯을 처리된 위젯 목록에 추가 (잘못된 방식임)
	};
};

//shared_ptr로 관리하는 클래스를 작성 할때, 그 클래스의 this포인터로 부터 shared_ptr을 안전하게 생성하는 방법.
//enable_shared_from_this는 현재 객체를 가리키는 shared_ptr를 생성하되 제어블록을 복제하지 않는 멤버 함수를 하나 정의한다.
//shared_from_this가 그것이다. this를 가리킴.
class Widget3 : public std::enable_shared_from_this<Widget3>
{
private:
	//생성자들..
public:
	void process()
	{
		//shared_from_this는 현재 객체에대한 제어블록을 조회하고 그 제어블록을 지칭하는 shared_ptr을 생성한다.
		//현재 객체의 제어블록이 있다고 가정하고 동작한다. 그런 shared_ptr이 존재하지 않는다면 함수의 행동은 정의되지 않는다.

		//shared_ptr가 유효한 객체를 가리키기도 전에 클라이언트가 shared_from_this를 호출하는 일을 방지하기 위해서
		//생성자를 private으로 선언한다.

		//그리고 클라이언트가 객체를 생성할 수 있도록 std::shared_ptr을 돌려주는 팩터리 함수를 제공한다.
		processedWidgets2.emplace_back(shared_from_this());
	};

	template<typename... Ts>
	static std::shared_ptr<Widget3> create(Ts&&... params)
	{
		return std::shared_ptr<Widget3>(params);
	};
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	//1.shared_ptr의 크기는 생포인터의 두배이다. (포인터+참조횟수)
	//2.참조 횟수를 담을 메모리를 반드시 동적으로 할당해야 한다 (객체는 참조횟수를 모른다)
	//3.참조 횟수의 증가와 감소가 반드시 원자적인 연산이어야 한다. (스레드)


	//참조 횟수가 증가하지 않는 경우. (이동생성)
	//기존 shared_ptr을 이동해서 새 shared_ptr을 생성하면 원본 shared_ptr은 null이된다.


	//삭제자 지정.
	auto loggingDel = [](Widget* pw)
	{
		cout << "삭제자 호출" << endl;
		delete pw;
	};


	std::unique_ptr<Widget, decltype(loggingDel)> upw(new Widget, loggingDel); //삭제자 형식이 포인터 형식의 일부임.
	std::shared_ptr<Widget> spw(new Widget, loggingDel); //삭제자 형식이 포인터 형식의 일부가 아님.

	//이는 컨테이너에 담을때 유연하다, 함수 인자로 넘길때도~
	auto customDelete1 = [](Widget* pw) {};
	auto customDelete2 = [](Widget* pw) {};

	std::shared_ptr<Widget> pw1(new Widget, customDelete1);
	std::shared_ptr<Widget> pw2(new Widget, customDelete2);

	//유니크 ptr은 형식 자체에 람다가 들어가므로 밑에 연산이 안됨.
	std::vector<std::shared_ptr<Widget>> vpw{ pw1, pw2 };

	//unique_ptr와의 다른 차이점은, 커스텀 삭제자를 지정해도 shared_ptr 객체의 크기가 변하지 않는다.
	//삭제자와 무관하게 shared_ptr 객체의 크기는 항상 포인터 두개 분량이다.
	//추가 메모리는 힙에 생성되기 때문에. 제어블록(참조횟수 + 삭제자 복사본) 여기서 가져다 씀.
	
	//생포인터로 여러 개의 shared_ptr을 생성하면 피지칭 객채에 대해 여러 개의 제어 블록이 만들어지므로
	//미정의 행동이 된다.
	auto pw = new Widget;
	
	std::shared_ptr<Widget> spw1(pw, loggingDel); //*pw에 대한 제어 블록이 생성됨.
	std::shared_ptr<Widget> spw2(pw, loggingDel); //*pw에 대한 두번째 제어 블록이 생성됨.
	//따라서 생성자에 생포인터를 넘겨주는 일을 피해야 한다. (make_shared)를 사용
	std::shared_ptr<Widget> spw3(new Widget, loggingDel); //직접 new를 사용 굳.
	std::shared_ptr<Widget> spw4(spw2); //spw3과 spw4는 동일한 제어블록을 사용한다.


	//생포인터와 this포인터로 빚어지는 사태.(상단 참조)

	//shared_ptr => unique_ptr은 불가능하므로 쉐어드를 쓰려면 고심.
	//shared_ptr로는 배열을 관리할 수 없다. 단일 객체를 가리키는 포인터만 염두하고 설계되어 있음.
	
	return 0;
}