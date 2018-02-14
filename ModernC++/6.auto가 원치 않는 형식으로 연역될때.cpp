#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include <unordered_map>
using namespace std;

std::vector<bool> features(const int& w)
{
	vector<bool> t;
	return t;
}

int main()
{
	int w;

	//std::vector<bool>::referance의 기능 중 하나가 bool로의 암묵적 변환이다. (bool&가 아니라 bool로의 변환)
	bool priority = features(w)[5];

	//std::vector<bool>의 operator[]가 돌려주는 것은 컨테이너의 한 요소에 대한 참조가 아니다.
	//std::vector<bool>::referance 형식의 객체이다.
	//std::vector<bool>이 자신의 bool들을 bool당 1비트의 압축된 형태로 표현하도록 명시되어 있다.
	//이런방법때문에 std::vector<bool>의 operator[]를 직접적으로 구현할 수 없다.
	//std::vector<T>의 operator[]는 T&를 돌력주도록 되어 있지만 C++에서 비트에 대한 참조는 금지되어 있다.
	//bool&를 직접 돌려줄 수 없다는 제약 때문에 bool&처럼 작동하는 객체를 돌려주는 우회책을 사용한 것이다!

	
	//priority2는 std::vector<bool>::referance 객체의 한 복사본이며, 해당 워드를 가리키는 포인터와 5번비트의 오프셋을 저장한다. 
	//std::vector<bool>::referance는 임시변수 이므로 다음행에서 파괴
	
	//std::vector<bool> 객체 temp를 리턴 -> temp operator[] -> std::vector<bool>::referance 객체 (워드블럭,워드안에는 비트블럭)
	//auto priority2는 워드 & 오프셋을 가지게 된다.
	//따라서 priority2는 대상을 잃은 포인터가 된다.
	auto priority2 = features(w)[5]; //미정의 행동!

	//auto someVer = 대리자;  "보이지 않는 대리자 클래스 형식의 표현식은 피해야 한다"

	//이때 해법은 auto를 버리는것이 아니다.
	auto priority3 = static_cast<bool>(features(w)[5]);

	return 0;
}