#include <iostream>
#include <memory>
#include <vector>
#include <list>
#include <tuple>
using namespace std;

std::vector<std::size_t> primeFactors(std::size_t x)
{
	std::vector<std::size_t> t;
	return t;
};

//constexpr �Լ�
//�����Ͻ� ���ϰ��� ����� �� �ִٸ� �׷��� �ϰ�, 
//�ƴϸ� �Ϲ����� �Լ��� �����ϰ� ��Ÿ�ӿ� �����Ͽ� 
//�� ���� ������ �Լ��� �ۼ����� �ʾƵ� �Ǵ� ������ ����

//underlying_type ������������ Ư��.
template<typename E>
constexpr typename std::underlying_type<E>::type toUType(E enumerator) noexcept
{
	return static_cast<typename std::underlying_type<E>::type>(enumerator);
	//return static_cast<std::underlying_type_t<E>>(enumerator); ��Ī �������.
}

//��Ī���� ����
template<typename E>
constexpr std::underlying_type_t<E> toUType2(E enumerator) noexcept
{
	return static_cast<std::underlying_type_t<E>>(enumerator);
}


template<typename E>
constexpr auto toUType3(E enumerator) noexcept
{
	return static_cast<typename std::underlying_type<E>::type>(enumerator);
	//return static_cast<std::underlying_type_t<E>>(enumerator); ��Ī �������.
}

//��Ī���� ����
template<typename E>
constexpr auto toUType4(E enumerator) noexcept
{
	return static_cast<std::underlying_type_t<E>>(enumerator);
}

int main()
{
	enum class Color2 : std::uint32_t { black, white, red };
	auto white = false; //����!

	//Color2 c = white; //����!
	Color2 c = Color2::white;

	auto c2 = Color2::white;

	//���� ���� enum�� �Ϲ������� ���� �������� ��ȯ�ȴ�.
	//=> �׸��� ���� �������κ��� �Ϲ������� �ε��Ҽ��� �������� ��ȯ�ȴ�.
	{
		enum Color { black, white, red };
		//auto white = false; //����! �̹����� �̹� white�� ����

		Color c = red;

		if (c < 14.5) //�÷��� double�� ��!?
		{
			auto primeFactors(c);
		}

		Color2 cc = Color2::red;

		//if (cc < 14.5) //����! Color2�� double�� ���� �� ����.
		{
		//	auto primeFactors(cc); //std::size_t�Լ��� Color2�� ������ �� ����.
		}
		
	}

	//���� ������ ����
	enum Color; //����!

	//�����Ϸ��� ���� �ִ� enum�� ������ �˰� ���� ������ �Ǵ� ����
	//���� �ִ� enum�� ���� ������ �⺻������ int�� ���� �����Ϸ��� �̹� �˰� �ִ�.
	//���������� ������ ���� ���� enum�� ���漱���� �����Ѵ�.
	enum class Color2 : std::uint32_t; //��������� ��������.

	//���� ���� enum�� ���� ���
	enum UserInfoFields {uiName, uiEmail, uiReputation};
	enum class UserInfoFields2 { uiName, uiEmail, uiReputation };

	using UserInfo = std::tuple<std::string, std::string, std::size_t>;

	UserInfo uInfo;

	auto val = std::get<uiEmail>(uInfo); //�Ϲ��� ��ȯ������ ����.
	auto val2 = std::get<static_cast<std::size_t>(UserInfoFields2::uiEmail)>(uInfo);

	auto val3 = std::get<toUType(UserInfoFields2::uiEmail)>(uInfo);
	return 0;
}