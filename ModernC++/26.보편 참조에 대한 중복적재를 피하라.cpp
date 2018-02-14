#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <chrono>
using namespace std;

std::multiset<std::string> names;

void logAndAdd(const std::string& name)
{
	auto now = std::chrono::system_clock::now();

	//로그를 기록한다.

	names.emplace(name);
}

//개선 버전
template<typename T>
void logAndAdd2(T&& name)
{
	auto now = std::chrono::system_clock::now();

	//로그를 기록한다.

	names.emplace(std::forward<T>(name));
}

//문자열 형태의 이름을 직접 얻지 못하는 클라이언트가 있다.
//그런 클라이언트는 원하는 이름을 얻는 색인만 가지고 있는 경우로 가정하고
std::string nameFromIdx(int idx)
{
	return "";
}

void logAndAdd2(int idx)
{
	auto now = std::chrono::system_clock::now();

	//로그를 기록한다.

	names.emplace(nameFromIdx(idx));
}

//색인을 받는 자유함수 대신 그와 같은 일을 하는 생성자를 가진 클래스를 도입하면 어떨까? 문제가 더 심각해진다.
//explicit 명시적 형변환만 가능하게
class Person
{
public:
	template<typename T>
	explicit Person(T&& n)
		: name(std::forward<T>(n))
	{
	}

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

//파생클래스에서는 더 심해진다
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

int main()
{
	std::string petName("Darla");

	logAndAdd(petName); //왼값 넘김 emplace(복사 일어남)

	logAndAdd(std::string("Persephone")); //오른값 넘김 emplace(복사 일어남)

	logAndAdd("Patty Dog"); //리터럴을 넘김 임시 string이 생기고 name이 임시 string에 묶인다. emplace(복사 일어남)

	//개선 버전 호출

	logAndAdd2(petName); //왼값 넘김 emplace(복사 일어남)

	logAndAdd2(std::string("Persephone")); //오른값 넘김 emplace(오른값을 이동한다)

	logAndAdd2("Patty Dog"); //임시 string객체를 복사하는 대신, multiset안에 string을 생성한다.

	//색인버전 로깅 함수 호출
	logAndAdd2(22); //중복적재는 통상적인 예상에 대해서만 정상 동작한다.

	short nameIdx = 22;
	//logAndAdd2(nameIdx); //오류! T는 short&를 연역한다. 따라서 색인 버전이 호출되지 않는다.
	                     //emplace에 전달될 값은 string인데 short가 들어 왔으므로 실패한다.

	//따라서 보편참조와 중복적재를 결합하는것은 거의 나쁜 선택이된다.

	//클래스 중복적재 함수 호출
	Person p("nancy");
	//auto cloneOfP(p); //컴파일 실패! 완벽전달자 생성자를 호출한다.
	                  //explicit Person(Person& n)으로 추론 하여 부합되게 된다.

	const Person cp("nancy");
	auto cloneOfCp(cp); //템플릿과 똑같다면 보통함수 호출이 우선이다 복사 생성자가 호출된다~!

	return 0;
}