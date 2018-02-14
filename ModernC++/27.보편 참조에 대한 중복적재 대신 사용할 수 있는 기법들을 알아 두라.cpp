#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <chrono>
using namespace std;

//보편 참조에 중복적재 함수를 사용할때 문제가 생기는것을 피하는 방법을 논의한다.
//(엉뚱하게 보편참조 함수가 불린다거나~ 26항목 참고)

//1.꼬리표 배분을 사용한다.
std::multiset<std::string> names;

//이함수를 int로 호출할 경우 안에서 string을 원하므로 오류가 발생한다.
template<typename T>
void logAndAdd(T&& name)
{
	auto now = std::chrono::system_clock::now();
	//로그를 기록한다.
	names.emplace(std::forward<T>(name));
}


//따라서 함수안에서 구분하는 버전을 추가해보자. (true_type, false_type가 꼬리표가 된다)
template<typename T>
void logAndAddImpl(T&& name, std::false_type)
{
	auto now = std::chrono::system_clock::now();
	//로그를 기록한다.
	names.emplace(std::forward<T>(name));
}

string nameFromIdx(int idx) 
{
	return "TEST"; 
}

void logAndAddImpl(int idx, std::true_type)
{
	names.emplace(nameFromIdx(idx));
}

template<typename T>
void logAndAdd2(T&& name)
{
	//int왼값이 들어 오면 T는 int&로 연역하는데 이는 정수형식이 아니다. 따라서 이건 못써 먹는다.
	//logAndAddImpl(std::forward<T>(name), std::is_integral<T>()); 
	 
	logAndAddImpl(std::forward<T>(name), std::is_integral<std::remove_reference_t<T>::type>());
}

//2.보편 참조를 받는 템플릿을 제한한다. 
//생성자에서 꼬리표 배분을 사용해도 컴파일러가 생성하는 함수에 의해서 꼬리표 배분이 적용되지 않을 위험이 있다.
//std::enable_if를 이용하면 컴파일러가 마치 특정 템플릿이 존재하지 않는 것처럼 행동하게 만들 수 있다. 이를 비활성된 템플릿이라고 부른다.
//std::enable_if를 사용하는 템플릿은 오직 그 std::enable_if에 지정된 조건이 만족될 때에만 활성화 된다.
//std::is_same 두형식이 같은지 판정
//std::decay 모든 참조 한정사와, const, volatile을 제거

class Person
{
public:
	//이 설계는 Person을 다른 어떤 Person으로 생성하는 경우, 그것이 왼값이든, 오른값이든
	//const이든 비const이든, volatile이든 비volatile이든 보편참조를 받는 생성자는 절대 호출되지 않는다.
	//(Person의 이동 생성자가 호출되기를 기대했는데 보편이 실행되서 여기서 막는것임)
	template<typename T,
		//typename = typename std::enable_if<!std::is_same<Person, typename std::decay<T>::type>::value>::type> (요거는 파생 클래스까지 커버가 안됨)
		//typename = typename std::enable_if<!std::is_base_of<Person, typename std::decay<T>::type>::value>::type> //파생 클래스까지 커버됨.
		//typename = std::enable_if_t<!std::is_base_of<Person, std::decay_t<T>::type>::value>> //C++14별칭 버전.
		typename = std::enable_if_t<!std::is_base_of<Person, std::decay_t<T>::type>::value //정수인수와 비정수 인수를 구분 추가.
		&& !std::is_integral<std::remove_reference<T>>::value
		>
	>
	explicit Person(T&& n)
		: name(std::forward<T>(n))
	{
	}

	//이제 정수면 이녀석이 불리겠징.
	explicit Person(int idx)
		: name(nameFromIdx(idx))
	{
	}

	//컴파일러가 작성함.
	//Person(const Person& rhs);
	//Person(Person&& rhs);
private:
	std::string name;
};

class SpecialPerson : public Person
{
public:
	SpecialPerson(const SpecialPerson& rhs)
		:Person(rhs) //완벽 전달 생성자를 호출.
	{
	}
	SpecialPerson(SpecialPerson&& rhs)
		:Person(std::move(rhs)) //완벽 전달 생성자를 호출.
	{
	}
};

//3.완벽 전달의 단점 (오류 메시지가 난해하다)
//완벽 전달에 기초한 접근 방식에서는 컴파일러가 const char16_t 들의 배열을 아무 불평없이 생성자의 매개변수에 묶는다.
//생성자는 그 매개변수를 Person의 string자료 멤버의 생성자에 전달하며, 그때야 호출자가 전달한 것 (const char16_t배열)과
//필요한 것(string 생성자가 받아들일 수 있는 임의의형식) 사이의 불일치가 발견된다.
//그래서 오류 메시지가 겁나 많고 모호하다. 보편 참조가 전달되는 횟수가 많을 수록 뭔가 더 장황한 메시지가 나온다.
//Person p(u"Kor");//"Kor"는 const char16_t 형식의 문자들로 이루어짐.


class Person1
{
public:
	template<typename T,
		typename = std::enable_if_t<!std::is_base_of<Person1, std::decay_t<T>::type>::value
		&& !std::is_integral<std::remove_reference<T>>::value
		>
	>
		explicit Person1(T&& n)
		: name(std::forward<T>(n))
	{
		//이경우 보편 참조 매개 변수가 std::string에 대한 초기치로 쓰일 것을 알고 있으므로,
		//그런 초기치로 사용하는것이 가능한지 점검하는 방법도 있다.
		static_assert(std::is_constructible<std::string, T>::value,
			"Parameter n can't be used to construct a std::string"
			);
		//안타 깝게도, 이 예에서 static_assert는 생성자의 본문에 있지만, 매개변수를 전달하는 코드는 그보다 앞에 있다.
		//따라서 에러매시지 열라 나오고 위에 어썰트 메시지가 나온다 -_-;
	}
private:
	std::string name;
};

int main()
{
	return 0;
}