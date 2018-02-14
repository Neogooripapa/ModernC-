#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include <unordered_map>
#include <atomic>
using namespace std;

class Widget
{
public:
	Widget() {};
	Widget(int i, bool b) {};
	Widget(int i, double d) {};
	Widget(std::initializer_list<long double> il) {};

	operator float() const {};
private:
	int x{ 0 }; //가능
	int y = 0;  //가능
	//int z(0);   //오류

};

class Widget2
{
public:
	Widget2(int i, bool b) {};
	Widget2(int i, double d) {};
	Widget2(std::initializer_list<bool> il) {};
};

class Widget3
{
public:
	Widget3() {};
	Widget3(std::initializer_list<int> il) {};
};

int main()
{
	//1.세가지 초기화중 어디서나 사용할 수 있는것은 중괄호 초기화(균일 초기화) 다.


	Widget w1;		//일반 생성자 호출
	Widget w2 = w1; //복사 생성자를 호출
	w1 = w2;		//operator =를 호출

	std::vector<int> v{ 1, 3, 5 }; //초기 내용지정

	std::atomic<int> ai1{ 0 }; //가능
	std::atomic<int> ai2(0);   //가능
	std::atomic<int> ai3 = 0;  //오류


	//2.좁히기 변환을 방지해준다

	double x, y, z;
	y = z = x = 1.f;

	int sum1(x + y + z);  //좁히기 변환을 점검하지 않는다.
	int sum2 = x + y + z; //좁히기 변환을 점검하지 않는다.
	//int sum{ x + y + z }; //좁히기 변환을 점검하므로 에러

	Widget t(); //함수를 선언하게 된다.
	Widget w{}; //중괄호를 이용하면 이런한 문제를 겪지 않는다.

	//3.중괄호 초기화의 단점
	Widget h1(10, true);
	Widget h2{ 10, true }; // initializer_list생성자 호출 10과 true가 long double로 변환됨
	Widget h3(10, 5.0);
	Widget h4{ 10, 5.0 }; // initializer_list생성자 호출 10과 5.0이 long double로 변환됨

	Widget h5(h1); //복사 생성자 호출
	Widget h6{ h1 }; //initializer_list 생성자 호출 h1가 float으로 변환되고 float long double로 변환됨.

	Widget h7(std::move(h1)); //이동 생성자 호출.
	Widget h8{ std::move(h1) }; //initializer_list 생성자 호출 h1가 float으로 변환되고 float long double로 변환됨.

	//Widget2 wiget{ 10, 5.0 }; //오류! 좁히기 변환이 필요함. bool에 넣을수 없는 자료형이므로

	Widget3 o1; //기본 생성자 호출
	Widget3 o2{}; //기본 생성자 호출
	Widget3 o3(); //함수 선언임!

	Widget3 o4({}); //initializer_list 생성자를 빈 초기치 목록으로 호출
	Widget3 o5{{}}; //initializer_list 생성자를 빈 초기치 목록으로 호출

	std::vector<int> v1(10, 20); //비 std::initializer_list 생성자를 사용
								 //모든 요소값이 20인 요소 10개
	std::vector<int> v2{ 10, 20 }; //std::initializer_list 생성자를 사용
								   //10 20 요소 추가

	return 0;
}