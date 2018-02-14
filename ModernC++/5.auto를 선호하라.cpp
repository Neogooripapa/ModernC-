#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include <unordered_map>
using namespace std;

int main()
{
	//1.auto는 std::function보다 좋다

	//auto x2; //오류! 초기치가 꼭 필요함
	
	//컴파일러만 알던 형식을 지정할 수 있다. r
	auto r = [](const auto& p1, const auto& p2) -> bool
	{
		return *p1 < *p2;
	};

	//호출가능한 객체라면 어떤것이든 가리킬수 있다. (즉 함수처럼 호출가능한것을 가리킬수 있다 람다같은거)
	std::function<bool(const std::unique_ptr<int>&, const std::unique_ptr<int>&)>
		ref = [](const std::unique_ptr<int>& p1, const std::unique_ptr<int>& p2) { return *p1 < *p2; };


	//위에서 똑같이를 클러져를담는 auto , 펑션은 auto의 승리다. 문장길이도 그렇고
	//auto로 선언된, 클로저를 담는 변수는 클로저와 같은 형식이며, 그 클로저에 요구되는 만큼의 메모리르 사용한다.
	//반면 std::function는 템플릿의 인스턴스이며, 그 크기는 주어진 서명에 대해 고정되어 있다. 이 크기가 클로저를 담는데
	//부족하면 힙메모리를 할당할 수 도 있다. 
	//결과적으로 std::function 객체는 auto로 선언된 객체보다 메모리를 더 많이 소비한다.
	//또한 인라인화를 제한하고, 간접함수 호출을 산출하는 구현 세부사항 때문에 auto보다 느리다. 그리고 메모리 부족도 유발할 수 있다.


	//2.auto는 형식 단축을 피할 수 있다.
	std::vector<int> v;
	unsigned sz = v.size(); //32비트에서 32비트 크기로 같지만 64비트에서는 std::vector<int>::size_type은 64비트이므로 잘릴 수 있다.

	auto sz2 = v.size(); //이렇게 하면 된다~!


	std::unordered_map<string, int> m; //여기서 들어가는 형식은 std::pair<const string,int> 이다

	//임시변수를 생성해서 std::pair<string,int> 에 std::pair<const string,int>를 넣으려고한다.
	//p = 임시변수 따라서 p는 임시변수의 주소를가지고 있다.
	for (const std::pair<string, int>& p : m) 
	{

	}

	const int a = 1;
	//int& ref = a; //이게 안되는것랑 같은 이지치
	const int& ref1 = a; //이거된고

	//따라서 정확한 형식으로 변형되는 auto를 쓰면 임시변수 생성을 막을수 있다.
	//p = 실제m안의 한요소를 가지는 주소를 가지게 된다.
	for (const auto& p : m)
	{

	}

	return 0;
}