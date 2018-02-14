#include <iostream>
#include <memory>
#include <vector>
#include <chrono>

using namespace std;

//std::move std::forward�� ���డ�� �ڵ带 ���� �������� �ʴ´�.
//�׳� ĳ������ �����ϴ� �Լ� (��ü�����δ� ���ø��̴�)

//std::move�� �־��� �μ��� ������ ���������� ĳ�����Ѵ�.
//std::forward�� Ư�� ������ �����ɶ����� �׷� ĳ������ �Ѵ�.

template<typename T>
typename remove_reference<T>::type&& move_11(T&& param)
{
	//�ް��� ������ ������ ������ ������ ��������ؼ� remove_reference ���
	//T&, T&& -> T 
	using ReturnType = typename remove_reference<T>::type&&; //T -> T&&�� �ȴ�.

	return static_cast<ReturnType>(param);
}

template<typename T>
decltype(auto) move_14(T&& param)
{
	//�ް��� ������ ������ ������ ������ ��������ؼ� remove_reference ���
	//T&, T&& -> T  ������ ��Ī���� �׸� 9����.
	using ReturnType = remove_reference_t<T>::type&&; //T -> T&&�� �ȴ�.

	return static_cast<ReturnType>(param);
}

//�������ε� �̵��ĺ��� �ƴѰ��
class string
{
public:
	string(const string& rhs); //���� ������.
	string(string&& rhs);      //�̵� ������.
};

class Annotation
{
public:
	explicit Annotation(const std::string text)
		: value(std::move(text)) //�̵��� �ƴ϶� ���簡�ȴ�~!
								 //const std::string&&�� ĳ���õȴ�. 
								 //���� �����ڰ� ȣ��ȴ�. const�� ���� �ް� ������ const ������ ������ ���°��� ���
	{

	};

private:
	std::string value;
};

//�� �������� ���� �ִ���
//�̵��� ������ ��ü�� const�� �������� ���ƾ��Ѵ�.const��ü������ �̵� ��û�� �Ҹ����� ���� �������� ��ȯ�ȴ�.
//move�� �̵��� �������� �ʴ´ٴ� �� &&�� �����. �̵��ڰ��� �������� �ʴ´�.

//forward
class Widget
{
};

void process(const Widget& lvalArg) { cout << "�ް� �Լ�" << endl; };
void process(Widget&& rvalArg) { cout << "������ �Լ�" << endl; };

template<typename T>
void logAndProcess(T&& param)
{
	auto now = std::chrono::system_clock::now(); //���� �ð��� ��´�.

	//param�� �ϳ��� �ް��̹Ƿ� void process(const Widget& lvalArg)�� ����ȴ�.
	//���� ���⼭ forward�� ����ϸ� && &������ Ȯ���ؼ� ĳ���� �Ѵ�.
	//�Ű����� T�� ��ȣȭ �Ǿ� �ִ°� �����ؼ� �ȴٰ� �ϳ�.
	process(std::forward<T>(param));
}

//move�� �ŷ�
class Widget2
{
public:
	Widget2(Widget2&& rhs)
		: s(std::move(rhs.s))
	{
		++moveCtorCalls;
	};
private:
	static std::size_t moveCtorCalls;
	std::string s;
};

class Widget3
{
public:
	Widget3(Widget3&& rhs)
		: s(std::forward<std::string>(rhs.s)) //���ʿ��� ��� �ٶ������� ���� ����.
		//s(std::forward<std::string&>(rhs.s)) //������ std::string& ���� �����ڰ� ȣ��ȴ�. 
	{
		++moveCtorCalls;
	};
private:
	static std::size_t moveCtorCalls;
	std::string s;
};

/*
template<class _Ty>
constexpr _Ty&& forward(remove_reference_t<_Ty>& _Arg) _NOEXCEPT
{
	// forward an lvalue as either an lvalue or an rvalue
	return (static_cast<_Ty&&>(_Arg));
}

template<class _Ty>
constexpr _Ty&& forward(remove_reference_t<_Ty>&& _Arg) _NOEXCEPT
{	
	// forward an rvalue as an rvalue
	static_assert(!is_lvalue_reference_v<_Ty>, "bad forward call");
	return (static_cast<_Ty&&>(_Arg));
}
*/

int main()
{
	Widget w;
	logAndProcess(w);
	logAndProcess(std::move(w));

	return 0;
}