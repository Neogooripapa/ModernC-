#include <iostream>
#include <memory>
#include <vector>
#include <list>
#include <algorithm>
#include <string>
using namespace std;


int f(int x) throw(); //예외를 방출하지 않음 C++98식
					  //예외 명세가 위반되면 호출 스택이 f를 호출한 지점에 도달할 때까지 풀리며, 그지점에서 몇가지 동작을한다.
int f(int x) noexcept; //예외를 방출하지 않음 C++11식
					   //프로그램 실행이 종료되기 전에 호출 스택이 풀릴 수도 있고 풀리지 않을 수도 있다.
					   //noexcept 함수에서 컴파일러 최적화기는 예외가 함수 바깥으로 전파될 수 있다고 해도 실행시점 스택을 풀기 가능한
					   //상태로 유지할 필요가 없다.
					   //예외가 noexcept 함수를 벗어난다고 해도 noexcept 함수 안에 객체들을 반드시 생성과 반대 순서로 파괴해야 하는것도 아님.


int f(int x) throw(); //최적화 여지가 적다
int f(int x);		  //최적화 여지가 적다
int f(int x) noexcept; //최적화 여지가 가장 크다.

//조건부 noexcept
//결론은 사용자 정의 클래스에 의존한다는것이다.
//밑에서는 클래스의 move생성자 && move대입연산자 에 의존하고 있다.
template<class T>
void swap(T& a, T& b) noexcept (std::is_nothrow_move_constructible<T>::value
									&& std::is_nothrow_move_assignable<T>::value)
								//T로부터 타입을 유추해서 키워드로 move생성자 && move대입연산자 인지 검사한다.
{

}

template<class T, size_t N>
void swap(T(&a)[N], T(&b)[N]) noexcept(noexcept(swap(*a, *b))) //a,b는 포인터 타입이기때문에 *로 접근해야 타입 추론이 된다.
								//위에 정의된 swap으로부터 noexcept(move생성자 && move대입연산자가 noexcept인지 검사한다)
{
	//이함수는 위에 noexcept() 연산에 의해서 noexcept함수가 될수도 있고 아닐 수도 있다.
}

template<class T1, class T2>
struct pair
{
public:
	void swap(pair& p) noexcept(noexcept(swap(first, p.first)) && noexcept(swap(second, p.second)) )
	{

	}
};

int main()
{
	//std::vector에서 push_back은 이동연산이 예외를 방출하지 않음이 확실한 경우에 복사를 이동으로 대체한다.
	//그렇지 않을 경우 복사한다. (이동중에 예외가 발생하면 이동해버린 객체들은 비어버리기 때문에 이전 벡터공간이 무너진다)
	//가능하면 이동하되 필요하면 복사한다.
	//생서자에 noexcept가 있음을 점검하여 실행한다. 
	//std::vector::push_back -> std::move_if_noexcept -> 이동 생성자의 noexcept여부에 따라 오른값 조건부 캐스팅을 수행한다.



	//예외의 중립적인 함수 => 스스로는 예외를 던지지 않지만 예외를 던지는 함수들을 호출할 수는 이다.
	//다른 함수가 예외를 던지면 예외 중립 합수는 그 예외를 그냥 통과 시킨다.
	//이처럼 그냥 통과하는 예외가 존재할 수 있으므로
	//예외 중립적 함수는 noexcept가 될 수 없다. 따라서 대부분의 함수는 noexcept가 아니다.

	//기본적으로 모든 메모리 해제 함수와, 모든 소멸자가는 암묵적인 noexcept이다.
	//noexcept(flase)로하면 소멸자도 noexcept로 만들 수 있다.
	
	//noecept함수에서 예외를 던지면 프로그램은 종료된다.

	return 0;
}