#include <iostream>
#include <vector>
using namespace std;

template<typename T>
class TD;


class Widget {};

template<typename T>
void f(const T& param) 
{
	//std::type_info::name은 반드시 주어진 형식을 템플릿 함수에 값 전달 처럼 해석한다.
	//따라서 참조성 및 const가 제거된것.
	cout << "T =" << typeid(T).name() << std::endl; //class Widget const * 잘못된 정보를 표시
	cout << "param =" << typeid(param).name() << std::endl; //class Widget const * 잘못된 정보를 표시
};

std::vector<Widget> createVec()
{
	vector<Widget> t;
	t.push_back(Widget());
	return t;
}

int main()
{
	//1. IDE 편집기를 이용해서 마우스를 올려봐서 확인가능
	const int a = 22;
	auto b = a; //auto에 마우스를 올리면 표신된다.

	//2. 정의부를 구현하지 않아서 오류를 발생하여 확인
	const int theAnswer = 42;
	auto x = theAnswer;
	auto y = &theAnswer;

	//TD<decltype(x)> xType; //'xType'은(는) 정의되지 않은 class 'TD<int>'을(를) 사용합니다.
	//TD<decltype(y)> yType; //'yType'은(는) 정의되지 않은 class 'TD<const int *>'을(를) 사용합니다.

	//3. 실행시점 출력
	std::cout << typeid(x).name() << std::endl;
	std::cout << typeid(y).name() << std::endl;

	const auto vw = createVec();

	if (!vw.empty())
	{
		f(&(vw[0])); //vw[0] -> vwm은 const로 선언되었기 때문에, const에대한 함수를 호출한다. const Widget&를 반환
					 //T -> Widget * const
					 //param-> const Widghet * const&
	}

	//이거랑 같은거임.
	int bb;
	const int& aa = bb;
	const auto& cc = &aa;

	//4.부스트로 정확한 형식 얻기

	/*
	#include <boost/type_index.hpp>

	template<typename T>
	void f(const T& param)
	{
		using boost::typeindex::type_id_with_cvr;

		cout << "T =" << type_id_with_cvr<T>().pretty_name() << endl;
		cout << "param =" << type_id_with_cvr<decltype(param)>().pretty_name() << endl;
	}
	*/

	//5.결국 사람 해석이 중요
	return 0;
}