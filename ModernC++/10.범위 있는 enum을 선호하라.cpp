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

//constexpr 함수
//컴파일시 리턴값을 계산할 수 있다면 그렇게 하고, 
//아니면 일반적인 함수와 동일하게 런타임에 결정하여 
//두 가지 버전의 함수를 작성하지 않아도 되는 편리함을 제공

//underlying_type 바탕형식으로 특질.
template<typename E>
constexpr typename std::underlying_type<E>::type toUType(E enumerator) noexcept
{
	return static_cast<typename std::underlying_type<E>::type>(enumerator);
	//return static_cast<std::underlying_type_t<E>>(enumerator); 별칭 선언버전.
}

//별칭선언 버전
template<typename E>
constexpr std::underlying_type_t<E> toUType2(E enumerator) noexcept
{
	return static_cast<std::underlying_type_t<E>>(enumerator);
}


template<typename E>
constexpr auto toUType3(E enumerator) noexcept
{
	return static_cast<typename std::underlying_type<E>::type>(enumerator);
	//return static_cast<std::underlying_type_t<E>>(enumerator); 별칭 선언버전.
}

//별칭선언 버전
template<typename E>
constexpr auto toUType4(E enumerator) noexcept
{
	return static_cast<std::underlying_type_t<E>>(enumerator);
}

int main()
{
	enum class Color2 : std::uint32_t { black, white, red };
	auto white = false; //성공!

	//Color2 c = white; //오류!
	Color2 c = Color2::white;

	auto c2 = Color2::white;

	//범위 없는 enum은 암묵적으로 정수 형식으로 변환된다.
	//=> 그리고 정수 형식으로부터 암무적으로 부동소수점 형식으로 변환된다.
	{
		enum Color { black, white, red };
		//auto white = false; //오류! 이범위에 이미 white가 있음

		Color c = red;

		if (c < 14.5) //컬러를 double과 비교!?
		{
			auto primeFactors(c);
		}

		Color2 cc = Color2::red;

		//if (cc < 14.5) //오류! Color2와 double을 비교할 수 없음.
		{
		//	auto primeFactors(cc); //std::size_t함수에 Color2를 전달할 수 없음.
		}
		
	}

	//전방 선언의 가능
	enum Color; //오류!

	//컴파일러가 범위 있는 enum의 형식을 알고 전방 선언이 되는 이유
	//범위 있는 enum의 바탕 형식은 기본적으로 int다 따라서 컴파일러는 이미 알고 있다.
	//바탕형식이 지정된 범위 없는 enum도 전방선언을 지원한다.
	enum class Color2 : std::uint32_t; //명시적으로 지정가능.

	//범위 없는 enum의 유용 사례
	enum UserInfoFields {uiName, uiEmail, uiReputation};
	enum class UserInfoFields2 { uiName, uiEmail, uiReputation };

	using UserInfo = std::tuple<std::string, std::string, std::size_t>;

	UserInfo uInfo;

	auto val = std::get<uiEmail>(uInfo); //암묵적 변환때문에 가능.
	auto val2 = std::get<static_cast<std::size_t>(UserInfoFields2::uiEmail)>(uInfo);

	auto val3 = std::get<toUType(UserInfoFields2::uiEmail)>(uInfo);
	return 0;
}