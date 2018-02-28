#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <chrono>
#include <array>
#include <functional>
using namespace std;

using Time = std::chrono::steady_clock::time_point; //�ð����� ������

enum class Sound {Beep, Siren, Whistle};

using Duration = std::chrono::steady_clock::duration; //�ð��� ����

using Volume = std::chrono::steady_clock::duration;

void setAlram(Time t, Sound s, Duration d)
{
}

void foo()
{
	auto setSoundL = 
		[](Sound s)
	{
		using namespace std::chrono;        

		//�ѽð� �ĺ��� ������ �溸�� 30�� �߻�.
		setAlram(steady_clock::now() + hours(1), s, seconds(30));
	};

	//C++14������ ǥ�� ���̻� ���� �̿��ؼ� ���� �����ϰ� �ۼ� �� �� �ִ�.
	auto setSoundL2 =
		[](Sound s)
	{
		using namespace std::chrono;
		using namespace std::literals;
		//�ѽð� �ĺ��� ������ �溸�� 30�� �߻�.
		setAlram(steady_clock::now() + 1h, s, 30s);
	};
}

//���� ���� ���� �ϴ� �Լ���std::bind�� �̿��ؼ� �ۼ��ϸ�.
void foo1()
{
	using namespace std::chrono;
	using namespace std::literals;

	using namespace std::placeholders; //_1�� ����ϴµ� �ʿ���.

	//�μ��� ������ �˼����� ���� setAlram�� ������ �����Ѵ�.
	//������ ��� steady_clock::now()�� ���� �Լ��� ȣ��Ǵ� �������� �򰡵ȴ�.
	//���ε�� ���ε带 �� �ð����� 1�ð��� �ǹ����Ƿ� �̻�������.
	auto setSoundB = std::bind(setAlram, steady_clock::now() + 1h, _1, 30s);

	//�� ������ �ٷ� �������� �� ǥ������ setAlram ȣ�� ������ �����϶� std::bind���� �˷��־�� �Ѵ�.
	//�׷����� ù std::bind�ȿ� �ΰ��� �Լ� ȣ���� �������Ѿ� �Ѵ�.
	auto setSoundB2 = std::bind(setAlram, 
								std::bind(std::plus<>(), std::bind(steady_clock::now), 1h) //�Լ��� ���ε��ϸ� �Լ� ��ü�� �Ҹ��� �������� �����ȴ�.
								, _1, 30s);

	//C++11
	auto setSoundB3 = std::bind(setAlram,
		std::bind(std::plus<steady_clock::time_point>(), std::bind(steady_clock::now), 1h)
		, _1, 30s);
}

//setAlarm�� �ߺ������ϸ� bind�� ���ο� ������ �߻��Ѵ�.
void setAlram(Time t, Sound s, Duration d, Volume v)
{
}

void foo2()
{
	using namespace std::chrono;
	using namespace std::literals;

	using namespace std::placeholders;

	auto setSoundL =
		[](Sound s)
	{
		using namespace std::chrono;

		setAlram(steady_clock::now() + 1h, s, 30s);
	};//���ٴ� �� �۵��ȴ�.

	//C++11
	//auto setSoundB3 = std::bind(setAlram,
	//	std::bind(std::plus<steady_clock::time_point>(), std::bind(steady_clock::now), 1h)
	//	, _1, 30s); //� setAlram������ �𸣹Ƿ� �����ϵ��� �ʴ´�.(�����Ϸ��� �˰� �ִ°��� �̸����̸� �̸������δ� ���̼� �ؼҰ� �ȵȴ�)

	using SetAlarm3ParamType = void(*)(Time t, Sound s, Duration d);

	auto setSoundB = std::bind(static_cast<SetAlarm3ParamType>(setAlram),
		std::bind(std::plus<>(), std::bind(steady_clock::now), 1h)
		, _1, 30s); //�̷��� ȣ���� �ȴ�.

	//������ ��ó�� �ϸ� ���ٿ� �������� �ϳ� �� �����.
	setSoundL(Sound::Siren); //�ζ��ε� ���ɼ��� ũ��.

	//����� �Լ� �����͸� ȣ���Ͽ� ȣ���� �Ͼ��. �Լ� �����͸� �̿��� ȣ���� �ζ��� ���ɼ��� ����.
	setSoundB(Sound::Siren); 
}

//C++14 ������ ����
void foo3()
{
	int lowVal = 10;
	int highVal = 20;

	auto betweenL = 
		[lowVal, highVal](const auto& val)
	{
		return lowVal <= val && val <= highVal; 
	};

	//bind���ϸ�
	using namespace std::placeholders;

	auto betweenB =
		std::bind(std::logical_and<>(),
			std::bind(std::less_equal<>(), lowVal, _1),
			std::bind(std::less_equal<>(), _1, highVal)
		);

	//C++11������ ��������� ���ĵ��� �����ؾ��ϹǷ� ������ ���� �ڵ尡 ���´�.
	auto betweenB2 =
		std::bind(std::logical_and<bool>(),
			std::bind(std::less_equal<int>(), lowVal, _1),
			std::bind(std::less_equal<int>(), _1, highVal)
		);

	//���� C++11���� ���ٰ� auto�� ���� ���ϹǷ� ������ �����ؾ� �Ѵ�.
	auto betweenL2 =
		[lowVal, highVal](const int& val)
	{
		return lowVal <= val && val <= highVal;
	};
}

//�ڸ�ǥ
enum class CompLevel { Low, Normal, High };

class Widget
{
};

Widget compress(const Widget& w, CompLevel lev)
{
	return Widget();
}

void foo4()
{
	Widget w;
	using namespace std::placeholders;
	
	//w�� ������ ����ȴ�. ������ ȣ�� ���������δ� �߷��� �����. �۵������ �ȴٸ� ����.
	auto compressRateB = std::bind(compress, w, _1);
	
	//������ �����ϴ� ����� �ִ�.
	auto compressRateB2 = std::bind(compress, std::ref(w), _1);

	//���ٴ� �׿� ���� ��Ȯ�ϴ�.
	auto compressRateL = 
		[w](CompLevel lev)
	{
		return compress(w, lev); //w�� ������ �������ǰ�, lev�� ������ ���޵ȴ�.
	};

	compressRateL(CompLevel::High); //�μ��� ������ ���޵ȴ�

	compressRateB(CompLevel::High); //���ε� ��ü�� ���޵Ǵ� ��� �μ��� ������ ���޵ȴ�.
								    //�̴� �׷� ��ü�� �Լ� ȣ�� �����ڰ� �Ϻ� ������ ����ϱ� �����̴�

	//C++14���� std::bind�� ����ϴ� ���� �մ��� ��찡 ����.
}

//C++11���� bind����� ����ȭ �Ǵ� ���
//1. �̵� ������ : C++11 ���ٴ� �̵� �������� �������� �ʴ´�. �׷��� ���ٿ� bind�� �������� �̵��������� �䳻���� �ִ�.(�׸�32)
//2. ������ �Լ� ��ü : ���ε� ��ü�� ���� �Լ� ȣ�� �����ڴ� �Ϻ� ������ ����ϱ� ������ �� � ������ �μ��鵵 ���� �� �ִ�.
//   (�� �׸�30���� ������ �Ϻ� ������ ���� �ȿ���) �̴� ��ü�� ���ø�ȭ�� �Լ� ȣ�� �����ڿ� ������ �Ҷ� �����ϴ�.



void main()
{
}