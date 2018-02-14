#include <iostream>
#include <memory>
#include <vector>
#include <chrono>

using namespace std;

//std::move std::forward는 실행가능 코드를 전혀 산출하지 않는다.
//그냥 캐스팅을 수행하는 함수 (구체적으로는 템플릿이다)

//std::move는 주어진 인수를 무조건 오른값으로 캐스팅한다.
//std::forward는 특정 조건이 만족될때에만 그런 캐스팅을 한다.

template<typename T>
typename remove_reference<T>::type&& move_11(T&& param)
{
	//왼값이 참조가 들어오면 오른값 참조로 만들기위해서 remove_reference 사용
	//T&, T&& -> T 
	using ReturnType = typename remove_reference<T>::type&&; //T -> T&&가 된다.

	return static_cast<ReturnType>(param);
}

template<typename T>
decltype(auto) move_14(T&& param)
{
	//왼값이 참조가 들어오면 오른값 참조로 만들기위해서 remove_reference 사용
	//T&, T&& -> T  비의존 별칭버전 항목 9참고.
	using ReturnType = remove_reference_t<T>::type&&; //T -> T&&가 된다.

	return static_cast<ReturnType>(param);
}

//오른값인데 이동후보가 아닌경우
class string
{
public:
	string(const string& rhs); //복사 생성자.
	string(string&& rhs);      //이동 생성자.
};

class Annotation
{
public:
	explicit Annotation(const std::string text)
		: value(std::move(text)) //이동이 아니라 복사가된다~!
								 //const std::string&&로 캐스팅된다. 
								 //복사 생성자가 호출된다. const에 대한 왼값 참조를 const 오른값 참조에 묶는것은 허용
	{

	};

private:
	std::string value;
};

//위 예제에서 배울수 있는점
//이동을 지원할 객체는 const로 선언하지 말아야한다.const객체에대한 이동 요청은 소리없이 복사 연산으로 변환된다.
//move가 이동을 해주지는 않는다는 점 &&로 만들뿐. 이동자격을 보장하지 않는다.

//forward
class Widget
{
};

void process(const Widget& lvalArg) { cout << "왼값 함수" << endl; };
void process(Widget&& rvalArg) { cout << "오른값 함수" << endl; };

template<typename T>
void logAndProcess(T&& param)
{
	auto now = std::chrono::system_clock::now(); //현재 시간을 얻는다.

	//param은 하나의 왼값이므로 void process(const Widget& lvalArg)만 실행된다.
	//따라서 여기서 forward를 사용하면 && &인지를 확인해서 캐스팅 한다.
	//매개변수 T에 부호화 되어 있는걸 복원해서 안다고 하네.
	process(std::forward<T>(param));
}

//move의 매력
class Widget2
{
public:
	Widget2(Widget2&& rhs)
		: s(std::move(rhs.s))
	{
		++moveCtorCalls;
	};
private:
	static std::size_t moveCtorCalls;
	std::string s;
};

class Widget3
{
public:
	Widget3(Widget3&& rhs)
		: s(std::forward<std::string>(rhs.s)) //관례에서 벗어난 바람직하지 않은 구현.
		//s(std::forward<std::string&>(rhs.s)) //형식이 std::string& 복사 생성자가 호출된다. 
	{
		++moveCtorCalls;
	};
private:
	static std::size_t moveCtorCalls;
	std::string s;
};

/*
template<class _Ty>
constexpr _Ty&& forward(remove_reference_t<_Ty>& _Arg) _NOEXCEPT
{
	// forward an lvalue as either an lvalue or an rvalue
	return (static_cast<_Ty&&>(_Arg));
}

template<class _Ty>
constexpr _Ty&& forward(remove_reference_t<_Ty>&& _Arg) _NOEXCEPT
{	
	// forward an rvalue as an rvalue
	static_assert(!is_lvalue_reference_v<_Ty>, "bad forward call");
	return (static_cast<_Ty&&>(_Arg));
}
*/

int main()
{
	Widget w;
	logAndProcess(w);
	logAndProcess(std::move(w));

	return 0;
}