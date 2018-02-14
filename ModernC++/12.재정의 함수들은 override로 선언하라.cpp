#include <iostream>
#include <memory>
#include <vector>
#include <list>
using namespace std;

//재정의 조건
//1.기반 클래스 함수가 반드시 가상 함수여야 한다.
//2.이름이 동일해야한다(가상함수는 예외)
//3.매개변수 형식들이 반드시 동일해야한다.
//4.const성이 반드시 동일해야 한다.
//5.반환 형식과 예외 명세가 반드시 호환되어야 한다.
//6.멤버 함수들의 참조 한정사들이 반드시 동일 해야 한다.

//가상함수가 아닌 멤버 함수에도 참조 한정사를 지정할 수 있다.
class Widget
{
public:
	void doWork() & { cout << "왼값"; }; //*this가 왼값일 때에만 적용된다.
	void doWork() && { cout << "오른값"; };  //*this가 오른값일 때에만 적용된다.
};

Widget&& makeWidget()
{
	return Widget();
}

//문제가 되는경우
class Base
{
public:
	virtual void mf1() const;
	virtual void mf2(int x);
	virtual void mf3() &;
	void mf4() const;
};

class Dervied : public Base
{
public:
	virtual void mf1();					//const 없음.
	virtual void mf2(unsigned int x);   //unsigned임
	virtual void mf3() &&;				//&&참조임
	void mf4() const;					//기반함수에 virtual이 없음.
};

//따라서 C++11은 재정의를 명시적으로 표현하는 방법을 제공한다.
//경고가 일어나무로 필수
class Dervied2 : public Base
{
public:
//	virtual void mf1() override ;				//const 없음.
//	virtual void mf2(unsigned int x) override;  //unsigned임
//	virtual void mf3() && override;				//&&참조임
//	virtual void mf4() const override;			//기반함수에 virtual이 없음.
};

//가상 함수에 final을 적용하면 파생 클래스에서 그 함수를 재정의할 수 없게 된다.
//클래스 자체에 걸면 그 클래스는 기반 함수로 쓰일 수 없게 된다.

class Warning
{
public:
	void override(); //오바라이드 키워드는 함수 선언의 끝에 나올 때에만 예약된 의미를 가진다.
					 //override와 final은 키워드가 아니라 식별자이다.
};

//참조 한정사를 붙여야 하는 상황
class A
{
public:
	using DataType = std::vector<double>;

	DataType& data() { return values; };

private:
	DataType values;
};

A&& makeA()
{
	return A();
}

class B
{
public:
	using DataType = std::vector<double>;

	DataType& data() & { return values; };
	DataType&& data() && { return std::move(values); };

private:
	DataType values;
};

B&& makeB()
{
	return B();
}

int main()
{
	makeWidget().doWork();

	Widget w;
	w.doWork();

	//

	A a;

	auto vals1 = a.data(); //내용이 100개있다면 100번 복사가 일어난다. 여기서 auto는 복사이므로&무시됨, & 생성자가 불린다.

	auto vals2 = makeA().data(); //내용이 100개있다면 100번 복사가 일어난다.

	B b;

	auto vals3 = b.data(); //내용이 100개있다면 100번 복사가 일어난다. 여기서 auto는 복사이므로&무시됨 , & 생성자가 불린다.

	auto vals4 = makeB().data(); //이동 생성됨. &&생성자가 불린다.
	
	//move는 &&로 변환만하고 &&생성자가 호출되는것이 핵심이다.
	//&&생성자에서는 포인터를 넘겨주고 해당 변수를 nullptr로 만들기 때문에 위에서 임시객체 소멸자에 포인터검사후 삭제를 해야한다.

	return 0;
}