#pragma once
#include <utility>
#include <memory>
#include <map>
#include <list>

#define lpCash Cash::GetInstance()

using MapInt = std::map<int, int>;

// 現金
class Cash
{
public:
	static Cash& GetInstance()
	{
		return *s_Instance;
	}

	bool Payment(MapInt inputMoney, int price);		// 決済処理
	bool ChangeCalculation(int surplus);			// お釣り計算処理
	MapInt GetChange();								// お釣りを返す

private:
	void ChangeClear();	// お釣りmapを初期化する

	struct CashDeleter
	{
		void operator()(Cash* cash) const
		{
			delete cash;
		}
	};

	Cash();
	~Cash();

	static std::unique_ptr<Cash, CashDeleter> s_Instance;

	MapInt change;	// お釣り　(first：金種　second：枚数)
	std::list<int>	moneyType;	// お釣りを出す際の金種リスト
};

