#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
using namespace std;

struct SomeDataStructure
{
};
class Widget
{
public:
	Widget() {};
	//rhs가 오른값 참조이기때문에 move는 당연하다.
	Widget(Widget&& rhs)
		: name(std::move(rhs.name)), 
		p(std::move(rhs.p))
	{
	}

	//newName는 보편참조이기 때문에 forward는 당연하다.
	template<typename T>
	void SetName(T&& newName)
	{
		name = std::forward<T>(newName);
	}

	//보편참조에 move?! 나쁘다.
	template<typename T>
	void SetName2(T&& newName)
	{
		name = std::move(newName);
	}
	
	//중복 적재 버전. 좋지 않다.
	//매개변수가 늘어난다면 중복 적재 버전은 2에 n승이 될것이다.
	void SetName3(const std::string& newName)
	{
		name = newName;
	}
	void SetName3(std::string&& newName) 
	{
		name = std::move(newName);
	}
private:
	std::string name;
	std::shared_ptr<SomeDataStructure> p;
};


int main()
{
	Widget w;

	string s = "what";

	w.SetName2(s); //이제 s값은 알수 없다.

	//임시 string생성되어 setName의 매개변수에 묶이고
	//그임시 string이 w의 자료 멤버로 이동한다.
	//생성자, 이동배정연사자, 소멸자
	w.SetName3("sdfd"); //const char* 타입이다. 리터럴은. 이게 string타입으로 형변환이 일어나면서 임시 객체가 생기는것.
	w.SetName("sdfd"); //보편참조 버전에서 T가 const char*로 연역하기 때문에 임시 객체가 생성되지 않는다.

	return 0;
}

//함수가 결과를 값으로 돌려준다면
//그리고 그것이 오른값 참조나 보편참조에 묶인 객체라면
//해당 참조를 돌려주는 return문에 move나 forward를 사용하는것이 바람직하다.

class Matrix
{
public:
	Matrix operator +=(const Matrix& rhs)
	{
		return rhs;
	}
};

Matrix operator+(Matrix&& lsh, const Matrix& rhs)
{
	lsh += rhs;
	return std::move(lsh); //반환값으로 이동한다.
	//return lsh; 반환 값으로 복사한다.
	//Matrix가 이동생성을 지원한다고 해도 move가 해가 되지 않는다. 자동으로 복사 생성자 호출됨.
	//또한 나중에 Matrix에 이동생성이 추가되면 이득이 된다.
};

class Fraction
{
};

template<typename T>
Fraction reduceAndCopy(T&& frac) //값 전달 방식의 반환값.
{
	frac.reduce();
	return std::forward<T>(frac); //보편참조를 위한 forward 오른값은 반환값으로 이동하고, 왼값은 반환값으로 복사한다.
}

//반환값으로 복사될 오른값 참조 매개변수에 std::move를 적용하면
//복사 생성이 이동 생성으로 변한다를 확대 해석하는 경우
//함수가 반환할 지역 변수에도 이러한 최적화를 적용할 수 있을것이라는 엉뚱한 생각.
class Widget2
{
};


//반환값 최적화의 조건
//1.지역 객체의 형식이 함수의 반환 형식과 일치
//2.그 지역 객체가 바로 함수의 반환값이 되어야 한다.

//복사버전
Widget makeWidget()
{
	Widget w;
	//... w를 설정한다.
	return w; 
	//w를 함수의 반환값을 위해 마련한 메모리안에 생성하여 복사를 피한다.(반환값 최적화)
	//만약 반환 경로가 복잡해서 최적화가 불가능한경우
	//return std::move(w);로 컴파일러가 치환한다. 결국 사용자가 return std::move(w)를 명시하는건 좋지 않다.
}

//이동버전
Widget makeWidget2()
{
	Widget w;
	//... w를 설정한다.
	//복사를 이동으로 바꿈으로서 최적화가 가능하다고 생각한다..

	//최적화가 일어나지 않는다.
	return std::move(w); //이 함수가 돌려주는것은, 지역객체 w가 아니라, move의 결과 w에 대한 참조이다. (지역 객체가 아니란 말)
	                     //따라서 컴파일러 최적화를 무너뜨린 격이된다.
}

//값 전달 방식의 함수 매개변수의 경우
Widget makeWidget(Widget w)
{
	return w;
}

Widget makeWidget2(Widget w)
{
	return std::move(w); //컴파일러는 이렇게 취급한다.
}

//정리하면 함수가 지역객체를 값으로 반환하는 경우
//그 지역 객체에 move를 적용한다고 해서 컴파일러에게 도움을 주지 않는다는것이다.

//합당한 경우는 더이상 사용하지 않을 변수를 다른 함수에 넘겨주는 경우