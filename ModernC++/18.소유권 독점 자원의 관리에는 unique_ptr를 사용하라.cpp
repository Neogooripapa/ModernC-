#include <iostream>
#include <memory>
#include <vector>
using namespace std;

class Investment 
{
public:
	virtual ~Investment() {};
};

class Stock : public Investment
{
public:
	virtual ~Stock() {};
};

class Bond : public Investment
{
public:
	virtual ~Bond() {};
};

class RealEstate : public Investment
{
public:
	virtual ~RealEstate() {};
};


//커스텀 삭제 함수를 등록할 수 있다.
auto delInvmt = [](Investment* pInvestment)
{
	std::cout << "삭제 로그" << endl;
	delete pInvestment;
};

template<typename... Ts>
//std::unique_ptr <Investment, decltype(delInvmt)> makeInvestment(Ts&&... params)
auto makeInvestment(Ts&&... params)
{
	std::unique_ptr<Investment, decltype(delInvmt)> pInv(nullptr, delInvmt);

	//new로 얻은 생포인터를 배정하는 문장은 컴파일 되지 않으므로 reset을 사용
	//생포인터에서 스마트 포인터 암무적변환이 성립되면 문제가 생기기때문에 막아놓은듯.
	if (1)
	{
		pInv.reset(new Stock(std::forward<Ts>(params)...));
	}
	else if (1)
	{
		pInv.reset(new Bond(std::forward<Ts>(params)...));
	}
	else if (1)
	{
		pInv.reset(new RealEstate(std::forward<Ts>(params)...));
	}

	return pInv;
}

//일반적으로 생 포인터와 스마트포인터 크기가 같다고 가정한다.
//그러나 함수포인터를 삭제자로 지정한경우에는 std::unique_ptr크기가 1워드에서 2워드로 증가한다.
//상태없는 함수 객체(캡처가 없는 람다)의 경우에는 크기 변화가 없다. 
//람다는 객체이기때문에 이름(타입)을 알면 스마트포인터에서 만들어서 호출하므로 가르킬 필요가 없다.
//캡처가 있는경우에는 캡처된 객체들의 값을 알아야 객체를 만들수 있으므로 크기가 커지게 된다.

//배열용 std::unique_ptr<T[]>은 그냥 알고만 있자.

std::vector<unique_ptr<Investment, decltype(delInvmt)>> g_vec;

void foo()
{
	unique_ptr <Investment, decltype(delInvmt)> ptr = makeInvestment();

	g_vec.push_back(std::move(ptr)); //여기서 가로채면 컨테이너가 삭제되는 시점에 포인터가 삭제된다.

	//g_vec.push_back(makeInvestment());
	cout << "함수호출 끝" << endl;
}

//유니크를 쉐어드로 변환 (쉽고 효율적이다)
std::shared_ptr<Investment> sp = makeInvestment();

int main()
{
	//파괴되지 않는경우
	//1.예외가 스레드의 주함수 바깥으로까지 전파되거나
	//2.noexcept 명세가 위반되면
	//3.std::abort나 어떤 종료함수 std::_Exit, std::exit, std::quick_exit가 호출되면
	foo();

	cout << "파괴" << endl;

	return 0;
}