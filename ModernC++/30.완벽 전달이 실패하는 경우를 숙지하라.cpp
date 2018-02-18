#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <chrono>
#include <array>
using namespace std;

//완벽전달은 단순히 객체들을 전달하는 것만이 아니라.
//그 객체들의 주요 특징, 형식, 왼값 오른값 여부, const 나 volatile 여부까지도 전달하는것을 말한다.

void f(const std::vector<int>& v)
{
}

//전달하는 함수는 필연적으로 제네릭 함수이다.
template<typename T>
void fwd(T&& param)
{
	f(std::forward<T>(param));
}

//표준 컨테이너의 emplace류 함수들은 이런식의 형태로 구현되어 있다.
template<typename... Ts>
void fwd2(Ts&&... params)
{
	f(std::forward<Ts>(params)...);
}

//f(표현식);   이호출이 하는 일과
//fwd(표현식); 이호출이 하는 일이 다르다면, fwd표현식을 f에 완벽하게 전달하지 못한것이다.



int main()
{
	f({ 1,2,3 }); //암적적 변화를하므로 OK

	//fwd({ 1,2,3 }); //오류! fwd의 매개변수가 std::initializer_list가 될수 없는 형태로 선언되어 있다.

	//다음 두 조건중 하나라도 만족하지 않으면 완벽전달은 실패한다.
	//1.fwd의 매개변수들 중 하나 이상에 대해 컴파일러가 형식을 연역하지 못한다.
	//2.fwd의 매개변수들 중 하나 이상에 대해 컴파일러가 형식을 잘못역역 한다.
	//  하나는 그형식으로는 fwd의 인스턴스를 컴파일 할 수 업는것이고.
	//  또 하나는 컴파일된다고 해도 fwd의 연역된 형식들을 이용해서 호출하는 f가 fwd에 전달된 인수들로
	//  f를 직접호출했을 때와 다르게 행동하는것이다. (f가 중복적재된경우 직접f를호출할대와 다른 중복적재 함수가 불리는경우)

	//흥미롭게도 auto변수는 중괄호 초기치로 초기화해도 그 형식이 잘 연역된다.
	auto il = { 1,2,3 };

	fwd(il); //ok il이 f로 완벽하게 전달된다.

	return 0;
}