#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
using namespace std;

class Widget
{
};

int main()
{
	//weak_ptr은 shared_ptr을 보강하는 포인터다.
	auto spw = make_shared<Widget>(); //참조개수 1

	weak_ptr<Widget> wpw(spw); //참조개수 1

	spw = nullptr; //참조 개수 0이되고 Wiget이 파괴 된다.
				   //이제wpw는 대상을 잃은 상태이다.

	//wpw가 객체를 가리키지 않는지 검사.
	if (wpw.expired())
	{
		//피지칭 객체에 접근하는 방식의 코드를 떠올릴 수 있지만,
		//weak_ptr에는 역참조 연산이 없으므로 허용되지 않는다.

		//제대로 된 용법은, weak_ptr의 만료여부를 점검하고 아직 만료되지 않았으면
		//피지칭 객체에 대한 접근을 돌려주는 연산을 하나의 원자적 연산으로 수행하는 것이다.

		shared_ptr<Widget> spw1 = wpw.lock(); //wpw가 만료이면 spw1은 널
		//auto spw2 = wpw.lock(); //위와 동일

		shared_ptr<Widget> spw3(wpw); //wpw 만료이면 std::bad_weak_ptr 예외가 발생한다.
	}

	return 0;
}

//weak_ptr이 유용한 예제 1 (캐싱) (관찰자 목록)
//이 함수의 비용이 크다고 가정하자. 따라서 위젯을 캐싱 및 캐시에서 삭제하는 함수가 필요해진다.
unique_ptr<const Widget> loadWidget(int widgetId)
{
	return unique_ptr<const Widget>();
}


//이함수 바깥에서 객체를 잃었다면 여기서 만들어서 줄것이고
//객체를 잃지 않았다면 캐싱된 객체르 줄것이다. 여기서 객체가 살아 있는지 판단하는 weak_ptr이 핵심이다.
shared_ptr<const Widget> fastLoadWidget(int widgetId)
{
	static std::unordered_map<int, weak_ptr<const Widget>> cache;

	auto objPtr = cache[widgetId].lock();

	if (!objPtr)
	{
		objPtr = loadWidget(widgetId);
		cache[widgetId] = objPtr;
	}

	return objPtr;
}

//weak_ptr이 유용한 예제 2 (순환고리 방지)
//상황 B객체를 A와 C객체가 동시에 소유권을 공유하는 상황 A ---> B <--- C
//이상태에서 B에서 다시 A를 가리키는 포인터가 필요하게 되었다고 하면 그포인터의 종류는? 
//A ---> B <--- C
//  <---
//1.생포인터 : A가 파괴된 사실을 B가 모르므로 미정의 동작이 발생할 수 있음.
//2.shared_ptr : 순환참조로 인해서 둘다 파괴되지 못한다( A가 파괴되기 위해선 B가파괴되어 함. 반대로 B가 파괴되기 위해서 A가 파괴되어야 함)
//3.weak_ptr : B가 A의 파괴 사실을 알 수 있고 참조 개수에도 영향을 주지 않으므로 해결된다.

//엄격한 트리구조에서 tip
//자식노드는 부모만이 소유하므로 부모가 파괴되면 자식도 파괴되어야 하기 때문에 unique_ptr이 좋다.
//자식에서 부모로도의 역링크는 생포인터를 사용해도 좋다. 자식의 수명이 부모보다 길수 없기 때문에. 즉 자식노드가 부모를 역참조할 일이 없다.

//weak_ptr는 객체의 소유권 공유에 참여하지 않으며, 따라서 피지칭 객체의 참조 횟수에 영향을 미치지 않는다.
//제어블록에는 두번째 참조 횟수가 있으며 그것이 바로 weak_ptr가 조작하는 참조 횟수이다.