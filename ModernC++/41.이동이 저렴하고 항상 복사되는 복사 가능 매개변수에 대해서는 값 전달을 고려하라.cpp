#include <iostream>
#include <memory>
#include <vector>
#include <list>
#include <functional>
#include <unordered_map>
#include <atomic>
#include <thread>
#include <mutex>
using namespace std;

class Widget
{
public:
	void addName(const std::string& newName)
	{
		names.push_back(newName); //복사 1회
	}

	void addName(const std::string&& newName)
	{
		names.push_back(std::move(newName)); //이동 1회.
	}
private:
	std::vector<string> names;
};

class Widget2
{
public:
	template<typename T>
	void addName(T&& newName) 
	{
		names.push_back(std::forward<T>(newName)); //왼값 복사1회, 오른값 이동 1회.
	}
private:
	std::vector<string> names;
};

class Widget3
{
public:
	//어차피 복사된값을 강제로 이동하기때문에 다른 곳에 영향이 없다.
	//보편참조가 안되는경우도 가능해진다.(30장)
	//템플릿이 아니기때문에 함수도 하나로 끝이다.
	
	//값 전달은 비용이 크다고 하지만 그것은 C++98얘기
	//C++98에서는 호출자가 무엇을 넘겨주던 복사생성에 의해 생성된다.
	//C++11에서는 newName는 왼값일 때에만 복사 생성되고, 오른값일 때에는 이동 생성에 의해 생성된다.

	void addName(std::string newName)//왼값 복사 생성1회, 오른값 이동 생성1회
	{
		names.push_back(std::move(newName));//무조건 이동 1회
	}
private:
	std::vector<string> names;
};

//복사 가능 매개변수에 대해서만 값 전달을 고려해야한다.
//복사 할 수 없는 매개변수는 반드시 이동 전용형식일 것이다. (값전달이 안된다는뜻)
//이동 전용 형식에서는 복사 생성자가 비활성화 되어 있기 때문에 오른값 참조를 받는 버전하나만 있으면 된다.

class Widget4
{
public:
	void setPtr(std::unique_ptr<std::string>&& ptr)
	{
		p = std::move(ptr);
	}
private:
	std::unique_ptr<std::string> p; //유니크 ptr은 이동전용 형식.
};

void foo()
{
	Widget4 w;

	//오른값 참조로 전달. 총비용은 이동1회.
	w.setPtr(std::make_unique<std::string>("Modern C++"));
}


class Widget5
{
public:
	void setPtr(std::unique_ptr<std::string> ptr)
	{
		p = std::move(ptr);
	}
private:
	std::unique_ptr<std::string> p; //유니크 ptr은 이동전용 형식.
};

void foo2()
{
	Widget5 w;

	//매개변수를 값으로 받는다면, 이동생성 1회, 이동배정 1회. 총2회.
	w.setPtr(std::make_unique<std::string>("Modern C++"));
}

void main()
{

}